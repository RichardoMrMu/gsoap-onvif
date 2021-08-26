#pragma once

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "glog/logging.h"
//#include "module/base.hpp"

namespace wl {

template<typename T>
class Channel {
public:
  Channel(size_t capacity=0)
    : capacity_(capacity) {}

  template<typename E>
  bool Send(E&& val) {
    static_assert(std::is_same<typename std::remove_cv<
                                  typename std::remove_reference<E>::type>::type,
                               T>::value,
                  "Types must match.");
    std::unique_lock<std::mutex> lock{mutex_};
    push_cv_.wait(lock, [this] {
      return internal_queue_.size() < capacity_ || capacity_ == 0 || close_.load(); 
    });
    if (close_.load()) return false;
    internal_queue_.push(std::forward<E>(val));
    pop_cv_.notify_one();
    return true;
  }

  template<typename E>
  bool Push(E&& val) {
    static_assert(std::is_same<typename std::remove_cv<
                  typename std::remove_reference<E>::type>::type,
                  T>::value,
                  "Types must match.");
    std::unique_lock<std::mutex> lock{ mutex_ };
    if (internal_queue_.size() == capacity_) { 
      internal_queue_.pop(); 
    }
    push_cv_.wait(lock, [this] {
      return internal_queue_.size() < capacity_ || capacity_ == 0 || close_.load();
    });
    if (close_.load()) return false;
    internal_queue_.push(std::forward<E>(val));
    pop_cv_.notify_one();
    return true;
  }

  bool Receive(T* val) {
    std::unique_lock<std::mutex> lock{mutex_};
    pop_cv_.wait(lock, [this] { return internal_queue_.size() > 0 || close_.load(); });
    if (close_.load() && internal_queue_.size() == 0) return false;
    *val = std::move(internal_queue_.front());
    internal_queue_.pop();
    push_cv_.notify_one();
    return true;
  }

  void Close() {
    CHECK_NE(close_.load(), true) << "Channel has been closed already, can not close twice.";
    close_.store(true);
    push_cv_.notify_all();
    pop_cv_.notify_all();
  }

  bool IsClosed() { return close_.load(); }

private:
  std::queue<T> internal_queue_;
  std::mutex mutex_;
  std::condition_variable push_cv_;
  std::condition_variable pop_cv_;
  size_t capacity_;
  std::atomic<bool> close_{false};

  //WL_DISABLE_COPY_AND_ASSIGN(Channel);
};

}  // namespace wl

#pragma once
#include <string>
#include <chrono>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <string>


namespace wl {
namespace fs {


std::string GetFullPath();

/*!
* \brief check whether the path exists or not
* \param path  path
* \return      true if exists else false
*/
bool IsExists(const std::string& path);
/*!
* \brief make a directory
* \note  if directory exists, do nothing and return true
* \param dpath   directory path
* \return        true if create success
*/
bool MakeDir(const std::string& dpath);
/*!
* \brief make a multi-level path, like /home/name/a/b/c/d
* \param path    directory path that you want create
* \return        true if create success
*/
bool CreatDirs(const std::string& path);
/*!
* \brief delete file
* \note  if file doesn't exists, do nothing and return true
* \param path  file path
* \return      true if the file not exists
*/
bool DeleteFile(const std::string& path);
/*!
* \brief join paths
* \param paths   many paths
* \return        joined path
*/
std::string JoinPath(std::initializer_list<std::string> paths);
/*!
* \brief list files in a directory with some file extensions
* \param dpath   directory path
* \param exts    extensions
* \return        file list, None if dpath doesn't exists
*/
std::vector<std::string> ListDir(const std::string& dpath, const std::vector<std::string>& exts);
/*!
* \brief list subdirectories in a given directory
* \param dpath   directory path
* \return        directories under the given directory path
*/
std::vector<std::string> ListSubDir(const std::string& dpath);
/*!
* \brief move file
* \param src_path  source file path
* \param dst_path  dest file path
* \return          true if move success
*/
bool MoveFile(const std::string& src_path, const std::string& dst_path);
/*!
* \brief get file name, [/path/to/]xxx[.yyy] --> xxx
* \param path  file path
* \return      file name
*/
std::string GetFileName(const std::string& path);
/*!
* \brief get file extension, [/path/to/]xxx.yyy --> yyy
* \param path  file path
* \return      file extension
*/
std::string GetFileExtension(const std::string& path);

struct datetime {
	std::string year;
	std::string month;
	std::string day;
	std::string time;
};

datetime DatetimeToString(time_t time);

class Timer {
	using Clock = std::chrono::system_clock;

public:
	void tic() {
		start_ = Clock::now();
	}
	void toc() {
		end_ = Clock::now();
	}
	double total_time() {
		std::chrono::duration<double> elapsed_seconds = end_ - start_;
		return elapsed_seconds.count();
	}
private:
	Clock::time_point start_, end_;
};


}  // namespace fs
}  // namespace wl

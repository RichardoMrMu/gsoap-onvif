#include "fs.hpp"
#include <cctype>
#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
// undef for fs api
#undef DeleteFile
#undef MoveFile
#else
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif  // WIN32
#include "glog/logging.h"
#ifdef WIN32
#define ACCESS(fileName,accessMode) _access(fileName,accessMode)
#define MKDIR(path) _mkdir(path)
#else
#define ACCESS(fileName,accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif
using std::vector;
using std::string;
namespace wl {
namespace fs {

string GetFullPath()
{
#ifdef WIN32
  char exeFullPath[MAX_PATH]; // Full path
  string strPath = "";

  GetModuleFileName(NULL, exeFullPath, MAX_PATH);
  strPath = (string)exeFullPath;    // Get full path of the file
  int pos = strPath.find_last_of('\\', strPath.length());
  return strPath.substr(0, pos);  // Return the directory without the file name
#else
  char szCurWorkPath[256];
  memset(szCurWorkPath, '\0', 256);
  int nRet = readlink("/proc/self/exe", szCurWorkPath, 256);
  if (nRet>256 || nRet<0) {
    return  "";
  }
  //上面已经获取到了可执行文件的据对路径了(包含文件名)，
  //下面的for循环是为了去除路径中的文件名，如果需要的话
  for (int i = nRet; i>0; i--) {
    if (szCurWorkPath[i] == '/' || szCurWorkPath[i] == '\\') {
      szCurWorkPath[i] = '\0';
      break;
    }
  }
  //这就是最终的文件路径，例如  "/usr/var"
  string szRet = szCurWorkPath;
  return szRet;
#endif
}



bool IsExists(const string& path) {
  if (access(path.c_str(), 0) != -1) {
    return true;
  }
  else {
    return false;
  }
}
bool MakeDir(const string& dpath) {
  if (IsExists(dpath) == true) {
    return true;
  }
  else {
    int rc;
#ifdef WIN32
    rc = mkdir(dpath.c_str());
#else
    // default mode, -rwxrwxr-x
    rc = mkdir(dpath.c_str(), 0775);
#endif  // WIN32
    if (rc == 0) {
      return true;
    }
    else {
      return false;
    }
  }
}
bool CreatDirs(const string& path) {
  int dirPathLen = path.length();
  if (dirPathLen > 2048) {
    return false;
  }
  char tmpDirPath[2048] = { 0 };
  for (int i = 0; i < dirPathLen; ++i) {
    tmpDirPath[i] = path[i];
    if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/' || i == (dirPathLen - 1)) {
      if (ACCESS(tmpDirPath, 0) != 0) {
        int ret = MKDIR(tmpDirPath);
        if (ret != 0) {
          return ret;
        }
      }
    }
  }
  return true;
}
bool DeleteFile(const string& path) {
  if (remove(path.c_str()) == 0) {
    return true;
  }
  else {
    return false;
  }
}
bool MoveFile(const string& src_path, const string& dst_path) {
  if (rename(src_path.c_str(), dst_path.c_str()) == 0) {
    return true;
  }
  else {
    return false;
  }
}
string JoinPath(std::initializer_list<string> args) {
  std::vector<string> paths(std::move(args));
  CHECK_GE(paths.size(), 2);
  string res = paths[0];
  for (int i = 1; i < paths.size(); i++) {
#ifdef WIN32
    res += "\\" + paths[i];
#else
    res += "/" + paths[i];
#endif
  }
  return res;
}
string GetFileName(const string& path) {
  int pos = path.find_last_of("/");
  if (pos == string::npos) {  // xxx.yyy
    return path.substr(0, path.find_last_of("."));
  }
  else {  // /path/to/xxx.yyy
    string tmp = path.substr(pos + 1);
    return tmp.substr(0, tmp.find_last_of("."));
  }
}
string GetFileExtension(const string& path) {
  int pos = path.find_last_of(".");
  if (pos == string::npos) {
    return "";
  }
  else {
    return path.substr(pos + 1);
  }
}
/*!
* \brief compare two string with case insensitive
* \param s1  string1
* \param s2  string2
* \return    true if equal else false
*/
static bool StringCompareCaseInsensitive(const string& s1, const string& s2) {
  if (s1.length() != s2.length()) return false;
  const int n = s1.length();
  for (int i = 0; i < n; i++) {
    if (std::tolower(s1[i]) != std::tolower(s2[i])) return false;
  }
  return true;
}
vector<string> ListDir(const string& dpath, const vector<string>& exts) {
  vector<string> lists;
  if (!IsExists(dpath)) {
    return lists;
  }
#ifdef WIN32
  //long fd;
  intptr_t fd;
  _finddata_t fi;
  string mp = dpath + "/*";
  if ((fd = _findfirst(mp.c_str(), &fi)) != -1) {
    do {
      string ext = GetFileExtension(fi.name);
      for (int i = 0; i < exts.size(); i++) {
        if (StringCompareCaseInsensitive(ext, exts[i])) {
          lists.push_back(fi.name);
          break;
        }
      }
    } while (_findnext(fd, &fi) == 0);
    _findclose(fd);
  }
#else
  DIR *dir;
  struct dirent *file;
  struct stat st;
  if (!(dir = opendir(dpath.c_str()))) {
    return lists;
  }
  while ((file = readdir(dir)) != NULL) {
    string fn(file->d_name);
    // remove '.', '..'
    if (fn == "." || fn == "..") {
      continue;
    }
    string ext = GetFileExtension(fn);
    for (int i = 0; i < exts.size(); i++) {
      if (StringCompareCaseInsensitive(ext, exts[i])) {
        lists.push_back(fn);
        break;
      }
    }
  }
  closedir(dir);
#endif  // WIN32
  return lists;
}
vector<string> ListSubDir(const string& dpath) {
  vector<string> lists;
  if (!IsExists(dpath)) {
    return lists;
  }
#ifdef WIN32
  long fd;
  _finddata_t fi;
  string mp = dpath + "/*";
  if ((fd = _findfirst(mp.c_str(), &fi)) == -1) {  // no file
    return lists;
  }
  do {
    if (fi.attrib & _A_SUBDIR) {
      string dn(fi.name);
      if (dn != "." && dn != "..") {
        lists.push_back(fi.name);
      }
    }
  } while (_findnext(fd, &fi) == 0);
  _findclose(fd);
#else
  DIR *dir;
  struct dirent *file;
  struct stat st;
  if (!(dir = opendir(dpath.c_str()))) {
    return lists;
  }
  while ((file = readdir(dir)) != NULL) {
    string fn(file->d_name);
    string fp = dpath + "/" + fn;
    if (stat(fp.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
      // remove '.', '..'
      if (fn == "." || fn == "..") {
        continue;
      }
      lists.push_back(fn);
    }
  }
  closedir(dir);
#endif  // WIN32
  return lists;
}

datetime DatetimeToString(time_t time) {
  tm *tm_ = localtime(&time);                // 将time_t格式转换为tm结构体
  int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
  year = tm_->tm_year + 1900;                // 临时变量，年，由于tm结构体存储的是从1900年开始的时间，所以临时变量int为tm_year加上1900。
  month = tm_->tm_mon + 1;                   // 临时变量，月，由于tm结构体的月份存储范围为0-11，所以临时变量int为tm_mon加上1。
  day = tm_->tm_mday;                        // 临时变量，日。
  hour = tm_->tm_hour;                       // 临时变量，时。
  minute = tm_->tm_min;                      // 临时变量，分。
  second = tm_->tm_sec;                      // 临时变量，秒。

  std::string yearStr = std::to_string(year);
  std::string monthStr = std::to_string(month);
  std::string dayStr = std::to_string(day);
  std::string hourStr = std::to_string(hour);
  std::string minuteStr, secondStr;
  if (minute < 10) {
    minuteStr = "0" + std::to_string(minute);
  }
  else {
    minuteStr = std::to_string(minute);
  }

  if (second < 10) {
    secondStr = "0" + std::to_string(second);
  }
  else {
    secondStr = std::to_string(second);
  }

	datetime datetimer;
	datetimer.year = yearStr;
	datetimer.month = monthStr;
	datetimer.day = dayStr;
	datetimer.time = hourStr + "-" + minuteStr + "-" + secondStr;
	return datetimer;
}
}  // namespace fs
}  // namespace wl
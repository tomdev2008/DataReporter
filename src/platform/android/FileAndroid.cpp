//
// Created by bingjian on 2018/11/5.
//
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <set>
#include "File.h"
#include "AndroidUtil.h"

namespace future {

    bool File::MkPath(const std::string &path) {

        size_t nPos = 0, nIndex = 0;
        std::string strPath = path;
        std::string strSub;
        int ret = 0;
        while ((nPos = strPath.find('/', nIndex)) != std::string::npos) {
            strSub = strPath.substr(0, nPos);
            if (!strSub.empty() && !IsFileExist(strSub)) {
                ret = mkdir(strSub.c_str(), ALLPERMS);
                if(ret != 0){
                    return false;
                }
            }

            nIndex = nPos + 1;
            while (strPath.length() > nIndex && strPath.at(nIndex) == '/') {
                strPath.replace(nPos, 2, "/");
            }

            if (strPath.length() <= nIndex) {
                break;
            }
        }
        if (strSub.compare(strPath) != 0 && !IsFileExist(strPath)){
            ret = mkdir(strPath.c_str(), ALLPERMS);
            if(ret != 0){
                return false;
            }
        }

        return true;
    }

    bool File::IsFileExist(const std::string &path) {
        struct stat st;
        int ret = 0;
        ret = stat(path.c_str(), &st);
        if (ret != 0) {
           return false;
        }

        return true;
    }

    bool File::ReName(const std::string &oldPath, const std::string &newPath) {

        int ret = 0;
        ret = rename(oldPath.c_str(),newPath.c_str());
        if(ret != 0){
            return false;
        }
        return true;
    }

    bool File::RemoveFile(const std::string &path) {

        struct stat st;
        int ret = 0;
        ret = stat(path.c_str(), &st);
        if (ret != 0) {
            return false;
        }

        if (S_ISDIR(st.st_mode)) {
            ret = rmdir(path.c_str());
            if (ret == 0) {
                return true;
            } else {
                return false;
            }
        }

        if (S_ISREG(st.st_mode)) {
            ret = remove(path.c_str());
            if (ret == 0) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

    std::shared_ptr<std::list<std::string> > File::FileList(const std::string &path) {
        std::shared_ptr<std::list<std::string> > retList = std::make_shared<std::list<std::string> >();
        DIR *dir = opendir(path.c_str());
        if (NULL == dir) {
            return retList;
        }

        std::string pathTmp;
        struct dirent *file;
        while ((file = readdir(dir)) != NULL) {
            if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
                continue;
            }
            pathTmp.clear();
            pathTmp.append(file->d_name);
            retList->push_back(pathTmp);
        }

        closedir(dir);
        return retList;
    }
}
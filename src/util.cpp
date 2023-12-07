
#include "util.hpp"


std::string wstringToString(std::wstring wstr_in) {
    int arrsize = (wstr_in.size() + 1) * 3 + 1;
    char* chr_out = new char[arrsize];
    WideCharToMultiByte(CP_UTF8, 0, wstr_in.c_str(), wstr_in.size() + 1, chr_out, arrsize, NULL, 0);
    std::string str_out(chr_out);
    delete chr_out;
    return str_out;
}

std::wstring stringToWstring(std::string str_in) {
    int arrsize = str_in.size() * 3 + 1;
    wchar_t* wchar_out = new wchar_t[arrsize];
    MultiByteToWideChar(CP_UTF8, 0, str_in.c_str(), str_in.size() + 1, wchar_out, arrsize);
    std::wstring wstr_out(wchar_out);
    delete wchar_out;
    return wstr_out;
}


// 指定したフォルダの中身を取得
std::vector<std::string> getFoldersAndFiles(std::wstring path) {
    HANDLE hFind;
    WIN32_FIND_DATA win32fd;
    std::vector<std::string> return_files;
    std::string tmp;
    char tmp_char[256];
    //拡張子付きファイルを全取得する
    hFind = FindFirstFile((path + L"\\\\*.py").c_str(), &win32fd);
    if (hFind != INVALID_HANDLE_VALUE){
        do {
            if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // ディレクトリは無視
            } else {
                tmp = wstringToString(win32fd.cFileName);
                return_files.push_back(tmp);

                FILETIME tmp_ftime = win32fd.ftLastWriteTime;
                SYSTEMTIME tmp_stime;
                FileTimeToSystemTime(&tmp_ftime, &tmp_stime);
                sprintf(tmp_char, "%d/%02d/%02d", tmp_stime.wYear, tmp_stime.wMonth, tmp_stime.wDay);
                tmp = tmp_char;
                return_files.push_back(tmp);
                long long datasize = win32fd.nFileSizeHigh;
                datasize = (datasize << 32) + win32fd.nFileSizeLow;
                if(datasize > (1<<20)){
                    tmp = std::to_string(datasize>>20) + "MB";
                }else if(datasize > (1<<10)){
                    tmp = std::to_string(datasize>>10) + "KB";
                }else{
                    tmp = "0KB";
                }
                std::ostringstream oss;
                oss << std::setw(6) << tmp;
                return_files.push_back(oss.str());
            }
        } while (FindNextFile(hFind, &win32fd));
    }
    FindClose(hFind);
    return return_files;
}

#ifndef IMGUI_PYTHON_UTIL
#define IMGUI_PYTHON_UTIL

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>


std::string wstringToString(std::wstring wstr_in);
std::wstring stringToWstring(std::string str_in);
std::vector<std::string> getFoldersAndFiles(std::wstring path);



#endif
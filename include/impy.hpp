#ifndef IMGUI_PYTHON_IMPY
#define IMGUI_PYTHON_IMPY

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>


#include <iostream>
#include <string>

namespace impy{
    bool run_script(char* filename);
    void draw();
}


#endif
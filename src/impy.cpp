
#include "impy.hpp"

namespace impy {
    
    // ================ Pythonで使う関数たち ================
    PyObject* Begin(PyObject *self, PyObject *args){
        static const char *text;
        static bool show;
        static int flag;

        if (!PyArg_ParseTuple(args, "spi", &text, &show, &flag))
            return PyBool_FromLong(false);

        return PyBool_FromLong(ImGui::Begin(text, &show, flag));
    }
    PyObject* End(PyObject *self, PyObject *args){
        ImGui::End();
        return PyBool_FromLong(true);
    }

    PyObject* Text(PyObject *self, PyObject *args){
        static const char *text;

        if (!PyArg_ParseTuple(args, "s", &text))
            return PyBool_FromLong(false);
        
        ImGui::Text(text);

        return PyBool_FromLong(true);
    }
    PyObject* Button(PyObject *self, PyObject *args){
        static const char *text;

        if (!PyArg_ParseTuple(args, "s", &text))
            return PyBool_FromLong(false);
        
        return PyBool_FromLong(ImGui::Button(text));
    }
    PyObject* InputText(PyObject *self, PyObject *args){
        static const char *text;
        static char *input;
        static int size;

        if (!PyArg_ParseTuple(args, "ssi", &text, &input, &size))
            return PyBool_FromLong(false);

        ImGui::InputText(text, input, size);

        return PyUnicode_FromString(input);
    }
    PyObject* SameLine(PyObject *self, PyObject *args){
        ImGui::SameLine();
        return PyBool_FromLong(true);
    }
    PyObject* SetNextItemWidth(PyObject *self, PyObject *args){
        static float width;

        if (!PyArg_ParseTuple(args, "f", &width))
            return PyBool_FromLong(false);

        ImGui::SetNextItemWidth(width);
        return PyBool_FromLong(true);
    }
    
    // ================ Pythonで使う関数の登録 ================
    static PyMethodDef impy_methods[] = {
        {"Begin",  Begin, METH_VARARGS, "ウィンドウの開始"},
        {"End",  End, METH_VARARGS, "ウィンドウの終了"},
        {"Text",  Text, METH_VARARGS, "テキスト"},
        {"Button",  Button, METH_VARARGS, "ボタン"},
        {"InputText",  InputText, METH_VARARGS, "テキスト入力"},
        {"SameLine",  SameLine, METH_VARARGS, "同じ行"},
        {"SetNextItemWidth",  SetNextItemWidth, METH_VARARGS, "次の横の長さ"},
        {NULL, NULL, 0, NULL}        /* Sentinel */
    };
    // ================ importの設定 ================
    static struct PyModuleDef impymodule = {
        PyModuleDef_HEAD_INIT,
        "impy",   /* name of module */
        NULL, /* module documentation, may be NULL */
        -1,       /* size of per-interpreter state of the module,
                    or -1 if the module keeps state in global variables. */
        impy_methods
    };
    // ================ 登録 ================
    PyMODINIT_FUNC PyInit_impy(void){
        return PyModule_Create(&impymodule);
    }

    // ================ Pythonの読み込み ================
    static PyObject *pModule = nullptr;
    bool run_script(char* filename){
        if(pModule != nullptr){
            Py_DECREF(pModule);
            Py_FinalizeEx();
            pModule = nullptr;
        }

        wchar_t PYHOME[] = L"python";
        wchar_t PYTHONPATH[] = L"python";
        std::string tmp_name = std::string(filename);
        std::string module_path = "program." + tmp_name.substr(0, tmp_name.size() -3);

        PyStatus status;
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        config.isolated = 1;
        config.home = PYHOME;
        config.pythonpath_env = PYTHONPATH;

        if (PyImport_AppendInittab("impy", PyInit_impy) == -1) {
            std::cout << "import impy failed" << std::endl;
            return false;
        }

        status = Py_InitializeFromConfig(&config);
        
        if (PyStatus_Exception(status)) {
            Py_ExitStatusException(status);
            std::cout << "Python environment mismatch" << std::endl;
            Py_FinalizeEx();
            return false;
        }
        
        // ファイル読み込み
        PyObject *pName = PyUnicode_DecodeFSDefault(module_path.c_str());
        std::cout << module_path << std::endl;
        
        if(pName == NULL){
            std::cout << "load file failed" << std::endl;
            Py_FinalizeEx();
            return false;
        }

        // ファイル実行
        pModule = PyImport_Import(pName);
        Py_DECREF(pName); // 参照を消しておく

        if(pModule == NULL){
            std::cout << "file run failed" << std::endl;
            Py_FinalizeEx();
            return false;
        }

        return true;
    }
    

    // ================ 描画ループを呼び出す ================
    void draw(){
        if(pModule == nullptr){
            return;
        }
        //pFuncに今回呼び出す関数を設定する
        PyObject *pFunc = PyObject_GetAttrString(pModule, "impy_draw");
        if(pFunc != nullptr){
            if(PyCallable_Check(pFunc)){
                PyObject *pArgs = PyTuple_New(0);
                //pFuncを実行する(pArgsはタプルでなければならない)
                PyObject *pTmp = PyObject_CallObject(pFunc, pArgs);
                Py_DECREF(pArgs); // 参照を消しておく

                //関数を実行
                int res_data;
                PyArg_Parse(pTmp, "i", &res_data);
                
                if(pTmp != nullptr){
                    Py_DECREF(pTmp); // 参照を消しておく
                }
                Py_XDECREF(pFunc); // 参照を消しておく
            }
        }
    }







    int python_test(){
        // 参考：https://docs.python.org/ja/3/extending/embedding.html
        
        wchar_t PYHOME[] = L"python";
        wchar_t PYTHONPATH[] = L"python";

        PyStatus status;
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        config.isolated = 1;
        config.home = PYHOME;
        config.pythonpath_env = PYTHONPATH;

        if (PyImport_AppendInittab("impy", PyInit_impy) == -1) {
            fprintf(stderr, "Error: could not extend in-built modules table\n");
            exit(1);
        }

        status = Py_InitializeFromConfig(&config);
        
        if (PyStatus_Exception(status)) {
            Py_ExitStatusException(status);
            std::cout << "NG" << std::endl << std::flush;
            return -1;
        }
        std::cout << "ok" << std::endl << std::flush;


        PyObject *pName = PyUnicode_DecodeFSDefault("program.sample_005");
        

        PyObject *pModule = PyImport_Import(pName);
        Py_DECREF(pName); // 参照を消しておく


        //pFuncに今回呼び出す関数を設定する(今回はfunc)
        PyObject *pFunc = PyObject_GetAttrString(pModule, "say_hello");
        PyCallable_Check(pFunc);

        PyObject *pArgs = PyTuple_New(0);
        //pFuncを実行する(pArgsはタプルでなければならない)
        PyObject *pTmp = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs); // 参照を消しておく

        //関数を実行
        PyObject* res_data;
        PyArg_Parse(pTmp, "O", res_data);
        printf("%d\n",res_data);
        
        Py_DECREF(pTmp); // 参照を消しておく
        Py_XDECREF(pFunc); // 参照を消しておく

        
        // PyRun_SimpleString("from time import time,ctime\n"
        //                    "print('Today is', ctime(time()))\n");
        // PyRun_SimpleString("hoge = 32\n");
        // PyRun_SimpleString("print(hoge)\n");
        // PyRun_SimpleString("import sys\n");
        // PyRun_SimpleString("print(sys.version)\n");
        
        if (Py_FinalizeEx() < 0) {
            exit(120);
        }
        
        // status = Py_InitializeFromConfig(&config);
        // PyRun_SimpleString("print(hoge)\n");
        // PyRun_SimpleString("print([123])\n");
        
        // if (Py_FinalizeEx() < 0) {
        //     exit(120);
        // }

        return 0;

    }
}
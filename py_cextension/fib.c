#include <stdio.h>
#include <Python.h>

// 第一个函数，print helloword
static PyObject *helloworld(PyObject *self, PyObject *args)
{
    printf("hello, world!\n");
    return Py_None;
}

// 第二个函数，斐波那契数列
int Cfib(int n)
{
    if (n < 2)
        return 1;
    else
        return Cfib(n - 1) + Cfib(n - 2);
}

static PyObject *fib(PyObject *self, PyObject *args)
{
    //  n用来存储python整数转换成c int的参数
    int n;
    // 注意是"i"，而不是'i'，前者返回指针，后者是个字符
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    // Py_BuildValue 应该是把类型转换为python类型，'i'应该是整数int
    return Py_BuildValue("i", Cfib(n));
}

// 下面是把每一个函数的名字，指针，参数类型，描述写成一个列表
static PyMethodDef myMethods[] = {
    {"hellworld", helloworld, METH_NOARGS, "Prints hello world"},
    {"fib", fib, METH_VARARGS, "Calculate fib(n)"},
    {NULL, NULL, 0, NULL}};

// 下面定义的应该是模块的参数
static struct PyModuleDef myModule = {
    PyModuleDef_HEAD_INIT,
    "myModule",
    "Test Moudle",
    -1,
    myMethods};

// 这里应该是构造模块
PyMODINIT_FUNC PyInit_myModule(void)
{
    return PyModule_Create(&myModule);
}

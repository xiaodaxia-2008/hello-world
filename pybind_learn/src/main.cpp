#include </home/xiaozhen/Workspace/pybind_learn/pybind11/include/pybind11/pybind11.h>
#include </home/xiaozhen/Workspace/pybind_learn/pybind11/include/pybind11/stl.h>
#include "fun.c"
// #include "MessageType.h"
#include "Server.c"

namespace py = pybind11;

PYBIND11_MODULE(example, m)
{
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";
    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");
    m.def("GetAge", &GetAge);
    m.def("CreatePerson", &StartPersonServer);
    m.def("GetPos", &GetPos, py::return_value_policy::reference);
    m.def("StartStateServer", &StartStateServer);

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

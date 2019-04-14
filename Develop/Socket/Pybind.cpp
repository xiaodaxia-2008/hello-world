#include <pybind11/pybind11.h>
#include "Server.c"
#include "Client.c"
// #include "MessageType.h"

namespace py = pybind11;

PYBIND11_MODULE(Motomini, m)
{
    m.doc() = "Motomini robot";
    m.def("Client", &Client);
    m.def("Server", &Server);
}
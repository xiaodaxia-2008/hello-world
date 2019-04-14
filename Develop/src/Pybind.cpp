#include <pybind11/pybind11.h>
// #include "Server.c"
// #include "MessageType.h"
#include <pthread.h>
#include <unistd.h>

void fun()
{
    sleep(3);
    printf("Hello from subthread after 3 seconds\n");
}

void call()
{
    pthread_t thrd;
    pthread_create(&thrd, NULL, (void *(*)(void *))fun, NULL);
    printf("Hello from main thread\n");
    pthread_join(thrd, NULL);
}

namespace py = pybind11;

PYBIND11_MODULE(Motomini, m)
{
    m.doc() = "Motomini robot";
    // m.def("Client", &Client);
    // m.def("BuildMsg", &BuildMsg);
    m.def("Call", &call);
}
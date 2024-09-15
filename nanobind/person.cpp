#include <nanobind/nanobind.h>

int add(int a, int b) {
    return a + b;
}

NB_MODULE(person_module, m) {
    m.def("add", &add);
}

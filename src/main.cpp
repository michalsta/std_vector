#include <string>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;
PYBIND11_MAKE_OPAQUE(std::vector<int>);

template <typename T>
class Wrapper
{
    std::vector<T> store;
public:
    Wrapper() {};
    void push_back(T t) { store.push_back(t); };
    T& operator[]( size_t pos ) { return store[pos]; };
};

template<typename T> void expose_vector(py::module_& m, const std::string& type_str)
{
    using Vec = Wrapper<T>;
    py::class_<Vec> V(m, ("std_vector_" + type_str).c_str());

    V.def(py::init<>());
    V.def("push_back", &Vec::push_back);
//    V.def("operator[]", &Vec::operator[])
}

void expose(py::module_& m)
{
    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");
}

PYBIND11_MODULE(std_vector_cpp, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: python_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    expose(m);
    py::bind_vector<std::vector<int>>(m, "VectorInt", py::module_local(false));

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");
    //py::bind_vector<std::vector<int>>(m, "VectorInt");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

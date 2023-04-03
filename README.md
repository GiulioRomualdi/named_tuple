<p align="center">
<h1 align="center">named_tuple </h1>
</p>
<p align="center">
<a href="https://isocpp.org"><img src="https://img.shields.io/badge/standard-C++17-blue.svg?style=flat&logo=c%2B%2B" alt="C++ Standard"/></a>
<a href="./LICENSE"><img src="https://img.shields.io/badge/license-BSD3-19c2d8.svg" alt="Size" /></a>
</p>

<p align="center">
<h4 align="center">🍦 Single header tagged tuple implementation 🍦</h4>
</p>

## Rationale behind the library

To access the content of an [`std::tuple`](https://en.cppreference.com/w/cpp/utility/tuple), we need to use [`std::get<>`](https://en.cppreference.com/w/cpp/utility/tuple/get) . This makes the code complex to read and maintain. 

The following example, taken from  [`std::tuple`](https://en.cppreference.com/w/cpp/utility/tuple), shows how to build and access a `std::tuple` using standard C++.
```c++
std::tuple<double, char, std::string> get_student(int id)
{
    switch (id)
    {
        case 0: return {3.8, 'A', "Lisa Simpson"};
        case 1: return {2.9, 'C', "Milhouse Van Houten"};
        case 2: return {1.7, 'D', "Ralph Wiggum"};
        case 3: return {0.6, 'F', "Bart Simpson"};
    }
 
    throw std::invalid_argument("id");
}

int main()
{
    const auto student = get_student(0);
    std::cout << "ID: 0, "
              << "GPA: " << std::get<0>(student0) << ", "
              << "grade: " << std::get<1>(student0) << ", "
              << "name: " << std::get<2>(student0) << '\n';
}
```
As you can imagine the following code can be complex to debug. Indeed if `std::get<>()` is called with the wrong index, the compiler will not throw any error but the message displayed in the console will be wrong. 

**:question: Is there a way to mitigate this?**

`named_tuple` tries to answer this question by implementing a single header library that will allow to access a tuple via string literals and rise a compile-time error in case a wrong string literals is used. 

### Basic usage

You can easily create a `ntuple::named_tuple` with the following code

```c++
#include <named_tuple/named_tuple.h>

// create a named tuple
using namespace ntuple::literals;
auto student0 = ntuple::make_named_tuple(ntuple::named_param<"GPA"_h, double>(),
                                         ntuple::named_param<"grade"_h, std::string>(),
                                         ntuple::named_param<"name"_h, std::string>());

ntuple::named_tuple<ntuple::named_param<"GPA"_h, double>(),
                    ntuple::named_param<"grade"_h, std::string>(),
                    ntuple::named_param<"name"_h, std::string>()> student1;
```

Once the `ntuple::named_tuple` has been created you can access the elements of the container with the following functions:

```c++
// get_from_hash<>() allows getting the reference to the parameter via a hash. 
// "name"_h returns the hash at compile time.
student0.get_from_hash<"name"_h>() = "Olivia";

// get<>() extracts the Ith element from the tuple.
// I must be an integer value in [0, sizeof...(Types))
student1.get<2>() = "Mark";

// std::get<>() extracts the Ith element from the tuple.
// I must be an integer value in [0, sizeof...(Types))
std::get<2>(student1) = "John";
```

The elements of the `ntuple::named_tuple` are accessible also via [Structured binding declaration](https://en.cppreference.com/w/cpp/language/structured_binding) as follows

```c++
auto& [GPA, grade, name] = student0;
```

In case you need to access to underlined tuple you can call the `to_tuple` method of `ntuple::named_tuple`. Similarly, you can build a `ntuple::named_tuple` from a `std::tuple` by calling the associate constructor or the copy assignment operator.

```c++
// Create a tuple from named_tuple
auto temp_tuple = student0.to_tuple();

// this calls the associated constructor.
ntuple::named_tuple<ntuple::named_param<"GPA"_h, double>(),
                    ntuple::named_param<"grade"_h, std::string>(),
                    ntuple::named_param<"name"_h, std::string>()> student2(temp_tuple);

// this calls the assignment operator
ntuple::named_tuple<ntuple::named_param<"GPA"_h, double>(),
                    ntuple::named_param<"grade"_h, std::string>(),
                    ntuple::named_param<"name"_h, std::string>()> student3;
student3 = temp_tuple;
```

:warning: These methods copy the values of `ntuple::named_tuple` into a `std::tuple` and vice-versa.

## Dependences

**`named_tuple`** is a self-contained library,  the only dependency you need is a sufficiently recent C++ compiler (full support to C++17).

In case you want to compile the tests you need to install [Catch2](https://github.com/catchorg/Catch2).

## Adding `named_tuple` to your project
`named_tuple` is a single-header file. You can add in three different ways

### Drag and drop
1. Drop [`named_tuple.h`](./include/named_tuple/named_tuple.h) wherever you like in your source tree
2. Enjoy `named_tuple.h` 🎉

### Use `FetchContent`
Drop the following lines in your `CMakeLists.txt`
```cmake
include(FetchContent)
FetchContent_Declare(
    named_tuple
    GIT_REPOSITORY https://github.com/GiulioRomualdi/named_tuple.git
    GIT_TAG        v0.1.0
)
FetchContent_MakeAvailable(named_tuple)
```

### Install the repository with `cmake`
Open a console and run
```console
git clone https://github.com/GiulioRomualdi/named_tuple.git
cd name_tuple
mkdir build && cd build
cmake ../
cmake --build .
[sudo] make install
```
If you want to enable tests set the `BUILD_TESTING` option to `ON`.

---

In case you use `FetchContent` and `cmake`, you can link `named_tuple` by adding the following lines in your `CMakeLists.txt`

```cmake
project(foo)
find_package(named_tuple REQUIRED)
add_executable(${PROJECT_NAME} src/foo.cpp)
target_link_libraries(${PROJECT_NAME} named_tuple::named_tuple)
```

## Bug reports and support

All types of [issues](https://github.com/GiulioRomualdi/named_tuple/issues/new) are welcome.

## Note
This library took inspiration from https://vitiy.info/named-tuple-for-cplusplus/. Moreover `named_tuple` is Here some other projects you may be interested to follow:

- Discussions on [StackOverflow](http://stackoverflow.com/questions/13065166/c11-tagged-tuple) and [GoogleGroups](https://groups.google.com/a/isocpp.org/forum/#!topic/std-proposals/N-kIXNrkTUk)
- [Inline Object Declaration](https://github.com/matt-42/iod)
- [named_types](https://github.com/duckie/named_types)

cmake_minimum_required(VERSION 2.8.12)
project(pybind11_example)

add_subdirectory(pybind11)
pybind11_add_module(example src/main.cpp)
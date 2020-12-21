cmake_minimum_required(VERSION 3.17)
project(MivniWetEx1_0)

set(CMAKE_CXX_STANDARD 11)

add_executable(MivniWetEx1_0 TimeCheck.cpp library.h library.cpp
        CoursesManager.h
 CoursesManager.cpp AVL.h)
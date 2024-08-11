cmake_minimum_required(VERSION 3.28)
project(tgen)

# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage("gh:gabime/spdlog@1.14.1")

set(CMAKE_CXX_STANDARD 20)

add_executable(tgen main.cpp
        Type.cpp
        Type.h
        Context.cpp
        Context.h
        ContextImpl.cpp
        ContextImpl.h
        Value.cpp
        Value.h
        EvaluationScope.cpp
        EvaluationScope.h
        Expr.cpp
        Expr.h
        ValueCast.cpp
        ValueCast.h
        TypeCasting.h)

target_link_libraries(tgen PRIVATE spdlog::spdlog)

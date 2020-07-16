cmake_minimum_required(VERSION 3.16)
project(getopt)

set(CMAKE_CXX_STANDARD 17)

add_executable(getopt main.cpp getopt.h getopt-ini.h)
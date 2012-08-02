project(passhash)
cmake_minimum_required(VERSION 2.6)
find_package(OpenSSL REQUIRED)
add_subdirectory(lib)
add_subdirectory(console)


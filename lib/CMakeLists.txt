project(libpasshash)
file(GLOB SOURCES *.c)
add_library(${PROJECT_NAME} OBJECT ${SOURCES})


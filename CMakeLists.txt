cmake_minimum_required(VERSION 2.8.10)
project(seq)

macro(show v)
  message("${v} is ${${v}}")
endmacro()

include_directories(src)

file(GLOB SRCS src/*.c src/*.cc src/*.h)
file(GLOB HDRS src/*.h)
add_library(seq ${SRCS})
install(TARGETS seq DESTINATION lib)
install(FILES ${HDRS} DESTINATION include)

add_subdirectory(matlab)
#enable_testing()
#add_subdirectory(test)

include(InstallRequiredSystemLibraries)
include(CPack)



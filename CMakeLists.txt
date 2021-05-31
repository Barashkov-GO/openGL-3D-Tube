cmake_minimum_required(VERSION 3.17)
project(mashGraf6)

set(CMAKE_CXX_STANDARD 14)

add_executable(mashGraf6 main.cpp stb_image.cpp)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(mashGraf6 glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(mashGraf6 OpenGL::GL)



#find_package(glad 3.3 REQUIRED)
#
#target_link_libraries(mashGraf2 glad)
#
#find_package(GLAD REQUIRED)
#
#target_link_libraries(mashGraf2 glad)



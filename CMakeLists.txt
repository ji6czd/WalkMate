cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)
project (Raspberry Pi Sonar)

add_executable (Sonar RaspberryPi/Sonar.cpp)
target_link_libraries(Sonar wiringPi)

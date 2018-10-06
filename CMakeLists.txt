cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)
project (RaspberryPiSonar)
find_package(Boost REQUIRED)
include_directories(
	"${Boost_INCLUDE_DIRS}")
add_executable (Sonar RaspberryPi/Sonar.cpp)
target_link_libraries(Sonar wiringPi)

cmake_minimum_required(VERSION 3.30)
project(Project_3_DSA)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project_3_DSA main.cpp)
find_package(Crow)
target_link_libraries(Project_3_DSA PUBLIC Crow::Crow)
# Include Crow headers
include_directories(/opt/homebrew/Cellar/crow) # Adjust path if installed manually
include_directories(/opt/homebrew/Cellar/crow/1.2.1.2/include/crow/)
include_directories(/opt/homebrew/Cellar/asio)
include_directories(templates)
# Add Boost library (required by Crow)
include_directories(/opt/homebrew/Cellar/boost)
cmake_minimum_required(VERSION 3.20)
project(cvPro)
set(Qt5_DIR "D:/Qt/Qt5.13.0/5.13.0/msvc2017_64/lib/cmake/Qt5")
set(OpenCV_DIR "D:/Programs/tools/opencv/build")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(
    Qt5
    REQUIRED
    COMPONENTS
    Widgets
    Core
    Gui
)

find_package(
    OpenCV
    REQUIRED
)

message(
    "OPENCV DIRS"
)

message(
    ${OpenCV_INCLUDE_DIRS}
)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(
    main
    src/main.cpp
    src/cvpro.ui
)

target_link_libraries(
    main
    ${OpenCV_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
cmake_minimum_required(VERSION 3.16)
project(app.gcs.system VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt modules
find_package(Qt6 COMPONENTS Core Gui Widgets SerialPort Charts REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/storage
)

# Source files
set(SOURCES
    src/main.cpp
    src/core/DataParser.cpp
    src/core/SerialWorker.cpp
    src/gui/MainWindow.cpp
    src/storage/Logger.cpp
)

# Header files
set(HEADERS
    src/common/TelemetryDef.h
    src/core/DataParser.h
    src/core/SerialWorker.h
    src/gui/MainWindow.h
    src/storage/Logger.h
)

# UI files
set(FORMS
    src/gui/MainWindow.ui
)

# Add executable
add_executable(app.gcs.system ${SOURCES} ${HEADERS} ${FORMS})

# Link libraries
target_link_libraries(app.gcs.system PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::SerialPort
    Qt6::Charts
)

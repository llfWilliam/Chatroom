cmake_minimum_required(VERSION 3.16)

project(Chatroom VERSION 0.1 LANGUAGES CXX)

# 启用 Qt 生成 UI 代码、MOC 代码、资源文件
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设定 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 组件（同时支持 Qt5 和 Qt6）
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network)

# 设置源文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui


    client.h
    client.cpp
)

# 创建 GUI 可执行文件（Qt6 需要 qt_add_executable）
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Chatroom
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(Chatroom ${PROJECT_SOURCES})
endif()

# MinGW GUI 需要 -mwindows
if (MINGW)
    target_link_options(Chatroom PRIVATE -mwindows)  # MinGW GUI 应用需要 -mwindows
endif()

# 设置 Chatroom 为 GUI 程序
set_target_properties(Chatroom PROPERTIES WIN32_EXECUTABLE TRUE)

# 链接 Qt6 库
target_link_libraries(Chatroom PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

# 创建服务器端可执行文件（⚠️ 不能加 WIN32_EXECUTABLE）
add_executable(ChatServer server.cpp server.h server_main.cpp
    server_main.h server_main.cpp)
set_target_properties(ChatServer PROPERTIES WIN32_EXECUTABLE FALSE)
target_link_libraries(ChatServer PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

# 安装目标
include(GNUInstallDirs)
install(TARGETS Chatroom
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6 需要 finalization
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Chatroom)
endif()

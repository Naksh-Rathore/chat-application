cmake_minimum_required(VERSION 3.20)

project(chat-application LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Conan dependencies
# -----------------------------
find_package(asio CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

# -----------------------------
# mrp library (shared core)
# -----------------------------
file(GLOB_RECURSE MRP_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/mrp/src/*.cpp
)

add_library(mrp STATIC ${MRP_SOURCES})

target_include_directories(mrp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/mrp/include
)

target_link_libraries(mrp PUBLIC
    asio::asio
    SQLite::SQLite3
)

# -----------------------------
# Client
# -----------------------------
file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/client/*.cpp
)

add_executable(client ${CLIENT_SOURCES})

target_link_libraries(client PRIVATE mrp)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# -----------------------------
# Server
# -----------------------------
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/server/*.cpp
)

add_executable(server ${SERVER_SOURCES})

target_link_libraries(server PRIVATE mrp)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# -----------------------------
# Warnings (optional)
# -----------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mrp PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(client PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(server PRIVATE -Wall -Wextra -Wpedantic)
endif()

cmake_minimum_required(VERSION 3.28.3)
project(jerboa_pure C)

set(CMAKE_C_STANDARD 11)

find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

add_definitions(-DMG_ENABLE_LINES=1)

add_executable(jerboa_pure main.c mongoose.c
        mongoose.h
        db.c
        db.h
        cache.c
        cache.h)

target_include_directories(jerboa_pure PRIVATE ${PostgreSQL_INCLUDE_DIRS} ${HIREDIS_INCLUDE_DIRS})
target_link_libraries(jerboa_pure
        ${PostgreSQL_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        m # Математическая библиотека
        dl # Динамическая линковка
)
cmake_minimum_required(VERSION 3.24)

project(glad LANGUAGES CXX)

add_library(glad STATIC)
target_sources(glad
        PRIVATE
        glad.cpp

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../
        FILES
        glad.h
        khrplatform.h
        )

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")
endif()

if(LINUX)
    target_link_libraries(glad ${CMAKE_DL_LIBS})
endif()

# INSTALL TARGETS
INSTALL(
        TARGETS glad
        CONFIGURATIONS Release
        FILE_SET HEADERS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
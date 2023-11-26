cmake_minimum_required(VERSION 3.16)
project(sfml-learn LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE INTERNAL "")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/sfml-learn)
# set(BUILD_SHARED_LIBS OFF)

set(SFML_VERSION 2.5.1)
set(CMAKE_BUILD_TYPE "Debug")
# set(WIN32 TRUE)

find_package(
  SFML ${SFML_VERSION}
  COMPONENTS system window graphics audio
  REQUIRED)
if(NOT SFML_FOUND)
  set(SFML_BUILD_GRAPHICS TRUE)
  set(SFML_BUILD_WINDOW TRUE)
  set(SFML_BUILD_AUDIO TRUE)
  include(FetchContent)
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG ${SFML_VERSION})
  FetchContent_MakeAvailable(SFML)
  find_package(SFML ${SFML_VERSION} COMPONENTS system window graphics audio)
endif()

set(SFML_LINKS sfml-window sfml-graphics sfml-audio)
file(GLOB_RECURSE source_list "src/*.cpp")
add_executable(sfml-learn ${source_list})
if(CMAKE_SYSTEM MATCHES Windows)
  target_link_libraries(sfml-learn PRIVATE opengl32 winmm gdi32 ${SFML_LINKS})
else()
  target_link_libraries(sfml-learn PRIVATE ${SFML_LINKS} fmt)
endif()

target_include_directories(sfml-learn PRIVATE ${LIB_DIR}/include include)
target_compile_features(sfml-learn PRIVATE cxx_std_20)

# if(WIN32) add_custom_command( TARGET sfml-learn COMMENT "Copy OpenAL DLL"
# PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
# ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll
# $<TARGET_FILE_DIR:sfml-learn> VERBATIM) endif()

install(TARGETS sfml-learn DESTINATION bin)

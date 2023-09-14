cmake_minimum_required(VERSION 3.16)
project(sfml-learn LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()


# include(FetchContent)
# FetchContent_Declare(SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 2.6.x)
# FetchContent_MakeAvailable(SFML)

add_executable(sfml-learn src/main.cpp)
target_link_libraries(sfml-learn PRIVATE sfml-graphics)
target_link_libraries(sfml-learn PRIVATE sfml-window)
target_link_libraries(sfml-learn PRIVATE sfml-system)
target_compile_features(sfml-learn PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET sfml-learn
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sfml-learn>
        VERBATIM)
endif()

install(TARGETS sfml-learn)

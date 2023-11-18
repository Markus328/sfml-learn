set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_RC_COMPILER "clang++ -target x86_64-pc-win32 -fno-PIC")

set(SFML_USE_STATIC_STD_LIBS
    ON
    CACHE BOOL "" FORCE)
set(SFML_STATIC_LIBRARIES TRUE)
add_definitions(-DSFML_STATIC)

set(LIB_DIR libs/windows/SFML-${SFML_VERSION})
set(SFML_DIR ${LIB_DIR})

link_directories(${LIB_DIR}/lib)
# set(WIN32 TRUE)

# include_directories( ./.xwin-cache/splat/sdk/include/ucrt
# ./.xwin-cache/splat/sdk/include/um ./.xwin-cache/splat/sdk/include/winrt
# ./.xwin-cache/splat/sdk/include/shared ./.xwin-cache/splat/crt/include)
# link_directories(./.xwin-cache/splat/sdk/lib/um
# ./.xwin-cache/splat/sdk/lib/ucrt ./.xwin-cache/splat/crt/lib)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH ${LIB_DIR} libs/windows/MSVC17)

# adjust the default behavior of the FIND_XXX() commands: search programs in the
# host environment set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)


set(CMAKE_MAKE_PROGRAM make)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".exe")
set(MSVC TRUE)
set(CMAKE_CXX_FLAGS "-target x86_64-pc-win32-msvc -fno-PIC -fuse-ld=lld-link")

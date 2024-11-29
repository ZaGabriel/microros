# Set the target system type
set(CMAKE_SYSTEM_NAME Linux)  # Change this to your target system
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Specify the compilers from the SDK
set(CMAKE_C_COMPILER /vtcs_toolchain/leipzig/usr/bin/aarch64-linux-gcc)
set(CMAKE_CXX_COMPILER /vtcs_toolchain/leipzig/usr/bin/aarch64-linux-g++)

set(CMAKE_C_COMPILER_WORKS 1 CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER_WORKS 1 CACHE INTERNAL "")

# Set the sysroot for the SDK (if applicable)
set(CMAKE_SYSROOT /vtcs_toolchain/leipzig/aarch64-buildroot-linux-gnu/sysroot)

# Add the SDK library path
set(SDK_LIBRARY_PATH /vtcs_toolchain/leipzig/usr/lib)

# Add the library path to the CMake search path
link_directories(${SDK_LIBRARY_PATH})

# Optionally, set the library paths for the linker
set(CMAKE_FIND_LIBRARY_PATH ${SDK_LIBRARY_PATH})

# Set compilation flags if necessary
set(CMAKE_C_FLAGS "-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os -g0 -D_FORTIFY_SOURCE=1" CACHE STRING "Buildroot CFLAGS")
set(CMAKE_CXX_FLAGS "-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os -g0 -D_FORTIFY_SOURCE=1" CACHE STRING "Buildroot CXXFLAGS")

set(__BIG_ENDIAN__ 0)
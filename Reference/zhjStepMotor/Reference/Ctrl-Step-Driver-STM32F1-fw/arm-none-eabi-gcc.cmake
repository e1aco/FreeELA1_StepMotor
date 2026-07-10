set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(TOOLCHAIN_PATH "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/14.2 rel1/bin")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc.exe)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-g++.exe)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc.exe)

set(CMAKE_AR ${TOOLCHAIN_PATH}/arm-none-eabi-ar.exe)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy.exe)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}/arm-none-eabi-objdump.exe)
set(SIZE ${TOOLCHAIN_PATH}/arm-none-eabi-size.exe)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

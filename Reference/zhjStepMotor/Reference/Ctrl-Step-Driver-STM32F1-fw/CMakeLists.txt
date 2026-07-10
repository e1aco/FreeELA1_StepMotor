#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.17)

# Auto-detect ARM GCC toolchain
if(NOT DEFINED TOOLCHAIN_PREFIX)
    # Common install paths on Windows
    set(TOOLCHAIN_CANDIDATES
        "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi"
        "C:/Program Files (x86)/GNU Arm Embedded Toolchain"
        "C:/Program Files/GNU Arm Embedded Toolchain"
        "C:/ST/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.12.3.rel1.win32_1.0.0.202401171346/tools"
        "C:/ST/STM32CubeIDE_1.14.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.12.3.rel1.win32_1.0.0.202310091425/tools"
    )
    foreach(CANDIDATE ${TOOLCHAIN_CANDIDATES})
        if(EXISTS "${CANDIDATE}")
            # Find the bin directory (may have version subdirectory like "14.2 rel1")
            file(GLOB TOOLCHAIN_VERSIONS LIST_DIRECTORIES true "${CANDIDATE}/*")
            foreach(VERSION_DIR ${TOOLCHAIN_VERSIONS})
                if(IS_DIRECTORY "${VERSION_DIR}")
                    set(BIN_DIR "${VERSION_DIR}/bin")
                    if(EXISTS "${BIN_DIR}/arm-none-eabi-gcc.exe")
                        set(TOOLCHAIN_PREFIX "${BIN_DIR}/arm-none-eabi")
                        set(TOOLCHAIN_BIN_DIR "${BIN_DIR}")
                        break()
                    endif()
                endif()
            endforeach()
            if(NOT TOOLCHAIN_PREFIX AND EXISTS "${CANDIDATE}/bin/arm-none-eabi-gcc.exe")
                set(TOOLCHAIN_PREFIX "${CANDIDATE}/bin/arm-none-eabi")
                set(TOOLCHAIN_BIN_DIR "${CANDIDATE}/bin")
            endif()
        endif()
        if(TOOLCHAIN_PREFIX)
            break()
        endif()
    endforeach()
endif()

# specify cross compilers and tools (prefer full path if auto-detected, fallback to short name)
if(TOOLCHAIN_PREFIX)
    message(STATUS "Found ARM toolchain: ${TOOLCHAIN_BIN_DIR}")
    if(NOT CMAKE_C_COMPILER)
        set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc.exe)
    endif()
    if(NOT CMAKE_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++.exe)
    endif()
    if(NOT CMAKE_ASM_COMPILER)
        set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc.exe)
    endif()
    if(NOT CMAKE_AR)
        set(CMAKE_AR ${TOOLCHAIN_PREFIX}-ar.exe)
    endif()
    if(NOT CMAKE_OBJCOPY)
        set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy.exe)
    endif()
    if(NOT CMAKE_OBJDUMP)
        set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump.exe)
    endif()
    if(NOT SIZE)
        set(SIZE ${TOOLCHAIN_PREFIX}-size.exe)
    endif()
else()
    message(STATUS "ARM toolchain not auto-detected, trying short names (ensure they are in PATH)")
    if(NOT CMAKE_C_COMPILER)
        set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    endif()
    if(NOT CMAKE_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    endif()
    if(NOT CMAKE_ASM_COMPILER)
        set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
    endif()
    if(NOT CMAKE_AR)
        set(CMAKE_AR arm-none-eabi-ar)
    endif()
    if(NOT CMAKE_OBJCOPY)
        set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    endif()
    if(NOT CMAKE_OBJDUMP)
        set(CMAKE_OBJDUMP arm-none-eabi-objdump)
    endif()
    if(NOT SIZE)
        set(SIZE arm-none-eabi-size)
    endif()
endif()
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(Ctrl-Step-STM32-fw C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(
        Core/Inc
        Drivers/STM32F1xx_HAL_Driver/Inc
        Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        Drivers/CMSIS/Device/ST/STM32F1xx/Include
        Drivers/CMSIS/Include
        Ctrl
        Ctrl/Driver
        Ctrl/Motor
        Ctrl/Sensor
        Ctrl/Signal
        UserApp
        Port
        Port/Platform
        Port/Platform/Memory
        Port/Platform/Utils
)

add_definitions(-DUSE_HAL_DRIVER -D__MICROLIB -DSTM32F1 -DSTM32F1xx -DSTM32F103xB)

file(GLOB_RECURSE SOURCES
        "startup/*.*"
        "Drivers/*.*"
        "Core/*.*"
        "Ctrl/*.*"
        "UserApp/*.*"
        "Port/*.*"
        )

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103CBTx_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

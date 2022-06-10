# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Device)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

include_directories(../common/_VS_KEIL/CMSIS/
                    ../common/_VS_KEIL/std_includes/
                    ../common/STM32Cube_F1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/
                    ../common/STM32Cube_F1/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/
                    ../common/STM32Cube_F1/Drivers/STM32F1xx_HAL_Driver/Inc/
                    ../common/STM32Cube_F1/Drivers/CMSIS/Device/ST/STM32F1xx/Include/
                    src/Hardware/HAL/
                    src/Hardware/CDC/
                    src/
                    )

add_definitions(-DSTM32F103xB)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # default constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd26812)   # The enum type %type% is unscoped. Prefer 'enum class' over 'enum' (Enum.3)


file(GLOB STM32F1             ../common/STM32Cube_F1/Drivers/STM32F1xx_HAL_Driver/Src/*.c
                              ../common/STM32Cube_F1/Drivers/STM32F1xx_HAL_Driver/Inc/*.h)
file(GLOB SRC                 src/*.*)
file(GLOB HARDWARE            src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS      src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL        src/Hardware/HAL/*.*)
file(GLOB MODULES_W25Q80DV    src/Modules/W25Q80DV/*.*)

source_group(STM32F1          FILES ${STM32F1})
source_group(src              FILES ${SRC})
source_group(Hardware         FILES ${HARDWARE})
source_group(Hardware/CMSIS   FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL     FILES ${HARDWARE_HAL})
source_group(Modules/W25Q80DV FILES ${MODULES_W25Q80DV})

set_source_files_properties(${STM32F1} ${HARDWARE_CMSIS}
                            src/Hardware/CDC/usbd_desc.c
                            src/Hardware/CDC/usb_device.c
                            src/Hardware/CDC/usbd_conf.c
                            PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(${PROJECT_NAME}
            ${STM32F1}
            ${SRC}
            ${HARDWARE}
            ${HARDWARE_CMSIS}
            ${HARDWARE_HAL}
            ${MODULES_W25Q80DV}
            )

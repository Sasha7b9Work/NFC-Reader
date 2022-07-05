# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Sample)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4101)
add_compile_options(/wd4201)
add_compile_options(/wd4365)
add_compile_options(/wd4456)
add_compile_options(/wd4458)
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # default constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5204)
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd26812)   # The enum type %type% is unscoped. Prefer 'enum class' over 'enum' (Enum.3)


file(GLOB SRC                *.h *.cpp)
file(GLOB CLRC663            CLRC663/*.*)
file(GLOB CR95LF             CR95LF/*.*)
file(GLOB DRV_SPI            Drv/Spi/*.*)
file(GLOB HARDWARE           Hardware/*.*)
file(GLOB SCMRTOS_COMMON     scmRTOS/Common/*.*)
file(GLOB MAIN               Main/*.*)

source_group(src             FILES ${SRC})
source_group(CLRC663         FILES ${CLRC663})
source_group(CR95LF          FILES ${CR95LF})
source_group(Drv/Spi         FILES ${DRV_SPI})
source_group(Hardware        FILES ${HARDWARE})
source_group(scmRTOS/Common  FILES ${SCMRTOS_COMMON})
source_group(Main            FILES ${MAIN})


add_library(${PROJECT_NAME}
            ${SRC}
            ${CLRC663}
            ${CR95LF}
            ${DRV_SPI}
            ${HARDWARE}
            ${SCMRTOS_COMMON}
            ${MAIN}
            )

add_library(LWIP_PORT INTERFACE)
target_include_directories(LWIP_PORT
    INTERFACE
       ${CMAKE_CURRENT_LIST_DIR}/port/lwip
#       ${PICO_LWIP_PATH}/src/include/
    )
    
#set(LWIP_CONTRIB_DIR ${PICO_LWIP_CONTRIB_PATH})
#include(${PICO_LWIP_CONTRIB_PATH}/Filelists.cmake)
#add_subdirectory(${PICO_LWIP_PATH} lwip)
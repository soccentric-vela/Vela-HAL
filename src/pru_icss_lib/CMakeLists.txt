add_library(pru_icss_lib pru_icss_lib.cpp pru_icss_lib.h pru_icss_contract.h)
target_include_directories(pru_icss_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pru_icss_lib PUBLIC lib_base)
install(TARGETS pru_icss_lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES pru_icss_lib.h pru_icss_contract.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vela_hal)

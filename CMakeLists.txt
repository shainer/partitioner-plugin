project(test)
cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
include (KDE4Macros)

include_directories( ${QT_INCLUDES}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_INSTALL_PREFIX}/include
                     ${CMAKE_INSTALL_PREFIX}/plugin/include )

set(test_SRCS
    main.cpp
    partitionerview.cpp
    buttonboxmodel.cpp
    devicetreemodel.cpp
)

kde4_add_executable(test ${test_SRCS})
target_link_libraries(test ${CMAKE_INSTALL_PREFIX}/lib/libsolid.so
                           ${QT_QTCORE_LIBRARY}
                           ${QT_QTGUI_LIBRARY}
                           
                           ${QT_QTDECLARATIVE_LIBRARY}
                           ${KDE4_KIO_LIBRARY})

install(DIRECTORY ./qml
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin
        FILES_MATCHING PATTERN "*.qml"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(FILES buttonboxmodel.h
              devicetreemodel.h
              partitionerview.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin/include)
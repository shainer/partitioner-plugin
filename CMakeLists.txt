project(test)
cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
include (KDE4Macros)

# Compiles with debugging symbols (Debug mode)
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS_DEBUG "-ggdb")
endif(CMAKE_COMPILER_IS_GNUCC)

include_directories( ${QT_INCLUDES}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_INSTALL_PREFIX}/include
                     ${CMAKE_INSTALL_PREFIX}/plugin/include )

set(test_SRCS
    main.cpp
    partitionerview.cpp
    executerthread.cpp

    buttonboxmodel.cpp
    devicetreemodel.cpp
    flagsmodel.cpp

    qmlcombobox.cpp
    qmlkdeicon.cpp
    pluginregister.cpp
)

kde4_add_executable(dolphinPartitioner ${test_SRCS})
target_link_libraries(dolphinPartitioner ${CMAKE_INSTALL_PREFIX}/lib/libsolid.so
                                         ${QT_QTCORE_LIBRARY}
                                         ${QT_QTGUI_LIBRARY}
                           
                                         ${QT_QTDECLARATIVE_LIBRARY}
                                         ${KDE4_KIO_LIBRARY}
                                         ${KDE4_UI_LIBRARY} )

install(FILES buttonboxmodel.h
              devicetreemodel.h
              flagsmodel.h
              partitionerview.h
              qmlcombobox.h
              qmlkdeicon.h
              pluginregister.h
              buttonnames.h
              executerthread.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin/include)

install(FILES QWidgets/qmldir
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin/qml/QWidgets)

install(TARGETS dolphinPartitioner ${INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(qml)
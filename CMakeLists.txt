project(dolphin-partitioner)
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
    pluginregister.cpp

    models/buttonboxmodel.cpp
    models/devicetreemodel.cpp
    models/flagsmodel.cpp

    qmlwidgets/qmlcombobox.cpp
    qmlwidgets/qmlkdeicon.cpp
)

kde4_add_executable(dolphin-partitioner ${test_SRCS})
target_link_libraries(dolphin-partitioner ${CMAKE_INSTALL_PREFIX}/lib/libsolid.so
                                          ${QT_QTCORE_LIBRARY}
                                          ${QT_QTGUI_LIBRARY}                           
                                          ${QT_QTDECLARATIVE_LIBRARY}
                                          ${KDE4_KIO_LIBRARY})

install(FILES partitionerview.h
              pluginregister.h
              buttonnames.h
              executerthread.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin/include)

install(FILES models/buttonboxmodel.h
              models/flagsmodel.h
              models/devicetreemodel.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin/include/models)

install(FILES qmlwidgets/qmlcombobox.h
              qmlwidgets/qmlkdeicon.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin/include/qmlwidgets)

install(FILES QWidgets/qmldir
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin/qml/QWidgets)

install(TARGETS dolphin-partitioner ${INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(qml)

install(DIRECTORY ./
        DESTINATION /etc/qml-plugin
        FILES_MATCHING PATTERN "*.qml"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

add_subdirectory(dialogs)
add_subdirectory(widgets)
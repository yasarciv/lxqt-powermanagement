file(GLOB DESKTOP_FILES_IN *.desktop.in)

# Translations **********************************
lxqt_translate_desktop(DESKTOP_FILES
    SOURCES
        ${DESKTOP_FILES_IN}
)
add_custom_target(lxqt_powermanagement_autostart_desktop_files ALL DEPENDS ${DESKTOP_FILES})
#************************************************

install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${LXQT_ETC_XDG_DIR}/autostart"
    COMPONENT Runtime
)

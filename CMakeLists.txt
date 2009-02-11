# Project Needs a name ofcourse
project(kcd)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
find_package(MusicBrainz REQUIRED)

include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
)

# We add our source code here
set(kcd_SRCS plasma-kcd.cpp
             infopanel.cpp
             controls.cpp
             cdhandler.cpp
             mbmanager.cpp
             options.cpp
             volumecontroller.cpp
             tracksdialog.cpp
)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_kcd ${kcd_SRCS})

target_link_libraries(plasma_applet_kcd
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
                      ${KDE4_KIO_LIBS} ${PHONON_LIBRARY} ${KDE4_SOLID_LIBRARY}
                      ${MUSICBRAINZ_LIBRARIES})
 

install(TARGETS plasma_applet_kcd
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-kcd.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})


#install(FILES
#        DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/)

#kde4_install_icons(${ICON_INSTALL_DIR})

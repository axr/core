include_directories(cairosdl)

set(axr_SOURCES
    cairosdl/cairosdl.c
    cairosdl/cairosdl.h
    axr.rc
)

if(WIN32)
    list(APPEND axr_SOURCES main_win32.cpp)
elseif(UNIX)
    list(APPEND axr_SOURCES main_x11.cpp)
endif()

add_executable(axr WIN32 ${axr_SOURCES})

target_link_libraries(axr AXR_core ${EXPAT_LIBRARIES} ${Boost_LIBRARIES} ${SDLMAIN_LIBRARY} ${SDL_LIBRARY} ${PANGO_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})

if(UNIX AND NOT APPLE)
    target_link_libraries(axr ${GLIB_LIBRARIES})
endif()
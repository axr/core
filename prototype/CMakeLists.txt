 ####################################################################
 #             a  A                                                        
 #            AM\/MA                                                         
 #           (MA:MMD                                                         
 #            :: VD
 #           ::  º                                                         
 #          ::                                                              
 #         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 #       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 #      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 #      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM                   
 #     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM                  
 #    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 #   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 #  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 #         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 #        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 #       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 #      TMMM MMMM                                                           
 #     TMMM  .MMM                                         
 #     TMM   .MMD       ARBITRARY·······XML········RENDERING                           
 #     TMM    MMA       ====================================                              
 #     TMN    MM                               
 #      MN    ZM                       
 #            MM,
 #
 # 
 #      COPYRIGHT: ©2011 - All Rights Reserved
 #
 #      LICENSE: see License.txt file
 #
 #      WEB: http://axr.vg
 #
 #      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 #      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 #      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 #      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 #      FITNESS FOR A PARTICULAR PURPOSE.
 #
 ####################################################################
 #
 #      FILE INFORMATION:
 #      =================
 #      Last changed: 2012/05/25
 #      HSS version: 1.0
 #      Core version: 0.47
 #      Revision: 20
 #
 ####################################################################

project(AXR)
cmake_minimum_required(VERSION 2.8)

if(UNIX)
    set(Boost_USE_STATIC_LIBS   ON)
    set(Boost_USE_MULTITHREADED ON)

    find_package(PkgConfig REQUIRED)
    find_package(EXPAT REQUIRED expat>=2.0)

    find_package(Boost COMPONENTS thread system program_options REQUIRED)
#    find_package(Threads REQUIRED)

    pkg_check_modules(GLIB REQUIRED glib-2.0)
    pkg_check_modules(SDL REQUIRED sdl)
    pkg_check_modules(PANGO REQUIRED pango)
    pkg_check_modules(CAIRO REQUIRED cairo)
    pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)
endif(UNIX)

include_directories(
    ${EXPAT_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${SDL_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${PANGOCAIRO_INCLUDE_DIRS}
)

add_subdirectory(core)

if(UNIX AND NOT APPLE)
    add_executable(axr
        linux/cairosdl/cairosdl
        linux/main
    )

    target_link_libraries(axr AXR_core ${EXPAT_LIBRARIES} ${Boost_LIBRARIES} ${GLIB_LIBRARIES} ${SDL_LIBRARIES} ${PANGO_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})
endif()

# Set distribution type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE distribution)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
set(CMAKE_C_FLAGS_DISTRIBUTION "-O3")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")

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

PROJECT(AXR)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)                       #This policy of cmake has changed recently. If explicitly not set gives a warning about it.


if (UNIX)                                           #Both Linux and Mac are UNIX
    set(Boost_USE_STATIC_LIBS   ON)
    set(Boost_USE_MULTITHREADED ON)

    find_package(PkgConfig REQUIRED)
    find_package (EXPAT REQUIRED expat>=2.0)        #required minimum version can be specified like this

    find_package(Boost COMPONENTS thread system REQUIRED)
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


ADD_SUBDIRECTORY(core)

ADD_EXECUTABLE(axr
    linux/cairosdl/cairosdl
    linux/main
)

LINK_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/core
    ${EXPAT_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIRS}
    ${GLIB_LIBRARY_DIRS}
    ${SDL_LIBRARY_DIRS}
    ${PANGO_LIBRARY_DIRS}
    ${CAIRO_LIBRARY_DIRS}
    ${PANGOCAIRO_LIBRARY_DIRS}
)

TARGET_LINK_LIBRARIES(axr AXR_core ${EXPAT_LIBRARIES} ${Boost_LIBRARIES} ${GLIB_LIBRARIES} ${SDL_LIBRARIES} ${PANGO_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGOCAIRO_LIBRARIES}) 

#Set distribution type
IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE distribution)
ENDIF (NOT CMAKE_BUILD_TYPE)

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
SET(CMAKE_C_FLAGS_DISTRIBUTION "-O3")

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
SET( CMAKE_C_FLAGS_DEBUG "-O0 -g")

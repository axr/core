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
 #      Last changed: 2012/06/19
 #      HSS version: 1.0
 #      Core version: 0.47
 #      Revision: 20
 #
 ####################################################################


FILE(GLOB AXR_CORE_SRCS *.cpp)
FILE(GLOB_RECURSE AXR_SRCS axr/*.cpp)
FILE(GLOB_RECURSE XML_SRCS xml/*.cpp)
FILE(GLOB_RECURSE HSS_SRCS hss/*.cpp)

IF (WIN32)
    FILE(GLOB_RECURSE WRAPPER_SRCS os/win/*.cpp)
ELSEIF(APPLE)
    FILE(GLOB_RECURSE WRAPPER_SRCS os/osx/*.cpp)
ELSE()
    FILE(GLOB_RECURSE WRAPPER_SRCS os/linux/*.cpp)
ENDIF()

ADD_LIBRARY(AXR_core            #make a static library AXR_core and add these files to it
            ${AXR_CORE_SRCS}
            ${AXR_SRCS}
            ${XML_SRCS}
            ${HSS_SRCS}
            ${WRAPPER_SRCS})

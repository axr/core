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

file(GLOB AXR_CORE_SRCS *.cpp *.h)
file(GLOB_RECURSE AXR_SRCS axr/*.cpp axr/*.h)
file(GLOB_RECURSE XML_SRCS xml/*.cpp xml/*.h)
file(GLOB_RECURSE HSS_SRCS hss/*.cpp hss/*.h)

if(WIN32)
    file(GLOB_RECURSE WRAPPER_SRCS os/win/*.cpp os/win/*.h)
elseif(APPLE)
    file(GLOB_RECURSE WRAPPER_SRCS os/osx/*.mm os/osx/*.h)
else()
    file(GLOB_RECURSE WRAPPER_SRCS os/linux/*.cpp os/linux/*.h)
endif()

source_group("AXR" FILES ${AXR_SRCS})
source_group("XML" FILES ${XML_SRCS})
source_group("HSS" FILES ${HSS_SRCS})
source_group("OS" FILES ${WRAPPER_SRCS})
source_group("" FILES ${AXR_CORE_SRCS})

add_library(AXR_core ${AXR_CORE_SRCS} ${AXR_SRCS} ${XML_SRCS} ${HSS_SRCS} ${WRAPPER_SRCS})

/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      TMMM MMMM                                                           
 *     TMMM  .MMM                                         
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     TMM    MMA       ====================================                              
 *     TMN    MM                               
 *      MN    ZM                       
 *            MM,
 *
 * 
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/10/08
 *      HSS version: 1.0
 *      Core version: 0.4
 *      Revision: 4
 *
 ********************************************************************/

#ifndef AXRDEBUGGING_H
#define AXRDEBUGGING_H

//Debug levels:
// - 0 No debug
// - 1 
// - 2 
// - 3 
// - 4 All messages

//you can set this as a preprocessor macros in your IDE to override
#ifndef AXR_DEBUG_LEVEL
//default level
#define AXR_DEBUG_LEVEL 0
#endif

//logging
#define std_log(what) std::cout << what << std::endl

#if AXR_DEBUG_LEVEL > 0
#define std_log1(what) std::cout << what << std::endl

#if AXR_DEBUG_LEVEL > 1
#define std_log2(what) std::cout << what << std::endl

#if AXR_DEBUG_LEVEL > 2
#define std_log3(what) std::cout << what << std::endl

#if AXR_DEBUG_LEVEL > 3
#define std_log4(what) std::cout << what << std::endl
#else
#define std_log4(what)
#endif //AXR_DEBUG_LEVEL > 3

#else
#define std_log3(what)
#define std_log4(what)
#endif //AXR_DEBUG_LEVEL > 2

#else //AXR_DEBUG_LEVEL < 0

#define std_log2(what)
#define std_log3(what)
#define std_log4(what)
#endif //AXR_DEBUG_LEVEL > 1

#else
#define std_log1(what)
#define std_log2(what)
#define std_log3(what)
#define std_log4(what)
#endif //AXR_DEBUG_LEVEL > 0


//security brakes for while loops
#if AXR_DEBUG_LEVEL > 0

#define AXR_DEBUG_BRAKE 9999
#define security_brake_init() int __axr_security_count = 0;
#define security_brake_reset() __axr_security_count = 0;
#define security_brake() if(__axr_security_count > AXR_DEBUG_BRAKE ){ std_log1("################ error: security brake activated ####################"); break; } else { __axr_security_count++; }

#else // AXR_DEBUG_LEVEL == 0

#define security_brake_init()
#define security_brake_reset()
#define security_brake()

#endif //AXR_DEBUG_LEVEL == 0

//indentation for output
#if AXR_DEBUG_LEVEL > 0

unsigned extern axr_output_debug_indent_count;
#define inc_output_indent() axr_output_debug_indent_count++;
#define dec_output_indent() axr_output_debug_indent_count--;
#define output_indent(what) std::string(axr_output_debug_indent_count*4, ' ').append(what)

#else //AXR_DEBUG_LEVEL == 0

#define inc_output_indent()
#define dec_output_indent()
#define output_indent(what) std::string(what)

#endif //AXR_DEBUG_LEVEL == 0


#endif //AXRDEBUGGING_H
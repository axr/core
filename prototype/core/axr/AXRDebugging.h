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
#define AXR_DEBUG_LEVEL 1
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

//debug channels
#if AXR_DEBUG_LEVEL > 0

/**
 *  These are the available channels:
 *
 *  Bit layout:
 *  0000 0000  0000 0000  0000 0000  0000 0001
 *  fedc abZY  XWVU TSRQ  PONM LKJI  HGFE DCBA
 *  
 *  A:	AXR_DEBUG_CH_ON                 Debug on/off
 *  B:	AXR_DEBUG_CH_OVERVIEW           Basic high level actions
 *  C:	AXR_DEBUG_CH_GENERAL            General actions in the system
 *  D:	AXR_DEBUG_CH_GENERAL_SPECIFIC   General actions, more fine grained
 *  E:	AXR_DEBUG_CH_            
 *  F:	AXR_DEBUG_CH_           
 *  G:	AXR_DEBUG_CH_XML                XML parsing events
 *  H:	AXR_DEBUG_CH_           
 *  I:	AXR_DEBUG_CH_           
 *  J:	AXR_DEBUG_CH_           
 *  K:	AXR_DEBUG_CH_HSS                HSS parsing events
 *  L:	AXR_DEBUG_CH_           
 *  M:	AXR_DEBUG_CH_OBSERVING          Adding, removing and notifying of observers
 *  N:	AXR_DEBUG_CH_EVENTS             Prints messages about the event handling
 *  O:	AXR_DEBUG_CH_EVENTS_SPECIFIC    Event handling, more specific
 *  P:	AXR_DEBUG_CH_           
 *  Q:	AXR_DEBUG_CH_           
 *  R:	AXR_DEBUG_CH_           
 *  S:	AXR_DEBUG_CH_           
 *  T:	AXR_DEBUG_CH_           
 *  U:	AXR_DEBUG_CH_           
 *  V:	AXR_DEBUG_CH_           
 *  W:	AXR_DEBUG_CH_           
 *  X:	AXR_DEBUG_CH_           
 *  Y:	AXR_DEBUG_CH_           
 *  Z:	AXR_DEBUG_CH_           
 *  a:	AXR_DEBUG_CH_           
 *  b:	AXR_DEBUG_CH_           
 *  c:	AXR_DEBUG_CH_FULL_FILENAMES     Normally only the filename is shown, activate this for full paths.
 *  d:	AXR_DEBUG_CH_           
 *  e:	AXR_DEBUG_CH_           
 *  f:	AXR_DEBUG_CH_           
 *  
 */

unsigned extern axr_debug_active_channels;
enum axr_debug_ch {
    AXR_DEBUG_CH_ON  = 1, //A
    AXR_DEBUG_CH_OVERVIEW = 1 << 1, //B
    AXR_DEBUG_CH_GENERAL = 1 << 2, //C
    AXR_DEBUG_CH_GENERAL_SPECIFIC = 1 << 3, //D
     //E
     //F
    AXR_DEBUG_CH_XML  = 1 << 6, //G
     //H
     //I
     //J
    AXR_DEBUG_CH_HSS  = 1 << 10, //K
    
    AXR_DEBUG_CH_OBSERVING = 1 << 12, //M
    AXR_DEBUG_CH_EVENTS = 1 << 13, //N
    AXR_DEBUG_CH_EVENTS_SPECIFIC = 1 << 14, //O
    
    AXR_DEBUG_CH_FULL_FILENAMES = 1 << 16, //c
};

#define axr_log(AXR_DEBUG_CH, message) if((axr_debug_active_channels & AXR_DEBUG_CH_ON) && (axr_debug_active_channels & (AXR_DEBUG_CH))) std_log(output_indent(message))
#define axr_log_inline(AXR_DEBUG_CH, message) if((axr_debug_active_channels & AXR_DEBUG_CH_ON) && (axr_debug_active_channels & (AXR_DEBUG_CH))) std::cout << message
#define axr_debug_activate_channel(AXR_DEBUG_CH) axr_debug_active_channels = (axr_debug_active_channels | (AXR_DEBUG_CH))
#else
#define axr_log(what)
#define axr_debug_activate_channel(AXR_DEBUG_CH)
#endif //AXR_DEBUG_LEVEL > 0

#endif //AXRDEBUGGING_H

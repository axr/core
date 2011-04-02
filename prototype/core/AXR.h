/*
 *  AXR.h
 *  core
 *
 *  Created by Miro Keller on 14/02/11.
 *  Copyright 2011 Miro Keller. All rights reserved.
 *
 */

#ifndef AXR_H
#define AXR_H

#ifdef AXR_PLATFORM_OSX
#include "os/OSX.h"
#elif defined AXR_PLATFORM_WIN
#include "os/Windows.h"
#elif defined AXR_PLATFORM_GNU
#include "os/GNU.h"
#endif

#include "xml/XMLParser.h"
#include "hss/hss.h"
#include "AXRController.h"

//Debug levels:
// - 0 No debug
// - 1 
// - 2 
// - 3 
// - 4 All messages

//you can set this as a preprocessor macro in your IDE if you prefer
#ifndef AXR_DEBUG_LEVEL
//default level
#define AXR_DEBUG_LEVEL 1
#endif

//logging
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
#define security_brake() if(__axr_security_count > AXR_DEBUG_BRAKE ){ break; } else { __axr_security_count++; }

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
#define output_indent(what) string(axr_output_debug_indent_count*4, ' ').append(what)

#else //AXR_DEBUG_LEVEL == 0

#define inc_output_indent()
#define dec_output_indent()
#define output_indent(what)

#endif //AXR_DEBUG_LEVEL == 0

#endif //AXR_H
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

//#include "xml/AXRXmlParser.h"
#include "hss/hss.h"

//Debug levels:
// - 0 No debug
// - 1 
// - 2 
// - 3 
// - 4 All messages

#define AXR_DEBUG_LEVEL 2


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
#endif

#else
#define std_log3(what)
#define std_log4(what)
#endif

#else
#define std_log2(what)
#define std_log3(what)
#define std_log4(what)
#endif

#else
#define std_log1(what)
#define std_log2(what)
#define std_log3(what)
#define std_log4(what)
#endif


//security brakes for while loops

#if AXR_DEBUG_LEVEL > 0

#define AXR_DEBUG_BRAKE 9999
#define security_brake_init() int __axr_security_count = 0;
#define security_brake_reset() __axr_security_count = 0;
#define security_brake() if(__axr_security_count > AXR_DEBUG_BRAKE ){ break; } else { __axr_security_count++; }

#else
#define security_brake_init()
#define security_brake_reset()
#define security_brake()

#endif


#endif
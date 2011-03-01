/*
 *  AXR.h
 *  core
 *
 *  Created by Miro Keller on 14/02/11.
 *  Copyright 2011 Miro Keller. All rights reserved.
 *
 */

#ifndef AXR_h
#define AXR_h

#ifdef AXR_PLATFORM_OSX
#include "os/OSX.h"
#elif defined AXR_PLATFORM_WIN
#include "os/Windows.h"
#elif defined AXR_PLATFORM_GNU
#include "os/GNU.h"
#endif

#include "xml/AXRXmlParser.h"

#endif
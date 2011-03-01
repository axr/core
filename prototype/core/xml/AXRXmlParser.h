/*
 *  AXRXmlParser.h
 *  core
 *
 *  Created by Miro Keller on 14/02/11.
 *  Copyright 2011 Miro Keller. All rights reserved.
 *
 */

#ifndef AXRXmlParser_h
#define	AXRXmlParser_h

//FIXME: figure out how to distribute the expat library with the project source
#include <expat.h>

class AXRXmlParser
{
public:
	AXRXmlParser();
	void parse(const char *);
};

extern "C" {
	
	static void XMLCALL elementStartCallback(void *userData, const char *element, const char **attributes);
	static void XMLCALL elementEndCallback(void *userData, const char *element);
	static void XMLCALL elementContentTextCallback(void *data, const XML_Char *text, int len);
}




#endif
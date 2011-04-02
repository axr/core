/*
 *  AXRXmlParser.cpp
 *  core
 *
 *  Created by Miro Keller on 14/02/11.
 *  Copyright 2011 Miro Keller. All rights reserved.
 *
 */

#include "xml/XMLParser.h"
#include <expat.h>

#include <stdio.h>
#include <string.h>
extern "C" {
	int Depth;

	static void XMLCALL elementStartCallback(void *data, const char *el, const char **attr)
	{
		int i;
		
		for (i = 0; i < Depth; i++)
			printf("  ");
		printf("%s\n", el);
		
		//printf("%s", el);
		for (i = 0; attr[i]; i += 2) {
			printf(" [%s='%s] '", attr[i], attr[i + 1]);
		}
		Depth++;
	}

	static void XMLCALL elementContentTextCallback(void *data, const XML_Char *text, int len)
	{
		if(text != (const XML_Char *)'\n' && (len != 1)){
			int i;
			for (i = 0; i < Depth; i++)
				printf("  ");
			for (i = 0; i < len; i++){
				putchar(text[i]);
			}
		}
	}

	static void XMLCALL elementEndCallback(void *data, const char *el)
	{
		printf(" /%s\n", el);
		Depth--;
	}
}

AXRXmlParser::AXRXmlParser()
{
	
}

void AXRXmlParser::parse(const char *path)
{
	#define BUFFSIZE 8125
	char Buff[BUFFSIZE];
	
	FILE * xmlfile;
	
	XML_Parser p = XML_ParserCreate(NULL);
	if (! p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}
	
	XML_SetElementHandler(p, elementStartCallback, elementEndCallback);
	XML_SetCharacterDataHandler(p, elementContentTextCallback);
	
	xmlfile = fopen(path, "r");
	unsigned done = 0;
	while (!done){		
		int len;
		len = (int)fread(Buff, 1, BUFFSIZE, xmlfile);
		done = feof(xmlfile);
		
		if (XML_Parse(p, Buff, len , done) == XML_STATUS_ERROR) {
			fprintf(stderr, "Parse error at line %i u:\n%s\n", (int)XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
			exit(-1);
		}
		
		done = 1;
		
	}
	fclose(xmlfile);
	
	XML_ParserFree(p);
}
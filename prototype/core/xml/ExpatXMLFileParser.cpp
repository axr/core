/* 
 * ExpatMM - C++ Wrapper for Expat available at http://expat.sourceforge.net/
 * Copyright (c) 2007, 2008, 2009 IntelliTree Solutions llc
 * Author: Coleman Kane <ckane@intellitree.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, please feel free
 * to contact the author listed above.
 */
/*
 * IntelliTree ExpatMM C++ XML Parser
 * Copyright (C) 2006, 2007 IntelliTree Solutions llc
 * Written by Coleman Kane <ckane@intellitree.com>
 *
 * ExpatXMLFileParser - read from a file-on-disk and parse its XML
 *
 */
#include "config.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_EXPAT_H
#include <expat.h>
#endif
#ifdef HAVE_STRING
#include <string>
#endif
#include "ExpatXMLFileParser.h"

using namespace expatmm;

ExpatXMLFileParser::ExpatXMLFileParser(std::string filename) : ExpatXMLParser() {
	fhandle = fopen(filename.c_str(), "r");
	fname = filename;
	if(fhandle != NULL)
	  setReadiness(true);
	else
	  setReadiness(false);
}

ssize_t
ExpatXMLFileParser::read_block(void) {
	if(fhandle == NULL)
	  return -1;

	size_t sz = fread(getBuffer(), sizeof(getBuffer()[0]), getBlockSize(),
		fhandle);
	ssize_t code = (ssize_t)sz;

	if(sz < getBlockSize()) {
	  if(feof(fhandle)) {
		setLastError(XML_ERROR_FINISHED);
		return sz;
	  }
	  if(ferror(fhandle)) {
		setStatus(XML_STATUS_ERROR);
		setLastError(XML_ERROR_NO_ELEMENTS);
		fseek(fhandle, -sz, SEEK_CUR);
	  }
	}

	if(sz == 0) {
	  setStatus(XML_STATUS_OK);
	  setLastError(XML_ERROR_FINISHED);
	  code = -1;
	}
	return code;
}

ExpatXMLFileParser::~ExpatXMLFileParser(void) {
  fclose(fhandle);
  fname = "";
}

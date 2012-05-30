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
 * IntelliTree ExpatMM C++ Parser
 * Copyright (C) 2006, 2007 IntelliTree Solutions llc
 * Written by Coleman Kane <ckane@intellitree.com>
 *
 * Header File, further description below
 *
 */

 /*
  * File Modified by Miro Keller
  *
  */

#ifndef _EXPATMM_EXPATXMLPARSER_H
#define _EXPATMM_EXPATXMLPARSER_H
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <expat.h>
//#include "expatmm-libdef.h"

#define XML_CHUNK_SIZE 10240

namespace expatmm {
    /**
     *  @brief Expatmm C to C++ bridge for Expat.
     */
  class ExpatXMLParser {
      
    private:
	  void register_default_handlers(void);

	  static XMLCALL void _element_start_handler(void *userData,
		  const XML_Char *name, const XML_Char **atts);
	  static XMLCALL void _element_end_handler(void *userData,
		  const XML_Char *name);
	  static XMLCALL void _character_data_handler(void *userData,
		  const XML_Char *s, int len);
	  static XMLCALL void _processing_instr_handler(void *userData,
		  const XML_Char *target, const XML_Char *data);
	  static XMLCALL void _comment_handler(void *userData,
		  const XML_Char *data);
	  static XMLCALL void _default_handler(void *userData,
		  const XML_Char *s, int len);
	  static XMLCALL void _cdata_start_handler(void *userData);
	  static XMLCALL void _cdata_end_handler(void *userData);
	protected:
      /* The handle for the parser (expat) */
	  XML_Parser expat_parser;
      
	  /* Temporary buffer where data is streamed in */
	  //XML_Char *xml_buffer;
	  //size_t xml_buffer_size;
      
	  /* Tells if the parser is ready to accept data */
	  bool valid_parser;
      
	  /* Status and Error codes in the event of unforseen events */
	  XML_Status status;
	  XML_Error last_error;
      
	  /*
		 Override this to implement your container-specific parser.

		 You must:
		   put new XML data into xml_buffer 
		   set status
		   set last_error
		   return the amount of XML_Char's written to xml_buffer

		   on error, return < 0. The contents of xml_buffer will be
		   thrown away in this event, so it is the derived class's
		   responsibility to reseek the "data cursor" to re-get any
		   data in the buffer on an error condition.

		   Use setReadiness, setStatus, and setLastError to handle
		   error, status, and control events and codes. 
      */
      virtual XML_Char *getBuffer(void) = 0;
	  //virtual XML_Char *getBuffer(void) { return xml_buffer; };
      virtual size_t getBlockSize(void) = 0;
	  //virtual size_t getBlockSize(void) { return xml_buffer_size; };
	  virtual ssize_t read_block(void);
	  virtual void setReadiness(bool ready) { valid_parser = ready; };
	  virtual void setStatus(XML_Status new_status) { status = new_status; };
	  virtual void setLastError(XML_Error new_last_error) {
		last_error = new_last_error;
	  };

	  /* Called by the static handler methods to process the XML Document */
	  virtual void StartElement(const XML_Char *name, const XML_Char **atts);
	  virtual void EndElement(const XML_Char *name);
	  virtual void CharacterData(const XML_Char *s, int len);
	  virtual void ProcessingInstruction(const XML_Char *target,
		  const XML_Char *data);
	  virtual void CommentData(const XML_Char *data);
	  virtual void DefaultHandler(const XML_Char *s, int len);
	  virtual void CDataStart(void);
	  virtual void CDataEnd(void);

	public:
	  
	  /* Create a new parser, using the default Chunk Size */
	  ExpatXMLParser(void);

	  /* Create a new parser, using a user-supplied chunk size */
	  ExpatXMLParser(size_t chunk_size);

	  /* Destructor that cleans up xml_buffer and parser */
	  virtual ~ExpatXMLParser(void);

	  /* Attempts to parse an entire XML source */
	  virtual bool Parse(void);

	  /* Expose status, error, and control codes to users */
	  virtual bool Ready(void) { return valid_parser; };
	  virtual XML_Error getLastError(void) { return last_error; };
	  virtual XML_Status getStatus(void) { return status; };
  };
};

#endif /* _EXPATMM_EXPATXMLPARSER_H */

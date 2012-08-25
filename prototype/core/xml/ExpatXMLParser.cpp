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
 * ExpatXMLParser - A C++ implementation wrapper around Expat that
 * exposes the event-driven architecture through a series of virtual
 * methods in a deriveable class.
 *
 */

/*
 * File Modified by Miro Keller
 * last modified: 2012/01/03
 */
#include "ExpatXMLParser.h"

using namespace expatmm;

ExpatXMLParser::ExpatXMLParser(void) {
  /* Initialize the validity of this parser to OFF */
  valid_parser = false;
  expat_parser = NULL;

  /* Allocate a buffer for streaming in data */
    //MK: we don't need this since the buffers are stored in AXRFiles
    //xml_buffer_size = XML_CHUNK_SIZE;
    //xml_buffer = new XML_Char[xml_buffer_size];

    //if(xml_buffer == NULL)
    //return;

  /* Allocate a new parser state-object */
  expat_parser = XML_ParserCreate(NULL);

  if(expat_parser == NULL) {
      //delete xml_buffer;
      //xml_buffer = NULL;
    return;
  }

    //memset(xml_buffer, 0, XML_CHUNK_SIZE * sizeof(XML_Char));

  /* Set the "ready" flag on this parser */
  valid_parser = true;
  XML_SetUserData(expat_parser, (void*)this);
  register_default_handlers();
}

ExpatXMLParser::ExpatXMLParser(size_t chunk_size) {
  /* Initialize the validity of this parser to OFF */
  valid_parser = false;
  expat_parser = NULL;

  /* Allocate a buffer for streaming in data */
    //MK: we don't need this since the buffers are stored in AXRFiles
    //xml_buffer_size = chunk_size;
    //xml_buffer = new XML_Char[xml_buffer_size];

    //if(xml_buffer == NULL)
    //return;

  /* Allocate a new parser state-object */
  expat_parser = XML_ParserCreate(NULL);

  if(expat_parser == NULL) {
      //delete xml_buffer;
      //xml_buffer = NULL;
    return;
  }

    //memset(xml_buffer, 0, chunk_size * sizeof(XML_Char));

  /* Set the "ready" flag on this parser */
  valid_parser = true;
  XML_SetUserData(expat_parser, (void*)this);
  register_default_handlers();
}

ExpatXMLParser::~ExpatXMLParser(void) {
  valid_parser = false;

  if(expat_parser != NULL) {
    XML_ParserFree(expat_parser);
    expat_parser = NULL;
  }

    //if(xml_buffer != NULL) {
      //delete xml_buffer;
      //xml_buffer = NULL;
    //}
}

/*
   This function causes Expat to register this's default static handlers
   with the Expat events.
*/
void
ExpatXMLParser::register_default_handlers(void) {
  XML_SetElementHandler(expat_parser, &ExpatXMLParser::_element_start_handler,
      &ExpatXMLParser::_element_end_handler);
  XML_SetCharacterDataHandler(expat_parser,
      &ExpatXMLParser::_character_data_handler);
  XML_SetProcessingInstructionHandler(expat_parser,
      &ExpatXMLParser::_processing_instr_handler);
  XML_SetCommentHandler(expat_parser, &ExpatXMLParser::_comment_handler);
  XML_SetCdataSectionHandler(expat_parser,
      &ExpatXMLParser::_cdata_start_handler,
      &ExpatXMLParser::_cdata_end_handler);
  XML_SetDefaultHandler(expat_parser, &ExpatXMLParser::_default_handler);
}

/*
   "empty" read_block that returns "no elements" if it is ever called.
   This should be overriden by derived classes implementing their own
   XML containers.

   Derived classes are expected to override this with their own
   version that handles container I/O, setting error and status
   codes, filling xml_buffer, and returning the size of the data
   read into xml_buffer.
*/
ssize_t
ExpatXMLParser::read_block(void) {
  last_error = XML_ERROR_NO_ELEMENTS;
  status = XML_STATUS_ERROR;
  return -1;
}

/*
   Generic Parser. Most derivations will simply call this, rather
   than implement their own. This will loop, processing XML data
   and calling the necessary handler code until an error is encountered.
*/
bool
ExpatXMLParser::Parse(void) {
  ssize_t bytes_read;

  /* Ensure that the parser is ready */
  if(!Ready())
    return false;

  /* Loop, reading the XML source block by block */
  while((bytes_read = read_block()) > 0) { //MK: fixed a bug here that caused an infinite loop when the file was empty
      XML_Status local_status =
        XML_Parse(expat_parser, getBuffer(), bytes_read, XML_FALSE);

      if(local_status != XML_STATUS_OK) {
        status = local_status;
        last_error = XML_GetErrorCode(expat_parser);
        break;
      }

      /* Break on successful "short read", in event of EOF */
      if(getLastError() == XML_ERROR_FINISHED)
        break;
  }

  /* Finalize the parser */
  if((getStatus() == XML_STATUS_OK) || (getLastError() == XML_ERROR_FINISHED)) {
    XML_Parse(expat_parser, getBuffer(), 0, XML_TRUE);
    return true;
  }

  /* Return false in the event of an error. The parser is not finalized
     on error. */
  return false;
}


/*
   **** INTERNAL HANDLER FUNCTIONS *****
   The expatmm protocol is to pass (this) as the userData argument
   in the XML_Parser structure. These static methods will convert
   handlers into upcalls to the instantiated class's virtual members
   to do the actual handling work.
*/
void XMLCALL
ExpatXMLParser::_element_start_handler(void *userData, const XML_Char *name,
    const XML_Char **atts) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->StartElement(name, atts);
}

void XMLCALL
ExpatXMLParser::_element_end_handler(void *userData, const XML_Char *name) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->EndElement(name);
}

void XMLCALL
ExpatXMLParser::_character_data_handler(void *userData,
    const XML_Char *s, int len) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->CharacterData(s, len);
}

void XMLCALL
ExpatXMLParser::_processing_instr_handler(void *userData,
    const XML_Char *target, const XML_Char *data) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->ProcessingInstruction(target, data);
}

void XMLCALL
ExpatXMLParser::_comment_handler(void *userData, const XML_Char *data) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->CommentData(data);
}

void XMLCALL
ExpatXMLParser::_default_handler(void *userData, const XML_Char *s, int len) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->DefaultHandler(s, len);
}

void XMLCALL
ExpatXMLParser::_cdata_start_handler(void *userData) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->CDataStart();
}

void XMLCALL
ExpatXMLParser::_cdata_end_handler(void *userData) {
  ExpatXMLParser *me = (ExpatXMLParser*)userData;

  if(me != NULL)
    me->CDataEnd();
}

void
ExpatXMLParser::StartElement(const XML_Char *name, const XML_Char **atts) {
  printf("[XML]Start:\t%s\n", name);
}

void
ExpatXMLParser::EndElement(const XML_Char *name) {
  printf("[XML]End:\t%s\n", name);
}

void
ExpatXMLParser::CharacterData(const XML_Char *s, int len) {

}

void
ExpatXMLParser::ProcessingInstruction(const XML_Char *target,
    const XML_Char *data) {

}

void
ExpatXMLParser::CommentData(const XML_Char *data) {

}

void
ExpatXMLParser::DefaultHandler(const XML_Char *s, int len) {

}

void
ExpatXMLParser::CDataStart(void) {

}

void
ExpatXMLParser::CDataEnd(void) {

}

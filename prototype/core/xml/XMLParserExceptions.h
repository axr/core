//
//  XMLParserExceptions.h
//  mac
//
//  Created by Miro Keller on 02/04/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef AXR_XMLPARSEREXCEPTIONS_H
#define AXR_XMLPARSEREXCEPTIONS_H


#include <exception>
#include <string>

namespace AXR {
    class XMLParserException
    {
    public:
        XMLParserException(std::string filename, int line, int column);
        
        std::string filename;
        int line;
        int column;
        virtual std::string toString();
    };
    
    class XMLUnexpectedEndOfSourceException : public XMLParserException
    {
    public:
        XMLUnexpectedEndOfSourceException(std::string filename, int line, int column);
        virtual std::string toString();
    };
    
    class XMLNoElementsException : public XMLParserException
    {
    public:
        XMLNoElementsException(std::string filename, int line, int column);
        virtual std::string toString();
    };
}


#endif //AXR_XMLPARSEREXCEPTIONS_H
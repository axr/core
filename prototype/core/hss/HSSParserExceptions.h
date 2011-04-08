/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MMMMMD                                                         
 *            :: VD
 *           ::  .                                                         
 *          ::                                                              
 *          M   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       .MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      AMMMMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      MM  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *      MM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *     :MM AMMMMMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *    :: MMMMMMMMMM    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *   .     MMMMM~     MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         MMMMM      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        MMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       MMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      MMMM MMMM                                                           
 *     AMMM  .MMM                                         
 *     MMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     MMM    MMA       ====================================                              
 *     DMN    MM                               
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
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSPARSEREXCEPTIONS_H
#define HSSPARSEREXCEPTIONS_H


#include <exception>
#include <string>
#include "HSSToken.h"

using namespace std;

class HSSParserException
{
public:
    HSSParserException(string filename, int line, int column);
    
    string filename;
    int line;
    int column;
    virtual string toString();
};


class HSSUnexpectedEndOfSourceException : public HSSParserException
{
public:
    HSSUnexpectedEndOfSourceException(string filename, int line, int column);
    virtual string toString();
};

class HSSExpectedTokenException : public HSSParserException
{
public:
    HSSExpectedTokenException(HSSTokenType type, string filename, int line, int column);
    HSSExpectedTokenException(HSSTokenType type, string value, string filename, int line, int column);
    virtual string toString();
    HSSTokenType type;
    string value;
};

class HSSUnexpectedTokenException : public HSSParserException
{
public:
    HSSUnexpectedTokenException(HSSTokenType type, string filename, int line, int column);
    HSSUnexpectedTokenException(HSSTokenType type, string value, string filename, int line, int column);
    virtual string toString();
    HSSTokenType type;
    string value;
};

class HSSUnexpectedObjectTypeException : public HSSParserException
{
public:
    HSSUnexpectedObjectTypeException(string type, string filename, int line, int column);
    virtual string toString();
    string type;
};

class HSSWrongHexLengthException : public HSSParserException
{
public:
    HSSWrongHexLengthException(int length, string filename, int line, int column);
    virtual string toString();
    int length;
};

#endif
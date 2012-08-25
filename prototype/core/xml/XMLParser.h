/********************************************************************
 *             a  A
 *            AM\/MA
 *           (MA:MMD
 *            :: VD
 *           ::  º
 *          ::
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM
 *      TMMM MMMM
 *     TMMM  .MMM
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING
 *     TMM    MMA       ====================================
 *     TMN    MM
 *      MN    ZM
 *            MM,
 *
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
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
 ********************************************************************/

#ifndef AXR_XMLPARSER_H
#define AXR_XMLPARSER_H

#include <string>
#include <sys/types.h>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <expat.h>
#include "AXRWrapper.h"
#include "ExpatXMLParser.h"
#include "HSSContainer.h"

namespace AXR {


    class AXRController;

    class XMLParser : public expatmm::ExpatXMLParser {
    public:

        typedef boost::shared_ptr<XMLParser> p;
        typedef boost::function<void (std::string)> addSheetCallback;
        typedef boost::function<void (HSSContainer::p)> addContainerCallback;

        XMLParser(AXRController * theController);
        //the caller is responsible for maintaining the existence of the controller
        //until after the parser is deallocated
        //XMLParser(AXRController * controller, std::string filepath, std::string filename);
        virtual ~XMLParser();

        bool loadFile(AXRFile::p file);

        std::string getFilePath();
        std::string getFileName();

        //weak ptr
        AXRController * controller;

    protected:
        virtual XML_Char *getBuffer(void);
        virtual size_t getBlockSize(void);
        virtual ssize_t read_block(void);
        virtual void StartElement(const XML_Char *name, const XML_Char **attrs);
        virtual void EndElement(const XML_Char *name);
        virtual void ProcessingInstruction(const XML_Char *target, const XML_Char *data);
        virtual void CharacterData(const XML_Char *s, int len);

    private:
        AXRFile::p file;

        XML_Char currentPIChar;
        void readPIChar(XML_Char nextChar);
        void readNextPIAttr();
    };
}


#endif

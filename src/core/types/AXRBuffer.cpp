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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include "axr.h"
#include "AXRBuffer.h"

using namespace AXR;


AXRBuffer::AXRBuffer(const HSSString & buffer, const HSSString & sourceUrl)
: _buffer(buffer)
, _sourceUrl(sourceUrl)
, _valid(true)
{
    
}

AXRBuffer::AXRBuffer(AXRDocument * document, const HSSString & filePath)
: _sourceUrl()
, _valid(true)
{
    document->platform()->readFile(filePath, &this->_buffer, &this->_sourceUrl, &this->_valid);
}

AXRBuffer::AXRBuffer(HSSString buffer)
: _buffer(buffer)
, _sourceUrl()
, _valid(true)
{
    
}

AXRBuffer::~AXRBuffer()
{
}

AXRString AXRBuffer::sourceUrl() const
{
    return this->_sourceUrl;
}

AXRString AXRBuffer::getFileName() const
{
    std::string val = this->_sourceUrl.data();
    return HSSString(val.substr(val.find_last_of("/")+1));
}

HSSString & AXRBuffer::getBuffer()
{
    return this->_buffer;
}

bool AXRBuffer::isValid() const
{
    return this->_valid;
}

AXRString AXRBuffer::toString() const
{
    AXRString str = "AXRBuffer: with URL: ";
    str += this->_sourceUrl;

    return str;
}

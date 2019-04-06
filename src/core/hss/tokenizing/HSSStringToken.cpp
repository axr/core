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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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
#include "HSSStringToken.h"

using namespace AXR;

HSSStringToken::HSSStringToken(HSSTokenType type, qint64 line, qint64 column)
: HSSToken(type, line, column)
, _hasArguments(false)
{
    this->type = type;
}

HSSStringToken::~HSSStringToken()
{
}

void HSSStringToken::setValue(AXRString newValue)
{
    this->stringValue = newValue;
}

AXRString HSSStringToken::getString()
{
    return this->stringValue;
}

bool HSSStringToken::equals(HSSTokenType otherType, AXRString otherValue)
{
    return otherType == this->type && otherValue == this->stringValue;
}

AXRString HSSStringToken::toString()
{
    HSSString tokenstr = this->tokenStringRepresentation(this->type);
    return "HSSStringToken of type: " + tokenstr + " and value: " + this->stringValue;
}

void HSSStringToken::setHasArguments(bool newValue)
{
    this->_hasArguments = newValue;
}

bool HSSStringToken::hasArguments() const
{
    return this->_hasArguments;
}

void HSSStringToken::addArgument(QSharedPointer<AXR::HSSParserNode> parserNode)
{
    this->_arguments.push_back(parserNode);
}

const std::vector<QSharedPointer<HSSParserNode> > & HSSStringToken::getArguments() const
{
    return this->_arguments;
}

void HSSStringToken::addIndex(size_t index)
{
    this->_indexes.push_back(index);
}

const std::vector<size_t> & HSSStringToken::getIndexes() const
{
    return this->_indexes;
}

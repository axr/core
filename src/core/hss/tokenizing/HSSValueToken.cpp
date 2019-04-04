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
#include "HSSValueToken.h"

using namespace AXR;

HSSValueToken::HSSValueToken(HSSTokenType type, AXRString value, qint64 line, qint64 column)
: HSSToken(type, line, column)
{
    this->type = type;
    if (this->isNumeric())
    {
        this->longValue = value.toDouble();
    }
    else
    {
        this->stringValue = value;
    }
}

HSSValueToken::HSSValueToken(HSSTokenType type, char value, qint64 line, qint64 column)
: HSSToken(type, line, column)
{
    //AXRString tempstr (1, value);
    this->type = type;
    this->stringValue = AXRString(1, value);
}

HSSValueToken::HSSValueToken(HSSTokenType type, HSSUnit value, qint64 line, qint64 column)
: HSSToken(type, line, column)
{
    this->type = type;
    this->longValue = value;
}

HSSValueToken::~HSSValueToken()
{
}

AXRString HSSValueToken::getString()
{
    return this->stringValue;
}

HSSUnit HSSValueToken::getLong()
{
    return this->longValue;
}

bool HSSValueToken::equals(HSSTokenType otherType, AXRString otherValue)
{
    return otherType == this->type && otherValue == this->stringValue;
}

bool HSSValueToken::equals(HSSTokenType otherType, HSSUnit otherValue)
{
    return otherType == this->type && otherValue == this->longValue;
}

AXRString HSSValueToken::toString()
{
    AXRString tokenstr = this->tokenStringRepresentation(this->type);
    if (this->isNumeric())
    {
        return AXRString("HSSValueToken of type: %1 and value: %2").arg(tokenstr).arg(this->longValue);
    }
    else
    {
        return "HSSValueToken of type: " + tokenstr + " and value: " + this->stringValue;
    }
}

bool HSSValueToken::isNumeric()
{
    return this->type == HSSNumber || this->type == HSSPercentageNumber;
}

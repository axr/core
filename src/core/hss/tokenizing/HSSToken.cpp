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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
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

#include <QMap>
#include "HSSToken.h"

using namespace AXR;

HSSToken::HSSToken(HSSTokenType type, unsigned line, unsigned column)
{
    this->type = type;
    this->line = line;
    this->column = column;
}

HSSToken::~HSSToken()
{
}

bool HSSToken::isA(HSSTokenType otherType)
{
    return otherType == this->type;
}

HSSTokenType HSSToken::getType()
{
    return this->type;
}

AXRString HSSToken::toString()
{
    AXRString tokenstr = HSSToken::tokenStringRepresentation(this->type);
    return "HSSToken of type: " + tokenstr;
}

AXRString HSSToken::tokenStringRepresentation(HSSTokenType type)
{
    static QMap<HSSTokenType, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSNone] = "HSSNone";
        types[HSSIdentifier] = "HSSIdentifier";
        types[HSSNumber] = "HSSNumber";
        types[HSSPercentageNumber] = "HSSPercentageNumber";
        types[HSSHexNumber] = "HSSHexNumber";
        types[HSSDoubleQuoteString] = "HSSDoubleQuoteString";
        types[HSSSingleQuoteString] = "HSSSingleQuoteString";
        types[HSSSymbol] = "HSSSymbol";
        types[HSSWhitespace] = "HSSWhitespace";
        types[HSSInstructionSign] = "HSSInstructionSign";
        types[HSSObjectSign] = "HSSObjectSign";
        types[HSSBlockOpen] = "HSSBlockOpen";
        types[HSSBlockClose] = "HSSBlockClose";
        types[HSSComma] = "HSSComma";
        types[HSSColon] = "HSSColon";
        types[HSSEndOfStatement] = "HSSEndOfStatement";
        types[HSSParenthesisOpen] = "HSSParenthesisOpen";
        types[HSSParenthesisClose] = "HSSParenthesisClose";
        types[HSSLineComment] = "HSSLineComment";
        types[HSSBlockComment] = "HSSBlockComment";
    }

    return types[type];
}

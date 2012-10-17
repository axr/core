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

#ifndef HSSTOKEN_H
#define HSSTOKEN_H

#include <QSharedPointer>
#include "AXRString.h"
#include "AXRGlobal.h"

namespace AXR
{
    enum AXR_API HSSTokenType
    {
        HSSNone = 0,
        HSSIdentifier,
        HSSNumber,
        HSSPercentageNumber,
        HSSHexNumber,
        HSSDoubleQuoteString,
        HSSSingleQuoteString,
        HSSSymbol,
        HSSWhitespace,
        HSSInstructionSign,
        HSSObjectSign,
        HSSBlockOpen,
        HSSBlockClose,
        HSSComma,
        HSSColon,
        HSSEndOfStatement,
        HSSParenthesisOpen,
        HSSParenthesisClose,
        HSSLineComment,
        HSSBlockComment,
        HSSNegator,
        HSSAmpersand
    };

    class AXR_API HSSToken
    {
    public:
        typedef QSharedPointer<HSSToken> p;

        static AXRString tokenStringRepresentation(HSSTokenType type);

        HSSToken(HSSTokenType type, unsigned line, unsigned column);
        virtual ~HSSToken();
        bool isA(HSSTokenType otherType);
        HSSTokenType getType();
        virtual AXRString toString();

        unsigned line;
        unsigned column;

    protected:
        HSSTokenType type;
    };
}

#endif

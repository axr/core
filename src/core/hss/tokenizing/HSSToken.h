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

#ifndef HSSTOKEN_H
#define HSSTOKEN_H

#include "HSSTokenType.h"

namespace AXR
{
    class AXR_API HSSToken
    {
    public:
        static HSSString tokenStringRepresentation(HSSTokenType type);

        HSSToken(HSSTokenType type, long long line, long long column);
        HSSToken(HSSTokenType type, HSSString value, long long line, long long column);
        HSSToken(HSSTokenType type, HSSUnit value, long long line, long long column);
        virtual ~HSSToken();
        bool isA(HSSTokenType otherType) const;
        HSSTokenType getType() const;
        virtual HSSString toString();

        long long line;
        long long column;
        
        HSSString getString();
        void setString(HSSString newValue);

        HSSUnit getNumber();
        bool equals(HSSTokenType otherType, HSSString otherValue);
        bool equals(HSSTokenType otherType, HSSUnit otherValue);
        bool isNumeric();

    protected:
        HSSTokenType type;
        HSSString _value;
        HSSUnit _number;
        
    };
}

#endif

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

#ifndef HSSVALUETOKEN_H
#define HSSVALUETOKEN_H

#include <boost/shared_ptr.hpp>
#include "AXRGlobal.h"
#include "HSSToken.h"

#define VALUE_TOKEN(thetoken) (boost::static_pointer_cast<HSSValueToken>(thetoken))

namespace AXR
{
    class AXR_API HSSValueToken : public HSSToken
    {
    public:
        typedef boost::shared_ptr<HSSValueToken> p;

        HSSValueToken(HSSTokenType type, AXRString value, unsigned line, unsigned column);
        HSSValueToken(HSSTokenType type, char value, unsigned line, unsigned column);
        HSSValueToken(HSSTokenType type, double long value, unsigned line, unsigned column);
        virtual ~HSSValueToken();
        AXRString getString();
        double long getLong();
        bool equals(HSSTokenType otherType, AXRString otherValue);
        bool equals(HSSTokenType otherType, double long otherValue);
        AXRString toString();
        bool isNumeric();

    private:
        AXRString stringValue;
        double long longValue;
    };
}

#endif

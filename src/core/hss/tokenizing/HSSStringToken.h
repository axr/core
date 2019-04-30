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

#ifndef HSSSTRINGTOKEN_H
#define HSSSTRINGTOKEN_H

#include "AXRGlobal.h"
#include "HSSToken.h"

namespace AXR
{
    class HSSParserNode;

    class AXR_API HSSStringToken : public HSSToken
    {
    public:
        HSSStringToken(HSSTokenType type, qint64 line, qint64 column);
        virtual ~HSSStringToken();
        void setValue(AXRString newValue);

        bool equals(HSSTokenType otherType, AXRString otherValue);
        AXRString toString();
        
        void setHasArguments(bool newValue);
        bool hasArguments() const;
        
        void addArgument(QSharedPointer<HSSParserNode> parserNode);
        const std::vector<QSharedPointer<HSSParserNode> > & getArguments() const;
        void addIndex(size_t index);
        const std::vector<size_t> & getIndexes() const;

    private:
        bool _hasArguments;
        std::vector<QSharedPointer<HSSParserNode> > _arguments;
        std::vector<size_t> _indexes;
    };
}

#endif

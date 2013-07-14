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

#ifndef HSSLAYOUT_H
#define HSSLAYOUT_H

#include "HSSAbstractVisitor.h"
#include "HSSTypeEnums.h"

template <class T> class QSharedPointer;

namespace AXR
{
    class HSSContainer;
    class HSSLayoutLine;
    class HSSObject;
    class HSSParserNode;
    class HSSTextBlock;
    
    class AXR_API HSSLayout : public HSSAbstractVisitor
    {
        Q_DISABLE_COPY(HSSLayout)
    public:
        HSSLayout();
        virtual ~HSSLayout();
        
        virtual void initializeVisit();
        virtual void visit(HSSContainer &container);
        virtual void visit(HSSTextBlock &textBlock);
        virtual void finalizeVisit();
        
        void setDocument(AXRDocument* document);
        
    private:
        class Private;
        Private *d;
        
        inline bool _overlaps(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const;
        inline bool _overlaps_horizontal(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const;
        inline bool _overlaps_vertical(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const;
        inline void _placeOnAlignmentPoint(const HSSContainer & container, const QSharedPointer<HSSDisplayObject> & child);
        inline bool _layoutTick() const;
        inline void _setGlobalPosition(const HSSContainer & container, const QSharedPointer<HSSDisplayObject> & child) const;
        inline const QSharedPointer<HSSLayoutLine> _getTargetGroup(const QSharedPointer<HSSDisplayObject> & child, std::vector<QSharedPointer<HSSLayoutLine> > & groups) const;
        inline void _setPositions(const HSSContainer & container, const QSharedPointer<HSSLayoutLine> & group) const;
        inline void _lineArrangeX(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startX, HSSUnit lineWidth) const;
        inline void _lineArrangeY(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startY, HSSUnit lineHeight) const;
        inline bool _arrangeLines(const HSSContainer & container, const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const;
        inline bool _distributeLines(const HSSContainer & container, const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const;
        inline HSSUnit _getLineHeight(const QSharedPointer<HSSLayoutLine> & line) const;
    };
}

#endif
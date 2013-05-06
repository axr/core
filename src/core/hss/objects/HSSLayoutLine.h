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

#ifndef HSSLAYOUTLINE_H
#define HSSLAYOUTLINE_H

#include "HSSClonable.h"
#include "HSSObservable.h"

namespace AXR
{
    class HSSDisplayObject;
    class HSSSimpleSelection;
    class HSSContainer;

    /**
     *  @brief Layout lines are used by HSSContainer to lay out elements according
     *  to the layout algorithm. They represent a set of elements that are flowing
     *  together in a direction. They also contain sublines, each of whose is also
     *  a layout line. When there are lines, the objects vector should not be used.
     */
    class AXR_API HSSLayoutLine : public HSSObservable, public HSSClonable
    {
    public:
        HSSUnit x;
        HSSUnit y;
        HSSUnit width;
        HSSUnit height;
        HSSDirectionValue direction;
        HSSContainer * owner;
        AXRString name;
        bool complete;
        std::vector<QSharedPointer<HSSLayoutLine> >lines;
        HSSSimpleSelection * objects;

        HSSLayoutLine(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, HSSDirectionValue direction, HSSContainer * owner);
        HSSLayoutLine(const QSharedPointer<HSSDisplayObject> & child, HSSDirectionValue direction, HSSContainer * owner);
        HSSLayoutLine(const HSSLayoutLine & orig);
        QSharedPointer<HSSLayoutLine> clone() const;
        virtual ~HSSLayoutLine();
        static bool heightGreater(const QSharedPointer<HSSLayoutLine> & x, const QSharedPointer<HSSLayoutLine> & y);
        void convertObjectsIntoLines();
        void mergeWith(const QSharedPointer<HSSLayoutLine> & groupB);
        void add(const QSharedPointer<HSSDisplayObject> & child);
        void arrange();
        void distribute();
        HSSUnit getAlignX();
        HSSUnit getAlignY();

        inline bool overlaps(const QSharedPointer<HSSLayoutLine> & groupB)
        {
            if (this->overlapsX(groupB) && this->overlapsY(groupB))
            {
                return true;
            }
            return false;
        }
        inline bool overlapsX(const QSharedPointer<HSSLayoutLine> & groupB)
        {
            if (
                (this->x + this->width) > groupB->x
                && (this->x) < (groupB->x + groupB->width)
                )
            {
                return true;
            }
            return false;
        }
        inline bool overlapsY(const QSharedPointer<HSSLayoutLine> & groupB)
        {
            if (
                (this->y + this->height) > groupB->y
                && (this->y) < (groupB->y + groupB->height)
                )
            {
                return true;
            }
            return false;
        }
        inline bool overlapsX(const QSharedPointer<HSSDisplayObject> & child)
        {
            if (
                (this->x + this->width) > (child->x - child->leftMargin)
                && (this->x) < (child->x + child->outerWidth)
                )
            {
                return true;
            }
            return false;
        }
        inline bool overlapsY(const QSharedPointer<HSSDisplayObject> & child)
        {
            if (
                (this->y + this->height) > (child->y - child->topMargin)
                && (this->y) < (child->y + child->outerHeight)
                )
            {
                return true;
            }
            return false;
        }

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        inline HSSUnit _clamp(const HSSUnit & value, const HSSUnit & min, const HSSUnit & max) const;
    };
}
#endif /* defined(HSSLAYOUTLINE_H) */

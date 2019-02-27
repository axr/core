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

#ifndef HSSSELECTOR_H
#define HSSSELECTOR_H

#include "HSSParserNode.h"

namespace AXR
{
    class HSSSelection;
    class HSSSimpleSelection;

    /**
     *  @brief Abstract base class for a common interface for all selector nodes
     */
    class AXR_API HSSSelector : public HSSParserNode
    {
    public:
        ~HSSSelector();

        /**
         * Reduces the selection according its selector type
         */
        virtual QSharedPointer<HSSSelection> filterSelection(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications) = 0;

        bool getNegating() const;
        void setNegating(bool value);

        bool isA(HSSSelectorType otherType) const;
        HSSSelectorType getSelectorType() const;
        void setSelectorType(HSSSelectorType newType);

        bool isA(HSSCombinatorType otherType) const;

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        const void observeForContentTreeChanges(QSharedPointer<HSSSelection> scope, bool getParents);
        void contentTreeChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj);

    protected:
        /**
         *  Creates a new instance of a simple selector, for use of the subclasses.
         *  @param type     The type of the selector node.
         */
        HSSSelector(HSSSelectorType type, AXRController * controller);

    private:
        HSSSelectorType _selectorType;
        bool _negating;
    };
}

#endif

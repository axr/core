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

#ifndef HSSSELECTOR_H
#define HSSSELECTOR_H

#include "HSSDisplayObject.h"
#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief Abstract base class for a common interface for all selector nodes
     */
    class HSSSelector : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSSelector> p;

        ~HSSSelector();

        /**
         * Reduces the selection according its selector type
         */
        virtual std::vector<HSSDisplayObject::p> filterSelection(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing) = 0;

        bool getNegating() const;
        void setNegating(bool value);

        bool isA(HSSSelectorType otherType);
        HSSSelectorType getSelectorType();

        bool isA(HSSCombinatorType otherType);

    protected:
        /**
         *  Creates a new instance of a simple selector, for use of the subclasses.
         *  @param type     The type of the selector node.
         */
        HSSSelector(HSSSelectorType type);

    private:
        HSSSelectorType _selectorType;
        bool _negating;
    };
}

#endif

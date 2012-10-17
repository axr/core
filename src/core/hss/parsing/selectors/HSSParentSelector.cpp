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

#include <set>
#include "AXRInitializer.h"
#include "HSSParentSelector.h"

using namespace AXR;

HSSParentSelector::HSSParentSelector()
: HSSNameSelector("@parent")
{

}

HSSParentSelector::p HSSParentSelector::clone() const
{
    return qSharedPointerCast<HSSParentSelector, HSSClonable > (this->cloneImpl());
}

AXRString HSSParentSelector::toString()
{
    return "@parent selector";
}

HSSClonable::p HSSParentSelector::cloneImpl() const
{
    return HSSClonable::p(new HSSParentSelector(*this));
}

AXRString HSSParentSelector::getElementName()
{
    return "@parent";
}

std::vector<HSSDisplayObject::p> HSSParentSelector::filterSelection(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing)
{
    std::vector<HSSDisplayObject::p> ret;
    std::vector<HSSDisplayObject::p>::const_iterator scopeIt;
    std::vector<HSSDisplayObject::p>::iterator readIt, writeIt;
    std::set<HSSDisplayObject::p> tmpset;

    for (scopeIt = scope.begin(); scopeIt != scope.end(); ++scopeIt)
    {
        const HSSDisplayObject::p & theDO = *scopeIt;
        const HSSDisplayObject::p & parent = theDO->getParent();
        if (parent)
            ret.push_back(parent);
    }

    for (readIt = ret.begin(), writeIt = ret.begin(); readIt != ret.end(); ++readIt)
    {
        if (tmpset.insert(*readIt).second)
        {
            *writeIt++ = *readIt;
        }
    }

    ret.erase(writeIt, ret.end());

    return ret;
}

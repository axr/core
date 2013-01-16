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
#include "AXRDocument.h"
#include "HSSParentSelector.h"

using namespace AXR;

HSSParentSelector::HSSParentSelector(AXRController * controller)
: HSSNameSelector("@parent", controller)
{

}

HSSParentSelector::p HSSParentSelector::clone() const
{
    return qSharedPointerCast<HSSParentSelector> (this->cloneImpl());
}

AXRString HSSParentSelector::toString()
{
    return "@parent selector";
}

HSSClonable::p HSSParentSelector::cloneImpl() const
{
    return HSSParentSelector::p(new HSSParentSelector(*this));
}

AXRString HSSParentSelector::getElementName()
{
    return "@parent";
}

HSSSelection::p HSSParentSelector::filterSelection(HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    HSSSimpleSelection::p ret(new HSSSimpleSelection());
    if (scope->isA(HSSSelectionTypeMultipleSelection))
    {
        HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(scope);
        for (HSSMultipleSelection::const_iterator it = multiSel->begin(); it != multiSel->end(); ++it)
        {
            this->_filterSimpleSelection(ret, *it);
        }
    }
    else if (scope->isA(HSSSelectionTypeSimpleSelection))
    {
        HSSSimpleSelection::p simpleSel = qSharedPointerCast<HSSSimpleSelection>(scope);
        this->_filterSimpleSelection(ret, simpleSel);
    }

    return ret;
}

inline void HSSParentSelector::_filterSimpleSelection(HSSSimpleSelection::p & ret, HSSSimpleSelection::p selection)
{
    HSSSimpleSelection::const_iterator scopeIt;
    HSSSimpleSelection::iterator readIt, writeIt;
    std::set<HSSDisplayObject::p> tmpset;

    for (scopeIt = selection->begin(); scopeIt != selection->end(); ++scopeIt)
    {
        const HSSDisplayObject::p & theDO = *scopeIt;
        const HSSDisplayObject::p & parent = theDO->getParent();
        if (parent)
            ret->add(parent);
    }

    for (readIt = ret->begin(), writeIt = ret->begin(); readIt != ret->end(); ++readIt)
    {
        if (tmpset.insert(*readIt).second)
        {
            *writeIt++ = *readIt;
        }
    }

    ret->erase(writeIt, ret->end());
}

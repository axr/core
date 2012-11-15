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

#include "HSSSimpleSelector.h"

using namespace AXR;

HSSSimpleSelector::HSSSimpleSelector(AXRController * controller)
: HSSSelector(HSSSelectorTypeSimpleSelector, controller)
{

}

HSSSimpleSelector::HSSSimpleSelector(const HSSSimpleSelector &orig)
: HSSSelector(orig)
{

}

HSSSimpleSelector::p HSSSimpleSelector::clone() const
{
    return qSharedPointerCast<HSSSimpleSelector> (this->cloneImpl());
}

AXRString HSSSimpleSelector::toString()
{
    return "Simple SimpleSelector";
}

HSSClonable::p HSSSimpleSelector::cloneImpl() const
{
    HSSSimpleSelector::p clone = HSSSimpleSelector::p(new HSSSimpleSelector(*this));
    clone->setName(this->name->clone());

    for (std::list<HSSFilter::p>::const_iterator it = this->filters.begin(); it != this->filters.end(); ++it)
    {
        const HSSFilter::p & flt = (*it);
        clone->filtersAdd(flt->clone());
    }

    return clone;
}

const std::list<HSSFilter::p> HSSSimpleSelector::getFilters() const
{
    return this->filters;
}

void HSSSimpleSelector::filtersAdd(HSSFilter::p filter)
{
    this->filters.push_back(filter);
    filter->setParentNode(this->shared_from_this());
}

HSSSelection::p HSSSimpleSelector::filterSelection(HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    HSSSelection::p selection;
    if (this->name)
    {
        selection = this->name->filterSelection(scope, thisObj, processing);
    }

    std::list<HSSFilter::p>::const_iterator it = this->filters.begin();
    return _recursiveFilterSelection(selection, it, this->filters.end(), processing);
}

HSSSelection::p HSSSimpleSelector::_recursiveFilterSelection(HSSSelection::p selection, std::list<HSSFilter::p>::const_iterator it, std::list<HSSFilter::p>::const_iterator end_it, bool processing)
{
    if (it == end_it)
    {
        return selection;
    }
    else
    {
        HSSSelection::p tempSel = (*it)->apply(selection, processing);
        ++it;
        return this->_recursiveFilterSelection(tempSel, it, end_it, processing);
    }
}

const HSSNameSelector::p HSSSimpleSelector::getName() const
{
    return this->name;
}

void HSSSimpleSelector::setName(HSSNameSelector::p name)
{
    this->name = name;
    name->setParentNode(this->shared_from_this());
}

void HSSSimpleSelector::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->name->setThisObj(value);

    for (std::list<HSSFilter::p>::iterator it = this->filters.begin(); it != this->filters.end(); ++it)
    {
        (*it)->setThisObj(value);
    }

    HSSSelector::setThisObj(value);
}

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

#include "HSSDisplayObject.h"
#include "HSSFilter.h"
#include "HSSNameSelector.h"
#include "HSSSelection.h"
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

QSharedPointer<HSSSimpleSelector> HSSSimpleSelector::clone() const
{
    return qSharedPointerCast<HSSSimpleSelector> (this->cloneImpl());
}

AXRString HSSSimpleSelector::toString()
{
    return "Simple SimpleSelector";
}

AXRString HSSSimpleSelector::stringRep()
{
    AXRString ret;
    ret.append(this->name->stringRep());
    foreach(QSharedPointer<HSSFilter> filter, this->filters)
    {
        ret.append(filter->stringRep());
    }
    return ret;
}

QSharedPointer<HSSClonable> HSSSimpleSelector::cloneImpl() const
{
    QSharedPointer<HSSSimpleSelector> clone = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(*this));
    clone->setName(this->name->clone());

    for (std::list<QSharedPointer<HSSFilter> >::const_iterator it = this->filters.begin(); it != this->filters.end(); ++it)
    {
        const QSharedPointer<HSSFilter> & flt = (*it);
        clone->filtersAdd(flt->clone());
    }

    return clone;
}

const std::list<QSharedPointer<HSSFilter> > HSSSimpleSelector::getFilters() const
{
    return this->filters;
}

void HSSSimpleSelector::filtersAdd(QSharedPointer<HSSFilter> filter)
{
    this->filters.push_back(filter);
    filter->setParentNode(this->shared_from_this());
}

QSharedPointer<HSSSelection> HSSSimpleSelector::filterSelection(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    QSharedPointer<HSSSelection> selection;
    if (this->name)
    {
        selection = this->name->filterSelection(scope, thisObj, processing);
    }

    std::list<QSharedPointer<HSSFilter> >::const_iterator it = this->filters.begin();
    return _recursiveFilterSelection(selection, it, this->filters.end(), processing);
}

QSharedPointer<HSSSelection> HSSSimpleSelector::_recursiveFilterSelection(QSharedPointer<HSSSelection> selection, std::list<QSharedPointer<HSSFilter> >::const_iterator it, std::list<QSharedPointer<HSSFilter> >::const_iterator end_it, bool processing)
{
    if (it == end_it)
    {
        return selection;
    }
    else
    {
        QSharedPointer<HSSSelection> tempSel = (*it)->apply(selection, processing);
        ++it;
        return this->_recursiveFilterSelection(tempSel, it, end_it, processing);
    }
}

const QSharedPointer<HSSNameSelector> HSSSimpleSelector::getName() const
{
    return this->name;
}

void HSSSimpleSelector::setName(QSharedPointer<HSSNameSelector> name)
{
    this->name = name;
    name->setParentNode(this->shared_from_this());
}

void HSSSimpleSelector::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->name->setThisObj(value);

    for (std::list<QSharedPointer<HSSFilter> >::iterator it = this->filters.begin(); it != this->filters.end(); ++it)
    {
        (*it)->setThisObj(value);
    }

    HSSSelector::setThisObj(value);
}

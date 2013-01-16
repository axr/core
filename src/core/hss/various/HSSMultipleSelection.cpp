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

#include "HSSMultipleSelection.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

class HSSMultipleSelection::Data : public QSharedData
{
public:
    Data() { }

    Data(const Data &other)
    : QSharedData(other), items(other.items) { }

    ~Data() { }

    /**
     *
     */
    std::vector<HSSSimpleSelection::p> items;
};

HSSMultipleSelection::HSSMultipleSelection()
: HSSSelection(HSSSelectionTypeMultipleSelection)
, d(new Data())
{
}

HSSMultipleSelection::HSSMultipleSelection(iterator a, iterator b)
: HSSSelection(HSSSelectionTypeMultipleSelection)
, d(new Data())
{
    std::vector<HSSSimpleSelection::p> newItems(a, b);
    this->d->items = newItems;
}

HSSMultipleSelection::HSSMultipleSelection(const_iterator a, const_iterator b)
: HSSSelection(HSSSelectionTypeMultipleSelection)
, d(new Data())
{
    std::vector<HSSSimpleSelection::p> newItems(a, b);
    this->d->items = newItems;
}

HSSMultipleSelection::HSSMultipleSelection(const HSSMultipleSelection &other)
: HSSSelection(other)
, d(other.d)
{
}

HSSMultipleSelection::~HSSMultipleSelection()
{
}

HSSMultipleSelection& HSSMultipleSelection::operator=(const HSSMultipleSelection &other)
{
    d = other.d;
    return *this;
}

void HSSMultipleSelection::add(HSSSimpleSelection::p item)
{
    this->d->items.push_back(item);
}

void HSSMultipleSelection::addSelection(HSSSelection::p item)
{
    if (item->isA(HSSSelectionTypeSimpleSelection))
    {
        HSSSimpleSelection::p simpleSel = qSharedPointerCast<HSSSimpleSelection>(item);
        this->add(simpleSel);
    }
    else if (item->isA(HSSSelectionTypeMultipleSelection))
    {
        HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(item);
        for (iterator it = multiSel->begin(); it != multiSel->end(); it++) {
            this->add(*it);
        }
    }
}

HSSMultipleSelection::iterator HSSMultipleSelection::begin()
{
    return this->d->items.begin();
}

HSSMultipleSelection::const_iterator HSSMultipleSelection::begin() const
{
    return this->d->items.begin();
}

HSSMultipleSelection::iterator HSSMultipleSelection::end()
{
    return this->d->items.end();
}

HSSMultipleSelection::const_iterator HSSMultipleSelection::end() const
{
    return this->d->items.end();
}

unsigned HSSMultipleSelection::size() const
{
    return this->d->items.size();
}

void HSSMultipleSelection::clear()
{
    return this->d->items.clear();
}

bool HSSMultipleSelection::empty() const
{
    return this->d->items.empty();
}

HSSSimpleSelection::p HSSMultipleSelection::front() const
{
    HSSSimpleSelection::p simpleSel = this->d->items.front();
    return simpleSel;
}

HSSSimpleSelection::p HSSMultipleSelection::back() const
{
    return this->d->items.back();
}

void HSSMultipleSelection::insert(iterator position, iterator first, iterator last)
{
    this->d->items.insert(position, first, last);
}

HSSMultipleSelection::iterator HSSMultipleSelection::erase(iterator position)
{
    return this->d->items.erase(position);
}

HSSMultipleSelection::iterator HSSMultipleSelection::erase(iterator first, iterator last)
{
    return this->d->items.erase(first, last);
}

std::vector< HSSSimpleSelection::p > HSSMultipleSelection::getItems() const
{
    return this->d->items;
}

HSSSimpleSelection::p HSSMultipleSelection::joinAll()
{
    HSSSimpleSelection::p ret(new HSSSimpleSelection());
    for (iterator it=this->d->items.begin(); it!=this->d->items.end(); it++) {
        HSSSimpleSelection::p inner = *it;
        for (HSSSimpleSelection::iterator it2 = inner->begin(); it2 != inner->end(); ++it2) {
            ret->add(*it2);
        }
    }
    return ret;
}

HSSMultipleSelection::p HSSMultipleSelection::splitAll()
{
    HSSMultipleSelection::p ret(new HSSMultipleSelection());
    for (iterator it=this->d->items.begin(); it!=this->d->items.end(); it++) {
        const HSSSimpleSelection::p & inner = *it;
        for (HSSSimpleSelection::iterator it2=inner->begin(); it2!=inner->end(); it2++) {
            HSSSimpleSelection::p newSel(new HSSSimpleSelection());
            newSel->add(*it2);
            ret->add(newSel);
        }
    }
    return ret;
}

AXRString HSSMultipleSelection::toString()
{
    AXRString tempstr = AXRString("HSSMultipleSelection: \n");
    if (this->d->items.size() > 0)
    {
        for (iterator it = this->d->items.begin(); it != this->d->items.end(); ++it)
        {
            tempstr.append("   ").append((*it)->toString());
        }
    }
    else
    {
        tempstr.append("WARNING: selection is empty!\n");
    }
    return tempstr;
}

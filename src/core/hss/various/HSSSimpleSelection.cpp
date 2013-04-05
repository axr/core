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

#include <QSharedData>
#include "HSSDisplayObject.h"
#include "HSSMultipleSelection.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

class HSSSimpleSelection::Data : public QSharedData
{
public:
    Data() { }

    Data(const Data &other)
    : QSharedData(other), items(other.items) { }

    ~Data() { }

    /**
     *
     */
    std::vector<QSharedPointer<HSSDisplayObject> > items;
};

QSharedPointer<HSSSimpleSelection> HSSSimpleSelection::null()
{
    QSharedPointer<HSSSimpleSelection> ret;
    return ret;
}

HSSSimpleSelection::HSSSimpleSelection()
: HSSSelection(HSSSelectionTypeSimpleSelection)
, d(new Data())
{
}

HSSSimpleSelection::HSSSimpleSelection(iterator a, iterator b)
: HSSSelection(HSSSelectionTypeSimpleSelection)
, d(new Data())
{
    std::vector<QSharedPointer<HSSDisplayObject> > newItems(a, b);
    this->d->items = newItems;
}

HSSSimpleSelection::HSSSimpleSelection(const_iterator a, const_iterator b)
: HSSSelection(HSSSelectionTypeSimpleSelection)
, d(new Data())
{
    std::vector<QSharedPointer<HSSDisplayObject> > newItems(a, b);
    this->d->items = newItems;
}

HSSSimpleSelection::HSSSimpleSelection(const HSSSimpleSelection &other)
: HSSSelection(other)
, d(other.d)
{
}

HSSSimpleSelection::HSSSimpleSelection(const std::vector<QWeakPointer<HSSDisplayObject> > &weakVector)
: HSSSelection(HSSSelectionTypeSimpleSelection)
, d(new Data())
{
    std::vector<QWeakPointer<HSSDisplayObject> >::const_iterator it;
    for (it=weakVector.begin(); it!=weakVector.end(); ++it) {
        QSharedPointer<HSSDisplayObject> theDO = (*it).toStrongRef();
        this->d->items.push_back(theDO);
    }
}

HSSSimpleSelection::~HSSSimpleSelection()
{
}

HSSSimpleSelection& HSSSimpleSelection::operator=(const HSSSimpleSelection &other)
{
    d = other.d;
    return *this;
}

void HSSSimpleSelection::add(QSharedPointer<HSSDisplayObject> theDO)
{
    this->d->items.push_back(theDO);
}

void HSSSimpleSelection::addSelection(QSharedPointer<HSSSelection> item)
{
    if (item->isA(HSSSelectionTypeSimpleSelection))
    {
        QSharedPointer<HSSSimpleSelection> simpleSel = qSharedPointerCast<HSSSimpleSelection>(item);
        for (const_iterator it = simpleSel->begin(); it != simpleSel->end(); ++it) {
            this->add(*it);
        }
    }
    else if (item->isA(HSSSelectionTypeMultipleSelection))
    {
        QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(item);
        for (HSSMultipleSelection::iterator it = multiSel->begin(); it != multiSel->end(); it++) {
            const QSharedPointer<HSSSimpleSelection> & simpleSel = *it;
            for (const_iterator it2 = simpleSel->begin(); it2 != simpleSel->end(); ++it2) {
                this->add(*it2);
            }
        }
    }
}

std::vector< QSharedPointer<HSSDisplayObject> >::iterator HSSSimpleSelection::begin()
{
    return this->d->items.begin();
}

HSSSimpleSelection::const_iterator HSSSimpleSelection::begin() const
{
    return this->d->items.begin();
}

std::vector< QSharedPointer<HSSDisplayObject> >::iterator HSSSimpleSelection::end()
{
    return this->d->items.end();
}

HSSSimpleSelection::const_iterator HSSSimpleSelection::end() const
{
    return this->d->items.end();
}

size_t HSSSimpleSelection::size() const
{
    return this->d->items.size();
}

void HSSSimpleSelection::clear()
{
    return this->d->items.clear();
}

bool HSSSimpleSelection::empty() const
{
    return this->d->items.empty();
}

QSharedPointer<HSSDisplayObject> HSSSimpleSelection::front() const
{
    return this->d->items.front();
}

QSharedPointer<HSSDisplayObject> HSSSimpleSelection::back() const
{
    return this->d->items.back();
}

void HSSSimpleSelection::insert(iterator position, iterator first, iterator last)
{
    this->d->items.insert(position, first, last);
}

HSSSimpleSelection::iterator HSSSimpleSelection::erase(iterator position)
{
    return this->d->items.erase(position);
}

HSSSimpleSelection::iterator HSSSimpleSelection::erase(iterator first, iterator last)
{
    return this->d->items.erase(first, last);
}

std::vector< QSharedPointer<HSSDisplayObject> > HSSSimpleSelection::getItems() const
{
    return this->d->items;
}

QSharedPointer<HSSSimpleSelection> HSSSimpleSelection::joinAll()
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection());
    for (iterator it=this->d->items.begin(); it!=this->d->items.end(); it++) {
        ret->add(*it);
    }
    return ret;
}

QSharedPointer<HSSMultipleSelection> HSSSimpleSelection::splitAll()
{
    QSharedPointer<HSSMultipleSelection> ret(new HSSMultipleSelection());
    for (iterator it=this->d->items.begin(); it!=this->d->items.end(); it++) {
        QSharedPointer<HSSSimpleSelection> newSel(new HSSSimpleSelection());
        newSel->add(*it);
        ret->add(newSel);
    }
    return ret;
}

AXRString HSSSimpleSelection::toString()
{
    AXRString tempstr = AXRString("HSSSimpleSelection with the following nodes: \n");
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

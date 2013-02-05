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

#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSDisplayObject.h"
#include "HSSAttrFunction.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSAttrFunction::HSSAttrFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeAttr, controller)
{
    this->observed = NULL;
}

HSSAttrFunction::HSSAttrFunction(const HSSAttrFunction & orig)
: HSSFunction(orig)
{
    this->attributeName = orig.attributeName;
    this->observed = NULL;
}

QSharedPointer<HSSFunction> HSSAttrFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

HSSAttrFunction::~HSSAttrFunction()
{
    if (this->observed)
    {
        this->observed->removeObserver(this->observedProperty, HSSObservablePropertyValue, this);
    }
}

const AXRString & HSSAttrFunction::getAttributeName() const
{
    return this->attributeName;
}

void HSSAttrFunction::setAttributeName(AXRString newValue)
{
    this->attributeName = newValue;
    this->setDirty(true);
}

const std::vector<QSharedPointer<HSSSelectorChain> > & HSSAttrFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSAttrFunction::setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > newValues)
{
    this->selectorChains = newValues;
    this->setDirty(true);
}

void HSSAttrFunction::selectorChainsAdd(QSharedPointer<HSSSelectorChain> & newSelectorChain)
{
    if (newSelectorChain)
    {
        std_log3("Added selector chain to HSSAttrFunction: " + newSelectorChain->toString());
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
    }
}

void HSSAttrFunction::selectorChainsRemove(unsigned int index)
{
    this->selectorChains.erase(this->selectorChains.begin() + index);
}

void HSSAttrFunction::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
}

QSharedPointer<HSSSelectorChain> & HSSAttrFunction::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

QSharedPointer<HSSSelectorChain> & HSSAttrFunction::selectorChainsLast()
{
    return this->selectorChains.back();
}

size_t HSSAttrFunction::selectorChainsSize() const
{
    return this->selectorChains.size();
}

QVariant HSSAttrFunction::_evaluate()
{
    QSharedPointer<HSSSimpleSelection> selection = this->getController()->select(this->selectorChains, this->scope, this->getThisObj())->joinAll();
    QSharedPointer<HSSDisplayObject> container = selection->front();
    this->_value = container->attributes[this->attributeName];

    //todo handle this
    //container->observe(this->attributeName, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSAttrFunction>(this, &HSSAttrFunction::valueChanged));

    //this->observed = container.get();
    return this->_value;
}

void HSSAttrFunction::valueChanged(HSSObservableProperty source, void*data)
{
    this->setDirty(true);
    this->_value = QVariant::fromValue(data);
    this->notifyObservers(HSSObservablePropertyValue, this->_value.value<void*>());
}

QSharedPointer<HSSClonable> HSSAttrFunction::cloneImpl() const
{
    QSharedPointer<HSSAttrFunction> clone = QSharedPointer<HSSAttrFunction>(new HSSAttrFunction(*this));

    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        QSharedPointer<HSSSelectorChain> clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }

    return clone;
}

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

#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSAttrFunction.h"

using namespace AXR;

HSSAttrFunction::HSSAttrFunction()
: HSSFunction(HSSFunctionTypeAttr)
{
    this->observed = NULL;
}

HSSAttrFunction::HSSAttrFunction(const HSSAttrFunction & orig)
: HSSFunction(orig)
{
    this->attributeName = orig.attributeName;
    this->observed = NULL;
}

HSSFunction::p HSSAttrFunction::clone() const
{
    return qSharedPointerCast<HSSFunction, HSSClonable > (this->cloneImpl());
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

const std::vector<HSSSelectorChain::p> & HSSAttrFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSAttrFunction::setSelectorChains(std::vector<HSSSelectorChain::p> newValues)
{
    this->selectorChains = newValues;
    this->setDirty(true);
}

void HSSAttrFunction::selectorChainsAdd(HSSSelectorChain::p & newSelectorChain)
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

HSSSelectorChain::p & HSSAttrFunction::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

HSSSelectorChain::p & HSSAttrFunction::selectorChainsLast()
{
    return this->selectorChains.back();
}

int HSSAttrFunction::selectorChainsSize()
{
    return this->selectorChains.size();
}

QVariant HSSAttrFunction::_evaluate()
{
    std::vector< std::vector<HSSDisplayObject::p> > selection = this->axrController->select(this->selectorChains, *this->scope, this->getThisObj());
    if (selection.empty())
    {
        // ignore
    }
    else if (!selection.empty() && !selection[0].empty())
    {
        HSSDisplayObject::p container = selection[0][0];
        this->_value = container->attributes[this->attributeName];

        //todo handle this
        //container->observe(this->attributeName, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSAttrFunction>(this, &HSSAttrFunction::valueChanged));

        //this->observed = container.get();
    }
    else
    {

    }
    return this->_value;
}

void HSSAttrFunction::valueChanged(HSSObservableProperty source, void*data)
{
    this->setDirty(true);
    this->_value = QVariant::fromValue(data);
    this->notifyObservers(HSSObservablePropertyValue, this->_value.value<void*>());
}

HSSClonable::p HSSAttrFunction::cloneImpl() const
{
    HSSAttrFunction::p clone = HSSAttrFunction::p(new HSSAttrFunction(*this));

    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        HSSSelectorChain::p clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }

    return clone;
}

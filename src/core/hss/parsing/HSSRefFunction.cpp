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

#include "errors.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSRefFunction.h"

using namespace AXR;

HSSRefFunction::HSSRefFunction()
: HSSFunction(HSSFunctionTypeRef)
{
    this->observed = NULL;
}

HSSRefFunction::HSSRefFunction(const HSSRefFunction & orig)
: HSSFunction(orig)
{
    this->modifier = orig.modifier;
    this->propertyName = orig.propertyName;
    this->observed = NULL;
}

HSSFunction::p HSSRefFunction::clone() const
{
    return boost::static_pointer_cast<HSSFunction, HSSClonable > (this->cloneImpl());
}

HSSRefFunction::~HSSRefFunction()
{
    if (this->observed != NULL)
    {
        this->observed->removeObserver(this->propertyName, HSSObservablePropertyValue, this);
    }
}

const std::string & HSSRefFunction::getModifier() const
{
    return this->modifier;
}

void HSSRefFunction::setModifier(std::string newValue)
{
    this->modifier = newValue;
    this->setDirty(true);
}

const HSSObservableProperty & HSSRefFunction::getPropertyName() const
{
    return this->propertyName;
}

void HSSRefFunction::setPropertyName(HSSObservableProperty newValue)
{
    this->propertyName = newValue;
    this->setDirty(true);
}

const std::vector<HSSSelectorChain::p> & HSSRefFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSRefFunction::setSelectorChains(std::vector<HSSSelectorChain::p> newValues)
{
    this->selectorChains = newValues;
    this->setDirty(true);
}

void HSSRefFunction::selectorChainsAdd(HSSSelectorChain::p & newSelectorChain)
{
    if (newSelectorChain)
    {
        std_log3("Added selector chain to HSSRefFunction: " + newSelectorChain->toString());
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
    }
}

void HSSRefFunction::selectorChainsRemove(unsigned int index)
{
    this->selectorChains.erase(this->selectorChains.begin() + index);
}

void HSSRefFunction::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
}

HSSSelectorChain::p & HSSRefFunction::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

HSSSelectorChain::p & HSSRefFunction::selectorChainsLast()
{
    return this->selectorChains.back();
}

const int HSSRefFunction::selectorChainsSize()
{
    return this->selectorChains.size();
}

boost::any HSSRefFunction::_evaluate()
{
    /**
     *  @todo this works only on numeric values, with other kind of data I don't know what will happen
     *  we need to figure out how to deal with non-numeric values here
     */

    std::vector< std::vector<HSSDisplayObject::p> > selection = this->axrController->select(this->selectorChains, *this->scope, this->getThisObj(), false);
    if (selection.size() == 0 || (selection.size() == 1 && selection[0].size() == 0))
    {
        // ignore
    }
    else if (selection.size() == 1 && selection[0].size() == 1)
    {
        HSSDisplayObject::p container = selection[0][0];
        this->_value = container->getProperty(this->propertyName);

        container->observe(this->propertyName, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSRefFunction > (this, &HSSRefFunction::valueChanged));

        this->observed = container.get();
    }
    else
    {
        /*
        if (this->modifier == "min"){
            unsigned i, size;
            long double tempval;
            HSSDisplayObject::p container;
            for (i=0, size = selection.size(); i<size; i++) {
                tempval = *(long double*)selection[i]->getProperty(this->propertyName);
                if (tempval < *(long double*)this->_value){
                    this->_value = &tempval;
                    container = selection[i];
                }
            }

            this->_value = container->getProperty(this->propertyName);

            container->observe(this->propertyName, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSRefFunction>(this, &HSSRefFunction::valueChanged));


        } else if (this->modifier == "max"){

        } else if (this->modifier == "avg"){

        }
         */
        throw AXRError::p(new AXRError("HSSRefFunciton", "Using modifiers in ref functions is not implemented yet"));
    }
    return this->_value;
}

void HSSRefFunction::valueChanged(HSSObservableProperty source, void*data)
{
    this->setDirty(true);
    this->_value = data;
    this->notifyObservers(HSSObservablePropertyValue, (void*) & this->_value);
}

HSSClonable::p HSSRefFunction::cloneImpl() const
{

    HSSRefFunction::p clone = HSSRefFunction::p(new HSSRefFunction(*this));

    HSSSelectorChain::const_it sIt;
    for (sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); sIt++)
    {
        HSSSelectorChain::p clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }
    return clone;
}

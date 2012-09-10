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

#include "errors.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSFlagFunction.h"

using namespace AXR;

HSSFlagFunctionType HSSFlagFunction::flagFunctionTypeFromString(std::string name)
{
    if (name == "flag")
    {
        return HSSFlagFunctionTypeFlag;
    }
    if (name == "unflag")
    {
        return HSSFlagFunctionTypeUnflag;
    }
    if (name == "toggleFlag")
    {
        return HSSFlagFunctionTypeToggleFlag;
    }

    return HSSFlagFunctionTypeNone;
}

HSSFlagFunction::HSSFlagFunction(HSSFlagFunctionType flagFunctionType)
: HSSFunction(HSSFunctionTypeFlag)
{
    this->_flagFunctionType = flagFunctionType;
}

HSSFlagFunction::HSSFlagFunction(const HSSFlagFunction & orig)
: HSSFunction(orig)
{
    this->_name = orig._name;
    this->_flagFunctionType = orig._flagFunctionType;
}

HSSFunction::p HSSFlagFunction::clone() const
{
    return boost::static_pointer_cast<HSSFunction, HSSClonable > (this->cloneImpl());
}

HSSFlagFunction::~HSSFlagFunction()
{

}

const std::string & HSSFlagFunction::getName()
{
    return this->_name;
}

void HSSFlagFunction::setName(std::string newValue)
{
    this->_name = newValue;
    this->setDirty(true);
}

const std::vector<HSSSelectorChain::p> & HSSFlagFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSFlagFunction::setSelectorChains(std::vector<HSSSelectorChain::p> newValues)
{
    this->selectorChains = newValues;
    this->setDirty(true);
}

void HSSFlagFunction::selectorChainsAdd(HSSSelectorChain::p & newSelectorChain)
{
    if (newSelectorChain)
    {
        std_log3("Added selector chain to HSSFlagFunction: " + newSelectorChain->toString());
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
    }
}

void HSSFlagFunction::selectorChainsRemove(unsigned int index)
{
    this->selectorChains.erase(this->selectorChains.begin() + index);
}

void HSSFlagFunction::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
}

HSSSelectorChain::p & HSSFlagFunction::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

HSSSelectorChain::p & HSSFlagFunction::selectorChainsLast()
{
    return this->selectorChains.back();
}

const int HSSFlagFunction::selectorChainsSize()
{
    return this->selectorChains.size();
}

boost::any HSSFlagFunction::_evaluate()
{
    return NULL;
}

boost::any HSSFlagFunction::_evaluate(std::deque<HSSParserNode::p> arguments)
{
    return this->_evaluate();
}

void HSSFlagFunction::valueChanged(HSSObservableProperty source, void*data)
{
    this->setDirty(true);
    this->_value = data;
    this->notifyObservers(HSSObservablePropertyValue, (void *) & this->_value);
}

HSSFlagFunctionType HSSFlagFunction::getFlagFunctionType()
{
    return this->_flagFunctionType;
}

HSSClonable::p HSSFlagFunction::cloneImpl() const
{
    HSSFlagFunction::p clone = HSSFlagFunction::p(new HSSFlagFunction(*this));

    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        HSSSelectorChain::p clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }

    return clone;
}

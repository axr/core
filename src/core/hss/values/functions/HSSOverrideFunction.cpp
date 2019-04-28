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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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

#include "axr.h"
#include "HSSOverrideFunction.h"

using namespace AXR;

HSSOverrideFunction::HSSOverrideFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeOverride, controller)
{
    
}

HSSOverrideFunction::~HSSOverrideFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSOverrideFunction: destructing override function");
    this->cleanTrackedObservers();
}

HSSOverrideFunction::HSSOverrideFunction(const HSSOverrideFunction & orig)
: HSSFunction(orig)
{
    this->selectorChains = orig.selectorChains;
    this->properties = orig.properties;
}

QSharedPointer<HSSFunction> HSSOverrideFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

AXRString HSSOverrideFunction::toString()
{
    AXRString tempstr = AXRString("HSSOverrideFunction\n");
    return tempstr;
}

void HSSOverrideFunction::setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains)
{
    this->selectorChains = selectorChains;
    for (std::vector<QSharedPointer<HSSSelectorChain> >::iterator it = this->selectorChains.begin(); it != this->selectorChains.end(); it++) {
        QSharedPointer<HSSSelectorChain> selChain = *it;
        selChain->setThisObj(this->getThisObj());
    }
}

std::vector<QSharedPointer<HSSSelectorChain> > HSSOverrideFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSOverrideFunction::setThisObj(QSharedPointer<AXR::HSSDisplayObject> newThis)
{
    HSSFunction::setThisObj(newThis);
    for (std::vector<QSharedPointer<HSSSelectorChain> >::iterator it = this->selectorChains.begin(); it != this->selectorChains.end(); it++) {
        QSharedPointer<HSSSelectorChain> selChain = *it;
        selChain->setThisObj(this->getThisObj());
    }
}

QSharedPointer<HSSObject> HSSOverrideFunction::_evaluate()
{
    QSharedPointer<HSSSelection> selection = this->getController()->select(this->getSelectorChains(), this->scope, this->getThisObj());
    QSharedPointer<HSSSimpleSelection> joinedSel = selection->joinAll();
    for (HSSSimpleSelection::iterator it = joinedSel->begin(); it != joinedSel->end(); ++it) {
        QSharedPointer<HSSDisplayObject> theDO = *it;
        
        theDO->setOverrides(this->getProperties());
        theDO->setNeedsRereadRules(true);
    }
    this->getController()->document()->setNeedsDisplay(true);
    this->setDirty(true);
    return this->_value;
}

QSharedPointer<HSSClonable> HSSOverrideFunction::cloneImpl() const
{
    QSharedPointer<HSSOverrideFunction> clone = QSharedPointer<HSSOverrideFunction>(new HSSOverrideFunction(*this));
    
    return clone;
}

std::vector<QSharedPointer<HSSPropertyDefinition> > HSSOverrideFunction::getProperties() const
{
    return this->properties;
}

void HSSOverrideFunction::propertiesAdd(QSharedPointer<HSSPropertyDefinition> & newProperty)
{
    if (newProperty)
    {
        newProperty->setParentNode(this->shared_from_this());
        this->properties.push_back(newProperty);
    }
}

void HSSOverrideFunction::propertiesRemove(off_t index)
{
    this->properties.erase(this->properties.begin() + index);
}

void HSSOverrideFunction::propertiesRemoveLast()
{
    this->properties.pop_back();
}

QSharedPointer<HSSPropertyDefinition> & HSSOverrideFunction::propertiesGet(size_t index)
{
    return this->properties[index];
}

QSharedPointer<HSSPropertyDefinition> & HSSOverrideFunction::propertiesLast()
{
    return this->properties.back();
}

size_t HSSOverrideFunction::propertiesSize() const
{
    return this->properties.size();
}

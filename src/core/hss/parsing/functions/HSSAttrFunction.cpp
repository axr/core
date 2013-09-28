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
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSDisplayObject.h"
#include "HSSAttrFunction.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "HSSValue.h"
#include <QStringList>

using namespace AXR;

HSSAttrFunction::HSSAttrFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeAttr, controller)
{

}

HSSAttrFunction::HSSAttrFunction(const HSSAttrFunction & orig)
: HSSFunction(orig)
{
    this->attributeName = orig.attributeName;
}

QSharedPointer<HSSFunction> HSSAttrFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

HSSAttrFunction::~HSSAttrFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSAttrFunction: destructing attr function");
    this->cleanTrackedObservers();
}

bool HSSAttrFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSFunction::equalTo(otherNode)) return false;
    QSharedPointer<HSSAttrFunction> castedNode = qSharedPointerCast<HSSAttrFunction>(otherNode);
    if ( this->attributeName != castedNode->attributeName ) return false;
    if ( this->_stringValue != castedNode->_stringValue ) return false;
    unsigned i = 0;
    Q_FOREACH(QSharedPointer<HSSSelectorChain> selectorChain, this->selectorChains)
    {
        if ( ! selectorChain->equalTo(castedNode->selectorChains[i])) return false;
        ++i;
    }
    return true;
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

QSharedPointer<HSSObject> HSSAttrFunction::_evaluate()
{
    QSharedPointer<HSSSimpleSelection> selection = this->getController()->select(this->selectorChains, this->scope, this->getThisObj())->joinAll();
    if(selection->size() > 0)
    {
        QSharedPointer<HSSDisplayObject> container = selection->front();
        this->_stringValue = container->attributes[this->attributeName];

        ///@todo handle this
        //container->observe(this->attributeName, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSAttrFunction>(this, &HSSAttrFunction::valueChanged));
    }
    else
    {
        QStringList selectorChainStrings;
        foreach (QSharedPointer<HSSSelectorChain> selectorChain, this->selectorChains)
        {
            selectorChainStrings.append(selectorChain->stringRep());
        }

        AXRWarning("HSSAttrFunction", AXRString("attr(%1) of %2 did not select any elements").arg(this->attributeName).arg(selectorChainStrings.join(", "))).raise();
        this->_stringValue = AXRString();
    }
    this->_value = HSSValue::valueFromParserNode(this->getController(), HSSStringConstant::stringToConstant(this->_stringValue, this->getController()), this->getThisObj(), this->scope);
    return this->_value;
}

void HSSAttrFunction::valueChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->setDirty(true);
    this->_value = theObj;
    this->notifyObservers("__impl_private__value", theObj);
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

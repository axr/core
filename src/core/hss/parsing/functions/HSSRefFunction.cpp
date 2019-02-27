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
#include "AXRError.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSPercentageConstant.h"
#include "HSSPropertyPath.h"
#include "HSSPropertyPathNode.h"
#include "HSSRefFunction.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSSimpleSelector.h"
#include "HSSValue.h"

using namespace AXR;

HSSRefFunction::HSSRefFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeRef, controller)
{
    this->_isAttached = false;
    this->_logsSelections = true;

}

HSSRefFunction::HSSRefFunction(const HSSRefFunction & orig)
: HSSFunction(orig)
{
    this->modifier = orig.modifier;
    this->propertyPath = orig.propertyPath;
    this->_isAttached = orig._isAttached;
    this->_logsSelections = orig._logsSelections;
}

QSharedPointer<HSSFunction> HSSRefFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

HSSRefFunction::~HSSRefFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRefFunction: destructing ref function");
    this->cleanTrackedObservers();
}

bool HSSRefFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSFunction::equalTo(otherNode)) return false;
    QSharedPointer<HSSRefFunction> castedNode = qSharedPointerCast<HSSRefFunction>(otherNode);
    if ( this->modifier != castedNode->modifier ) return false;
    if ( ! this->propertyPath->equalTo(castedNode->propertyPath) ) return false;
    unsigned i = 0;
    Q_FOREACH(QSharedPointer<HSSSelectorChain> selectorChain, this->selectorChains)
    {
        if ( ! selectorChain->equalTo(castedNode->selectorChains[i])) return false;
        ++i;
    }
    return true;
}

const AXRString & HSSRefFunction::getModifier() const
{
    return this->modifier;
}

void HSSRefFunction::setModifier(AXRString newValue)
{
    this->modifier = newValue;
    this->setDirty(true);
}

const QSharedPointer<HSSPropertyPath> HSSRefFunction::getPropertyPath() const
{
    return this->propertyPath;
}

void HSSRefFunction::setPropertyName(AXRString newValue)
{
    QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(newValue, this->getController()));
    QSharedPointer<HSSPropertyPath> ppath(new HSSPropertyPath(this->getController()));
    ppath->add(ppn);
    this->propertyPath = ppath;
    this->setDirty(true);
}

void HSSRefFunction::setPropertyPath(QSharedPointer<HSSPropertyPath> newValue)
{
    this->propertyPath = newValue;
    this->setDirty(true);
}

const std::vector<QSharedPointer<HSSSelectorChain> > & HSSRefFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSRefFunction::setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > newValues)
{
    this->selectorChains = newValues;
    this->setDirty(true);
    for (HSSSelectorChain::it it = this->selectorChains.begin(); it != this->selectorChains.end(); ++it)
    {
        (*it)->setParentNode(this->shared_from_this());
    }
}

void HSSRefFunction::selectorChainsAdd(QSharedPointer<HSSSelectorChain> & newSelectorChain)
{
    if (newSelectorChain)
    {
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
        this->setDirty(true);
    }
}

void HSSRefFunction::selectorChainsRemove(unsigned int index)
{
    this->selectorChains.erase(this->selectorChains.begin() + index);
    this->setDirty(true);
}

void HSSRefFunction::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
    this->setDirty(true);
}

QSharedPointer<HSSSelectorChain> & HSSRefFunction::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

QSharedPointer<HSSSelectorChain> & HSSRefFunction::selectorChainsLast()
{
    return this->selectorChains.back();
}

size_t HSSRefFunction::selectorChainsSize() const
{
    return this->selectorChains.size();
}

QSharedPointer<HSSObject> HSSRefFunction::_evaluate()
{
    QSharedPointer<HSSObject> errorState;
    AXRController * theController = this->getController();
    if (!this->logsSelections())
    {
        theController->setLogsSelections(false);
    }
    QSharedPointer<HSSSimpleSelection> selection = theController->select(this->selectorChains, this->scope, this->getThisObj(), !this->_isAttached, false, false)->joinAll();
    if (!this->logsSelections())
    {
        theController->setLogsSelections(true);
    }
    this->_isAttached = true;
    if (selection->empty())
    {
        // ignore
    }
    else if (selection->size() == 1)
    {
        QSharedPointer<HSSPropertyPath> ppath = this->getPropertyPath()->clone();
        AXRString tlp = this->getPropertyPath()->front()->getPropertyName();
        QSharedPointer<HSSDisplayObject> container = selection->front();
        QSharedPointer<HSSObject> ret;
        this->_value = this->_getValueByPath(container, ppath);

        if (container.data() != this->getTrackedObserver("__impl_private__refValue") || this->getTrackedProperty("__impl_private__refValue") != tlp)
        {
            container->observe(tlp, "__impl_private__refValue", this, new HSSValueChangedCallback<HSSRefFunction > (this, &HSSRefFunction::valueChanged));
        }
        if (this->getPropertyPath()->size() > 1)
        {
            QSharedPointer<HSSObject> tlo = container->getComputedValue(tlp);
            if (tlo)
            {
                tlo->observe("__impl_private__replace", "__impl_private__refValue", this, new HSSValueChangedCallback<HSSRefFunction > (this, &HSSRefFunction::replaceChanged));
            }
        }
    }
    else
    {
        /*
         if (this->modifier == "min"){
         unsigned i, size;
         HSSUnit tempval;
         QSharedPointer<HSSDisplayObject> container;
         for (i=0, size = selection.size(); i<size; ++i) {
         tempval = *(HSSUnit*)selection[i]->getProperty(this->propertyName);
         if (tempval < *(HSSUnit*)this->_value){
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
        AXRError("HSSRefFunciton", "Using modifiers in ref functions is not implemented yet").raise();
        return errorState;
    }
    return this->_value;
}

void HSSRefFunction::valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> remoteObj)
{
    QSharedPointer<HSSObject> ret;
    if (remoteObj)
    {
        if (remoteObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(remoteObj);

            QSharedPointer<HSSParserNode> parserNode = valueObj->getValue();
            if (parserNode)
            {
                switch (parserNode->getType())
                {
                    case HSSParserNodeTypeFunctionCall:
                    {
                        QSharedPointer<HSSObject> remoteObj2 = qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
                        if (remoteObj2) {
                            ret = remoteObj2;
                        }
                        break;
                    }

                    case HSSParserNodeTypeExpression:
                        ret = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(qSharedPointerCast<HSSExpression>(parserNode)->evaluate(), this->getController()), parserNode->getSpecificity(), this->getThisObj(), this->scope);
                        break;

                    default:
                        ret = remoteObj->clone();
                        break;
                }
            }
            else
            {
                ret = remoteObj->clone();
            }
        }
        else
        {
            ret = remoteObj->clone();
        }

        if (this->getPropertyPath()->size() > 1)
        {
            QSharedPointer<HSSPropertyPath> ppath = this->getPropertyPath()->clone();
            ppath->popFront();
            ret = this->_getValueByPath(ret, ppath);
        }
    }
    this->_value = ret;
    this->notifyObservers("__impl_private__remoteValue", ret);
}

void HSSRefFunction::replace(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSPropertyPath> ppath = this->getPropertyPath()->clone();
    this->_value = this->_getValueByPath(theObj, ppath);

    AXRString tlp = this->getPropertyPath()->front()->getPropertyName();
    if (theObj && (theObj != this->getTrackedObserver("__impl_private__refValue") || this->getTrackedProperty("__impl_private__refValue") != tlp))
    {
        theObj->observe(tlp, "__impl_private__refValue", this, new HSSValueChangedCallback<HSSRefFunction > (this, &HSSRefFunction::valueChanged));
    }
}

void HSSRefFunction::replaceChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> remoteObj)
{
    QSharedPointer<HSSPropertyPath> ppath = this->getPropertyPath()->clone();
    ppath->popFront();
    this->_value = this->_getValueByPath(remoteObj, ppath);

    AXRString tlp = this->getPropertyPath()->front()->getPropertyName();
    if (remoteObj && (remoteObj != this->getTrackedObserver("__impl_private__refValue") || this->getTrackedProperty("__impl_private__refValue") != tlp))
    {
        remoteObj->observe(tlp, "__impl_private__refValue", this, new HSSValueChangedCallback<HSSRefFunction > (this, &HSSRefFunction::valueChanged));
    }
    if (this->getPropertyPath()->size() > 1)
    {
        remoteObj->observe("__impl_private__replace", "__impl_private__refValue", this, new HSSValueChangedCallback<HSSRefFunction > (this, &HSSRefFunction::replaceChanged));
    }
    this->notifyObservers("__impl_private__remoteValue", this->_value);
}

QSharedPointer<HSSClonable> HSSRefFunction::cloneImpl() const
{
    QSharedPointer<HSSRefFunction> clone = QSharedPointer<HSSRefFunction>(new HSSRefFunction(*this));

    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        QSharedPointer<HSSSelectorChain> clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }

    return clone;
}

QSharedPointer<HSSObject> HSSRefFunction::_getValueByPath(QSharedPointer<HSSObject> object, QSharedPointer<HSSPropertyPath> path)
{
    QSharedPointer<HSSObject> ret;
    QSharedPointer<HSSPropertyPathNode> ppn = path->popFront();
    AXRString propertyName = ppn->getPropertyName();
    QSharedPointer<HSSObject> remoteObj;
    remoteObj = object->getComputedValue(propertyName);
    if (remoteObj)
    {
        if (path->size() != 0)
        {
            ret = this->_getValueByPath(remoteObj, path);
        }
        else
        {
            if (remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(remoteObj);

                QSharedPointer<HSSParserNode> parserNode = valueObj->getValue();
                if (parserNode)
                {
                    switch (parserNode->getType())
                    {
                        case HSSParserNodeTypeFunctionCall:
                        {
                            QSharedPointer<HSSObject> remoteObj2 = qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
                            if (remoteObj2) {
                                ret = remoteObj2->clone();
                            }
                            break;
                        }

                        case HSSParserNodeTypeExpression:
                            ret = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(qSharedPointerCast<HSSExpression>(parserNode)->evaluate(), this->getController()), parserNode->getSpecificity(), this->getThisObj(), this->scope);
                            break;

                        default:
                            ret = remoteObj->clone();
                            break;
                    }
                }
                else
                {
                    ret = valueObj->clone();
                }
            }
            else
            {
                ret = remoteObj->clone();
            }
        }
    }
    return ret;
}

void HSSRefFunction::setLogsSelections(bool newValue)
{
    this->_logsSelections = newValue;
}

const bool HSSRefFunction::logsSelections() const
{
    return this->_logsSelections;
}

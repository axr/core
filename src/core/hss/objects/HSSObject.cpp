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

#include "axr.h"
#include "HSSObject.h"

using namespace AXR;

AXRString HSSObject::objectTypeToString(HSSObjectType type)
{
    static std::map<HSSObjectType, AXRString> typestrings;
    if (typestrings.empty())
    {
        typestrings[HSSObjectTypeContainer] = "container";
        typestrings[HSSObjectTypeTextBlock] = "textBlock";
        typestrings[HSSObjectTypeStroke] = "stroke";
        typestrings[HSSObjectTypeGeneric] = "object";
        typestrings[HSSObjectTypeValue] = "value";
        typestrings[HSSObjectTypeMargin] = "margin";
        typestrings[HSSObjectTypeRgb] = "rgb";
        typestrings[HSSObjectTypeGradient] = "linearGradient";
        typestrings[HSSObjectTypeGradient] = "radialGradient";
        typestrings[HSSObjectTypeColorStop] = "colorStop";
        typestrings[HSSObjectTypeFont] = "font";
        typestrings[HSSObjectTypeShape] = "shape";
        typestrings[HSSObjectTypeAction] = "request";
    }
    return typestrings[type];
}

QSharedPointer<HSSObject> HSSObject::newObjectWithType(AXRString type, AXRController * controller)
{
    static std::map<AXRString, HSSObjectType> types;
    if (types.empty())
    {
        types["container"] = HSSObjectTypeContainer;
        types["stroke"] = HSSObjectTypeStroke;
        types["object"] = HSSObjectTypeGeneric;
        types["value"] = HSSObjectTypeValue;
        types["margin"] = HSSObjectTypeMargin;
        types["rgb"] = HSSObjectTypeRgb;
        types["linearGradient"] = HSSObjectTypeGradient;
        types["radialGradient"] = HSSObjectTypeGradient;
        types["colorStop"] = HSSObjectTypeColorStop;
        types["font"] = HSSObjectTypeFont;
        types["rectangle"] = HSSObjectTypeShape;
        types["roundedRect"] = HSSObjectTypeShape;
        types["circle"] = HSSObjectTypeShape;
        types["polygon"] = HSSObjectTypeShape;
        types["request"] = HSSObjectTypeAction;
    }

    HSSObjectType objectType = HSSObjectTypeNone;
    if (types.count(type))
    {
        objectType = types[type];
    }

    switch (objectType)
    {
    case HSSObjectTypeContainer:
    {
        return QSharedPointer<HSSContainer>(new HSSContainer(controller));
    }

    case HSSObjectTypeStroke:
    {
        /**
         *  @todo stroke tyes?
         */
        return QSharedPointer<HSSStroke>(new HSSStroke(controller));
    }

    case HSSObjectTypeGradient:
    {
        if (type == "linearGradient")
        {
            return QSharedPointer<HSSLinearGradient>(new HSSLinearGradient(controller));
        }
        else if (type == "radialGradient")
        {
            return QSharedPointer<HSSRadialGradient>(new HSSRadialGradient(controller));
        }
    }

    case HSSObjectTypeColorStop:
    {
        return QSharedPointer<HSSColorStop>(new HSSColorStop(controller));
    }

    case HSSObjectTypeValue:
    {
        return QSharedPointer<HSSValue>(new HSSValue(controller));
    }

    case HSSObjectTypeMargin:
    {
        return QSharedPointer<HSSMargin>(new HSSMargin(controller));
    }

    case HSSObjectTypeRgb:
    {
        return QSharedPointer<HSSRgb>(new HSSRgb(controller));
    }

    case HSSObjectTypeFont:
    {
        return QSharedPointer<HSSFont>(new HSSFont(controller));
    }

    case HSSObjectTypeShape:
    {
        if (type == "rectangle")
        {
            return QSharedPointer<HSSRectangle>(new HSSRectangle(controller));
        }
        else if (type == "roundedRect")
        {
            return QSharedPointer<HSSRoundedRect>(new HSSRoundedRect(controller));
        }
        else if (type == "circle")
        {
            return QSharedPointer<HSSCircle>(new HSSCircle(controller));
        }
        else if (type == "polygon")
        {
            return QSharedPointer<HSSPolygon>(new HSSPolygon(controller));
        }
    }

    case HSSObjectTypeAction:
    {
        if (type == "request")
        {
            return QSharedPointer<HSSRequest>(new HSSRequest(controller));
        }
    }
    default:
        return QSharedPointer<HSSObject>();
    }

    return QSharedPointer<HSSObject>();
}

HSSObject::HSSObject(HSSObjectType type, AXRController * controller)
{
    this->_initialize();
    this->_isNamed = false;
    this->name = "";
    this->_debugName = "unnamed";
    this->type = type;
    this->shorthandIndex = 0;
    this->axrController = controller;
    this->_hostProperty = "";
    this->_specificity = 0.0;
    this->_isDefaultPropertyPathObject = false;
    this->_expectsIsAIncluded = false;
}

HSSObject::HSSObject(const HSSObject & orig)
{
    this->_initialize();
    this->name = orig.name;
    this->_debugName = orig._debugName;
    this->type = orig.type;
    this->_isNamed = orig._isNamed;
    this->scope = orig.scope;
    this->thisObj = orig.thisObj;
    this->axrController = orig.axrController;
    this->shorthandIndex = orig.shorthandIndex;
    this->_hostProperty = orig._hostProperty;
    this->_specificity = orig._specificity;
    this->_isDefaultPropertyPathObject = orig._isDefaultPropertyPathObject;
    this->_expectsIsAIncluded = orig._expectsIsAIncluded;

    //copy the shorthand list
    std::vector<AXRString>::const_iterator it;
    for (it = orig.shorthandProperties.begin(); it != orig.shorthandProperties.end(); ++it)
    {
        const AXRString & value = *it;
        this->shorthandProperties.push_back(value);
    }
    //copy the default values
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator defaultIt;
    for (defaultIt = orig._defaultValues.begin(); defaultIt != orig._defaultValues.end(); ++defaultIt)
    {
        this->_defaultValues[defaultIt->first] = defaultIt->second->clone();
    }
    //copy the stack values
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator stackIt;
    for (stackIt = orig._stackValues.begin(); stackIt != orig._stackValues.end(); ++stackIt)
    {
        this->_stackValues[stackIt->first] = stackIt->second->clone();
    }
    //copy the overrides
    std::map<AXRString, QSharedPointer<HSSPropertyDefinition> >::const_iterator ovIt;
    for (ovIt = orig._overrides.begin(); ovIt != orig._overrides.end(); ++ovIt)
    {
        this->_overrides[ovIt->first] = ovIt->second->clone();
    }
    //copy the computed values
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator computedIt;
    for (computedIt = orig._computedValues.begin(); computedIt != orig._computedValues.end(); ++computedIt)
    {
        this->_computedValues[computedIt->first] = computedIt->second->clone();
    }
    //shallow copy the applied isA rules
    std::vector<QSharedPointer<HSSRule> >::const_iterator isAIt;
    for (isAIt = orig._appliedIsARules.begin(); isAIt != orig._appliedIsARules.end(); ++isAIt)
    {
        const QSharedPointer<HSSRule> & isARule = *isAIt;
        this->_appliedIsARules.push_back(isARule);
    }
    //copy the objDefRules
    std::deque<QSharedPointer<HSSRule> >::const_iterator objDefRulesIt;
    for (objDefRulesIt = orig._objDefRules.begin(); objDefRulesIt != orig._objDefRules.end(); ++objDefRulesIt)
    {
        const QSharedPointer<HSSRule> & objDefRule = *objDefRulesIt;
        this->_objDefRules.push_back(objDefRule->clone());
    }
}

void HSSObject::_initialize()
{
    this->addCallback("isA", new HSSObserveCallback<HSSObject>(this, &HSSObject::setIsA));
    this->addListenCallback("isA", new HSSObserveCallback<HSSObject>(this, &HSSObject::listenIsA));
}

QSharedPointer<HSSObject> HSSObject::clone() const
{
    return qSharedPointerCast<HSSObject> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSObject::cloneImpl() const
{
    return QSharedPointer<HSSObject>(new HSSObject(*this));
}

HSSObject::~HSSObject()
{

}

bool HSSObject::isKeyword(AXRString value, AXRString property)
{
    return (value == "default" || value == "inherit" || value == "undefined");
}

bool HSSObject::isFunction(AXRString value, AXRString property)
{
    if (value == "min"
            || value == "max"
            || value == "floor"
            || value == "ceil"
            || value == "round"
            || value == "ref"
            || value == "sel"
            || value == "flag"
            || value == "unflag"
            || value == "addFlag"
            || value == "takeFlag"
            || value == "replaceFlag"
            || value == "attr"
            || value == "log"
            || value == "override"
            || value == "startTimer"
            || value == "stopTimer"
            || value == "toggleTimer"
            || value == "insert"
            )
    {
        return true;
    }
    else
    {
        return this->getController()->document()->isCustomFunction(value);
    }
}

AXRString HSSObject::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSObject: ").append(this->name);
    }
    else
    {
        return "Anonymous HSSObject";
    }
}

bool HSSObject::equalTo(QSharedPointer<HSSObject> otherObj)
{
    //check wether pointers are the same
    if (this == otherObj.data()) return true;
    //check wether of same type
    if (otherObj->type != this->type) return false;
    //check wether the same amount of values
    if (this->_computedValues.size() != otherObj->_computedValues.size()) return false;
    //check wether the values are equal
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator computedIt;
    for (computedIt = this->_computedValues.begin(); computedIt != this->_computedValues.end(); ++computedIt)
    {
        if ( otherObj->_computedValues.count(computedIt->first) == 0) return false;
        if ( ! computedIt->second->equalTo(otherObj->_computedValues[computedIt->first])) return false;
    }
    return true;
}

bool HSSObject::isNamed() const
{
    return this->_isNamed;
}

void HSSObject::setName(AXRString newName)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSObject: setting name to " + newName);
    this->name = newName;
    this->_isNamed = true;
    this->_debugName = newName.data();
}

AXRString HSSObject::getName()
{
    return this->name;
}

void HSSObject::dropName()
{
    this->name = "";
    this->_isNamed = false;
}

AXRString HSSObject::defaultObjectType()
{
    return "value";
}

AXRString HSSObject::defaultObjectType(AXRString property)
{
    return "value";
}

AXRString HSSObject::getPropertyForCurrentValue()
{
    bool done = false;
    AXRString ret;

    security_brake_init();
    while (!done)
    {
        if (this->shorthandProperties.size() > this->shorthandIndex)
        {
            ret = this->shorthandProperties[this->shorthandIndex];
            if (this->skipShorthand.find(ret) == this->skipShorthand.end())
            {
                return ret;
            }
        }

        done = !this->shorthandNext();

        security_brake();
    }
    security_brake_reset();
    return "";
}

void HSSObject::setShorthandProperties(std::vector<AXRString> newValues)
{
    this->shorthandProperties = newValues;
    this->shorthandReset();
}

void HSSObject::shorthandSkip(AXRString propertyName)
{
    this->skipShorthand[propertyName] = true;
}

bool HSSObject::shorthandNext()
{
    if (this->shorthandIndex < this->shorthandProperties.size() + 1)
    {
        this->shorthandIndex++;
        return true;
    }
    return false;
}

void HSSObject::shorthandReset()
{
    this->shorthandIndex = 0;
}

size_t HSSObject::getShorthandIndex() const
{
    return this->shorthandIndex;
}

void HSSObject::setShorthandIndex(size_t newValue)
{
    if (newValue < this->shorthandProperties.size())
    {
        this->shorthandIndex = newValue;
    }
    else
    {
        this->shorthandIndex = this->shorthandProperties.size();
    }
}

void HSSObject::setIsA(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSObject> preparedObj = this->_setIsAPrepare(theObj);
    if(preparedObj){
      this->_setIsA(preparedObj);
    }
}

QSharedPointer<HSSObject> HSSObject::_setIsAPrepare(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSObject> ret;
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
        if (!parserNode)
        {
            return ret;
        }
        switch (parserNode->getType())
        {
            case HSSParserNodeTypeObjectNameConstant:
            {
                try
                {
                    QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (parserNode);
                    QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
                    objdef->applyStack();
                    objdef->applyRules();
                    QSharedPointer<HSSObject> remoteObj = objdef->getObject();
                    remoteObj->setSpecificity(theObj->getSpecificity());
                    ret = remoteObj;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                if (parserNode->isA(HSSFunctionTypeRef))
                {
                    QSharedPointer<HSSRefFunction> refFunction = qSharedPointerCast<HSSRefFunction>(parserNode);
                    QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSRefFunction>(refFunction)->evaluate();
                    if (remoteObj)
                    {
                        remoteObj->setSpecificity(theObj->getSpecificity());
                        ret = remoteObj;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    else if (theObj->isA(HSSObjectTypeMultipleValue))
    {
        QSharedPointer<HSSMultipleValue> newMultiVal = QSharedPointer<HSSMultipleValue>(new HSSMultipleValue(this->getController()));
        const std::vector<QSharedPointer<HSSObject> > values = qSharedPointerCast<HSSMultipleValue>(theObj)->getValues();
        std::vector<QSharedPointer<HSSObject> >::const_iterator it;
        for (it = values.begin(); it != values.end(); ++it)
        {
            const QSharedPointer<HSSObject> & mvObj = *it;
            newMultiVal->add(this->_setIsAPrepare(mvObj));
        }
        ret = newMultiVal;
    }
    else
    {
        ret = theObj;
    }
    return ret;
}

void HSSObject::_setIsA(QSharedPointer<HSSObject> theObj)
{
    this->_applyIsAObject(theObj);
}

void HSSObject::_applyIsAObject(QSharedPointer<HSSObject> theObj)
{
    if (theObj && theObj->type == this->type)
    {
        std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator it;
        for (it = theObj->_computedValues.begin(); it != theObj->_computedValues.end(); ++it)
        {
            AXRString propertyName = it->first;
            QSharedPointer<HSSObject> isAObj = it->second;
            if (!this->expectsIsAIncluded() || propertyName != "isA")
            {
                this->setComputed(propertyName, isAObj);
            }
            if (propertyName != "isA")
            {
                this->setStackValue(propertyName, isAObj);
            }
            this->setNeedsDefault(propertyName, false);
        }
    }
    else
    {
        AXRError("HSSObject", "Object referenced by isA property is not of the same object type or does not exist.").raise();
    }
}

void HSSObject::listenIsA(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> theValue = qSharedPointerCast<HSSValue>(theObj)->getValue();
        if (!theValue) {
            return;
        }
        switch (theValue->getType())
        {
            case HSSParserNodeTypeFunctionCall:
            {
                theObj->observe("__impl_private__valueChanged", "isA", this, new HSSValueChangedCallback<HSSObject>(this, &HSSObject::isAChanged));
                break;
            }

            default:
                break;
        }
    }
}

void HSSObject::isAChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->_setIsA(theObj);
}

QSharedPointer<HSSSimpleSelection> HSSObject::getScope() const
{
    return this->scope;
}

void HSSObject::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
    //propagate values
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator computedIt;
    for (computedIt = this->_computedValues.begin(); computedIt != this->_computedValues.end(); ++computedIt)
    {
        computedIt->second->setScope(newScope);
    }
}

void HSSObject::setController(AXRController * controller)
{
    this->axrController = controller;
}

AXRController * HSSObject::getController() const
{
    return this->axrController;
}

void HSSObject::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
    //propagate values
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator computedIt;
    for (computedIt = this->_computedValues.begin(); computedIt != this->_computedValues.end(); ++computedIt)
    {
        computedIt->second->setThisObj(value);
    }
}

QSharedPointer<HSSDisplayObject> HSSObject::getThisObj() const
{
    return thisObj;
}

bool HSSObject::isA(HSSObjectType otherType) const
{
    return otherType == this->type;
}

HSSObjectType HSSObject::getObjectType() const
{
    return this->type;
}

bool HSSObject::isA(HSSShapeType otherType) const
{
    return false;
}

HSSShapeType HSSObject::getShapeType() const
{
    return HSSShapeTypeNone;
}

bool HSSObject::isA(HSSRequestModeType otherType) const
{
    return false;
}

HSSRequestModeType HSSObject::getRequestModeType() const
{
    return HSSRequestModeTypeNone;
}

bool HSSObject::isA(HSSEventType otherType) const
{
    return false;
}

HSSEventType HSSObject::getEventType() const
{
    return HSSEventTypeNone;
}

bool HSSObject::isA(HSSActionType otherType) const
{
    return false;
}

HSSActionType HSSObject::getActionType() const
{
    return HSSActionTypeNone;
}

bool HSSObject::isA(HSSGradientType otherType) const
{
    return false;
}

HSSGradientType HSSObject::getGradientType() const
{
    return HSSGradientTypeNone;
}

bool HSSObject::isA(HSSSelectionType otherType) const
{
    return false;
}

HSSSelectionType HSSObject::getSelectionType() const
{
    return HSSSelectionTypeNone;
}

QSharedPointer<HSSObject> HSSObject::shared_from_this()
{
    if (!ptr)
        ptr = QWeakPointer<HSSObject>(this);

    return ptr.toStrongRef();
}

bool HSSObject::validate(AXRString propertyName, QSharedPointer<HSSParserNode> & value) const
{
    return true;
}

void HSSObject::clearProperties()
{
    this->clearStackValues();
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator it;
    for (it = this->_computedValues.begin(); it != this->_computedValues.end(); ++it)
    {
        QSharedPointer<HSSObject> theObj = it->second;
        theObj->setSpecificity(0);
    }
    this->_needsDefault.clear();
}

void HSSObject::addCallback(AXRString name, HSSAbstractStackCallback *callback)
{
    this->_stackCallbacks[name] = callback;
}

void HSSObject::addCallback(AXRString name, HSSAbstractComputeCallback *callback)
{
    this->_computeCallbacks[name] = callback;
}

void HSSObject::addCallback(AXRString name, HSSAbstractObserveCallback *setCallback)
{
    this->_setCallbacks[name] = setCallback;
}

void HSSObject::addCallback(AXRString name, HSSAbstractObserveCallback *listenCallback, HSSAbstractObserveCallback *notifyCallback)
{
    this->_listenCallbacks[name] = listenCallback;
    this->_notifyCallbacks[name] = notifyCallback;
}

void HSSObject::addCallback(AXRString name, HSSAbstractComputeCallback *computeCallback, HSSAbstractObserveCallback *listenCallback, HSSAbstractObserveCallback *notifyCallback)
{
    this->_computeCallbacks[name] = computeCallback;
    this->_listenCallbacks[name] = listenCallback;
    this->_notifyCallbacks[name] = notifyCallback;
}

void HSSObject::addListenCallback(AXRString name, HSSAbstractObserveCallback *listenCallback)
{
    this->_listenCallbacks[name] = listenCallback;
}

void HSSObject::addNotifyCallback(AXRString name, HSSAbstractObserveCallback *notifyCallback)
{
    this->_notifyCallbacks[name] = notifyCallback;
}

void HSSObject::clearDefaultValues()
{
    this->_defaultValues.clear();
}

void HSSObject::setDefaults()
{
    //do nothing
}

void HSSObject::setDefaultValue(AXRString propertyName, QSharedPointer<HSSObject> theObj)
{
    this->_defaultValues[propertyName] = theObj;
}

QSharedPointer<HSSObject> HSSObject::getDefaultValue(AXRString propertyName) const
{
    QSharedPointer<HSSObject> ret;
    if (this->_defaultValues.count(propertyName))
    {
        return this->_defaultValues.at(propertyName);
    }
    return ret;
}

void HSSObject::setDefault(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    QSharedPointer<HSSObject> theObj = this->computeValue(propertyName, parserNode);
    if (theObj)
    {
        this->setDefaultValue(propertyName, theObj);
    }
}

void HSSObject::setDefault(AXRString propertyName, HSSUnit value)
{
    this->setDefault(propertyName, this->numberToConstant(value));
}

void HSSObject::setDefaultPercentage(AXRString propertyName, HSSUnit value)
{
    this->setDefault(propertyName, this->percentageToConstant(value));
}

void HSSObject::setDefault(AXRString propertyName, AXRString value)
{
    this->setDefault(propertyName, this->stringToConstant(value));
}

void HSSObject::setDefaultKw(AXRString propertyName, AXRString value)
{
    this->setDefault(propertyName, this->stringToKeyword(value));
}

void HSSObject::fillWithDefaults()
{
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator it;
    for (it = this->_defaultValues.begin(); it != this->_defaultValues.end(); ++it)
    {
        AXRString propertyName = it->first;
        QSharedPointer<HSSObject> defaultValue = it->second;
        if (this->needsDefault(propertyName))
        {
            this->setComputed(propertyName, defaultValue);
        }
    }
}

bool HSSObject::needsDefault(AXRString propertyName)
{
    if (this->_needsDefault.count(propertyName))
    {
        return this->_needsDefault[propertyName];
    }
    return true;
}

void HSSObject::setNeedsDefault(AXRString propertyName, bool value)
{
    this->_needsDefault[propertyName] = value;
}

bool HSSObject::hasStackValue(AXRString property) const
{
    return this->_stackValues.count(property) > 0;
}

QSharedPointer<HSSObject> HSSObject::getStackValue(AXRString property) const
{
    if (this->hasStackValue(property))
    {
        return this->_stackValues.at(property);
    }
    return QSharedPointer<HSSObject>();
}

void HSSObject::clearStackValues()
{
    this->_stackValues.clear();
}

void HSSObject::setStackNode(AXRString propertyName, QSharedPointer<AXR::HSSParserNode> parserNode)
{
    //ignore invalid values
    ///@todo this is not doing anything anywhere, yet. This needs to be discussed
    ///and implemented accordingly in all the subclasses
    if (!this->validate(propertyName, parserNode))
    {
        return;
    }
    //handle with callback
    if (this->_stackCallbacks.count(propertyName))
    {
        HSSAbstractStackCallback * callback = this->_stackCallbacks[propertyName];
        callback->call(parserNode);
        return;
    }

    if (propertyName == "isA")
    {
        this->appendStackValue(propertyName, this->computeValue(propertyName, parserNode));
    }
    else
    {
        this->setStackValue(propertyName, this->computeValue(propertyName, parserNode));
    }
}

void HSSObject::setStackValue(AXRString propertyName, QSharedPointer<HSSObject> theObject)
{
    if (!theObject) return;
    if (this->_stackValues.count(propertyName))
    {
        QSharedPointer<HSSObject> current = this->_stackValues[propertyName];
        if (current->isDefaultPropertyPathObject())
        {
            const std::vector<QSharedPointer<HSSPropertyDefinition> > & modifiers = current->modifierGet(propertyName);
            std::vector<QSharedPointer<HSSPropertyDefinition> >::const_iterator it;
            for (it = modifiers.begin(); it != modifiers.end(); ++it)
            {
                const QSharedPointer<HSSPropertyDefinition> & propDef = *it;
                QSharedPointer<HSSPropertyPath> propPath = propDef->getPaths().front();
                propPath->setStackNode(theObject, propDef->getValue());
            }
        }
        if (theObject->getSpecificity() >= current->getSpecificity())
        {
            this->_stackValues[propertyName] = theObject;
            this->setNeedsDefault(propertyName, false);
        }
    }
    else
    {
        this->_stackValues[propertyName] = theObject;
        this->setNeedsDefault(propertyName, false);
    }
}

void HSSObject::appendStackValue(AXRString propertyName, QSharedPointer<HSSObject> theObject)
{
    if (!theObject) return;
    if (this->_stackValues.count(propertyName) == 0)
    {
        this->setStackValue(propertyName, theObject);
    }
    else
    {
        QSharedPointer<HSSObject> stackValue = this->_stackValues[propertyName];
        if (stackValue)
        {
            if (stackValue->isA(HSSObjectTypeMultipleValue))
            {
                QSharedPointer<HSSMultipleValue> multiVal = qSharedPointerCast<HSSMultipleValue> (stackValue);
                multiVal->add(theObject);
                multiVal->setSpecificity(theObject->getSpecificity());
            }
            else
            {
                QSharedPointer<HSSMultipleValue> multiVal(new HSSMultipleValue(this->getController()));
                multiVal->add(stackValue);
                multiVal->add(theObject);
                multiVal->setSpecificity(theObject->getSpecificity());
                this->setStackValue(propertyName, multiVal);
            }
        }
    }
}

void HSSObject::commitStackValues()
{
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator it;
    for (it = this->_stackValues.begin(); it != this->_stackValues.end(); ++it)
    {
        this->setComputed(it->first, it->second);
    }
}

QSharedPointer<HSSObject> HSSObject::computeValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    QSharedPointer<HSSObject> errorState;
    //set the hosting property
    parserNode->setHostProperty(propertyName);

    if (parserNode->isA(HSSParserNodeTypeObjectNameConstant))
    {
        QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (parserNode);
        HSSUnit specificity = parserNode->getSpecificity();
        AXRString namestring = objname->getValue();
        QSharedPointer<HSSObjectDefinition> searchResult = this->getController()->objectTreeNodeNamed(namestring);
        if (searchResult)
        {
            parserNode = searchResult->clone();
            parserNode->setSpecificity(specificity);
        }
        else
        {
            AXRError("HSSObject", AXRString("Object with name ") + namestring + " was not found").raise();
            return errorState;
        }
    }

    //handle multiple values
    if (parserNode->isA(HSSParserNodeTypeMultipleValueDefinition))
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValue> valueList(new HSSMultipleValue(this->getController()));
        valueList->setSpecificity(parserNode->getSpecificity());
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (parserNode);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            valueList->add(this->computeValue(propertyName, *iterator));
        }
        return valueList;
    }

    //let the specific subclass handle the property
    if (this->_computeCallbacks.count(propertyName))
    {
        HSSAbstractComputeCallback * callback = this->_computeCallbacks[propertyName];
        return callback->call(parserNode);
    }

    return this->computeObject(parserNode, propertyName);
}

QSharedPointer<HSSObject> HSSObject::computeObject(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    QSharedPointer<HSSObject> theObj;

    //handle object definitions
    if (parserNode->isA(HSSStatementTypeObjectDefinition))
    {
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (parserNode);
        objdef->applyStack();
        objdef->applyRules();
        theObj = objdef->getObject();
        theObj->setSpecificity(objdef->getSpecificity());
    }
    else
    {
        //no special handling, just wrap in a @value
        theObj = this->computeValueObject(parserNode, propertyName);
    }

    return theObj;
}

QSharedPointer<HSSObject> HSSObject::computeValueObject(QSharedPointer<HSSParserNode> parserNode)
{
    QSharedPointer<HSSDisplayObject> thisObj;
    QSharedPointer<HSSSimpleSelection> scope;

    if (this->isA(HSSObjectTypeContainer) || this->isA(HSSObjectTypeTextBlock))
    {
        thisObj = qSharedPointerCast<HSSDisplayObject>(this->shared_from_this());
        QSharedPointer<HSSContainer> parent = thisObj->getParent();
        if (parent)
        {
            scope = parent->getChildren();
        }
        else
        {
            QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this->getController()));
            rootScope->add(thisObj);
            scope = rootScope;
        }
    }
    else
    {
        thisObj = this->getThisObj();
        scope = this->getScope();
    }
    QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), parserNode, parserNode->getSpecificity(), thisObj, scope);
    return valueObj;
}

QSharedPointer<HSSObject> HSSObject::computeValueObject(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (!parserNode)
    {
        AXRWarning("HSSObject", "Creating value object from empty parser node.");
        return QSharedPointer<HSSValue>();
    }
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            if (qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "inherit")
            {
                QSharedPointer<HSSParserNode> newNode = this->_inheritProperty(propertyName);
                newNode->setSpecificity(parserNode->getSpecificity());
                parserNode = newNode;
            }
            break;
        }

        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSParserNode> newNode = this->getPercentageExpression(parserNode, propertyName);
            newNode->setSpecificity(parserNode->getSpecificity());
            parserNode = newNode;
            break;
        }

        default:
            break;
    }

    QSharedPointer<HSSObject> computedObj = this->computeValueObject(parserNode);
    if (computedObj)
    {
        QSharedPointer<HSSValue> ret = qSharedPointerCast<HSSValue>(computedObj);
        ret->setHostProperty(propertyName);
        return ret;
    }
    return computedObj; //error state
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpression(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (!parserNode || !parserNode->isA(HSSParserNodeTypePercentageConstant))
    {
        AXRError("HSSObject", "You can only create a percentage expression from a percentage constant.");
        return QSharedPointer<HSSParserNode>();
    }

    //by default, get the same property from the parent
    HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
    return this->getPercentageExpressionFromParent(number, propertyName);
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpressionFromParent(HSSUnit number, AXRString propertyName)
{
    QSharedPointer<HSSParentSelector> parentSelector(new HSSParentSelector(this->getController()));
    QSharedPointer<HSSSimpleSelector> simpleSelector(new HSSSimpleSelector(this->getController()));
    simpleSelector->setName(parentSelector);
    return this->getPercentageExpression(number, simpleSelector, propertyName);
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpressionFromThis(HSSUnit number, AXRString propertyName)
{
    QSharedPointer<HSSThisSelector> thisSelector(new HSSThisSelector(this->getController()));
    QSharedPointer<HSSSimpleSelector> simpleSelector(new HSSSimpleSelector(this->getController()));
    simpleSelector->setName(thisSelector);
    return this->getPercentageExpression(number, simpleSelector, propertyName);
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpression(HSSUnit number, QSharedPointer<HSSSimpleSelector> target, AXRString propertyName)
{
    //left
    QSharedPointer<HSSRefFunction> refFunc(new HSSRefFunction(this->getController()));
    refFunc->setPropertyName(propertyName);
    std::vector< QSharedPointer<HSSSelectorChain> > selectorChains;
    QSharedPointer<HSSSelectorChain> selectorChain(new HSSSelectorChain(this->getController()));

    selectorChain->add(target);
    selectorChains.push_back(selectorChain);
    refFunc->setSelectorChains(selectorChains);
    refFunc->setLogsSelections(false);

    //right
    QSharedPointer<HSSNumberConstant> right = this->numberToConstant(number);

    QSharedPointer<HSSMultiplication> ret(new HSSMultiplication(refFunc, right, this->getController()));
    return ret;
}

void HSSObject::setComputedValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode, HSSUnit specificity)
{
    QSharedPointer<HSSObject> theObj = this->computeValue(propertyName, parserNode);
    if (theObj)
    {
        theObj->setSpecificity(specificity);
        this->setComputed(propertyName, theObj);
    }
}

void HSSObject::setComputedValue(AXRString propertyName, HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue(propertyName, this->numberToConstant(value), specificity);
}

void HSSObject::setComputedBool(AXRString propertyName, bool value, HSSUnit specificity)
{
    this->setComputedValue(propertyName, this->stringToKeyword(value ? "yes" : "no"), specificity);
}

void HSSObject::setComputedValue(AXRString propertyName, AXRString value, HSSUnit specificity)
{
    this->setComputedValue(propertyName, this->stringToConstant(value), specificity);
}

QSharedPointer<HSSNumberConstant> HSSObject::numberToConstant(HSSUnit value)
{
    return QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(value, this->getController()));
}

QSharedPointer<HSSPercentageConstant> HSSObject::percentageToConstant(HSSUnit value)
{
    return QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(value, this->getController()));
}

QSharedPointer<HSSStringConstant> HSSObject::stringToConstant(AXRString value)
{
    return HSSStringConstant::stringToConstant(value, this->getController());
}

QSharedPointer<HSSKeywordConstant> HSSObject::stringToKeyword(AXRString value)
{
    QSharedPointer<HSSKeywordConstant> ret = QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(value, this->getController()));
    return ret;
}

void HSSObject::setComputed(AXRString propertyName, QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSObject> currentValue = this->getComputedValue(propertyName);

    //prepare
    theObj->setHostProperty(propertyName);
    theObj->commitStackValues();
    //if the object is equal, reset the specificity
    if (currentValue && currentValue->equalTo(theObj))
    {
        currentValue->setSpecificity(theObj->getSpecificity());
        
        //set this object and scope
        ///@todo this should target any subclass of display object
        if (this->isA(HSSObjectTypeContainer) || this->isA(HSSObjectTypeTextBlock))
        {
            QSharedPointer<HSSDisplayObject> thisDO = qSharedPointerCast<HSSDisplayObject>(this->shared_from_this());
            theObj->setThisObj(thisDO);
            QSharedPointer<HSSContainer> parent = thisDO->getParent();
            if (parent)
            {
                currentValue->setScope(parent->getChildren());
            }
            else
            {
                QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this->getController()));
                rootScope->add(thisDO);
                currentValue->setScope(rootScope);
            }
        }
        else
        {
            currentValue->setThisObj(this->getThisObj());
            currentValue->setScope(this->getScope());
        }
        return;
    }
    if (!currentValue || (currentValue->getSpecificity() <= theObj->getSpecificity()))
    {
        //replace
        if (currentValue)
        {
            currentValue->replace(theObj);
        }
        ///@todo this should target any subclass of display object
        if (this->isA(HSSObjectTypeContainer) || this->isA(HSSObjectTypeTextBlock))
        {
            QSharedPointer<HSSDisplayObject> thisDO = qSharedPointerCast<HSSDisplayObject>(this->shared_from_this());
            theObj->setThisObj(thisDO);
            QSharedPointer<HSSContainer> parent = thisDO->getParent();
            if (parent)
            {
                theObj->setScope(parent->getChildren());
            }
            else
            {
                QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this->getController()));
                rootScope->add(thisDO);
                theObj->setScope(rootScope);
            }
        }
        else
        {
            theObj->setThisObj(this->getThisObj());
            theObj->setScope(this->getScope());
        }

        //listen
        if (this->_listenCallbacks.count(propertyName))
        {
            HSSAbstractObserveCallback * callback = this->_listenCallbacks[propertyName];
            callback->call(theObj);
        }
        else
        {
            if (theObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode)
                {
                    switch (parserNode->getType())
                    {
                        case HSSParserNodeTypeExpression:
                        case HSSParserNodeTypeFunctionCall:
                        {
                            theObj->observe("__impl_private__valueChanged", propertyName, this, new HSSValueChangedCallback<HSSObject>(this, &HSSObject::propertyChanged));
                            break;
                        }

                        default:
                            break;
                    }
                }
            }
        }
        //set
        if (this->_setCallbacks.count(propertyName))
        {
            HSSAbstractObserveCallback * callback = this->_setCallbacks[propertyName];
            callback->call(theObj);
        }
        else
        {
            this->_simpleInsertComputed(propertyName, theObj);
        }
        //notify
        if (this->_notifyCallbacks.count(propertyName))
        {
            HSSAbstractObserveCallback * callback = this->_notifyCallbacks[propertyName];
            callback->call(theObj);
        }
        else
        {
            this->notifyObservers(propertyName, theObj);
        }
    }
}

void HSSObject::_simpleInsertComputed(AXRString propertyName, QSharedPointer<HSSObject> theObj)
{
    this->_computedValues[propertyName] = theObj;
}

QSharedPointer<HSSObject> HSSObject::getComputedValue(AXRString property) const
{
    QSharedPointer<HSSObject> ret;
    if (this->_computedValues.count(property))
    {
        ret = this->_computedValues.at(property);
    }
    return ret;
}

HSSUnit HSSObject::getComputedNumber(AXRString property) const
{
    QSharedPointer<HSSObject> value = this->getComputedValue(property);
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    QSharedPointer<HSSObject> defaultValue = this->getDefaultValue(property);
    if (defaultValue && defaultValue->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(defaultValue)->getNumber();
    }
    return 0.;
}

HSSUnit HSSObject::getComputedBool(AXRString property) const
{
    QSharedPointer<HSSObject> value = this->getComputedValue(property);
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getBool();
    }
    QSharedPointer<HSSObject> defaultValue = this->getDefaultValue(property);
    if (defaultValue && defaultValue->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(defaultValue)->getBool();
    }
    return FALSE;
}

AXRString HSSObject::getComputedString(AXRString property) const
{
    QSharedPointer<HSSObject> value = this->getComputedValue(property);
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getString();
    }
    QSharedPointer<HSSObject> defaultValue = this->getDefaultValue(property);
    if (defaultValue && defaultValue->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(defaultValue)->getString();
    }
    return "";
}

QSharedPointer<HSSObject> HSSObject::getComputedObject(AXRString property) const
{
    QSharedPointer<HSSObject> value = this->getComputedValue(property);
    if (value)
    {
        if (value->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(value);
            if (valueObj->getValue())
            {
                return qSharedPointerCast<HSSValue>(value)->getObject();
            }
        }
        return value;
    }
    QSharedPointer<HSSObject> defaultValue = this->getDefaultValue(property);
    if (defaultValue && defaultValue->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(defaultValue)->getObject();
    }
    return value;
}

void HSSObject::clearComputedValues()
{
    this->_computedValues.clear();
}

QSharedPointer<HSSObject> HSSObject::inheritValue(AXRString inheritProperty, AXRString hostProperty)
{
    return this->computeValueObject(this->_inheritProperty(inheritProperty), hostProperty);
}

QSharedPointer<HSSParserNode> HSSObject::_inheritProperty(AXRString propertyName) const
{
    QSharedPointer<HSSRefFunction> refFunc(new HSSRefFunction(this->getController()));
    refFunc->setPropertyName(propertyName);
    std::vector< QSharedPointer<HSSSelectorChain> > selectorChains;
    QSharedPointer<HSSSelectorChain> selectorChain(new HSSSelectorChain(this->getController()));
    QSharedPointer<HSSSimpleSelector> simpleSelector(new HSSSimpleSelector(this->getController()));
    simpleSelector->setName(QSharedPointer<HSSParentSelector>(new HSSParentSelector(this->getController())));
    selectorChain->add(simpleSelector);
    selectorChains.push_back(selectorChain);
    refFunc->setSelectorChains(selectorChains);
    refFunc->setLogsSelections(false);
    return refFunc;
}

void HSSObject::propertyChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    //notify
    if (this->_notifyCallbacks.count(source))
    {
        HSSAbstractObserveCallback * callback = this->_notifyCallbacks[source];
        callback->call(theObj);
    }
    else
    {
        this->notifyObservers(source, theObj);
    }
}

AXRString HSSObject::getHostProperty() const
{
    return this->_hostProperty;
}

void HSSObject::setHostProperty(AXRString newValue)
{
    this->_hostProperty = newValue;
}

HSSUnit HSSObject::getSpecificity() const
{
    return this->_specificity;
}

void HSSObject::setSpecificity(HSSUnit newValue)
{
    this->_specificity = newValue;
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator it;
    for (it = this->_stackValues.begin(); it != this->_stackValues.end(); ++it)
    {
        QSharedPointer<HSSObject> theObj = it->second;
        theObj->setSpecificity(newValue);
    }
    std::map<AXRString, QSharedPointer<HSSObject> >::iterator it2;
    for (it2 = this->_computedValues.begin(); it2 != this->_computedValues.end(); ++it2)
    {
        QSharedPointer<HSSObject> theObj = it2->second;
        theObj->setSpecificity(newValue);
    }
}

void HSSObject::replace(QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("__impl_private__replace", theObj);
}

void HSSObject::replaceByPropertyPath(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSObject> currentValue = this->getComputedValue(source);
    const std::vector<QSharedPointer<HSSPropertyDefinition> > & modifiers = currentValue->modifierGet(source);
    std::vector<QSharedPointer<HSSPropertyDefinition> >::const_iterator it;
    for (it = modifiers.begin(); it != modifiers.end(); ++it)
    {
        const QSharedPointer<HSSPropertyDefinition> & propDef = *it;
        QSharedPointer<HSSPropertyPath> propPath = propDef->getPaths().front();
        propPath->applyModifier(theObj, propDef->getValue());
    }
}

bool HSSObject::isDefaultPropertyPathObject() const
{
    return this->_isDefaultPropertyPathObject;
}

void HSSObject::setDefaultPropertyPathObject(bool newValue)
{
    this->_isDefaultPropertyPathObject = newValue;
}

bool HSSObject::expectsIsAIncluded() const
{
    return this->_expectsIsAIncluded;
}

void HSSObject::setExpectsIsAIncluded(bool value)
{
    this->_expectsIsAIncluded = value;
}

void HSSObject::objDefRulesAdd(QSharedPointer<HSSRule> rule)
{
    this->_objDefRules.push_back(rule);
}

void HSSObject::objDefRulesPrepend(QSharedPointer<HSSRule> rule)
{
    this->_objDefRules.push_front(rule);
}

std::deque<QSharedPointer<HSSRule> > HSSObject::getObjDefRules() const
{
    return this->_objDefRules;
}

void HSSObject::modifierAdd(AXRString propertyName, QSharedPointer<HSSPropertyDefinition> propertyDef)
{
    if (this->_modifiers.count(propertyName))
    {
        std::vector<QSharedPointer<HSSPropertyDefinition> > vector = this->_modifiers[propertyName];
        vector.push_back(propertyDef);
        this->_modifiers[propertyName] = vector;
    }
    else
    {
        std::vector<QSharedPointer<HSSPropertyDefinition> > vector;
        vector.push_back(propertyDef);
        this->_modifiers[propertyName] = vector;
    }
}

std::vector<QSharedPointer<HSSPropertyDefinition> > HSSObject::modifierGet(AXRString propertyName) const
{
    if (this->_modifiers.count(propertyName))
    {
        return this->_modifiers.at(propertyName);
    }
    return std::vector<QSharedPointer<HSSPropertyDefinition> >();
}

void HSSObject::unlinkPropertyPaths()
{
    std::map<AXRString, QSharedPointer<HSSObject> >::const_iterator it;
    for (it = this->_computedValues.begin(); it != this->_computedValues.end(); ++it)
    {
        const QSharedPointer<HSSObject> &theObj = it->second;
        const AXRString hostProperty = theObj->getHostProperty();
        AXRString trackedProperty = this->getTrackedProperty(hostProperty);
        if (trackedProperty == "__impl_private__replace")
        {
            HSSObservable * theObservable = this->getTrackedObserver(hostProperty);
            if (theObservable != NULL)
            {
                theObservable->removeObserver("__impl_private__replace", hostProperty, this);
            }
        }
    }
}

void HSSObject::setOverrides(std::vector<QSharedPointer<HSSPropertyDefinition> > overrides)
{
    std::vector<QSharedPointer<HSSPropertyDefinition> >::iterator it;
    for (it = overrides.begin(); it != overrides.end(); ++it) {
        QSharedPointer<HSSPropertyDefinition> item = *it;
        this->addOverride(item);
    }
}

void HSSObject::addOverride(QSharedPointer<HSSPropertyDefinition> item)
{
    this->_overrides.push_back(item);
}

std::map<AXRString, QSharedPointer<HSSPropertyDefinition> > HSSObject::getOverrides() const
{
    return this->_overrides;
}

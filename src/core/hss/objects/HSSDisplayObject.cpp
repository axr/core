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

#include <cmath>
#include <QStack>
#include <QVariant>
#include <QtGlobal>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSEvent.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSFlag.h"
#include "HSSFont.h"
#include "HSSKeywordConstant.h"
#include "HSSLinearGradient.h"
#include "HSSMargin.h"
#include "HSSMouseEvent.h"
#include "HSSMultipleValue.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSNumberConstant.h"
#include "HSSObject.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSParserNode.h"
#include "HSSPercentageConstant.h"
#include "HSSPoint.h"
#include "HSSPropertyDefinition.h"
#include "HSSPropertyPath.h"
#include "HSSPropertyPathNode.h"
#include "HSSRadialGradient.h"
#include "HSSRect.h"
#include "HSSRgb.h"
#include "HSSRule.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSSimpleSelector.h"
#include "HSSStringConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSDisplayObject::HSSDisplayObject(HSSObjectType type, AXRController * controller)
: HSSObject(type, controller)
{
    this->_initialize();
}

void HSSDisplayObject::_initialize()
{
    this->_isDirty = true;
    this->_needsRereadRules = true;
    this->_needsSurface = true;
    this->_needsLayout = true;
    this->_debugName = "unnamed";

    this->x = this->y = this->globalX = this->globalY = 0.;

    this->_anchorXdefault = this->_anchorYdefault = true;

    this->drawIndex = 0;
    this->_index = 0;
    this->tabIndex = 1;
    this->zoomFactor = 1;
    /**
     *  @todo change to camelCase
     */
    this->does_float = false;
    this->heightByContent = false;
    this->widthByContent = false;
    this->_isRoot = false;
    this->_hasOwnWidth = false;
    this->_hasOwnHeight = false;

    this->elementName = AXRString();

    this->_isHover = false;
    this->_isPress = false;
    this->_layoutFlagIsInSecondaryGroup = false;
    this->_layoutFlagIsInSecondaryLine = false;
    this->_layoutFlagVerticallyAligned = false;
    this->_layoutFlagLockTop = false;
    this->_layoutLockTopPosition = 0.;
    this->_layoutFlagLockBottom = false;
    this->_layoutLockBottomPosition = 0.;

    this->addCallback("width", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::listenWidth), new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyWidth));
    this->addCallback("height", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::listenHeight), new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyHeight));
    this->addCallback("anchorX", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAnchorX));
    this->addNotifyCallback("anchorX", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAnchorX));
    this->addCallback("anchorY", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAnchorY));
    this->addNotifyCallback("anchorY", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAnchorY));
    this->addCallback("alignX", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAlignX));
    this->addNotifyCallback("alignX", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAlignX));
    this->addCallback("alignY", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAlignY));
    this->addNotifyCallback("alignY", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAlignY));
    this->addCallback("background", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeBackground));
}

HSSDisplayObject::HSSDisplayObject(const HSSDisplayObject & orig)
: HSSObject(orig)
{
    this->_initialize();
    this->attributes = orig.attributes;
    this->elementName = orig.elementName;
    this->rules = orig.rules; //shallow copy
    this->_flagsStatus = orig._flagsStatus;

    this->drawIndex = orig.drawIndex;
    this->_index = orig._index;
    this->tabIndex = orig.tabIndex;
    this->heightByContent = orig.heightByContent;
    this->widthByContent = orig.widthByContent;

    this->_isHover = orig._isHover;
    this->_isPress = orig._isPress;
}

HSSDisplayObject::~HSSDisplayObject()
{
    axr_log(LoggerChannelGeneralSpecific, AXRString("destroying display object with name ").append(this->name));
    this->cleanTrackedObservers();
}

void HSSDisplayObject::setDefaults()
{
    this->setDefaultKw("background", "no");
    this->setDefaultKw("content", "no");
    this->setDefaultKw("on", "no");
    this->setDefaultKw("border", "no");
    if (this->isRoot())
    {
        this->setDefaultKw("visible", "yes");
    }
    else
    {
        this->setDefaultPercentage("width", 100.);
        this->setDefaultKw("height", "content");
        this->setDefaultKw("anchorX", "no");
        this->setDefaultKw("anchorY", "no");
        this->setDefaultKw("flow", "yes");
        this->setDefaultKw("contained", "yes");
        this->setDefaultKw("alignX", "auto");
        this->setDefaultKw("alignY", "auto");
        this->setDefaultKw("visible", "inherit");
    }
}

AXRString HSSDisplayObject::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSDisplayObject: ").append(this->name);
    }
    else
    {
        return "Anonymous HSSDisplayObject";
    }
}

AXRString HSSDisplayObject::defaultObjectType()
{
    return "displayObject";
}

AXRString HSSDisplayObject::defaultObjectType(AXRString property)
{
    if (property == "margin")
    {
        return "margin";
    }
    else if (property == "padding")
    {
        return "margin";
    }
    else if (property == "border")
    {
        return "lineBorder";
    }
    else if (property == "background")
    {
        return "linearGradient";
    }
    else if (property == "content")
    {
        return "image";
    }
    else if (property == "transform")
    {
        return "rotate";
    }
    else if (property == "effects")
    {
        return "shadow";
    }
    else if (property == "animation")
    {
        return "transition";
    }
    else if (property == "on")
    {
        return "click";
    }
    else if (property == "mask")
    {
        return "image";
    }
    else if (property == "font")
    {
        return "font";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSDisplayObject::isKeyword(AXRString value, AXRString property)
{
    if (property == "anchorX"
            || property == "anchorY"
            || property == "alignX"
            || property == "alignY"
            || property == "lineAlign"
            )
    {
        if (value == "top"
                || value == "middle"
                || value == "center"
                || value == "bottom"
                || value == "left"
                || value == "right"
                || value == "yes"
                || value == "no"
                )
        {
            return true;
        }
    }
    else if (property == "background")
    {
        if (value == "black" || value == "white" || value == "transparent")
        {
            return true;
        }
    }
    else if (property == "flow" || property == "visible" || property == "contained")
    {
        if (value == "yes" || value == "no")
        {
            return true;
        }
    }
    else if (property == "width")
    {
        if (value == "content")
        {
            return true;
        }
    }

    //    else if (property == "font") {
    //        if (   value == "thin"
    //            || value == "ultralight"
    //            || value == "light"
    //            || value == "normal"
    //            || value == "medium"
    //            || value == "semibold"
    //            || value == "bold"
    //            || value == "ultrabold"
    //            || value == "heavy"
    //            || value == "ultraheavy"){
    //            return true;
    //        }
    //    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getPercentageExpression(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (!parserNode || !parserNode->isA(HSSParserNodeTypePercentageConstant))
    {
        AXRError("HSSDisplayObject", "You can only create a percentage expression from a percentage constant.");
        return QSharedPointer<HSSParserNode>();
    }

    static QMap<AXRString, AXRString> parentMappings;
    if (parentMappings.empty())
    {
        parentMappings.insert("width", "innerWidth");
        parentMappings.insert("height", "innerHeight");
        parentMappings.insert("alignX", "innerWidth");
        parentMappings.insert("alignY", "innerHeight");
    }

    if (parentMappings.contains(propertyName))
    {
        HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
        return this->getPercentageExpressionFromParent(number, parentMappings[propertyName]);
    }

    static QMap<AXRString, AXRString> thisMappings;
    if (parentMappings.empty())
    {
        thisMappings.insert("anchorX", "width");
        thisMappings.insert("anchorY", "height");
    }

    if (thisMappings.contains(propertyName))
    {
        HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
        return this->getPercentageExpressionFromThis(number, thisMappings[propertyName]);
    }

    return HSSObject::getPercentageExpression(parserNode, propertyName);
}

bool HSSDisplayObject::canHaveChildren()
{
    return false;
}

QSharedPointer<HSSContainer> HSSDisplayObject::getParent() const
{
    QSharedPointer<HSSContainer> parent = this->parent.toStrongRef();
    return parent;
}

void HSSDisplayObject::setParent(QSharedPointer<HSSContainer> parent)
{
    this->parent = pp(parent);
}

void HSSDisplayObject::removeFromParent()
{
    this->getParent()->remove(this->getIndex());
}

QSharedPointer<HSSSimpleSelection> HSSDisplayObject::getSiblings()
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection());
    QSharedPointer<HSSSimpleSelection> children = this->getParent()->getChildren();
    for (HSSSimpleSelection::const_iterator it = children->begin(); it != children->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> theDO = *it;
        if(theDO.data() != this)
        {
            ret->add(theDO);
        }
    }
    return ret;
}

QSharedPointer<HSSSimpleSelection> HSSDisplayObject::getNextSiblings()
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection());
    QSharedPointer<HSSSimpleSelection> children = this->getParent()->getChildren();
    HSSSimpleSelection::iterator thisPos = std::find(children->begin(), children->end(), this->shared_from_this());
    ret->insert(ret->end(), thisPos+1, children->end());
    return ret;
}

QSharedPointer<HSSSimpleSelection> HSSDisplayObject::getPreviousSiblings()
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection());
    QSharedPointer<HSSSimpleSelection> children = this->getParent()->getChildren();
    HSSSimpleSelection::iterator thisPos = std::find(children->begin(), children->end(), this->shared_from_this());
    ret->insert(ret->end(), children->begin(), thisPos);
    return ret;
}

void HSSDisplayObject::setIndex(size_t newIndex)
{
    this->_index = newIndex;
}

size_t HSSDisplayObject::getIndex() const
{
    return this->_index;
}

void HSSDisplayObject::attributesAdd(AXRString name, AXRString value)
{
    this->attributes[name] = value;
}

void HSSDisplayObject::attributesRemove(AXRString name)
{
    this->attributes.erase(name);
}

AXRString HSSDisplayObject::getElementName()
{
    return this->elementName;
}

void HSSDisplayObject::setElementName(AXRString newName)
{
    this->elementName = newName;
    this->_debugName = newName.toStdString();
}

//rules
void HSSDisplayObject::rulesAdd(QSharedPointer<HSSRule> newRule, HSSRuleState defaultState)
{
    if (!this->hasRule(newRule))
    {
        newRule->observe("__impl_private__ruleChanged", "__impl_private__ruleAdd", this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::ruleChanged));

        newRule->appliedToAdd(this->shared_from_this());

        QSharedPointer<HSSRuleStatus> newRS = QSharedPointer<HSSRuleStatus>(new HSSRuleStatus());
        newRS->state = defaultState;
        newRS->rule = newRule;

        this->rules.push_back(newRS);

        //iterate over the property defs and try to find an isA property
        const std::vector<QSharedPointer<HSSPropertyDefinition> > & props = newRule->getProperties();
        for (HSSPropertyDefinition::const_it it = props.begin(); it != props.end(); ++it)
        {
            QSharedPointer<HSSPropertyDefinition> propdef = *it;
            Q_FOREACH(QSharedPointer<HSSPropertyPath> path, propdef->getPaths()){
                if (path->size() == 1 && path->front()->getPropertyName() == "isA")
                {
                    this->rulesAddIsAChildren(propdef, defaultState, newRule);
                }
            }
        }
    }
}

void HSSDisplayObject::rulesAddIsAChildren(QSharedPointer<HSSPropertyDefinition> propdef, HSSRuleState defaultState, QSharedPointer<HSSRule> & parentRule)
{
    QSharedPointer<HSSParserNode> value = propdef->getValue();

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());

            if (this->isA(HSSObjectTypeContainer))
            {
                QSharedPointer<HSSContainer> thisCont = qSharedPointerCast<HSSContainer > (this->shared_from_this());
                this->getController()->currentContext().push(thisCont);

                //iterate over the property defs and try to find an isA property
                const std::deque<QSharedPointer<HSSPropertyDefinition> > & props = objdef->getProperties();
                for (std::deque<QSharedPointer<HSSPropertyDefinition> >::const_iterator pIt =
                     props.begin(); pIt != props.end(); ++pIt)
                {
                    QSharedPointer<HSSPropertyDefinition> propdef = *pIt;
                    Q_FOREACH(QSharedPointer<HSSPropertyPath> path, propdef->getPaths()){
                        if (path->size() == 1 && path->front()->getPropertyName() == "isA")
                        {
                            this->rulesAddIsAChildren(propdef, defaultState, parentRule);
                        }
                    }
                }

                const std::deque<QSharedPointer<HSSRule> > rules = objdef->getRules();
                for (std::deque<QSharedPointer<HSSRule> >::const_reverse_iterator it =
                     rules.rbegin(); it != rules.rend(); ++it)
                {
                    QSharedPointer<HSSRule> clonedRule = (*it)->clone();
                    //we attach the cloned rule to the rule that has the isA
                    parentRule->childrenPrependOnce(clonedRule);
                }
                this->getController()->currentContext().pop();
            }
        }
        catch (const AXRError &e)
        {
            e.raise();
        }

        break;
    }

    default:
    {
        break;
    }
    }
}

QSharedPointer<HSSRule> HSSDisplayObject::rulesGet(unsigned index)
{
    return this->rules[index]->rule;
}

void HSSDisplayObject::rulesRemove(unsigned index)
{
    QSharedPointer<HSSRule> rule = this->rules[index]->rule;
    Q_FOREACH(QSharedPointer<HSSSelectorChain> selectorChain, rule->getSelectorChains())
    {
        Q_FOREACH(QSharedPointer<HSSParserNode> parserNode, selectorChain->getNodeList())
        {
            if (parserNode->isA(HSSSelectorTypeSimpleSelector))
            {
                QSharedPointer<HSSSimpleSelector> simpleSelector = qSharedPointerCast<HSSSimpleSelector>(parserNode);
                Q_FOREACH(QSharedPointer<HSSFilter> filter, simpleSelector->getFilters())
                {
                    if (filter->isA(HSSFilterTypeFlag))
                    {
                        QSharedPointer<HSSFlag> theFlag = qSharedPointerCast<HSSFlag>(filter);
                        this->removeFlag(theFlag);
                    }
                }
            }
        }
    }

    this->rules.erase(this->rules.begin() + index);
    this->setNeedsRereadRules(true);
}

void HSSDisplayObject::rulesRemoveLast()
{
    this->rules.pop_back();
    this->setNeedsRereadRules(true);
}

size_t HSSDisplayObject::rulesSize() const
{
    return this->rules.size();
}

void HSSDisplayObject::setRuleStatus(QSharedPointer<HSSRule> rule, HSSRuleState newValue)
{
    bool changed = false;
    bool found = false;

    std::vector<QSharedPointer<HSSRuleStatus> >::iterator it;
    for (it = this->rules.begin(); it != this->rules.end(); ++it)
    {
        QSharedPointer<HSSRuleStatus> rs = *it;
        QSharedPointer<HSSRule> existingRule = rs->rule;
        if (existingRule.data() == rule.data())
        {
            found = true;
            if (newValue == HSSRuleStatePurge)
            {
                if (rs->state == HSSRuleStateOn || rs->state == HSSRuleStateActivate)
                {
                    rs->state = newValue;
                    changed = true;
                    break;
                }
            }
            else
            {
                if (rs->state != newValue)
                {
                    rs->state = newValue;
                    changed = true;
                    break;
                }
            }
        }
    }

    if (!found)
    {
        this->rulesAdd(rule, newValue);
        changed = true;
    }

    if (changed)
    {
        this->setNeedsRereadRules(true);
        this->getController()->document()->setNeedsDisplay(true);
    }
}

bool HSSDisplayObject::hasRule(QSharedPointer<HSSRule> rule)
{
    bool found = false;
    std::vector<QSharedPointer<HSSRuleStatus> >::iterator it;
    for (it = this->rules.begin(); it != this->rules.end(); ++it)
    {
        QSharedPointer<HSSRuleStatus> status = *it;
        if (rule == status->rule)
        {
            found = true;
        }
    }
    return found;
}

void HSSDisplayObject::setNeedsRereadRules(bool value)
{
    this->_needsRereadRules = value;
}

bool HSSDisplayObject::needsRereadRules()
{
    return this->_needsRereadRules;
}

void HSSDisplayObject::setNeedsSurface(bool value)
{
    this->_needsSurface = value;
}

bool HSSDisplayObject::needsSurface()
{
    return this->_needsSurface;
}

void HSSDisplayObject::setDirty(bool value)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSDisplayObject: setting dirty on " + this->elementName);
    this->_isDirty = value;
}

bool HSSDisplayObject::isDirty()
{
    return this->_isDirty;
}

void HSSDisplayObject::accept(HSSAbstractVisitor* visitor, HSSVisitorFilterFlags filterFlags)
{
}

//layout
void HSSDisplayObject::setNeedsLayout(bool value)
{
    this->_needsLayout = value;
}

bool HSSDisplayObject::needsLayout() const
{
    return this->_needsLayout;
}

void HSSDisplayObject::recursiveResetLayout()
{
    this->setNeedsLayout(true);
}

void HSSDisplayObject::setGlobalX(HSSUnit newValue)
{
    this->globalX = newValue;
}

void HSSDisplayObject::setGlobalY(HSSUnit newValue)
{
    this->globalY = newValue;
}

void HSSDisplayObject::setWidth(HSSUnit value)
{
    this->setComputedValue("width", value);
}

const HSSUnit HSSDisplayObject::getWidth() const
{
    return this->getComputedNumber("width");
}

const HSSUnit HSSDisplayObject::getInnerWidth() const
{
    return this->getComputedNumber("innerWidth");
}

const HSSUnit HSSDisplayObject::getOuterWidth() const
{
    return this->getComputedNumber("outerWidth");
}

void HSSDisplayObject::listenWidth(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        switch (qSharedPointerCast<HSSValue>(theObj)->getValue()->getType())
        {
            case HSSParserNodeTypeExpression:
            case HSSParserNodeTypeFunctionCall:
            {
                theObj->observe("__impl_private__valueChanged", "width", this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::widthChanged));
                break;
            }

            default:
                break;
        }
    }
}

void HSSDisplayObject::notifyWidth(QSharedPointer<HSSObject> theObj)
{
    this->widthByContent = false;
    this->_hasOwnWidth = false;
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "content")
        {
            this->widthByContent = true;
        }
        else if (parserNode->isA(HSSParserNodeTypeNumberConstant))
        {
            this->_hasOwnWidth = true;
        }
    }
    this->_setInnerWidth();
    this->_setOuterWidth();
    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
    this->notifyObservers("width", theObj);
}

void HSSDisplayObject::widthChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    //FIXME: handle width by content
    //    if (this->widthByContent)
    //    {
    //        this->width = floor(*(HSSUnit*) data);
    //    }

    this->_setInnerWidth();
    this->_setOuterWidth();

    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
    this->notifyObservers("width", this->getComputedValue("width"));
}

void HSSDisplayObject::setHeight(HSSUnit value)
{
    this->setComputedValue("height", value);
}

const HSSUnit HSSDisplayObject::getHeight() const
{
    return this->getComputedNumber("height");
}

const HSSUnit HSSDisplayObject::getInnerHeight() const
{
    return this->getComputedNumber("innerHeight");
}

const HSSUnit HSSDisplayObject::getOuterHeight() const
{
    return this->getComputedNumber("outerHeight");
}

void HSSDisplayObject::listenHeight(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        switch (qSharedPointerCast<HSSValue>(theObj)->getValue()->getType())
        {
            case HSSParserNodeTypeFunctionCall:
            case HSSParserNodeTypeExpression:
            {
                theObj->observe("__impl_private__valueChanged", "height", this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::heightChanged));
                break;
            }

            default:
                break;
        }
    }
}

void HSSDisplayObject::notifyHeight(QSharedPointer<HSSObject> theObj)
{
    this->heightByContent = false;
    const QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        this->_hasOwnHeight = !parent->heightByContent;
        parent->setNeedsLayout(true);
    }
    else
    {
        this->_hasOwnHeight = true;
    }
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "content")
        {
            this->heightByContent = true;
            this->_hasOwnHeight = true;
        }
        else if (parserNode->isA(HSSParserNodeTypeNumberConstant))
        {
            this->_hasOwnHeight = true;
        }
    }
    this->_setInnerHeight();
    this->_setOuterHeight();
    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
    this->notifyObservers("height", theObj);
}

void HSSDisplayObject::heightChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    const QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->_setInnerHeight();
    this->_setOuterHeight();
    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
    this->notifyObservers("height", this->getComputedValue("height"));
}

void HSSDisplayObject::setAnchorX(HSSUnit value)
{
    this->setComputedValue("anchorX", value);
}

const HSSUnit HSSDisplayObject::getAnchorX() const
{
    return this->getComputedNumber("anchorX");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAnchorX(QSharedPointer<HSSParserNode> parserNode)
{
    this->_anchorXdefault = false;
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "no" || kwValue == "default")
            {
                this->_anchorXdefault = true;
            }
            else if (kwValue == "left")
            {
                return this->computeValueObject(this->percentageToConstant(0), "anchorX");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "anchorX");
            }
            else if (kwValue == "right")
            {
                return this->computeValueObject(this->percentageToConstant(100), "anchorX");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "anchorX");
}

void HSSDisplayObject::notifyAnchorX(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorX", theObj);
}

void HSSDisplayObject::anchorXChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->_anchorXdefault = false;
    QSharedPointer<HSSObject> currentValue = this->getComputedValue("anchorX");
    if (currentValue->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(currentValue)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant> (parserNode);
            if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
            {
                this->_anchorXdefault = true;
            }
        }
    }
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorX", theObj);
}

void HSSDisplayObject::setAnchorY(HSSUnit value)
{
    this->setComputedValue("anchorY", value);
}

const HSSUnit HSSDisplayObject::getAnchorY() const
{
    return this->getComputedNumber("anchorY");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAnchorY(QSharedPointer<HSSParserNode> parserNode)
{
    this->_anchorYdefault = false;
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "no" || kwValue == "default")
            {
                this->_anchorYdefault = true;
            }
            else if (kwValue == "top")
            {
                return this->computeValueObject(this->percentageToConstant(0), "anchorY");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "anchorY");
            }
            else if (kwValue == "bottom")
            {
                return this->computeValueObject(this->percentageToConstant(100), "anchorY");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "anchorY");
}

void HSSDisplayObject::notifyAnchorY(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorY", theObj);
}

void HSSDisplayObject::anchorYChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->_anchorYdefault = false;
    QSharedPointer<HSSObject> currentValue = this->getComputedValue("anchorY");
    if (currentValue->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(currentValue)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant> (parserNode);
            if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
            {
                this->_anchorYdefault = true;
            }
        }
    }
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorY", theObj);
}


//flow
bool HSSDisplayObject::getFlow()
{
    return this->getComputedBool("flow");
}

//contained
bool HSSDisplayObject::getContained()
{
    return this->getComputedBool("contained");
}

void HSSDisplayObject::setAlignX(HSSUnit value)
{
    this->setComputedValue("alignX", value);
}

const HSSUnit HSSDisplayObject::getAlignX() const
{
    return this->getComputedNumber("alignX");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAlignX(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "auto")
            {
                return this->inheritValue("contentAlignX", "alignX");
            }
            else if (kwValue == "left")
            {
                return this->computeValueObject(this->percentageToConstant(0), "alignX");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "alignX");
            }
            else if (kwValue == "right")
            {
                return this->computeValueObject(this->percentageToConstant(100), "alignX");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "alignX");
}

void HSSDisplayObject::notifyAlignX(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignX", theObj);
}

void HSSDisplayObject::alignXChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignX", theObj);
}

void HSSDisplayObject::setAlignY(HSSUnit value)
{
    this->setComputedValue("alignY", value);
}

const HSSUnit HSSDisplayObject::getAlignY() const
{
    return this->getComputedNumber("alignY");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAlignY(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "auto")
            {
                return this->inheritValue("contentAlignY", "alignY");
            }
            else if (kwValue == "top")
            {
                return this->computeValueObject(this->percentageToConstant(0), "alignY");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "alignY");
            }
            else if (kwValue == "bottom")
            {
                return this->computeValueObject(this->percentageToConstant(100), "alignY");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "alignY");
}

void HSSDisplayObject::notifyAlignY(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignY", theObj);
}

void HSSDisplayObject::alignYChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignY", theObj);
}

const HSSUnit HSSDisplayObject::getLineAlign() const
{
    return this->getComputedNumber("lineAlign");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeLineAlign(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "top" || kwValue == "left")
            {
                return this->computeValueObject(this->percentageToConstant(0));
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50));
            }
            else if (kwValue == "bottom" || kwValue == "right")
            {
                return this->computeValueObject(this->percentageToConstant(100));
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "lineAlign");
}

const QSharedPointer<HSSObject> HSSDisplayObject::getBackground() const
{
    return this->getComputedObject("background");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeBackground(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "no")
            {
                return this->computeValueObject(parserNode);
            }
            else if (kwValue == "black")
            {
                return HSSRgb::blackColor(this->getController());
            }
            else if (kwValue == "white")
            {
                return HSSRgb::whiteColor(this->getController());
            }
            else if (kwValue == "transparent")
            {
                return HSSRgb::transparentColor(this->getController());
            }
            break;
        }

        default:
            break;
    }

    return this->computeObject(parserNode, "background");
}

//font
const QSharedPointer<HSSFont> HSSDisplayObject::getFont() const
{
    QSharedPointer<HSSObject> computed = this->getComputedValue("font");;
    if (computed && computed->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSValue>(computed)->getObject();
        if (remoteObj && remoteObj->isA(HSSObjectTypeFont))
        {
            return qSharedPointerCast<HSSFont>(remoteObj);
        }
    }
    else if (computed && computed->isA(HSSObjectTypeFont)){
        return qSharedPointerCast<HSSFont>(computed);
    }
    return HSSFont::defaultFont(this->getController());
}

//on
const QSharedPointer<HSSObject> HSSDisplayObject::getOn() const
{
    return this->getComputedValue("on");
}

bool HSSDisplayObject::fireEvent(HSSEventType type)
{
    bool fired = false;

    QSharedPointer<HSSObject> computed = this->getComputedValue("on");;
    if (computed)
    {
        if (computed->isA(HSSObjectTypeEvent))
        {
            fired = this->_fireEvent(qSharedPointerCast<HSSEvent>(computed), type);
        }
        else if(computed->isA(HSSObjectTypeMultipleValue))
        {
            bool allFired = true;
            Q_FOREACH(QSharedPointer<HSSObject> comp, qSharedPointerCast<HSSMultipleValue>(computed)->getValues())
            {
                if (comp->isA(HSSObjectTypeEvent))
                {
                    QSharedPointer<HSSEvent> theEvent = qSharedPointerCast<HSSEvent>(comp);
                    if ( ! this->_fireEvent(qSharedPointerCast<HSSEvent>(computed), type))
                    {
                        allFired = false;
                    }
                }
            }
            fired = allFired;
        }
    }

    return fired;
}

bool HSSDisplayObject::_fireEvent(QSharedPointer<HSSEvent> theEvent, HSSEventType eventType)
{
    bool fired = false;
    if (theEvent->isA(eventType))
    {
        axr_log(LoggerChannelEvents, "HSSDisplayObject: firing event: " + theEvent->toString());
        theEvent->fire();
        fired = true;
    }
    return fired;
}

//margin
const QSharedPointer<HSSObject> HSSDisplayObject::getMargin() const
{
    return this->getComputedValue("margin");
}

void HSSDisplayObject::setTopMargin(HSSUnit value)
{
    this->setComputedValue("topMargin", value);
}

const HSSUnit HSSDisplayObject::getTopMargin() const
{
    return this->getComputedNumber("topMargin");
}

void HSSDisplayObject::setRightMargin(HSSUnit value)
{
    this->setComputedValue("rightMargin", value);
}

const HSSUnit HSSDisplayObject::getRightMargin() const
{
    return this->getComputedNumber("rightMargin");
}

void HSSDisplayObject::setBottomMargin(HSSUnit value)
{
    this->setComputedValue("bottomMargin", value);
}

const HSSUnit HSSDisplayObject::getBottomMargin() const
{
    return this->getComputedNumber("bottomMargin");
}

void HSSDisplayObject::setLeftMargin(HSSUnit value)
{
    this->setComputedValue("leftMargin", value);
}

const HSSUnit HSSDisplayObject::getLeftMargin() const
{
    return this->getComputedNumber("leftMargin");
}

void HSSDisplayObject::horizontalMarginChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->_setOuterWidth();
    this->notifyObservers("margin", theObj);
}

void HSSDisplayObject::verticalMarginChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->_setOuterHeight();
    this->notifyObservers("margin", theObj);
}

//padding
const QSharedPointer<HSSObject> HSSDisplayObject::getPadding() const
{
    return this->getComputedValue("padding");
}

void HSSDisplayObject::setTopPadding(HSSUnit value)
{
    this->setComputedValue("topPadding", value);
}

const HSSUnit HSSDisplayObject::getTopPadding() const
{
    return this->getComputedNumber("topPadding");
}

void HSSDisplayObject::setRightPadding(HSSUnit value)
{
    this->setComputedValue("rightPadding", value);
}

const HSSUnit HSSDisplayObject::getRightPadding() const
{
    return this->getComputedNumber("rightPadding");
}

void HSSDisplayObject::setBottomPadding(HSSUnit value)
{
    this->setComputedValue("bottomPadding", value);
}

const HSSUnit HSSDisplayObject::getBottomPadding() const
{
    return this->getComputedNumber("bottomPadding");
}

void HSSDisplayObject::setLeftPadding(HSSUnit value)
{
    this->setComputedValue("leftPadding", value);
}

const HSSUnit HSSDisplayObject::getLeftPadding() const
{
    return this->getComputedNumber("leftPadding");
}

const QSharedPointer<HSSObject> HSSDisplayObject::getBorder() const
{
    return this->getComputedValue("border");
}

const bool HSSDisplayObject::getVisible() const
{
    return this->getComputedBool("visible");
}

void HSSDisplayObject::_setInnerWidth()
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit innerWidth = this->getWidth();
    HSSUnit rightPadding = 0, leftPadding = 0;
    QSharedPointer<HSSObject> paddingObj = this->getPadding();
    if (paddingObj)
    {
        if (paddingObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(paddingObj);
            innerWidth -= theMargin->getLeft() + theMargin->getRight();
            rightPadding += theMargin->getRight();
            leftPadding += theMargin->getLeft();
        }
        else if(paddingObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(QSharedPointer<HSSObject> pObj, qSharedPointerCast<HSSMultipleValue>(paddingObj)->getValues())
            {
                if (pObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(pObj);
                    innerWidth -= theMargin->getLeft() + theMargin->getRight();
                    rightPadding += theMargin->getRight();
                    leftPadding += theMargin->getLeft();
                }
            }
        }
        else if(paddingObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(paddingObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit paddingSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                rightPadding += paddingSize;
                leftPadding += paddingSize;
                innerWidth -= (paddingSize*2);
            }
        }
    }
    this->setRightPadding(rightPadding);
    this->setLeftPadding(leftPadding);
    innerWidth = qMax(0., innerWidth);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(innerWidth, this->getController())));
    this->setComputed("innerWidth", computedValue);
}

void HSSDisplayObject::_setInnerHeight()
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit innerHeight = this->getHeight();
    HSSUnit topPadding = 0, bottomPadding = 0;
    QSharedPointer<HSSObject> paddingObj = this->getPadding();
    if (paddingObj)
    {
        if (paddingObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(paddingObj);
            innerHeight -= theMargin->getTop() + theMargin->getBottom();
            topPadding += theMargin->getTop();
            bottomPadding += theMargin->getBottom();
        }
        else if(paddingObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(QSharedPointer<HSSObject> pObj, qSharedPointerCast<HSSMultipleValue>(paddingObj)->getValues())
            {
                if (pObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(pObj);
                    innerHeight -= theMargin->getTop() + theMargin->getBottom();
                    topPadding += theMargin->getTop();
                    bottomPadding += theMargin->getBottom();
                }
            }
        }
        else if(paddingObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(paddingObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit paddingSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                topPadding += paddingSize;
                bottomPadding += paddingSize;
                innerHeight -= (paddingSize*2);
            }
        }
    }
    this->setTopPadding(topPadding);
    this->setBottomPadding(bottomPadding);
    innerHeight = qMax(0., innerHeight);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(innerHeight, this->getController())));
    this->setComputed("innerHeight", computedValue);
}

void HSSDisplayObject::_setOuterWidth()
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit outerWidth = this->getWidth();
    HSSUnit rightMargin = 0, leftMargin = 0;
    QSharedPointer<HSSObject> marginObj = this->getMargin();
    if (marginObj)
    {
        if (marginObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(marginObj);
            outerWidth += theMargin->getLeft() + theMargin->getRight();
            rightMargin += theMargin->getRight();
            leftMargin += theMargin->getLeft();
        }
        else if(marginObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(QSharedPointer<HSSObject> mObj, qSharedPointerCast<HSSMultipleValue>(marginObj)->getValues())
            {
                if (mObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(mObj);
                    outerWidth += theMargin->getLeft() + theMargin->getRight();
                    rightMargin += theMargin->getRight();
                    leftMargin += theMargin->getLeft();
                }
            }
        }
        else if(marginObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(marginObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit marginSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                rightMargin += marginSize;
                leftMargin += marginSize;
                outerWidth += (marginSize*2);
            }
        }
    }
    this->setRightMargin(rightMargin);
    this->setLeftMargin(leftMargin);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(outerWidth, this->getController())));
    this->setComputed("outerWidth", computedValue);
}

void HSSDisplayObject::_setOuterHeight()
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit outerHeight = this->getHeight();
    HSSUnit topMargin = 0, bottomMargin = 0;
    QSharedPointer<HSSObject> marginObj = this->getMargin();
    if (marginObj)
    {
        if (marginObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(marginObj);
            outerHeight += theMargin->getTop() + theMargin->getBottom();
            topMargin += theMargin->getTop();
            bottomMargin += theMargin->getBottom();
        }
        else if(marginObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(QSharedPointer<HSSObject> mObj, qSharedPointerCast<HSSMultipleValue>(marginObj)->getValues())
            {
                if (mObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(mObj);
                    outerHeight += theMargin->getTop() + theMargin->getBottom();
                    topMargin += theMargin->getTop();
                    bottomMargin += theMargin->getBottom();
                }
            }
        }
        else if(marginObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(marginObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit marginSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                topMargin += marginSize;
                bottomMargin += marginSize;
                outerHeight += (marginSize*2);
            }
        }
    }
    this->setTopMargin(topMargin);
    this->setBottomMargin(bottomMargin);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(outerHeight, this->getController())));
    this->setComputed("outerHeight", computedValue);
}

bool HSSDisplayObject::handleEvent(HSSInputEvent *event)
{
    if (!event)
        return false;

    HSSMouseEvent *mouseEvent = dynamic_cast<HSSMouseEvent*>(event);
    if (mouseEvent)
    {
        return this->handleMouseEvent(mouseEvent);
    }
    else
    {
        switch (event->type())
        {
            case HSSEventTypeLoad:
            {
                return this->fireEvent(event->type());
            }

            case HSSEventTypeExitedWindow:
            {
                if (this->_isHover)
                {
                    this->setHover(false);
                }

                return this->fireEvent(event->type());
            }

            default:
                AXRError("HSSDisplayObject", "Unknown event type").raise();
                break;
        }
    }

    return false;
}

bool HSSDisplayObject::handleMouseEvent(HSSMouseEvent *event)
{
    if (!event)
        return false;

    const HSSRect containerBounds(this->globalX, this->globalY, this->getWidth(), this->getHeight());

    switch (event->type())
    {
        case HSSEventTypeMouseDown:
        {
            if (containerBounds.contains(event->position()))
            {
                this->setPress(true);
                return this->fireEvent(event->type());
            }

            break;
        }
        case HSSEventTypeMouseUp:
        {
            this->setPress(false);

            if (containerBounds.contains(event->position()))
            {
                return this->fireEvent(event->type());
            }

            break;
        }
        case HSSEventTypeClick:
        case HSSEventTypeDoubleClick:
        {
            if (containerBounds.contains(event->position()))
                return this->fireEvent(event->type());

            break;
        }
        case HSSEventTypeMouseMove:
        {
            if (containerBounds.contains(event->position()))
            {
                if (!this->_isHover)
                {
                    this->setHover(true);
                }

                return this->fireEvent(event->type());
            }
            else if (this->_isHover)
            {
                this->setHover(false);
                return true;
            }

            break;
        }
        default:
            break;
    }

    return false;
}

void HSSDisplayObject::setHover(bool newValue)
{
    if (this->_isHover != newValue)
    {
        this->_isHover = newValue;
        if (newValue)
        {
            this->flagsActivate("hover");
        }
        else
        {
            this->flagsDeactivate("hover");
        }
    }
}

bool HSSDisplayObject::isHover()
{
    return this->_isHover;
}

void HSSDisplayObject::setPress(bool newValue)
{
    if (this->_isPress != newValue)
    {
        this->_isPress = newValue;
        if (newValue)
        {
            this->flagsActivate("press");
        }
        else
        {
            this->flagsDeactivate("press");
        }
    }
}

bool HSSDisplayObject::isPress()
{
    return this->_isPress;
}

void HSSDisplayObject::ruleChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    //HSSRule * theRule = (HSSRule*)data;
    this->setNeedsRereadRules(true);
    this->getController()->document()->setNeedsDisplay(true);
}

void HSSDisplayObject::createFlag(QSharedPointer<HSSFlag> flag, HSSRuleState defaultValue)
{
    this->_flagsStatus[flag->getName()] = defaultValue;
    this->_flags[flag->getName()].push_back(flag);
}

void HSSDisplayObject::removeFlag(QSharedPointer<HSSFlag> flag)
{
    AXRString name = flag->getName();
    if (this->hasFlag(name))
    {
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        Q_FOREACH(QSharedPointer<HSSFlag> existingFlag, flags)
        {
            if (existingFlag->equalTo(flag))
            {
                flags.erase(std::remove(flags.begin(), flags.end(), flag), flags.end());
            }
        }
        if (flags.size() == 0)
        {
            this->_flags.remove(name);
            this->_flagsStatus.remove(name);
        }
    }
}

bool HSSDisplayObject::hasFlag(AXRString name)
{
    if (this->_flagsStatus.find(name) != this->_flagsStatus.end())
    {
        return true;
    }
    return false;
}

HSSRuleState HSSDisplayObject::flagState(AXRString name)
{
    if (this->_flagsStatus.find(name) != this->_flagsStatus.end())
    {
        return this->_flagsStatus[name];
    }
    return HSSRuleStateOff;
}

void HSSDisplayObject::flagsActivate(AXRString name)
{
    if (this->hasFlag(name))
    {
        //std_log("activate flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = HSSRuleStateActivate;
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<QSharedPointer<HSSFlag> >::iterator it;
        for (it = flags.begin(); it != flags.end(); ++it)
        {
            QSharedPointer<HSSFlag> theFlag = *it;
            theFlag->setThisObj(this->shared_from_this());
            theFlag->flagChanged(newValue);
        }
        this->_flagsStatus[name] = HSSRuleStateOn;
    }
    else
    {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

void HSSDisplayObject::flagsDeactivate(AXRString name)
{
    if (name == "*")
    {
        //std_log("deactivating all flags on element "+this->getElementName());
        for (QMap<AXRString, std::vector<QSharedPointer<HSSFlag> >  >::const_iterator it = this->_flags.begin(); it != this->_flags.end(); ++it)
        {
            HSSRuleState newValue = HSSRuleStatePurge;
            std::vector<QSharedPointer<HSSFlag> > flags = it.value();
            this->_flagsStatus[it.key()] = newValue;

            for (std::vector<QSharedPointer<HSSFlag> >::iterator it = flags.begin(); it != flags.end(); ++it)
            {
                QSharedPointer<HSSFlag> theFlag = *it;
                theFlag->setThisObj(this->shared_from_this());
                theFlag->flagChanged(newValue);
            }
        }
    }
    else if (this->hasFlag(name))
    {
        //std_log("deactivate flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = HSSRuleStatePurge;
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        for (std::vector<QSharedPointer<HSSFlag> >::iterator it = flags.begin(); it != flags.end(); ++it)
        {
            QSharedPointer<HSSFlag> theFlag = *it;
            theFlag->setThisObj(this->shared_from_this());
            theFlag->flagChanged(newValue);
        }

        this->_flagsStatus[name] = HSSRuleStateOff;
    }
    else
    {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

void HSSDisplayObject::flagsToggle(AXRString name)
{
    if (this->hasFlag(name))
    {
        //std_log("toggle flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = (this->_flagsStatus[name] == HSSRuleStateOn ? HSSRuleStatePurge : HSSRuleStateActivate);
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<QSharedPointer<HSSFlag> >::iterator it;
        for (it = flags.begin(); it != flags.end(); ++it)
        {
            QSharedPointer<HSSFlag> theFlag = *it;
            theFlag->setThisObj(this->shared_from_this());
            theFlag->flagChanged(newValue);
        }
        this->_flagsStatus[name] = (newValue == HSSRuleStateActivate ? HSSRuleStateOn : HSSRuleStateOff);
    }
    else
    {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

bool HSSDisplayObject::isRoot()
{
    return this->_isRoot;
}

void HSSDisplayObject::setRoot(bool newValue)
{
    this->_isRoot = newValue;
}

bool HSSDisplayObject::hasOwnWidth() const
{
    return this->_hasOwnWidth;
}

void HSSDisplayObject::setHasOwnWidth(bool newValue)
{
    this->_hasOwnWidth = newValue;
}

bool HSSDisplayObject::hasOwnHeight() const
{
    return this->_hasOwnHeight;
}

void HSSDisplayObject::setHasOwnHeight(bool newValue)
{
    this->_hasOwnHeight = newValue;
}

QSharedPointer<HSSDisplayObject> HSSDisplayObject::shared_from_this()
{
    return qSharedPointerCast<HSSDisplayObject > (HSSObject::shared_from_this());
}

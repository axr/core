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
#include "HSSRadialGradient.h"
#include "HSSRect.h"
#include "HSSRgb.h"
#include "HSSRule.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSDisplayObject::HSSDisplayObject(HSSObjectType type, AXRController * controller)
: HSSObject(type, controller)
{
    this->initialize();
}

void HSSDisplayObject::initialize()
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
    this->contentText = AXRString();

    this->_isHover = false;
    this->_isPress = false;
    this->_layoutFlagIsInSecondaryGroup = false;
    this->_layoutFlagIsInSecondaryLine = false;
    this->_layoutFlagVerticallyAligned = false;
    this->_layoutFlagLockTop = false;
    this->_layoutLockTopPosition = 0.;
    this->_layoutFlagLockBottom = false;
    this->_layoutLockBottomPosition = 0.;
}

HSSDisplayObject::HSSDisplayObject(const HSSDisplayObject & orig)
: HSSObject(orig)
{
    this->initialize();
    this->attributes = orig.attributes;
    this->elementName = orig.elementName;
    this->contentText = orig.contentText;
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

bool HSSDisplayObject::canHaveChildren()
{
    return false;
}

QSharedPointer<HSSContainer> HSSDisplayObject::getParent()
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

AXRString HSSDisplayObject::getContentText()
{
    return this->contentText;
}

void HSSDisplayObject::setContentText(const AXRString &text)
{
    this->contentText = text;
    this->registerProperty(HSSObservablePropertyContent, QVariant::fromValue(text));
}

void HSSDisplayObject::appendContentText(const AXRString &text)
{
    this->contentText.append(text);
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
        //the target property is set to HSSObservablePropertyValue -- should this be something else?
        newRule->observe("value", "value", this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::ruleChanged));

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
            if (propdef->getNames().contains("isA"))
            {
                this->rulesAddIsAChildren(propdef, defaultState, newRule);
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
                    if (propdef->getNames().contains("isA"))
                    {
                        this->rulesAddIsAChildren(propdef, defaultState, parentRule);
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
    this->rules.erase(this->rules.begin() + index);
}

void HSSDisplayObject::rulesRemoveLast()
{
    this->rules.pop_back();
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

void HSSDisplayObject::accept(HSSAbstractVisitor* visitor, bool traverse)
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

void HSSDisplayObject::_setInnerWidth(bool notify)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit innerWidth = this->width;
    this->rightPadding = this->leftPadding = 0;
    for (it = this->padding.begin(); it != this->padding.end(); ++it)
    {
        QSharedPointer<HSSMargin> theMargin = *it;
        innerWidth -= theMargin->getLeft() + theMargin->getRight();
        this->rightPadding += theMargin->getRight();
        this->leftPadding += theMargin->getLeft();
    }
    this->innerWidth = innerWidth;
    if(this->innerWidth < 0)
    {
        this->innerWidth = 0;
    }
    if (notify)
        this->notifyObservers(HSSObservablePropertyInnerWidth, &this->innerWidth);
}

void HSSDisplayObject::_setInnerHeight(bool notify)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit innerHeight = this->height;
    this->topPadding = this->bottomPadding = 0;
    for (it = this->padding.begin(); it != this->padding.end(); ++it)
    {
        QSharedPointer<HSSMargin> theMargin = *it;
        innerHeight -= theMargin->getTop() + theMargin->getBottom();
        this->topPadding += theMargin->getTop();
        this->bottomPadding += theMargin->getBottom();
    }
    this->innerHeight = innerHeight;
    if(this->innerHeight < 0)
    {
        this->innerHeight = 0;
    }
    if (notify)
        this->notifyObservers(HSSObservablePropertyInnerHeight, &this->innerHeight);
}

void HSSDisplayObject::_setOuterWidth(bool notify)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit outerWidth = this->width;
    this->rightMargin = this->leftMargin = 0;
    for (it = this->margin.begin(); it != this->margin.end(); ++it)
    {
        QSharedPointer<HSSMargin> theMargin = *it;
        outerWidth += theMargin->getLeft() + theMargin->getRight();
        this->rightMargin += theMargin->getRight();
        this->leftMargin += theMargin->getLeft();
    }
    this->outerWidth = outerWidth;
    if (notify)
        this->notifyObservers(HSSObservablePropertyOuterWidth, &this->outerWidth);
}

void HSSDisplayObject::_setOuterHeight(bool notify)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit outerHeight = this->height;
    this->topMargin = this->bottomMargin = 0;
    for (it = this->margin.begin(); it != this->margin.end(); ++it)
    {
        QSharedPointer<HSSMargin> theMargin = *it;
        outerHeight += theMargin->getTop() + theMargin->getBottom();
        this->topMargin += theMargin->getTop();
        this->bottomMargin += theMargin->getBottom();
    }
    this->outerHeight = outerHeight;
    if (notify)
        this->notifyObservers(HSSObservablePropertyOuterHeight, &this->outerHeight);
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

    const HSSRect containerBounds(globalX, globalY, width, height);

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

void HSSDisplayObject::ruleChanged(HSSObservableProperty source, void*data)
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

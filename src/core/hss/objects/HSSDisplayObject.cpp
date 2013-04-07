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
#include "HSSRgb.h"
#include "HSSRule.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "HSSValue.h"

using namespace AXR;

Q_DECLARE_METATYPE(HSSUnit*)

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

    this->x = this->y
            = this->globalX = this->globalY
            = this->width = this->height
            = this->innerWidth = this->outerWidth
            = this->innerHeight = this->outerHeight
            = this->topPadding = this->rightPadding = this->bottomPadding = this->leftPadding
            = this->topMargin = this->rightMargin = this->bottomMargin = this->leftMargin
            = this->anchorX = this->anchorY
            = this->alignX = this->alignY
            = 0.;

    this->_anchorXdefault = this->_anchorYdefault = true;

    this->drawIndex = 0;
    this->_index = 0;
    this->tabIndex = 1;
    this->zoomFactor = 1;
    this->flow = true;
    this->visible = true;
    this->contained = true;
    /**
     *  @todo change to camelCase
     */
    this->does_float = false;
    this->heightByContent = false;
    this->widthByContent = false;
    this->_isRoot = false;

    this->elementName = AXRString();
    this->contentText = AXRString();

    this->borderBleeding = 0.;

    this->registerProperty(HSSObservablePropertyAlignX, QVariant::fromValue(&this->alignX));
    this->registerProperty(HSSObservablePropertyAlignY, QVariant::fromValue(&this->alignY));
    this->registerProperty(HSSObservablePropertyAnchorX, QVariant::fromValue(&this->anchorX));
    this->registerProperty(HSSObservablePropertyAnchorY, QVariant::fromValue(&this->anchorY));
    this->registerProperty(HSSObservablePropertyFlow, QVariant::fromValue(&this->flow));
    this->registerProperty(HSSObservablePropertyContained, QVariant::fromValue(&this->contained));
    this->registerProperty(HSSObservablePropertyHeight, QVariant::fromValue(&this->height));
    this->registerProperty(HSSObservablePropertyWidth, QVariant::fromValue(&this->width));
    this->registerProperty(HSSObservablePropertyBackground, QVariant::fromValue(&this->background));
    this->registerProperty(HSSObservablePropertyFont, QVariant::fromValue(&this->font));
    this->registerProperty(HSSObservablePropertyOn, QVariant::fromValue(&this->on));
    this->registerProperty(HSSObservablePropertyMargin, QVariant::fromValue(&this->margin));
    this->registerProperty(HSSObservablePropertyPadding, QVariant::fromValue(&this->padding));
    this->registerProperty(HSSObservablePropertyBorder, QVariant::fromValue(&this->border));
    this->registerProperty(HSSObservablePropertyVisible, QVariant::fromValue(&this->visible));

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
    if (this->observedWidth)
    {
        this->observedWidth->removeObserver(this->observedWidthProperty, HSSObservablePropertyWidth, this);
    }
    if (this->observedHeight)
    {
        this->observedHeight->removeObserver(this->observedHeightProperty, HSSObservablePropertyHeight, this);
    }
    if (this->observedAnchorX)
    {
        this->observedAnchorX->removeObserver(this->observedAnchorXProperty, HSSObservablePropertyAnchorX, this);
    }
    if (this->observedAnchorY)
    {
        this->observedAnchorY->removeObserver(this->observedAnchorYProperty, HSSObservablePropertyAnchorY, this);
    }
    if (this->observedAlignX)
    {
        this->observedAlignX->removeObserver(this->observedAlignXProperty, HSSObservablePropertyAlignX, this);
    }
    if (this->observedAlignY)
    {
        this->observedAlignY->removeObserver(this->observedAlignYProperty, HSSObservablePropertyAlignY, this);
    }
    if (this->observedBackground)
    {
        this->observedBackground->removeObserver(this->observedBackgroundProperty, HSSObservablePropertyBackground, this);
    }
    if (this->observedContent)
    {
        this->observedContent->removeObserver(this->observedContentProperty, HSSObservablePropertyContent, this);
    }
    if (this->observedFont)
    {
        this->observedFont->removeObserver(this->observedFontProperty, HSSObservablePropertyFont, this);
    }
    if (this->observedOn)
    {
        this->observedOn->removeObserver(this->observedOnProperty, HSSObservablePropertyOn, this);
    }
    if (this->observedMargin)
    {
        this->observedMargin->removeObserver(this->observedMarginProperty, HSSObservablePropertyMargin, this);
    }
    if (this->observedPadding)
    {
        this->observedPadding->removeObserver(this->observedPaddingProperty, HSSObservablePropertyPadding, this);
    }
    if (this->observedBorder)
    {
        this->observedBorder->removeObserver(this->observedBorderProperty, HSSObservablePropertyBorder, this);
    }
    if (this->observedVisible)
    {
        this->observedVisible->removeObserver(this->observedVisibleProperty, HSSObservablePropertyVisible, this);
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
        newRule->observe(HSSObservablePropertyValue, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::ruleChanged));

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
            if (propdef->getName() == "isA")
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
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());

            if (this->isA(HSSObjectTypeContainer))
            {
                QSharedPointer<HSSContainer> thisCont = qSharedPointerCast<HSSContainer > (this->shared_from_this());
                this->getController()->currentContext.push(thisCont);

                //iterate over the property defs and try to find an isA property
                const std::deque<QSharedPointer<HSSPropertyDefinition> > & props = objdef->getProperties();
                std::deque<QSharedPointer<HSSPropertyDefinition> >::const_iterator pIt;
                for (pIt = props.begin(); pIt != props.end(); ++pIt)
                {
                    QSharedPointer<HSSPropertyDefinition> propdef = *pIt;
                    if (propdef->getName() == "isA")
                    {
                        this->rulesAddIsAChildren(propdef, defaultState, parentRule);
                    }
                }

                HSSRule::const_it it;
                const std::deque<QSharedPointer<HSSRule> > rules = objdef->getRules();
                AXRController * controller = this->getController();
                for (it = rules.begin(); it != rules.end(); ++it)
                {
                    QSharedPointer<HSSRule> clonedRule = (*it)->clone();
                    QSharedPointer<HSSSelection> children = thisCont->getChildren();
                    controller->recursiveMatchRulesToDisplayObjects(clonedRule, children, thisCont, true);
                    controller->recursiveSetRuleState(clonedRule, children, thisCont, HSSRuleStateOn);
                    //we get the children again, which now include any newly created containers
                    children = thisCont->getChildren();
                    clonedRule->setOriginalScope(children->joinAll());
                    //we attach the cloned rule to the rule that has the isA
                    parentRule->childrenAdd(clonedRule);
                }
                this->getController()->currentContext.pop();
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

void HSSDisplayObject::readDefinitionObjects()
{
    if (this->_needsRereadRules)
    {
        unsigned i, j;

        this->setDefaults();

        //if this is root, we use the window width and height of the render
        if (this->isRoot())
        {
            AXRController * controller = this->getController();
            //width
            QSharedPointer<HSSNumberConstant> newDWidth(new HSSNumberConstant(controller->document()->getWindowWidth(), controller));
            this->setDWidth(newDWidth);
            //height
            QSharedPointer<HSSNumberConstant> newDHeight(new HSSNumberConstant(controller->document()->getWindowHeight(), controller));
            this->setDHeight(newDHeight);
        }

        AXRString propertyName;
        for (i = 0; i<this->rules.size(); ++i)
        {
            QSharedPointer<HSSRuleStatus> & ruleStatus = this->rules[i];
            switch (ruleStatus->state)
            {
            case HSSRuleStateActivate:
            {
                ruleStatus->state = HSSRuleStateOn;
                //fall through
            }

            case HSSRuleStateOn:
            {
                QSharedPointer<HSSRule> & rule = ruleStatus->rule;
                for (j = 0; j < rule->propertiesSize(); ++j)
                {
                    try
                    {
                        QSharedPointer<HSSPropertyDefinition>& propertyDefinition = rule->propertiesGet(j);
                        propertyName = propertyDefinition->getName();
                        this->setPropertyWithName(propertyName, propertyDefinition->getValue());
                    }
                    catch (const AXRError &e)
                    {
                        e.raise();
                    }
                }
                break;
            }

            case HSSRuleStatePurge:
            {
                ruleStatus->state = HSSRuleStateOff;
                break;
            }

            default:
                break;
            }
        }

        this->_needsRereadRules = false;
    }
}

void HSSDisplayObject::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyWidth:
        this->setDWidth(value);
        break;
    case HSSObservablePropertyHeight:
        this->setDHeight(value);
        break;
    case HSSObservablePropertyAnchorX:
        this->setDAnchorX(value);
        break;
    case HSSObservablePropertyAnchorY:
        this->setDAnchorY(value);
        break;
    case HSSObservablePropertyFlow:
        this->setDFlow(value);
        break;
    case HSSObservablePropertyContained:
        this->setDContained(value);
        break;
    case HSSObservablePropertyAlignX:
        this->setDAlignX(value);
        break;
    case HSSObservablePropertyAlignY:
        this->setDAlignY(value);
        break;
    case HSSObservablePropertyBackground:
        this->setDBackground(value);
        break;
    case HSSObservablePropertyContent:
        this->setDContent(value);
        break;
    case HSSObservablePropertyFont:
        this->setDFont(value);
        break;
    case HSSObservablePropertyOn:
        this->setDOn(value);
        break;
    case HSSObservablePropertyMargin:
        this->setDMargin(value);
        break;
    case HSSObservablePropertyPadding:
        this->setDPadding(value);
        break;
    case HSSObservablePropertyBorder:
        this->setDBorder(value);
        break;
    case HSSObservablePropertyVisible:
        this->setDVisible(value);
        break;

    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

void HSSDisplayObject::setNeedsRereadRules(bool value)
{
    this->_needsRereadRules = value;
}

bool HSSDisplayObject::needsRereadRules()
{
    return this->_needsRereadRules;
}

void HSSDisplayObject::recursiveReadDefinitionObjects()
{
    this->readDefinitionObjects();
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

void HSSDisplayObject::layout()
{
    //do nothing
}

void HSSDisplayObject::recursiveLayout()
{
    this->layout();
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

HSSUnit HSSDisplayObject::getWidth()
{
    return this->width;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDWidth()
{
    return this->dWidth;
}

void HSSDisplayObject::setDWidth(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    bool needsPostProcess = true;
    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            this->setDWidth(objdef);
            valid = true;
            needsPostProcess = false;

        }
        catch (const AXRError &e)
        {
            e.raise();

        }

        break;
    }

    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyInnerWidth;
        if (this->observedWidth)
        {
            this->observedWidth->removeObserver(this->observedWidthProperty, HSSObservablePropertyWidth, this);
        }
        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        if (parentContainer)
        {
            this->width = floor(this->_evaluatePropertyValue(
                                                     &HSSDisplayObject::widthChanged,
                                                     value,
                                                     parentContainer->innerWidth,
                                                     observedProperty,
                                                     parentContainer,
                                                     HSSObservablePropertyWidth,
                                                     this->observedWidth,
                                                     this->observedWidthProperty,
                                                     parentContainer->getChildren()
                                                     ));
            this->widthByContent = false;
        }
        else
        {
            this->width = floor(this->_evaluatePropertyValue(
                                                     NULL,
                                                     value,
                                                     0,
                                                     observedProperty,
                                                     this->shared_from_this(),
                                                     HSSObservablePropertyWidth,
                                                     this->observedWidth,
                                                     this->observedWidthProperty,
                                                     HSSSimpleSelection::null()
                                                     ));
            this->widthByContent = false;
        }
        valid = true;
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        AXRString kwValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (kwValue == "inherit")
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                QVariant remoteValue = parent->getProperty(HSSObservablePropertyWidth);
                if (remoteValue.canConvert<HSSUnit*>())
                {
                    this->width = *remoteValue.value<HSSUnit*>();
                    this->widthByContent = false;
                    if (this->observedWidth)
                    {
                        this->observedWidth->removeObserver(this->observedWidthProperty, HSSObservablePropertyWidth, this);
                    }
                    parent->observe(HSSObservablePropertyWidth, HSSObservablePropertyWidth, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::widthChanged));
                    this->observedWidth = parent;
                    this->observedWidthProperty = HSSObservablePropertyWidth;
                    valid = true;
                }
            }
        }
        else if (kwValue == "content")
        {
            if (this->observedWidth)
            {
                this->observedWidth->removeObserver(this->observedWidthProperty, HSSObservablePropertyWidth, this);
                this->observedWidth.clear();
            }
            this->width = 0;
            this->widthByContent = true;
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
        QSharedPointer<HSSContainer> parent = this->getParent();
        if (parent)
        {
            objdef->setScope(parent->getChildren());
        }
        else if (this->isA(HSSObjectTypeContainer))
        {
            HSSContainer * thisCont = static_cast<HSSContainer *> (this);
            objdef->setScope(thisCont->getChildren());
        }
        objdef->setThisObj(this->shared_from_this());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> remoteValue = qSharedPointerCast<HSSValue > (theobj)->getDValue();
            try
            {
                this->setDWidth(remoteValue);
                valid = true;
                needsPostProcess = false;
            }
            catch (const AXRError &e)
            {
                e.raise();
            }
        }
        break;
    }

    default:
        break;
    }

    if (valid)
    {
        if (needsPostProcess)
        {
            this->dWidth = value;
            this->_setInnerWidth();
            this->_setOuterWidth();
            this->notifyObservers(HSSObservablePropertyWidth, &this->width);
            this->setNeedsSurface(true);
            this->setNeedsLayout(true);
            this->setDirty(true);
        }
    }
    else
    {
        throw AXRWarning("HSSDisplayObject", "Invalid value for width of " + this->getElementName());
    }
    this->notifyObservers(HSSObservablePropertyWidth, &this->width);
}

void HSSDisplayObject::widthChanged(HSSObservableProperty source, void*data)
{
    HSSParserNodeType nodeType = this->dWidth->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> widthValue = qSharedPointerCast<HSSPercentageConstant > (this->dWidth);
        this->width = floor(widthValue->getValue(*(HSSUnit*) data));
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> widthExpression = qSharedPointerCast<HSSExpression > (this->dWidth);
        widthExpression->setPercentageBase(*(HSSUnit*) data);
        this->width = floor(widthExpression->evaluate());
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        this->width = floor(*(HSSUnit*) data);
    }

    default:
        break;
    }

    this->_setInnerWidth();
    this->_setOuterWidth();

    this->notifyObservers(HSSObservablePropertyWidth, &this->width);
    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
}

HSSUnit HSSDisplayObject::getHeight()
{
    return this->height;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDHeight()
{
    return this->dHeight;
}

void HSSDisplayObject::setDHeight(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    bool needsPostProcess = true;
    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            this->setDHeight(objdef);
            valid = true;
            needsPostProcess = false;

        }
        catch (const AXRError &e)
        {
            e.raise();

        }

        break;
    }

    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyInnerHeight;
        if (this->observedHeight)
        {
            this->observedHeight->removeObserver(this->observedHeightProperty, HSSObservablePropertyHeight, this);
        }
        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        if (parentContainer)
        {
            this->height = floor(this->_evaluatePropertyValue(
                                                      &HSSDisplayObject::heightChanged,
                                                      value,
                                                      parentContainer->innerHeight,
                                                      observedProperty,
                                                      parentContainer,
                                                      HSSObservablePropertyHeight,
                                                      this->observedHeight,
                                                      this->observedHeightProperty,
                                                      parentContainer->getChildren()
                                                      ));
            this->heightByContent = false;
        }
        else
        {
            this->height = floor(this->_evaluatePropertyValue(
                                                      NULL,
                                                      value,
                                                      0,
                                                      observedProperty,
                                                      this->shared_from_this(),
                                                      HSSObservablePropertyHeight,
                                                      this->observedHeight,
                                                      this->observedHeightProperty,
                                                      HSSSimpleSelection::null()
                                                      ));
            this->heightByContent = false;
        }
        valid = true;
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        AXRString kwValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (kwValue == "inherit")
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                QVariant remoteValue = parent->getProperty(HSSObservablePropertyHeight);
                if (remoteValue.canConvert<HSSUnit*>())
                {
                    this->height = *remoteValue.value<HSSUnit*>();
                    this->heightByContent = false;
                    if (this->observedHeight)
                    {
                        this->observedHeight->removeObserver(this->observedHeightProperty, HSSObservablePropertyHeight, this);
                    }
                    parent->observe(HSSObservablePropertyHeight, HSSObservablePropertyHeight, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::heightChanged));
                    this->observedHeight = parent;
                    this->observedHeightProperty = HSSObservablePropertyHeight;
                    valid = true;
                }
            }
        }
        else if (kwValue == "content")
        {
            if (this->observedHeight)
            {
                this->observedHeight->removeObserver(this->observedHeightProperty, HSSObservablePropertyHeight, this);
                this->observedHeight.clear();
            }
            this->height = 0;
            this->heightByContent = true;
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
        QSharedPointer<HSSContainer> parent = this->getParent();
        if (parent)
        {
            objdef->setScope(parent->getChildren());
        }
        else if (this->isA(HSSObjectTypeContainer))
        {
            HSSContainer * thisCont = static_cast<HSSContainer *> (this);
            objdef->setScope(thisCont->getChildren());
        }
        objdef->setThisObj(this->shared_from_this());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> remoteValue = qSharedPointerCast<HSSValue > (theobj)->getDValue();
            try
            {
                this->setDHeight(remoteValue);
                valid = true;
                needsPostProcess = false;
            }
            catch (const AXRError &e)
            {
                e.raise();
            }
        }
        break;
    }

    default:
        break;
    }

    if (valid)
    {
        if (needsPostProcess)
        {
            this->dHeight = value;
            this->_setInnerHeight();
            this->_setOuterHeight();
            this->notifyObservers(HSSObservablePropertyHeight, &this->height);
            this->setNeedsSurface(true);
            this->setNeedsLayout(true);
            this->setDirty(true);
        }
    }
    else
    {
        throw AXRWarning("HSSDisplayObject", "Invalid value for height of " + this->getElementName());
    }
    this->notifyObservers(HSSObservablePropertyHeight, &this->height);
}

void HSSDisplayObject::heightChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dHeight->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> heightValue = qSharedPointerCast<HSSPercentageConstant > (this->dHeight);
        this->height = floor(heightValue->getValue(*(HSSUnit*) data));
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> heightExpression = qSharedPointerCast<HSSExpression > (this->dHeight);
        heightExpression->setPercentageBase(*(HSSUnit*) data);
        this->height = floor(heightExpression->evaluate());
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->height = floor(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        this->height = floor(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->_setInnerHeight();
    this->_setOuterHeight();

    this->notifyObservers(HSSObservablePropertyHeight, &this->height);
    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
}

HSSUnit HSSDisplayObject::getAnchorX()
{
    return this->anchorX;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDAnchorX()
{
    return this->dAnchorX;
}

void HSSDisplayObject::setDAnchorX(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    this->dAnchorX = value;
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        if (this->observedAnchorX)
        {
            this->observedAnchorX->removeObserver(this->observedAnchorXProperty, HSSObservablePropertyAnchorX, this);
        }
        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        QSharedPointer<HSSSimpleSelection> scope;
        if (parentContainer)
        {
            scope = parentContainer->getChildren();
        }
        this->anchorX = this->_evaluatePropertyValue(
                                             &HSSDisplayObject::anchorXChanged,
                                             value,
                                             this->width,
                                             observedProperty,
                                             this->shared_from_this(),
                                             HSSObservablePropertyAnchorX,
                                             this->observedAnchorX,
                                             this->observedAnchorXProperty,
                                             scope
                                             );
        this->_anchorXdefault = false;
        valid = true;
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        if (this->observedAnchorX)
        {
            this->observedAnchorX->removeObserver(this->observedAnchorXProperty, HSSObservablePropertyAnchorX, this);
        }
        //we don't need to observe anything here, since it will be handled by the layout algorithm
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
        {
            this->_anchorXdefault = true;
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for anchorX of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyAnchorX, &this->anchorX);
}

void HSSDisplayObject::anchorXChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAnchorX->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAnchorX);
        this->anchorX = percentageValue->getValue(*(HSSUnit*) data);
        this->_anchorXdefault = false;
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (this->dAnchorX);
        this->anchorX = expressionValue->evaluate();
        this->_anchorXdefault = false;
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        this->anchorX = *(HSSUnit*) data;
        this->_anchorXdefault = false;
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (this->dAnchorX);
        if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
        {
            this->_anchorXdefault = true;
            this->anchorX = 0.;
        }
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyAnchorX, &this->anchorX);
}

HSSUnit HSSDisplayObject::getAnchorY()
{
    return this->anchorY;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDAnchorY()
{
    return this->dAnchorY;
}

void HSSDisplayObject::setDAnchorY(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    this->dAnchorY = value;
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
        if (this->observedAnchorY)
        {
            this->observedAnchorY->removeObserver(this->observedAnchorYProperty, HSSObservablePropertyAnchorY, this);
        }
        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        QSharedPointer<HSSSimpleSelection> scope;
        if (parentContainer)
        {
            scope = parentContainer->getChildren();
        }
        this->anchorY = this->_evaluatePropertyValue(
                                             &HSSDisplayObject::anchorYChanged,
                                             value,
                                             this->height,
                                             observedProperty,
                                             this->shared_from_this(),
                                             HSSObservablePropertyAnchorY,
                                             this->observedAnchorY,
                                             this->observedAnchorYProperty,
                                             scope
                                             );
        this->_anchorYdefault = false;
        valid = true;
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        if (this->observedAnchorY)
        {
            this->observedAnchorY->removeObserver(this->observedAnchorYProperty, HSSObservablePropertyAnchorY, this);
        }
        //we don't need to observe anything here, since it will be handled by the layout algorithm
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
        {
            this->_anchorYdefault = true;
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for anchorY of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyAnchorY, &this->anchorY);
}

void HSSDisplayObject::anchorYChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAnchorY->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAnchorY);
        this->anchorY = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (this->dAnchorY);
        this->anchorY = expressionValue->evaluate();
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        this->anchorY = *(HSSUnit*) data;
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (this->dAnchorY);
        if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
        {
            this->_anchorYdefault = true;
            this->anchorY = 0.;
        }
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyAnchorY, &this->anchorY);
}


//flow

bool HSSDisplayObject::getFlow()
{
    return this->flow;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDFlow()
{
    return this->dFlow;
}

void HSSDisplayObject::setDFlow(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dFlow = value;
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            this->setDFlow(objdef);
            valid = true;

        }
        catch (const AXRError &e)
        {
            e.raise();

        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dFlow = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.type() == QVariant::Bool)
            {
                this->flow = remoteValue.toBool();
            }
            else
            {
                this->flow = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyFlow, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::flowChanged));
            valid = true;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "yes")
        {
            this->flow = true;
            valid = true;
            break;
        }
        else if (keywordValue->getValue() == "no")
        {
            this->flow = false;
            valid = true;
            break;
        }
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dFlow = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        QSharedPointer<HSSContainer> parent = this->getParent();
        if (parent)
        {
            objdef->setScope(parent->getChildren());
        }
        else if (this->isA(HSSObjectTypeContainer))
        {
            HSSContainer * thisCont = static_cast<HSSContainer *> (this);
            objdef->setScope(thisCont->getChildren());
        }
        objdef->setThisObj(this->shared_from_this());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->flow = (bool)qSharedPointerCast<HSSValue>(theobj)->getIntValue();
            axr_log(LoggerChannelObsolete0, "########### FIXME #################");
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for flow of " + this->name);

    this->notifyObservers(HSSObservablePropertyFlow, &this->flow);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSDisplayObject::flowChanged(HSSObservableProperty source, void*data)
{
    switch (this->dFlow->getType())
    {
    case HSSParserNodeTypeFunctionCall:
        this->flow = *(bool*)data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyFlow, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

//contained

bool HSSDisplayObject::getContained()
{
    return this->contained;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDContained()
{
    return this->dContained;
}

void HSSDisplayObject::setDContained(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dContained = value;
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            this->setDContained(objdef);
            valid = true;

        }
        catch (const AXRError &e)
        {
            e.raise();

        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dContained = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<bool* >())
            {
                this->contained = *remoteValue.value<bool*>();
                valid = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyContained, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::containedChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "yes")
        {
            this->contained = true;
            valid = true;
            break;
        }
        else if (keywordValue->getValue() == "no")
        {
            this->contained = false;
            valid = true;
            break;
        }
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dContained = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        QSharedPointer<HSSContainer> parent = this->getParent();
        if (parent)
        {
            objdef->setScope(parent->getChildren());
        }
        else if (this->isA(HSSObjectTypeContainer))
        {
            HSSContainer * thisCont = static_cast<HSSContainer *> (this);
            objdef->setScope(thisCont->getChildren());
        }
        objdef->setThisObj(this->shared_from_this());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->contained = (bool)qSharedPointerCast<HSSValue>(theobj)->getIntValue();
            axr_log(LoggerChannelObsolete0, "########### FIXME #################");
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for contained of " + this->name);

    this->notifyObservers(HSSObservablePropertyContained, &this->contained);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSDisplayObject::containedChanged(HSSObservableProperty source, void*data)
{
    switch (this->dContained->getType())
    {
    case HSSParserNodeTypeFunctionCall:
        this->contained = *(bool*)data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyContained, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSDisplayObject::getAlignX()
{
    return this->alignX;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDAlignX()
{
    return this->dAlignX;
}

void HSSDisplayObject::setDAlignX(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeKeywordConstant:
    case HSSParserNodeTypeFunctionCall:
        break;
    default:
        throw AXRWarning("HSSDisplayObject", "Invalid value for alignX of " + this->getElementName());
    }

    if (this->observedAlignX)
    {
        this->observedAlignX->removeObserver(this->observedAlignXProperty, HSSObservablePropertyAlignX, this);
        this->observedAlignX.clear();
    }
    this->dAlignX = value;

    if (value->isA(HSSParserNodeTypeKeywordConstant))
    {

        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "auto")
        {
            QSharedPointer<HSSContainer> parentContainer = this->getParent();
            if (parentContainer)
            {
                this->alignX = parentContainer->contentAlignX;
                parentContainer->observe(HSSObservablePropertyContentAlignX, HSSObservablePropertyAlignX, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::alignXChanged));
                this->observedAlignX = parentContainer;
                this->observedAlignXProperty = HSSObservablePropertyContentAlignX;
            }
            else
            {
                this->alignX = 0;
            }
            //            this->alignX = 0;

        }
        else if (keywordValue->getValue() == "left")
        {
            this->setDAlignX(QSharedPointer<HSSParserNode>(new HSSNumberConstant(0, this->getController())));
        }
        else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center")
        {
            this->setDAlignX(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(50, this->getController())));
        }
        else if (keywordValue->getValue() == "right")
        {
            this->setDAlignX(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(100, this->getController())));
        }
        else
        {
            throw AXRWarning("HSSDisplayObject", "Invalid value for alignX of " + this->getElementName());
        }

    }
    else
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        if (parentContainer)
        {
            this->alignX = this->_evaluatePropertyValue(
                                                &HSSDisplayObject::alignXChanged,
                                                value,
                                                parentContainer->width,
                                                observedProperty,
                                                parentContainer,
                                                HSSObservablePropertyAlignX,
                                                this->observedAlignX,
                                                this->observedAlignXProperty,
                                                parentContainer->getChildren()
                                                );
            parentContainer->setNeedsLayout(true);
        }
        else
        {
            this->alignX = this->_evaluatePropertyValue(
                                                NULL,
                                                value,
                                                0,
                                                observedProperty,
                                                this->shared_from_this(),
                                                HSSObservablePropertyAlignX,
                                                this->observedAlignX,
                                                this->observedAlignXProperty,
                                                HSSSimpleSelection::null()
                                                );
        }
    }

    this->notifyObservers(HSSObservablePropertyAlignX, &this->alignX);
}

void HSSDisplayObject::alignXChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAlignX->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAlignX);
        this->alignX = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (this->dAlignX);
        this->alignX = expressionValue->evaluate();
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->alignX = *(HSSUnit*) data;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        this->alignX = *(HSSUnit*) data;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyAlignX, &this->alignX);
}

HSSUnit HSSDisplayObject::getAlignY()
{
    return this->alignY;
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getDAlignY()
{
    return this->dAlignX;
}

void HSSDisplayObject::setDAlignY(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeKeywordConstant:
    case HSSParserNodeTypeFunctionCall:
        break;
    default:
        throw AXRWarning("HSSDisplayObject", "Invalid value for alignY of " + this->getElementName());
    }

    if (this->observedAlignY)
    {
        this->observedAlignY->removeObserver(this->observedAlignYProperty, HSSObservablePropertyAlignY, this);
        this->observedAlignY.clear();
    }
    this->dAlignY = value;

    if (value->isA(HSSParserNodeTypeKeywordConstant))
    {

        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "auto")
        {
            QSharedPointer<HSSContainer> parentContainer = this->getParent();
            if (parentContainer)
            {
                this->alignY = parentContainer->contentAlignY;
                parentContainer->observe(HSSObservablePropertyContentAlignY, HSSObservablePropertyAlignY, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::alignYChanged));
                this->observedAlignY = parentContainer;
                this->observedAlignYProperty = HSSObservablePropertyContentAlignY;
            }
            else
            {
                this->alignY = 0;
            }
            //            this->alignY = 0;
        }
        else if (keywordValue->getValue() == "top")
        {
            this->setDAlignY(QSharedPointer<HSSParserNode>(new HSSNumberConstant(0, this->getController())));
        }
        else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center")
        {
            this->setDAlignY(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(50, this->getController())));
        }
        else if (keywordValue->getValue() == "bottom")
        {
            this->setDAlignY(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(100, this->getController())));
        }
        else
        {
            throw AXRWarning("HSSDisplayObject", "Invalid value for alignY of " + this->getElementName());
        }

    }
    else
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        if (parentContainer)
        {
            this->alignY = this->_evaluatePropertyValue(
                                                &HSSDisplayObject::alignYChanged,
                                                value,
                                                parentContainer->height,
                                                observedProperty,
                                                parentContainer,
                                                HSSObservablePropertyAlignY,
                                                this->observedAlignY,
                                                this->observedAlignYProperty,
                                                parentContainer->getChildren()
                                                );
            parentContainer->setNeedsLayout(true);
        }
        else
        {
            this->alignY = this->_evaluatePropertyValue(
                                                NULL,
                                                value,
                                                0,
                                                observedProperty,
                                                this->shared_from_this(),
                                                HSSObservablePropertyAlignY,
                                                this->observedAlignY,
                                                this->observedAlignYProperty,
                                                HSSSimpleSelection::null()
                                                );
        }
    }

    this->notifyObservers(HSSObservablePropertyAlignY, &this->alignY);
}

void HSSDisplayObject::alignYChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAlignY->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAlignY);
        this->alignY = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (this->dAlignY);
        this->alignY = expressionValue->evaluate();
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->alignY = *(HSSUnit*) data;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        this->alignY = *(HSSUnit*) data;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyAlignY, &this->alignY);
}


//background

QSharedPointer<HSSParserNode> HSSDisplayObject::getDBackground()
{
    return this->dBackground;
}

void HSSDisplayObject::setDBackground(QSharedPointer<HSSParserNode> value)
{
    this->background.clear();
    this->dBackground = value;
    this->addDBackground(value);
}

void HSSDisplayObject::addDBackground(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDBackground(*iterator);
        }
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            this->addDBackground(this->getController()->objectTreeGet(objname->getValue()));
            valid = true;
        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<std::vector<QSharedPointer<HSSObject> >*>())
            {
                std::vector<QSharedPointer<HSSObject> > values = *remoteValue.value<std::vector<QSharedPointer<HSSObject> >*>();

                for (std::vector<QSharedPointer<HSSObject> >::const_iterator it = values.begin(); it != values.end(); ++it)
                {
                    this->background.push_back(*it);
                }

                valid = true;
            }
        }
        else
        {
            valid = false;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(value);
        AXRString kwValue = theKW->getValue();
        if (kwValue == "no")
        {
            valid = true;
        }
        else if (kwValue == "black")
        {
            this->background.push_back(HSSRgb::blackColor(this->getController()));
            valid = true;
        }
        else if (kwValue == "white")
        {
            this->background.push_back(HSSRgb::whiteColor(this->getController()));
            valid = true;
        }
        else if (kwValue == "transparent")
        {
            this->background.push_back(HSSRgb::transparentColor(this->getController()));
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dBackground = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeRgb) || objdef->getObject()->isA(HSSObjectTypeGradient))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyBackground, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::backgroundChanged));
            this->background.push_back(theObj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
    {
        throw AXRWarning("HSSDisplayObject", "Invalid value for background of " + this->getElementName());
    }

    this->notifyObservers(HSSObservablePropertyBackground, &this->background);
}

void HSSDisplayObject::backgroundChanged(HSSObservableProperty source, void*data)
{

}

//content

QSharedPointer<HSSParserNode> HSSDisplayObject::getDContent()
{
    return this->dContent;
}

void HSSDisplayObject::setDContent(QSharedPointer<HSSParserNode> value)
{
    this->content.clear();
    this->dContent = value;
    this->addDContent(value);
}

void HSSDisplayObject::addDContent(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDContent(*iterator);
        }
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            this->addDContent(this->getController()->objectTreeGet(objname->getValue()));
            valid = true;

        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct)
        {
            switch (fnct->getFunctionType())
            {
            case HSSFunctionTypeRef:
            {
                QSharedPointer<HSSContainer> parent = this->getParent();
                if (parent)
                {
                    fnct->setScope(parent->getChildren());
                }
                else if (this->isA(HSSObjectTypeContainer))
                {
                    HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                    fnct->setScope(thisCont->getChildren());
                }
                fnct->setThisObj(this->shared_from_this());
                QVariant remoteValue = fnct->evaluate();
                if (remoteValue.canConvert<AXRString>())
                {
                    try
                    {
                        AXRString theVal = remoteValue.value<AXRString>();
                        this->setContentText(theVal);
                        valid = true;
                    }
                    catch (const AXRError &e)
                    {
                        e.raise();
                    }
                }

                break;
            }

            case HSSFunctionTypeAttr:
            {
                QSharedPointer<HSSContainer> parent = this->getParent();
                if (parent)
                {
                    fnct->setScope(parent->getChildren());
                }
                else if (this->isA(HSSObjectTypeContainer))
                {
                    HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                    fnct->setScope(thisCont->getChildren());
                }
                fnct->setThisObj(this->shared_from_this());
                QVariant remoteValue = fnct->evaluate();
                if (remoteValue.canConvert<AXRString>())
                {
                    AXRString theVal = remoteValue.value<AXRString>();
                    this->setContentText(theVal);
                    valid = true;
                }
                else
                {
                    this->setContentText("");
                }

                break;
            }

            default:
                break;
            }
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        if (qSharedPointerCast<HSSKeywordConstant > (value)->getValue() == "no")
        {
            valid = true;
        }
        break;
    }

    case HSSParserNodeTypeStringConstant:
    {
        this->setContentText(qSharedPointerCast<HSSStringConstant > (value)->getValue());
        valid = true;
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dContent = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeRgb) || objdef->getObject()->isA(HSSObjectTypeGradient))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyContent, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::contentChanged));
            this->content.push_back(theObj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
    {
        throw AXRWarning("HSSDisplayObject", "Invalid value for content of " + this->getElementName());
    }

    this->notifyObservers(HSSObservablePropertyContent, &this->content);
}

void HSSDisplayObject::contentChanged(HSSObservableProperty source, void*data)
{
    axr_log(LoggerChannelObsolete0, "unimplemented yet");
}

const QSharedPointer<HSSParserNode> HSSDisplayObject::getDFont() const
{
    return this->dFont;
}

void HSSDisplayObject::setDFont(QSharedPointer<HSSParserNode> value)
{
    this->font.clear();
    this->dFont = value;
    this->addDFont(value);
}

void HSSDisplayObject::addDFont(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDFont(*iterator);
        }
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();

            QSharedPointer<HSSObject> obj = qSharedPointerCast<HSSObject > (objdef->getObject());
            switch (obj->getObjectType())
            {
            case HSSObjectTypeFont:
                this->font.push_back(qSharedPointerCast<HSSFont > (obj));
                valid = true;
                break;

            default:
                break;
            }

        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<std::vector<QSharedPointer<HSSFont> > *>())
            {
                std::vector<QSharedPointer<HSSFont> > values = *remoteValue.value<std::vector<QSharedPointer<HSSFont> >*>();
                for (std::vector<QSharedPointer<HSSFont> >::const_iterator it = values.begin(); it != values.end(); ++it)
                {
                    this->font.push_back(*it);
                }
                valid = true;
            }
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        AXRString kwValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (kwValue == "inherit")
        {
            if (this->observedFont)
            {
                this->observedFont->removeObserver(this->observedFontProperty, HSSObservablePropertyFont, this);
            }
            QSharedPointer<HSSContainer> parent = this->getParent();
            QVariant remoteValue = parent->getProperty(HSSObservablePropertyFont);
            if (remoteValue.canConvert<std::vector<QSharedPointer<HSSFont> > *>())
            {
                this->font = *remoteValue.value<std::vector<QSharedPointer<HSSFont> > *>();
                parent->observe(HSSObservablePropertyFont, HSSObservablePropertyFont, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::fontChanged));
                valid = true;
            }
        }
        break;
    }

    default:
        break;

    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dFont = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeFont))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }

            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyFont, HSSObservablePropertyFont, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::fontChanged));
            this->font.push_back(qSharedPointerCast<HSSFont > (theObj));
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for font of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyFont, &this->font);
}

void HSSDisplayObject::fontChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dFont->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeObjectNameConstant:
    case HSSParserNodeTypeFunctionCall:
    case HSSParserNodeTypeKeywordConstant:
    {
        this->font = *(std::vector<QSharedPointer<HSSFont> > *) data;
        this->setDirty(true);
        break;
    }

    default:
        break;
    }

    switch (this->dFont->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
        this->font = *(std::vector<QSharedPointer<HSSFont> > *) data;
        this->setDirty(true);
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyFont, &this->font);
}


//on

QSharedPointer<HSSParserNode> HSSDisplayObject::getDOn()
{
    return this->dOn;
}

void HSSDisplayObject::setDOn(QSharedPointer<HSSParserNode> value)
{
    this->on.clear();
    this->dOn = value;
    this->addDOn(value);
}

void HSSDisplayObject::addDOn(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDOn(*iterator);
        }
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            this->addDOn(this->getController()->objectTreeGet(objname->getValue()));
            valid = true;

        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> > >* >())
            {
                try
                {
                    QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> > > values = *remoteValue.value<QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> > >* >();
                    QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> > >::const_iterator it;
                    for (it=values.begin(); it!=values.end(); ++it) {
                        const HSSEventType & type = it.key();
                        const std::vector<QSharedPointer<HSSObject> > events = it.value();

                        if(this->on.contains(type))
                        {
                            std::vector<QSharedPointer<HSSObject> > existingEvents = this->on[type];
                            for (std::vector<QSharedPointer<HSSObject> >::const_iterator it2 = events.begin(); it2 != events.end(); ++it2) {
                                existingEvents.push_back(*it2);
                            }
                        }
                        else
                        {
                            this->on[type] = events;
                        }
                    }
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        if (qSharedPointerCast<HSSKeywordConstant > (value)->getValue() == "no")
        {
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dOn = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
        if (objdef->getObject()->isA(HSSObjectTypeEvent))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }

            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();

            QSharedPointer<HSSEvent> theEvent = qSharedPointerCast<HSSEvent > (objdef->getObject());
            HSSEventType eventType = theEvent->getEventType();
            if (this->on.find(eventType) == this->on.end())
            {
                std::vector<QSharedPointer<HSSObject> >newVector;
                this->on[eventType] = newVector;
            }
            this->on[eventType].push_back(theEvent);

            theEvent->observe(HSSObservablePropertyValue, HSSObservablePropertyOn, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::onChanged));
            valid = true;
        }

        break;
    }


    default:
        break;
    }

    if (!valid)
    {
        throw AXRWarning("HSSGradient", "Invalid value for on of " + this->getName());
    }

    this->notifyObservers(HSSObservablePropertyOn, &this->on);
}

void HSSDisplayObject::onChanged(HSSObservableProperty source, void*data)
{

}

bool HSSDisplayObject::fireEvent(HSSEventType type)
{
    bool fired = false;
    if (this->on.find(type) != this->on.end())
    {
        std::vector<QSharedPointer<HSSObject> > events = this->on[type];
        HSSObject::it it;
        for (it = events.begin(); it != events.end(); ++it)
        {
            if ((*it)->isA(HSSObjectTypeEvent))
            {
                QSharedPointer<HSSEvent> theEvent = qSharedPointerCast<HSSEvent > (*it);
                axr_log(LoggerChannelEvents, "HSSDisplayObject: firing event: " + theEvent->toString());
                theEvent->fire();
                fired = true;
            }
        }
    }

    if (fired)
    {
        axr_log(LoggerChannelEvents, "HSSDisplayObject: fired event");
    }

    return fired;
}

const QSharedPointer<HSSParserNode> HSSDisplayObject::getDMargin() const
{
    return this->dMargin;
}

void HSSDisplayObject::setDMargin(QSharedPointer<HSSParserNode> value)
{
    this->margin.clear();
    this->dMargin = value;
    this->addDMargin(value);
}

void HSSDisplayObject::addDMargin(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDMargin(*iterator);
        }
        this->_setOuterWidth();
        this->_setOuterHeight();
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();

            QSharedPointer<HSSObject> obj = qSharedPointerCast<HSSObject > (objdef->getObject());
            switch (obj->getObjectType())
            {
            case HSSObjectTypeMargin:
                this->margin.push_back(qSharedPointerCast<HSSMargin > (obj));
                valid = true;
                this->_setOuterWidth();
                this->_setOuterHeight();
                break;

            default:
                break;
            }

        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<std::vector<QSharedPointer<HSSMargin> >* >())
            {
                try
                {
                    std::vector<QSharedPointer<HSSMargin> > values = *remoteValue.value<std::vector<QSharedPointer<HSSMargin> >* >();
                    for (std::vector<QSharedPointer<HSSMargin> >::const_iterator it = values.begin(); it != values.end(); ++it)
                    {
                        this->margin.push_back(*it);
                    }
                    this->_setOuterWidth();
                    this->_setOuterHeight();
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "no")
        {
            valid = true;
            this->_setOuterWidth();
            this->_setOuterHeight();
            break;
        }
    }

    case HSSParserNodeTypeNumberConstant:
    {
        QSharedPointer<HSSMargin> theMargin = QSharedPointer<HSSMargin>(new HSSMargin(this->getController()));
        theMargin->setDSize(value);
        this->margin.push_back(theMargin);
        this->_setOuterWidth();
        this->_setOuterHeight();
        valid = true;
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dMargin = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }

            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyTarget, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::marginChanged));
            this->margin.push_back(qSharedPointerCast<HSSMargin > (theObj));
            this->_setOuterWidth();
            this->_setOuterHeight();
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for margin of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyMargin, &this->margin);
}

void HSSDisplayObject::marginChanged(HSSObservableProperty source, void*data)
{

}

const QSharedPointer<HSSParserNode> HSSDisplayObject::getDPadding() const
{
    return this->dPadding;
}

void HSSDisplayObject::setDPadding(QSharedPointer<HSSParserNode> value)
{
    this->padding.clear();
    this->dPadding = value;
    this->addDPadding(value);
}

void HSSDisplayObject::addDPadding(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDPadding(*iterator);
        }
        this->_setInnerWidth();
        this->_setInnerHeight();
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();

            QSharedPointer<HSSObject> obj = qSharedPointerCast<HSSObject > (objdef->getObject());
            switch (obj->getObjectType())
            {
            case HSSObjectTypeMargin:
                this->padding.push_back(qSharedPointerCast<HSSMargin > (obj));
                valid = true;
                this->_setInnerWidth();
                this->_setInnerHeight();
                break;

            default:
                break;
            }

        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<std::vector<QSharedPointer<HSSMargin> >* >())
            {
                try
                {
                    std::vector<QSharedPointer<HSSMargin> > values = *remoteValue.value<std::vector<QSharedPointer<HSSMargin> >* >();
                    for (std::vector<QSharedPointer<HSSMargin> >::const_iterator it = values.begin(); it != values.end(); ++it)
                    {
                        this->padding.push_back(*it);
                    }
                    this->_setInnerWidth();
                    this->_setInnerHeight();
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "no")
        {
            valid = true;
            this->_setInnerWidth();
            this->_setInnerHeight();
            break;
        }
    }

    case HSSParserNodeTypeNumberConstant:
    {
        QSharedPointer<HSSMargin> thePadding = QSharedPointer<HSSMargin>(new HSSMargin(this->getController()));
        thePadding->setDSize(value);
        this->padding.push_back(thePadding);
        this->_setInnerWidth();
        this->_setInnerHeight();
        valid = true;
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dPadding = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }

            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyTarget, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::paddingChanged));
            this->padding.push_back(qSharedPointerCast<HSSMargin > (theObj));
            this->_setInnerWidth();
            this->_setInnerHeight();
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for padding of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyPadding, &this->padding);
}

void HSSDisplayObject::paddingChanged(HSSObservableProperty source, void*data)
{

}

const QSharedPointer<HSSParserNode> HSSDisplayObject::getDBorder() const
{
    return this->dBorder;
}

void HSSDisplayObject::setDBorder(QSharedPointer<HSSParserNode> value)
{
    this->border.clear();
    this->dBorder = value;
    this->addDBorder(value);
}

void HSSDisplayObject::addDBorder(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDBorder(*iterator);
        }
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();

            QSharedPointer<HSSObject> obj = qSharedPointerCast<HSSObject > (objdef->getObject());
            switch (obj->getObjectType())
            {
            case HSSObjectTypeBorder:
                this->border.push_back(qSharedPointerCast<HSSBorder > (obj));
                valid = true;
                break;

            default:
                break;
            }

        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<std::vector<QSharedPointer<HSSBorder> >* >())
            {
                try
                {
                    std::vector<QSharedPointer<HSSBorder> > values = *remoteValue.value<std::vector<QSharedPointer<HSSBorder> >* >();
                    for (std::vector<QSharedPointer<HSSBorder> >::const_iterator it = values.begin(); it != values.end(); ++it)
                    {
                        this->border.push_back(*it);
                    }
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "no")
        {
            valid = true;
            break;
        }
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dBorder = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeBorder))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                objdef->setScope(thisCont->getChildren());
            }

            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyTarget, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::borderChanged));
            this->border.push_back(qSharedPointerCast<HSSBorder > (theObj));
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for border of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyBorder, &this->border);
}

void HSSDisplayObject::borderChanged(HSSObservableProperty source, void*data)
{

}

bool HSSDisplayObject::getVisible() const
{
    return this->visible;
}

const QSharedPointer<HSSParserNode> HSSDisplayObject::getDVisible() const
{
    return this->dVisible;
}

void HSSDisplayObject::setDVisible(QSharedPointer<HSSParserNode> value)
{
    this->dVisible = value;
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer * thisCont = static_cast<HSSContainer *> (this);
                fnct->setScope(thisCont->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<bool* >())
            {
                this->visible = *remoteValue.value<bool*>();
                valid = true;
            }

            if (this->observedVisible)
            {
                this->observedVisible->removeObserver(this->observedVisibleProperty, HSSObservablePropertyVisible, this);
            }
            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyVisible, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::visibleChanged));
            this->observedVisible = fnct;
            this->observedVisibleProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        AXRString kwValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (kwValue == "inherit")
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                QVariant remoteValue = parent->getProperty(HSSObservablePropertyVisible);
                if (remoteValue.canConvert<bool *>())
                {
                    this->visible = *(remoteValue.value<bool *>());
                    if (this->observedVisible)
                    {
                        this->observedVisible->removeObserver(this->observedVisibleProperty, HSSObservablePropertyVisible, this);
                    }
                    parent->observe(HSSObservablePropertyVisible, HSSObservablePropertyVisible, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::visibleChanged));
                    this->observedVisible = parent;
                    this->observedVisibleProperty = HSSObservablePropertyVisible;
                }
            }
            else
            {
                if (this->observedVisible)
                {
                    this->observedVisible->removeObserver(this->observedVisibleProperty, HSSObservablePropertyVisible, this);
                    this->observedVisible.clear();
                }
                this->visible = true;
            }

            valid = true;
        }
        else if (kwValue == "yes" || kwValue == "no")
        {
            if (this->observedVisible)
            {
                this->observedVisible->removeObserver(this->observedVisibleProperty, HSSObservablePropertyVisible, this);
                this->observedVisible.clear();
            }
            this->visible = (kwValue == "yes");
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for visible of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyVisible, &this->visible);
}

void HSSDisplayObject::visibleChanged(HSSObservableProperty source, void*data)
{
    HSSParserNodeType nodeType = this->dVisible->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeFunctionCall:
    case HSSParserNodeTypeKeywordConstant:
    {
        this->visible = *(bool *) data;
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyVisible, &this->visible);
}


//defaults

void HSSDisplayObject::setDefaults()
{
    AXRController * controller = this->getController();

    //width
    QSharedPointer<HSSPercentageConstant> newDWidth(new HSSPercentageConstant(100., controller));
    this->setDWidth(newDWidth);
    //height
    QSharedPointer<HSSKeywordConstant> newDHeight(new HSSKeywordConstant("content", controller));
    this->setDHeight(newDHeight);
    //anchorX
    QSharedPointer<HSSKeywordConstant> newDAnchorX(new HSSKeywordConstant("no", controller));
    this->setDAnchorX(newDAnchorX);
    //anchorY
    QSharedPointer<HSSKeywordConstant> newDAnchorY(new HSSKeywordConstant("no", controller));
    this->setDAnchorY(newDAnchorY);
    //flow
    QSharedPointer<HSSKeywordConstant> newDFlow(new HSSKeywordConstant("yes", controller));
    this->setDFlow(newDFlow);
    //alignX
    QSharedPointer<HSSKeywordConstant> newDAlignX(new HSSKeywordConstant("auto", controller));
    this->setDAlignX(newDAlignX);
    //alignY
    QSharedPointer<HSSKeywordConstant> newDAlignY(new HSSKeywordConstant("auto", controller));
    this->setDAlignY(newDAlignY);
    //background
    QSharedPointer<HSSKeywordConstant> newDBackground(new HSSKeywordConstant("no", controller));
    this->setDBackground(newDBackground);
    //content
    QSharedPointer<HSSKeywordConstant> newDContent(new HSSKeywordConstant("no", controller));
    this->setDContent(newDContent);
    //on
    QSharedPointer<HSSKeywordConstant> newDOn(new HSSKeywordConstant("no", controller));
    this->setDOn(newDOn);
    //border
    QSharedPointer<HSSKeywordConstant> newDBorder(new HSSKeywordConstant("no", controller));
    this->setDBorder(newDBorder);
    //visible
    QSharedPointer<HSSKeywordConstant> newDVisible(new HSSKeywordConstant("inherit", controller));
    this->setDVisible(newDVisible);
}

HSSUnit HSSDisplayObject::_evaluatePropertyValue(
                                             void(HSSDisplayObject::*callback)(HSSObservableProperty property, void* data),
                                             QSharedPointer<HSSParserNode> value,
                                             HSSUnit percentageBase,
                                             HSSObservableProperty observedProperty,
                                             QSharedPointer<HSSObservable> observedObject,
                                             HSSObservableProperty observedSourceProperty,
                                             QSharedPointer<HSSObservable> &observedStore,
                                             HSSObservableProperty &observedStoreProperty,
                                             QSharedPointer<HSSSimpleSelection> scope
                                             )
{
    HSSUnit ret = 0.;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        QSharedPointer<HSSNumberConstant> numberValue = qSharedPointerCast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        observedStore.clear();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        if (callback)
        {
            observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSDisplayObject > (this, callback));
            observedStore = observedObject;
            observedStoreProperty = observedProperty;
        }
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setPercentageObserved(observedProperty, observedObject.data());
        expressionValue->setScope(scope);
        expressionValue->setThisObj(this->shared_from_this());
        ret = expressionValue->evaluate();
        if (callback)
        {
            observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSDisplayObject > (this, callback));
            observedStore = observedObject;
            observedStoreProperty = observedProperty;
        }
        else
        {
            observedStore.clear();
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        fnct->setPercentageBase(percentageBase);
        fnct->setPercentageObserved(observedProperty, observedObject.data());
        fnct->setScope(scope);
        fnct->setThisObj(this->shared_from_this());

        QVariant remoteValue = fnct->evaluate();
        if (remoteValue.canConvert<HSSUnit*>())
        {
            ret = *remoteValue.value<HSSUnit*>();
        }
        else if (remoteValue.canConvert<AXRString>())
        {
            ret = (remoteValue.value<AXRString>()).toLong();
        }

        if (callback)
        {
            fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSDisplayObject > (this, callback));
            observedStore = fnct;
            observedStoreProperty = HSSObservablePropertyValue;
        }
        else
        {
            observedStore.clear();
        }

        break;
    }

    default:
        break;
    }

    return ret;
}

void HSSDisplayObject::_setInnerWidth()
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
    this->notifyObservers(HSSObservablePropertyInnerWidth, &this->innerWidth);
}

void HSSDisplayObject::_setInnerHeight()
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
    this->notifyObservers(HSSObservablePropertyInnerHeight, &this->innerHeight);
}

void HSSDisplayObject::_setOuterWidth()
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
    this->notifyObservers(HSSObservablePropertyOuterWidth, &this->outerWidth);
}

void HSSDisplayObject::_setOuterHeight()
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
    this->notifyObservers(HSSObservablePropertyOuterHeight, &this->outerHeight);
}

bool HSSDisplayObject::handleEvent(HSSEventType type, void* data)
{
    switch (type)
    {
    case HSSEventTypeMouseDown:
    {
        HSSPoint thePoint = *(HSSPoint*) data;

        if (this->globalX < thePoint.x && this->globalX + this->width >= thePoint.x
            && this->globalY < thePoint.y && this->globalY + this->height >= thePoint.y)
        {
            if (!this->_isPress)
            {
                this->setPress(true);
            }

            return this->fireEvent(type);
        }
        break;
    }
    case HSSEventTypeMouseUp:
    {
        HSSPoint thePoint = *(HSSPoint*) data;

        if (this->_isPress)
        {
            this->setPress(false);
        }

        if (this->globalX < thePoint.x && this->globalX + this->width >= thePoint.x
            && this->globalY < thePoint.y && this->globalY + this->height >= thePoint.y)
        {

            return this->fireEvent(type);
        }
        break;
    }
    case HSSEventTypeClick:
    case HSSEventTypeDoubleClick:
    {
        HSSPoint thePoint = *(HSSPoint*) data;

        if (this->globalX < thePoint.x && this->globalX + this->width >= thePoint.x
                && this->globalY < thePoint.y && this->globalY + this->height >= thePoint.y)
        {

            //std_log(this->getElementName());
            return this->fireEvent(type);
        }
        break;
    }

    case HSSEventTypeLoad:
    {
        return this->fireEvent(type);
    }


    case HSSEventTypeMouseMove:
    {
        HSSPoint thePoint = *(HSSPoint*) data;

        if (this->globalX < thePoint.x && this->globalX + this->width >= thePoint.x
                && this->globalY < thePoint.y && this->globalY + this->height >= thePoint.y)
        {

            if (!this->_isHover)
            {
                this->setHover(true);
            }
            //std_log(this->getElementName());
            return this->fireEvent(type);
        }
        else if (this->_isHover)
        {
            this->setHover(false);
        }
        break;
    }

    case HSSEventTypeExitedWindow:
    {
        if (this->_isHover)
        {
            this->setHover(false);
        }

        return this->fireEvent(type);
    }

    default:
        AXRError("HSSDisplayObject", "Unknown event type").raise();
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

QSharedPointer<HSSDisplayObject> HSSDisplayObject::shared_from_this()
{
    return qSharedPointerCast<HSSDisplayObject > (HSSObject::shared_from_this());
}

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

#include <cmath>
#include <QVariant>
#include <QImage>
#include <QPainterPath>
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRWarning.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSFlag.h"
#include "HSSLinearGradient.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSParserNode.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h";
#include "HSSStringConstant.h"

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
    this->backgroundSurface = new QImage();
    this->foregroundSurface = new QImage();
    this->bordersSurface = new QImage();
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

    this->drawIndex = this->_index = 0;
    this->tabIndex = this->zoomFactor = 1;
    this->flow = this->visible = true;
    this->contained = true;
    /**
     *  @todo change to camelCase
     */
    this->does_float = false;
    this->heightByContent = this->widthByContent = false;
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
    this->registerProperty(HSSObservablePropertyContent, QVariant::fromValue(&this->content));
    this->registerProperty(HSSObservablePropertyFont, QVariant::fromValue(&this->font));
    this->registerProperty(HSSObservablePropertyOn, QVariant::fromValue(&this->on));
    this->registerProperty(HSSObservablePropertyBorder, QVariant::fromValue(&this->border));
    this->registerProperty(HSSObservablePropertyVisible, QVariant::fromValue(&this->visible));

    this->_isHover = false;
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
}

HSSDisplayObject::~HSSDisplayObject()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("destroying display object with name ").append(this->name));
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

    delete this->backgroundSurface;
    delete this->foregroundSurface;
    delete this->bordersSurface;
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

HSSSimpleSelection::p HSSDisplayObject::getSiblings()
{
    HSSSimpleSelection::p ret(new HSSSimpleSelection());
    HSSSimpleSelection::p children = this->getParent()->getChildren();
    for (HSSSimpleSelection::const_iterator it = children->begin(); it != children->end(); ++it)
    {
        const HSSDisplayObject::p theDO = *it;
        if(theDO.data() != this)
        {
            ret->add(theDO);
        }
    }
    return ret;
}

HSSSimpleSelection::p HSSDisplayObject::getNextSiblings()
{
    HSSSimpleSelection::p ret(new HSSSimpleSelection());
    HSSSimpleSelection::p children = this->getParent()->getChildren();
    HSSSimpleSelection::iterator thisPos = std::find(children->begin(), children->end(), this->shared_from_this());
    ret->insert(ret->end(), thisPos+1, children->end());
    return ret;
}

HSSSimpleSelection::p HSSDisplayObject::getPreviousSiblings()
{
    HSSSimpleSelection::p ret(new HSSSimpleSelection());
    HSSSimpleSelection::p children = this->getParent()->getChildren();
    HSSSimpleSelection::iterator thisPos = std::find(children->begin(), children->end(), this->shared_from_this());
    ret->insert(ret->end(), children->begin(), thisPos);
    return ret;
}

void HSSDisplayObject::setIndex(unsigned newIndex)
{
    this->_index = newIndex;
}

unsigned HSSDisplayObject::getIndex()
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

void HSSDisplayObject::rulesAdd(HSSRule::p newRule, HSSRuleState defaultState)
{
    if (!this->hasRule(newRule))
    {
        //the target property is set to HSSObservablePropertyValue -- should this be something else?
        newRule->observe(HSSObservablePropertyValue, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::ruleChanged));

        newRule->appliedToAdd(this->shared_from_this());

        HSSRuleStatus::p newRS = HSSRuleStatus::p(new HSSRuleStatus());
        newRS->state = defaultState;
        newRS->rule = newRule;

        this->rules.push_back(newRS);

        //iterate over the property defs and try to find an isA property
        const std::vector<HSSPropertyDefinition::p> & props = newRule->getProperties();
        HSSPropertyDefinition::const_it it;
        for (it = props.begin(); it != props.end(); ++it)
        {
            HSSPropertyDefinition::p propdef = *it;
            if (propdef->getName() == "isA")
            {
                this->rulesAddIsAChildren(propdef, defaultState);
            }
        }
    }
}

void HSSDisplayObject::rulesAddIsAChildren(HSSPropertyDefinition::p propdef, HSSRuleState defaultState)
{
    HSSParserNode::p value = propdef->getValue();

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());

            if (this->isA(HSSObjectTypeContainer))
            {
                HSSContainer::p thisCont = qSharedPointerCast<HSSContainer > (this->shared_from_this());
                this->getController()->currentContext.push(thisCont);
                HSSRule::const_it it;
                const std::deque<HSSRule::p> rules = objdef->getRules();
                AXRController * controller = this->getController();
                for (it = rules.begin(); it != rules.end(); ++it)
                {
                    HSSRule::p clonedRule = (*it)->clone();
                    HSSSelection::p children = thisCont->getChildren();
                    controller->recursiveMatchRulesToDisplayObjects(clonedRule, children, thisCont, true);
                    controller->activateRules(clonedRule, children);
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

HSSRule::p HSSDisplayObject::rulesGet(unsigned index)
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

int HSSDisplayObject::rulesSize()
{
    return this->rules.size();
}

void HSSDisplayObject::setRuleStatus(HSSRule::p rule, HSSRuleState newValue)
{
    bool changed = false;
    bool found = false;

    std::vector<HSSRuleStatus::p>::iterator it;
    for (it = this->rules.begin(); it != this->rules.end(); ++it)
    {
        HSSRuleStatus::p rs = *it;
        HSSRule::p existingRule = rs->rule;
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

bool HSSDisplayObject::hasRule(HSSRule::p rule)
{
    bool found = false;
    std::vector<HSSRuleStatus::p>::iterator it;
    for (it = this->rules.begin(); it != this->rules.end(); ++it)
    {
        HSSRuleStatus::p status = *it;
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
            AXRRender::p render = controller->document()->getRender();
            //width
            HSSNumberConstant::p newDWidth(new HSSNumberConstant(render->getWindowWidth(), controller));
            this->setDWidth(newDWidth);
            //height
            HSSNumberConstant::p newDHeight(new HSSNumberConstant(render->getWindowHeight(), controller));
            this->setDHeight(newDHeight);
        }

        AXRString propertyName;
        for (i = 0; i<this->rules.size(); ++i)
        {
            HSSRuleStatus::p & ruleStatus = this->rules[i];
            switch (ruleStatus->state)
            {
            case HSSRuleStateActivate:
            {
                ruleStatus->state = HSSRuleStateOn;
                //fall through
            }

            case HSSRuleStateOn:
            {
                HSSRule::p & rule = ruleStatus->rule;
                for (j = 0; j < rule->propertiesSize(); ++j)
                {
                    try
                    {
                        HSSPropertyDefinition::p& propertyDefinition = rule->propertiesGet(j);
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

void HSSDisplayObject::setProperty(HSSObservableProperty name, HSSParserNode::p value)
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

void HSSDisplayObject::regenerateSurfaces(bool force)
{
    if (force || this->_needsSurface)
    {
        delete this->backgroundSurface;
        this->backgroundSurface = new QImage(ceil(this->width), ceil(this->height), QImage::Format_ARGB32_Premultiplied);
        this->backgroundSurface->fill(Qt::transparent);

        delete this->foregroundSurface;
        this->foregroundSurface = new QImage(ceil(this->width), ceil(this->height), QImage::Format_ARGB32_Premultiplied);
        this->foregroundSurface->fill(Qt::transparent);

        if (this->border.size() > 0)
        {
            // Get the sum of all sizes
            this->borderBleeding = 0;
            for (HSSBorder::it it = this->border.begin(); it != this->border.end(); ++it)
            {
                this->borderBleeding += (*it)->getSize();
            }

            delete this->bordersSurface;
            this->bordersSurface = new QImage(ceil(this->width + this->borderBleeding + this->borderBleeding), ceil(this->height + this->borderBleeding + this->borderBleeding), QImage::Format_ARGB32_Premultiplied);
            this->bordersSurface->fill(Qt::transparent);
        }

        this->setDirty(true);
        this->_needsSurface = false;

        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("HSSDisplayObject: created a new surface width:%1 height:%2").arg(this->width).arg(this->height));
    }
}

void HSSDisplayObject::recursiveRegenerateSurfaces(bool force)
{
    this->regenerateSurfaces(force);
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
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSDisplayObject: setting dirty on " + this->elementName);
    this->_isDirty = value;
}

bool HSSDisplayObject::isDirty()
{
    return this->_isDirty;
}

void HSSDisplayObject::draw(QPainter &painter)
{
    AXRDocument *document = this->getController()->document();
    if (document->showLayoutSteps())
    {
        document->nextLayoutTick();
        if (document->layoutChildDone()) return;
    }

    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSDisplayObject: drawing " + this->elementName);
    if (this->_isDirty)
    {
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("HSSDisplayObject: redrawing contents of %1 with x: %2 and y: %3").arg(this->elementName).arg(this->x).arg(this->y));

        this->_isDirty = false;
        this->drawBackground();
        this->drawForeground();
        if (this->border.size() > 0)
        {
            this->drawBorders();
        }
    }

    if (this->visible)
    {
        painter.drawImage(QPointF(this->globalX, this->globalY), *this->backgroundSurface);
        painter.drawImage(QPointF(this->globalX, this->globalY), *this->foregroundSurface);

        if (this->border.size() > 0)
        {
            painter.drawImage(QPointF(this->globalX - this->borderBleeding, this->globalY - this->borderBleeding), *this->bordersSurface);
        }
    }
}

void HSSDisplayObject::drawBackground()
{
    this->backgroundSurface->fill(Qt::transparent);

    QPainter painter(this->backgroundSurface);
    if (this->getController()->document()->getRender()->globalAntialiasingEnabled())
        painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRect(0, 0, this->width, this->height);
    this->_drawBackground(painter, path);
}

void HSSDisplayObject::_drawBackground(QPainter &painter, const QPainterPath &path)
{
    for (std::vector<HSSObject::p>::iterator it = this->background.begin(); it != this->background.end(); ++it)
    {
        HSSObject::p theobj = *it;
        switch (theobj->getObjectType())
        {
        case HSSObjectTypeRgb:
        {
            HSSRgb::p color = qSharedPointerCast<HSSRgb > (theobj);

            HSSUnit r = 0., g = 0., b = 0., a = 0;
            if (color)
            {
                r = color->getRed();
                g = color->getGreen();
                b = color->getBlue();
                a = color->getAlpha();
            }

            painter.fillPath(path, QColor(r, g, b, a));

            break;
        }

        case HSSObjectTypeGradient:
        {
            HSSLinearGradient::p grad = qSharedPointerCast<HSSLinearGradient > (theobj);
            grad->draw(painter, path);
            break;
        }

        default:
            break;
        }
    }
}

void HSSDisplayObject::drawForeground()
{
}

void HSSDisplayObject::drawBorders()
{
}

void HSSDisplayObject::recursiveDraw(QPainter &painter)
{
    this->draw(painter);
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

HSSParserNode::p HSSDisplayObject::getDWidth()
{
    return this->dWidth;
}

void HSSDisplayObject::setDWidth(HSSParserNode::p value)
{
    bool valid = false;
    bool needsPostProcess = true;
    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
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
        HSSContainer::p parentContainer = this->getParent();
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
            HSSContainer::p parent = this->getParent();
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
        HSSContainer::p parent = this->getParent();
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
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            HSSParserNode::p remoteValue = qSharedPointerCast<HSSValue > (theobj)->getDValue();
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
        HSSPercentageConstant::p widthValue = qSharedPointerCast<HSSPercentageConstant > (this->dWidth);
        this->width = floor(widthValue->getValue(*(HSSUnit*) data));
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p widthExpression = qSharedPointerCast<HSSExpression > (this->dWidth);
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

HSSParserNode::p HSSDisplayObject::getDHeight()
{
    return this->dHeight;
}

void HSSDisplayObject::setDHeight(HSSParserNode::p value)
{
    bool valid = false;
    bool needsPostProcess = true;
    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
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
        HSSContainer::p parentContainer = this->getParent();
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
            HSSContainer::p parent = this->getParent();
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
        HSSContainer::p parent = this->getParent();
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
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            HSSParserNode::p remoteValue = qSharedPointerCast<HSSValue > (theobj)->getDValue();
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
        HSSPercentageConstant::p heightValue = qSharedPointerCast<HSSPercentageConstant > (this->dHeight);
        this->height = floor(heightValue->getValue(*(HSSUnit*) data));
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p heightExpression = qSharedPointerCast<HSSExpression > (this->dHeight);
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

HSSParserNode::p HSSDisplayObject::getDAnchorX()
{
    return this->dAnchorX;
}

void HSSDisplayObject::setDAnchorX(HSSParserNode::p value)
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
        HSSContainer::p parentContainer = this->getParent();
        HSSSimpleSelection::p scope;
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
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
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAnchorX);
        this->anchorX = percentageValue->getValue(*(HSSUnit*) data);
        this->_anchorXdefault = false;
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression > (this->dAnchorX);
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (this->dAnchorX);
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

HSSParserNode::p HSSDisplayObject::getDAnchorY()
{
    return this->dAnchorY;
}

void HSSDisplayObject::setDAnchorY(HSSParserNode::p value)
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
        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        if (this->observedAnchorY)
        {
            this->observedAnchorY->removeObserver(this->observedAnchorYProperty, HSSObservablePropertyAnchorY, this);
        }
        HSSContainer::p parentContainer = this->getParent();
        HSSSimpleSelection::p scope;
        if (parentContainer)
        {
            scope = parentContainer->getChildren();
        }
        this->anchorY = this->_evaluatePropertyValue(
                                             &HSSDisplayObject::anchorYChanged,
                                             value,
                                             this->width,
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
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
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAnchorY);
        this->anchorY = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression > (this->dAnchorY);
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (this->dAnchorY);
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

HSSParserNode::p HSSDisplayObject::getDFlow()
{
    return this->dFlow;
}

void HSSDisplayObject::setDFlow(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dFlow = value;
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            HSSContainer::p parent = this->getParent();
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        HSSContainer::p parent = this->getParent();
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
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->flow = (bool)qSharedPointerCast<HSSValue>(theobj)->getIntValue();
            std_log("########### FIXME #################");
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

HSSParserNode::p HSSDisplayObject::getDContained()
{
    return this->dContained;
}

void HSSDisplayObject::setDContained(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dContained = value;
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            HSSContainer::p parent = this->getParent();
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
                this->contained = remoteValue.toBool();
            }
            else
            {
                this->contained = false;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyContained, this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::containedChanged));
            valid = true;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        HSSContainer::p parent = this->getParent();
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
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->contained = (bool)qSharedPointerCast<HSSValue>(theobj)->getIntValue();
            std_log("########### FIXME #################");
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

HSSParserNode::p HSSDisplayObject::getDAlignX()
{
    return this->dAlignX;
}

void HSSDisplayObject::setDAlignX(HSSParserNode::p value)
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

        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "auto")
        {
            HSSContainer::p parentContainer = this->getParent();
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
            this->setDAlignX(HSSParserNode::p(new HSSNumberConstant(0, this->getController())));
        }
        else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center")
        {
            this->setDAlignX(HSSParserNode::p(new HSSPercentageConstant(50, this->getController())));
        }
        else if (keywordValue->getValue() == "right")
        {
            this->setDAlignX(HSSParserNode::p(new HSSPercentageConstant(100, this->getController())));
        }
        else
        {
            throw AXRWarning("HSSDisplayObject", "Invalid value for alignX of " + this->getElementName());
        }

    }
    else
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        HSSContainer::p parentContainer = this->getParent();
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
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAlignX);
        this->alignX = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression > (this->dAlignX);
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

HSSParserNode::p HSSDisplayObject::getDAlignY()
{
    return this->dAlignX;
}

void HSSDisplayObject::setDAlignY(HSSParserNode::p value)
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

        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "auto")
        {
            HSSContainer::p parentContainer = this->getParent();
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
            this->setDAlignY(HSSParserNode::p(new HSSNumberConstant(0, this->getController())));
        }
        else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center")
        {
            this->setDAlignY(HSSParserNode::p(new HSSPercentageConstant(50, this->getController())));
        }
        else if (keywordValue->getValue() == "bottom")
        {
            this->setDAlignY(HSSParserNode::p(new HSSPercentageConstant(100, this->getController())));
        }
        else
        {
            throw AXRWarning("HSSDisplayObject", "Invalid value for alignY of " + this->getElementName());
        }

    }
    else
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
        HSSContainer::p parentContainer = this->getParent();
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
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAlignY);
        this->alignY = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression > (this->dAlignY);
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

HSSParserNode::p HSSDisplayObject::getDBackground()
{
    return this->dBackground;
}

void HSSDisplayObject::setDBackground(HSSParserNode::p value)
{
    this->background.clear();
    this->dBackground = value;
    this->addDBackground(value);
}

void HSSDisplayObject::addDBackground(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
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
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            HSSContainer::p parent = this->getParent();
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
            if (remoteValue.canConvert<std::vector<HSSObject::p>*>())
            {
                std::vector<HSSObject::p> values = *remoteValue.value<std::vector<HSSObject::p>*>();

                for (std::vector<HSSObject::p>::const_iterator it = values.begin(); it != values.end(); ++it)
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
        HSSKeywordConstant::p theKW = qSharedPointerCast<HSSKeywordConstant>(value);
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeRgb) || objdef->getObject()->isA(HSSObjectTypeGradient))
        {
            HSSContainer::p parent = this->getParent();
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
            HSSObject::p theObj = objdef->getObject();
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

HSSParserNode::p HSSDisplayObject::getDContent()
{
    return this->dContent;
}

void HSSDisplayObject::setDContent(HSSParserNode::p value)
{
    this->content.clear();
    this->dContent = value;
    this->addDContent(value);
}

void HSSDisplayObject::addDContent(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
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
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct)
        {
            switch (fnct->getFunctionType())
            {
            case HSSFunctionTypeRef:
            {
                HSSContainer::p parent = this->getParent();
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
                if (remoteValue.canConvert<HSSParserNode::p>())
                {
                    try
                    {
                        HSSParserNode::p theVal = remoteValue.value<HSSParserNode::p>();
                        this->addDContent(theVal);
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
                HSSContainer::p parent = this->getParent();
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeRgb) || objdef->getObject()->isA(HSSObjectTypeGradient))
        {
            HSSContainer::p parent = this->getParent();
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
            HSSObject::p theObj = objdef->getObject();
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
    std_log("unimplemented yet");
}

const HSSParserNode::p HSSDisplayObject::getDFont() const
{
    return this->dFont;
}

void HSSDisplayObject::setDFont(HSSParserNode::p value)
{
    this->font.clear();
    this->dFont = value;
    this->addDFont(value);
}

void HSSDisplayObject::addDFont(HSSParserNode::p value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
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
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
            HSSContainer::p parent = this->getParent();
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

            HSSObject::p obj = qSharedPointerCast<HSSObject > (objdef->getObject());
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            HSSContainer::p parent = this->getParent();
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
            if (remoteValue.canConvert<HSSParserNode::p>())
            {
                HSSParserNode::p theVal = remoteValue.value<HSSParserNode::p>();
                this->addDFont(theVal);
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
            HSSContainer::p parent = this->getParent();
            QVariant remoteValue = parent->getProperty(HSSObservablePropertyFont);
            if (remoteValue.canConvert<std::vector<HSSFont::p> *>())
            {
                this->font = *remoteValue.value<std::vector<HSSFont::p> *>();
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeFont))
        {
            HSSContainer::p parent = this->getParent();
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
            HSSObject::p theObj = objdef->getObject();
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
        this->font = *(std::vector<HSSFont::p> *) data;
        this->setDirty(true);
        break;
    }

    default:
        break;
    }

    switch (this->dFont->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
        this->font = *(std::vector<HSSFont::p> *) data;
        this->setDirty(true);
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyFont, &this->font);
}


//on

HSSParserNode::p HSSDisplayObject::getDOn()
{
    return this->dOn;
}

void HSSDisplayObject::setDOn(HSSParserNode::p value)
{
    this->on.clear();
    this->dOn = value;
    this->addDOn(value);
}

void HSSDisplayObject::addDOn(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
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
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            HSSContainer::p parent = this->getParent();
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
            if (remoteValue.canConvert<HSSParserNode::p>())
            {
                try
                {
                    HSSParserNode::p theVal = remoteValue.value<HSSParserNode::p>();
                    this->addDOn(theVal);
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
        if (objdef->getObject()->isA(HSSObjectTypeEvent))
        {
            HSSContainer::p parent = this->getParent();
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
            HSSObject::p theObj = objdef->getObject();

            HSSEvent::p theEvent = qSharedPointerCast<HSSEvent > (objdef->getObject());
            HSSEventType eventType = theEvent->getEventType();
            if (this->on.find(eventType) == this->on.end())
            {
                std::vector<HSSObject::p>newVector;
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
        std::vector<HSSObject::p> events = this->on[type];
        HSSObject::it it;
        for (it = events.begin(); it != events.end(); ++it)
        {
            if ((*it)->isA(HSSObjectTypeEvent))
            {
                HSSEvent::p theEvent = qSharedPointerCast<HSSEvent > (*it);
                axr_log(AXR_DEBUG_CH_EVENTS_SPECIFIC, "HSSDisplayObject: firing event: " + theEvent->toString());
                theEvent->fire();
                fired = true;
            }
        }
    }

    if (fired)
    {
        axr_log(AXR_DEBUG_CH_EVENTS | AXR_DEBUG_CH_EVENTS_SPECIFIC, "HSSDisplayObject: fired event");
    }

    return fired;
}

const HSSParserNode::p HSSDisplayObject::getDMargin() const
{
    return this->dMargin;
}

void HSSDisplayObject::setDMargin(HSSParserNode::p value)
{
    this->margin.clear();
    this->dMargin = value;
    this->addDMargin(value);
}

void HSSDisplayObject::addDMargin(HSSParserNode::p value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
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
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
            HSSContainer::p parent = this->getParent();
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

            HSSObject::p obj = qSharedPointerCast<HSSObject > (objdef->getObject());
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            HSSContainer::p parent = this->getParent();
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
            if (remoteValue.canConvert<HSSParserNode::p>())
            {
                try
                {
                    HSSParserNode::p theVal = remoteValue.value<HSSParserNode::p>();
                    this->addDMargin(theVal);
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
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
        HSSMargin::p theMargin = HSSMargin::p(new HSSMargin(this->getController()));
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeMargin))
        {
            HSSContainer::p parent = this->getParent();
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
            HSSObject::p theObj = objdef->getObject();
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

const HSSParserNode::p HSSDisplayObject::getDPadding() const
{
    return this->dPadding;
}

void HSSDisplayObject::setDPadding(HSSParserNode::p value)
{
    this->padding.clear();
    this->dPadding = value;
    this->addDPadding(value);
}

void HSSDisplayObject::addDPadding(HSSParserNode::p value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
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
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
            HSSContainer::p parent = this->getParent();
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

            HSSObject::p obj = qSharedPointerCast<HSSObject > (objdef->getObject());
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            HSSContainer::p parent = this->getParent();
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
            if (remoteValue.canConvert<HSSParserNode::p>())
            {
                try
                {
                    HSSParserNode::p theVal = remoteValue.value<HSSParserNode::p>();
                    this->addDPadding(theVal);
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
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
        HSSMargin::p thePadding = HSSMargin::p(new HSSMargin(this->getController()));
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeMargin))
        {
            HSSContainer::p parent = this->getParent();
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
            HSSObject::p theObj = objdef->getObject();
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

const HSSParserNode::p HSSDisplayObject::getDBorder() const
{
    return this->dBorder;
}

void HSSDisplayObject::setDBorder(HSSParserNode::p value)
{
    this->border.clear();
    this->dBorder = value;
    this->addDBorder(value);
}

void HSSDisplayObject::addDBorder(HSSParserNode::p value)
{
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
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
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
            HSSContainer::p parent = this->getParent();
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

            HSSObject::p obj = qSharedPointerCast<HSSObject > (objdef->getObject());
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {

            HSSContainer::p parent = this->getParent();
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
            if (remoteValue.canConvert<HSSParserNode::p>())
            {
                try
                {
                    HSSParserNode::p theVal = remoteValue.value<HSSParserNode::p>();
                    this->addDBorder(theVal);
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
        HSSKeywordConstant::p keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
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
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeBorder))
        {
            HSSContainer::p parent = this->getParent();
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
            HSSObject::p theObj = objdef->getObject();
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

const HSSParserNode::p HSSDisplayObject::getDVisible() const
{
    return this->dVisible;
}

void HSSDisplayObject::setDVisible(HSSParserNode::p value)
{
    this->dVisible = value;
    bool valid = false;
    switch (value->getType())
    {
    case HSSParserNodeTypeFunctionCall:
    {
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            HSSContainer::p parent = this->getParent();
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
                this->visible = remoteValue.toBool();
                valid = true;
            }
            else
            {
                this->visible = true;
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
            HSSContainer::p parent = this->getParent();
            if (parent)
            {
                QVariant remoteValue = parent->getProperty(HSSObservablePropertyVisible);
                if (remoteValue.canConvert<bool>())
                {
                    this->visible = remoteValue.toBool();
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
    HSSPercentageConstant::p newDWidth(new HSSPercentageConstant(100., controller));
    this->setDWidth(newDWidth);
    //height
    HSSKeywordConstant::p newDHeight(new HSSKeywordConstant("content", controller));
    this->setDHeight(newDHeight);
    //anchorX
    HSSKeywordConstant::p newDAnchorX(new HSSKeywordConstant("no", controller));
    this->setDAnchorX(newDAnchorX);
    //anchorY
    HSSKeywordConstant::p newDAnchorY(new HSSKeywordConstant("no", controller));
    this->setDAnchorY(newDAnchorY);
    //flow
    HSSKeywordConstant::p newDFlow(new HSSKeywordConstant("yes", controller));
    this->setDFlow(newDFlow);
    //alignX
    HSSKeywordConstant::p newDAlignX(new HSSKeywordConstant("auto", controller));
    this->setDAlignX(newDAlignX);
    //alignY
    HSSKeywordConstant::p newDAlignY(new HSSKeywordConstant("auto", controller));
    this->setDAlignY(newDAlignY);
    //background
    HSSKeywordConstant::p newDBackground(new HSSKeywordConstant("no", controller));
    this->setDBackground(newDBackground);
    //content
    HSSKeywordConstant::p newDContent(new HSSKeywordConstant("no", controller));
    this->setDContent(newDContent);
    //on
    HSSKeywordConstant::p newDOn(new HSSKeywordConstant("no", controller));
    this->setDOn(newDOn);
    //border
    HSSKeywordConstant::p newDBorder(new HSSKeywordConstant("no", controller));
    this->setDBorder(newDBorder);
    //visible
    HSSKeywordConstant::p newDVisible(new HSSKeywordConstant("inherit", controller));
    this->setDVisible(newDVisible);
}

HSSUnit HSSDisplayObject::_evaluatePropertyValue(
                                             void(HSSDisplayObject::*callback)(HSSObservableProperty property, void* data),
                                             HSSParserNode::p value,
                                             HSSUnit percentageBase,
                                             HSSObservableProperty observedProperty,
                                             HSSObservable::p observedObject,
                                             HSSObservableProperty observedSourceProperty,
                                             HSSObservable::p &observedStore,
                                             HSSObservableProperty &observedStoreProperty,
                                             HSSSimpleSelection::p scope
                                             )
{
    HSSUnit ret = 0.;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        HSSNumberConstant::p numberValue = qSharedPointerCast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        observedStore.clear();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (value);
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
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setPercentageObserved(observedProperty, observedObject.data());
        expressionValue->setScope(scope);
        expressionValue->setThisObj(this->shared_from_this());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSDisplayObject > (this, callback));
            observedStore = expressionValue;
            observedStoreProperty = HSSObservablePropertyValue;
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
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
    std::vector<HSSMargin::p>::const_iterator it;
    HSSUnit innerWidth = this->width;
    this->rightPadding = this->leftPadding = 0;
    for (it = this->padding.begin(); it != this->padding.end(); ++it)
    {
        HSSMargin::p theMargin = *it;
        innerWidth -= theMargin->getLeft() + theMargin->getRight();
        this->rightPadding += theMargin->getRight();
        this->leftPadding += theMargin->getLeft();
    }
    this->innerWidth = innerWidth;
    this->notifyObservers(HSSObservablePropertyInnerWidth, &this->innerWidth);
}

void HSSDisplayObject::_setInnerHeight()
{
    std::vector<HSSMargin::p>::const_iterator it;
    HSSUnit innerHeight = this->height;
    this->topPadding = this->bottomPadding = 0;
    for (it = this->padding.begin(); it != this->padding.end(); ++it)
    {
        HSSMargin::p theMargin = *it;
        innerHeight -= theMargin->getTop() + theMargin->getBottom();
        this->topPadding += theMargin->getTop();
        this->bottomPadding += theMargin->getBottom();
    }
    this->innerHeight = innerHeight;
    this->notifyObservers(HSSObservablePropertyInnerHeight, &this->innerHeight);
}

void HSSDisplayObject::_setOuterWidth()
{
    std::vector<HSSMargin::p>::const_iterator it;
    HSSUnit outerWidth = this->width;
    this->rightMargin = this->leftMargin = 0;
    for (it = this->margin.begin(); it != this->margin.end(); ++it)
    {
        HSSMargin::p theMargin = *it;
        outerWidth += theMargin->getLeft() + theMargin->getRight();
        this->rightMargin += theMargin->getRight();
        this->leftMargin += theMargin->getLeft();
    }
    this->outerWidth = outerWidth;
    this->notifyObservers(HSSObservablePropertyOuterWidth, &this->outerWidth);
}

void HSSDisplayObject::_setOuterHeight()
{
    std::vector<HSSMargin::p>::const_iterator it;
    HSSUnit outerHeight = this->height;
    this->topMargin = this->bottomMargin = 0;
    for (it = this->margin.begin(); it != this->margin.end(); ++it)
    {
        HSSMargin::p theMargin = *it;
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
    case HSSEventTypeMouseUp:
    case HSSEventTypeClick:
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
        throw AXRError("HSSDisplayObject", "Unknown event type");
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

void HSSDisplayObject::ruleChanged(HSSObservableProperty source, void*data)
{
    //HSSRule * theRule = (HSSRule*)data;
    this->setNeedsRereadRules(true);
    this->getController()->document()->setNeedsDisplay(true);
}

void HSSDisplayObject::createFlag(HSSFlag::p flag, HSSRuleState defaultValue)
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
        std::vector<HSSFlag::p> flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<HSSFlag::p>::iterator it;
        for (it = flags.begin(); it != flags.end(); ++it)
        {
            HSSFlag::p theFlag = *it;
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
        for (QMap<AXRString, std::vector<HSSFlag::p> >::const_iterator it = this->_flags.begin(); it != this->_flags.end(); ++it)
        {
            HSSRuleState newValue = HSSRuleStatePurge;
            std::vector<HSSFlag::p> flags = it.value();
            this->_flagsStatus[it.key()] = newValue;

            for (std::vector<HSSFlag::p>::iterator it = flags.begin(); it != flags.end(); ++it)
            {
                HSSFlag::p theFlag = *it;
                theFlag->setThisObj(this->shared_from_this());
                theFlag->flagChanged(newValue);
            }
        }
    }
    else if (this->hasFlag(name))
    {
        //std_log("deactivate flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = HSSRuleStatePurge;
        std::vector<HSSFlag::p> flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        for (std::vector<HSSFlag::p>::iterator it = flags.begin(); it != flags.end(); ++it)
        {
            HSSFlag::p theFlag = *it;
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
        std::vector<HSSFlag::p> flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<HSSFlag::p>::iterator it;
        for (it = flags.begin(); it != flags.end(); ++it)
        {
            HSSFlag::p theFlag = *it;
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

HSSDisplayObject::p HSSDisplayObject::shared_from_this()
{
    return qSharedPointerCast<HSSDisplayObject > (HSSObject::shared_from_this());
}

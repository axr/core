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

#include <limits>
#include <QFont>
#include <QFontMetrics>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRString.h"
#include "AXRWarning.h"
#include "HSSAbstractVisitor.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSFont.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSRgb.h"
#include "HSSSelectorChain.h"
#include "HSSStringConstant.h"
#include "HSSTextBlock.h"
#include "HSSThisSelector.h"
#include "HSSValue.h"

using namespace AXR;

static int weightClassFromKeyword(AXRString keyword)
{
    if (keyword == "thin")
        return 100;
    if (keyword == "ultralight")
        return 200;
    if (keyword == "light")
        return 300;
    if (keyword == "normal")
        return 400;
    if (keyword == "medium")
        return 500;
    if (keyword == "semibold")
        return 600;
    if (keyword == "bold")
        return 700;
    if (keyword == "heavy")
        return 800;
    if (keyword == "black")
        return 900;

    return 400;
}

// From qfontconfigdatabase.cpp
static int getQtWeight(int fc_weight)
{
    int qtweight = QFont::Black;

    if (fc_weight <= (weightClassFromKeyword("light") + weightClassFromKeyword("medium")) / 2)
        qtweight = QFont::Light;
    else if (fc_weight <= (weightClassFromKeyword("medium") + weightClassFromKeyword("semibold")) / 2)
        qtweight = QFont::Normal;
    else if (fc_weight <= (weightClassFromKeyword("semibold") + weightClassFromKeyword("bold")) / 2)
        qtweight = QFont::DemiBold;
    else if (fc_weight <= (weightClassFromKeyword("bold") + weightClassFromKeyword("black")) / 2)
        qtweight = QFont::Bold;

    return qtweight;
}

static int getQtWeight(AXRString keyword)
{
    return getQtWeight(weightClassFromKeyword(keyword));
}

HSSTextTransformType HSSTextBlock::textTransformTypeFromString(AXRString value)
{
    static QMap<AXRString, HSSTextTransformType> types;
    if (types.isEmpty())
    {
        types["lowercase"] = HSSTextTransformTypeLowercase;
        types["uppercase"] = HSSTextTransformTypeUppercase;
        types["capitalize"] = HSSTextTransformTypeCapitalize;
        types["humanize"] = HSSTextTransformTypeHumanize;
    }

    HSSTextTransformType type = HSSTextTransformTypeNone;
    if (types.contains(value))
    {
        type = types[value];
    }

    return type;
}

HSSTextAlignType HSSTextBlock::textAlignTypeFromString(AXRString value)
{
    static QMap<AXRString, HSSTextAlignType>types;
    if (types.isEmpty())
    {
        types["left"] = HSSTextAlignTypeLeft;
        types["right"] = HSSTextAlignTypeRight;
        types["center"] = HSSTextAlignTypeCenter;
        types["justify"] = HSSTextAlignTypeJustify;
    }

    HSSTextAlignType type = HSSTextAlignTypeNone;
    if (types.contains(value))
    {
        type = types[value];
    }

    return type;
}

HSSTextBlock::HSSTextBlock(AXRController * controller)
: HSSDisplayObject(HSSObjectTypeTextBlock, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSTextBlock: creating text block object");

    this->registerProperty(HSSObservablePropertyText, QVariant::fromValue(&this->text));
    this->registerProperty(HSSObservablePropertyTransform, QVariant::fromValue(&this->transform));
    this->registerProperty(HSSObservablePropertyTextAlign, QVariant::fromValue(&this->textAlign));
    //    this->registerProperty(HSSObservablePropertyDirectionPrimary, QVariant::fromValue(&this->directionPrimary));
    //    this->registerProperty(HSSObservablePropertyDirectionSecondary, QVariant::fromValue(&this->directionSecondary));

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("text");
    shorthandProperties.push_back("textAlign");
    shorthandProperties.push_back("transform");
    shorthandProperties.push_back("direction");
    shorthandProperties.push_back("wrapDirection");
    this->setShorthandProperties(shorthandProperties);
}

HSSTextBlock::HSSTextBlock(const HSSTextBlock & orig)
: HSSDisplayObject(orig)
{

    this->registerProperty(HSSObservablePropertyText, QVariant::fromValue(&this->text));
    this->registerProperty(HSSObservablePropertyTransform, QVariant::fromValue(&this->transform));
    this->registerProperty(HSSObservablePropertyTextAlign, QVariant::fromValue(&this->textAlign));
    //    this->registerProperty(HSSObservablePropertyDirectionPrimary, QVariant::fromValue(&this->directionPrimary));
    //    this->registerProperty(HSSObservablePropertyDirectionSecondary, QVariant::fromValue(&this->directionSecondary));

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("text");
    shorthandProperties.push_back("textAlign");
    shorthandProperties.push_back("transform");
    shorthandProperties.push_back("direction");
    shorthandProperties.push_back("wrapDirection");
    this->setShorthandProperties(shorthandProperties);
}

QSharedPointer<HSSTextBlock> HSSTextBlock::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSTextBlock: cloning text block object");
    return qSharedPointerCast<HSSTextBlock> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSTextBlock::cloneImpl() const
{
    return QSharedPointer<HSSTextBlock>(new HSSTextBlock(*this));
}

HSSTextBlock::~HSSTextBlock()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSTextBlock: destructing text block object");
}

AXRString HSSTextBlock::defaultObjectType()
{
    return "textBlock";
}

bool HSSTextBlock::isKeyword(AXRString value, AXRString property)
{
    if (property == "transform")
    {
        if (value == "lowercase"
                || value == "uppercase"
                || value == "capitalize"
                || value == "humanize"
                || value == "no")
        {
            return true;
        }
    }
    else if (property == "textAlign")
    {
        if (value == "left"
                || value == "right"
                || value == "center"
                || value == "justify")
        {
            return true;
        }
    }
    else if (property == "direction" || property == "wrapDirection")
    {
        if (value == "ltr" || value == "rtl" || value == "ttb" || value == "btt")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSDisplayObject::isKeyword(value, property);
}

AXRString HSSTextBlock::toString()
{
    return "Text block with content:\n" + this->text;
}

void HSSTextBlock::setDefaults()
{
    HSSDisplayObject::setDefaults();
    AXRController * controller = this->getController();

    //transform
    QSharedPointer<HSSKeywordConstant> newDTransform(new HSSKeywordConstant("no", controller));
    this->setDTransform(newDTransform);
    //textAlign
    QSharedPointer<HSSKeywordConstant> newDTextAlign(new HSSKeywordConstant("inherit", controller));
    this->setDTextAlign(newDTextAlign);
    //font
    QSharedPointer<HSSKeywordConstant> newDFont(new HSSKeywordConstant("inherit", controller));
    this->setDFont(newDFont);
    //height
    this->setDHeight(QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("content", controller)));
}

void HSSTextBlock::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyTransform:
        this->setDTransform(value);
        break;
    case HSSObservablePropertyTextAlign:
        this->setDTransform(value);
        break;
        //        case HSSObservablePropertyDirectionPrimary:
        //            this->setDDirectionPrimary(value);
        //            break;
        //        case HSSObservablePropertyDirectionSecondary:
        //            this->setDDirectionSecondary(value);
        //            break;
    case HSSObservablePropertyText:
        this->setDText(value);
        break;
    default:
        HSSDisplayObject::setProperty(name, value);
        break;
    }
}

QFont HSSTextBlock::getFont() const
{
    QFont font_description;

    // Get the first font available
    QSharedPointer<HSSFont> theFont;
    if (font.size() > 0)
        theFont = *font.begin();

    if (theFont && !theFont->getFace().isEmpty())
        font_description.setFamily(theFont->getFace());
    else
        font_description.setFamily("monospace");

    // Set the weight of the font (bold, italic, etc.) if available
    if (theFont && theFont->getWeight())
        font_description.setWeight(getQtWeight(theFont->getWeight()->getValue()));
    else
        font_description.setWeight(QFont::Normal);

    font_description.setPointSizeF(theFont ? theFont->getSize() : HSSFont::DEFAULT_SIZE);

    return font_description;
}

void HSSTextBlock::accept(HSSAbstractVisitor* visitor, bool)
{
    visitor->visit(*this);
}


void HSSTextBlock::layout()
{
    if (this->needsLayout())
    {
        this->_needsLayout = false;

        int flags = 0;
        switch (this->textAlign)
        {
            case HSSTextAlignTypeLeft:
                flags = Qt::AlignLeft;
                break;
            case HSSTextAlignTypeRight:
                flags = Qt::AlignRight;
                break;
            case HSSTextAlignTypeCenter:
                flags = Qt::AlignCenter;
                break;
            case HSSTextAlignTypeJustify:
                flags = Qt::AlignJustify;
                break;
            default:
                break;
        }

        QFontMetrics fontMetrics(getFont());
        QRect bounds = fontMetrics.boundingRect(0, 0, static_cast<int>(this->width), std::numeric_limits<int>::max(), flags | Qt::TextWordWrap, this->getText());

        this->width = bounds.width();
        this->_setInnerWidth();
        this->_setOuterWidth();
        this->height = bounds.height();
        this->_setInnerHeight();
        this->_setOuterHeight();
        this->notifyObservers(HSSObservablePropertyHeight, &this->height);
        this->setNeedsSurface(true);
    }
}

HSSTextTransformType HSSTextBlock::getTransform()
{
    return this->transform;
}

QSharedPointer<HSSParserNode> HSSTextBlock::getDTransform()
{
    return this->dTransform;
}

void HSSTextBlock::setDTransform(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dTransform = value;
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
            this->setDTransform(objdef);
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
        this->dTransform = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSTextTransformType>())
            {
                this->transform = remoteValue.value<HSSTextTransformType>();
                valid = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTransform, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::transformChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->dTransform = value;
        this->transform = HSSTextBlock::textTransformTypeFromString(qSharedPointerCast<HSSKeywordConstant > (value)->getValue());
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
        this->dTransform = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getParent());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->transform = HSSTextBlock::textTransformTypeFromString(qSharedPointerCast<HSSValue>(theobj)->getStringValue());
            axr_log(LoggerChannelObsolete0, "######## FIXME ################");
            valid = true;
        }

        break;
    }
    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for transform of " + this->name);

    this->notifyObservers(HSSObservablePropertyTransform, &this->transform);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSTextBlock::transformChanged(HSSObservableProperty source, void *data)
{
    switch (this->dTransform->getType())
    {
    case HSSParserNodeTypeFunctionCall:
        this->transform = *(HSSTextTransformType *) data;
        break;
    default:
        break;
    }

    switch (this->dTransform->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
        this->transform = *(HSSTextTransformType *) data;
        break;
    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyTransform, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSTextAlignType HSSTextBlock::getTextAlign()
{
    return this->textAlign;
}

QSharedPointer<HSSParserNode> HSSTextBlock::getDTextAlign()
{
    return this->dTextAlign;
}

void HSSTextBlock::setDTextAlign(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dTextAlign = value;
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
            this->setDTextAlign(objdef);
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
        this->dTextAlign = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSTextAlignType>())
            {
                this->textAlign = remoteValue.value<HSSTextAlignType>();
                valid = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTextAlign, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::textAlignChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->dTextAlign = value;
        AXRString kwValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (kwValue == "inherit")
        {
            if (this->observedTextAlign)
            {
                this->observedTextAlign->removeObserver(this->observedTextAlignProperty, HSSObservablePropertyTextAlign, this);
            }
            QSharedPointer<HSSContainer> parent = this->getParent();
            QVariant remoteValue = parent->getProperty(HSSObservablePropertyTextAlign);
            if (remoteValue.canConvert<HSSTextAlignType*>())
            {
                this->textAlign = *remoteValue.value<HSSTextAlignType*>();
                parent->observe(HSSObservablePropertyTextAlign, HSSObservablePropertyTextAlign, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::textAlignChanged));
                valid = true;
            }
        }
        else
        {
            this->textAlign = HSSTextBlock::textAlignTypeFromString(kwValue);
            valid = true;
        }
        break;
    }

    default:
        valid = false;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dTextAlign = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getParent());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->textAlign = HSSTextBlock::textAlignTypeFromString(qSharedPointerCast<HSSValue>(theobj)->getStringValue());
            axr_log(LoggerChannelObsolete0, "######## FIXME ################");
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for textAlign of " + this->name);

    this->notifyObservers(HSSObservablePropertyTextAlign, &this->textAlign);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSTextBlock::textAlignChanged(HSSObservableProperty source, void *data)
{
    switch (this->dTextAlign->getType())
    {
    case HSSParserNodeTypeFunctionCall:
    case HSSParserNodeTypeKeywordConstant: //assuming 'inherit'
        this->textAlign = *(HSSTextAlignType *) data;
        break;
    default:
        break;
    }

    switch (this->dTextAlign->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
        this->textAlign = *(HSSTextAlignType *) data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyTextAlign, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

AXRString HSSTextBlock::getText()
{
    return this->text;
}

QSharedPointer<HSSParserNode> HSSTextBlock::getDText()
{
    return this->dText;
}

void HSSTextBlock::setDText(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeStringConstant:
    {
        this->dText = value;
        this->text = qSharedPointerCast<HSSStringConstant > (value)->getValue();
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
            this->setDText(objdef);
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
        this->dText = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<AXRString>())
            {
                this->text = remoteValue.value<AXRString>();
                valid = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyText, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::textChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->dText = value;
        QSharedPointer<HSSKeywordConstant> kwd = qSharedPointerCast<HSSKeywordConstant > (value);
        if (kwd->getValue() == "no")
        {
            this->text = "";
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
        this->dText = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getParent());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->text = qSharedPointerCast<HSSValue>(theobj)->getStringValue();
            axr_log(LoggerChannelObsolete0, "######## FIXME ################");
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for text of " + this->name);

    this->notifyObservers(HSSObservablePropertyText, &this->text);
}

void HSSTextBlock::textChanged(HSSObservableProperty source, void *data)
{
    switch (this->dText->getType())
    {
    case HSSParserNodeTypeFunctionCall:
        this->text = *(AXRString *)data;
        break;

    default:
        break;
    }

    switch (this->dText->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
        this->text = *(AXRString *)data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyText, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSTextBlock::trimContentText()
{
    this->text = this->text.trimmed();
}

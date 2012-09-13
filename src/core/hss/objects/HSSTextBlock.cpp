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

#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSFunctions.h"
#include "HSSObjectDefinition.h"
#include "HSSSelectorChain.h"
#include "HSSTextBlock.h"
#include "HSSThisSelector.h"
#include "HSSValue.h"

using namespace AXR;

static int weightClassFromKeyword(std::string keyword)
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

static int getQtWeight(std::string keyword)
{
    return getQtWeight(weightClassFromKeyword(keyword));
}

HSSTextTransformType HSSTextBlock::textTransformTypeFromString(std::string value)
{
    static boost::unordered_map<std::string, HSSTextTransformType>types;
    if (types.empty())
    {
        types["lowercase"] = HSSTextTransformTypeLowercase;
        types["uppercase"] = HSSTextTransformTypeUppercase;
        types["capitalize"] = HSSTextTransformTypeCapitalize;
        types["humanize"] = HSSTextTransformTypeHumanize;
    }

    HSSTextTransformType type = HSSTextTransformTypeNone;
    if (types.find(value) != types.end())
    {
        type = types[value];
    }
    return type;
}

HSSTextAlignType HSSTextBlock::textAlignTypeFromString(std::string value)
{
    static boost::unordered_map<std::string, HSSTextAlignType>types;
    if (types.empty())
    {
        types["left"] = HSSTextAlignTypeLeft;
        types["right"] = HSSTextAlignTypeRight;
        types["center"] = HSSTextAlignTypeCenter;
        types["justify"] = HSSTextAlignTypeJustify;
    }

    HSSTextAlignType type = HSSTextAlignTypeNone;
    if (types.find(value) != types.end())
    {
        type = types[value];
    }
    return type;
}

HSSTextBlock::HSSTextBlock()
: HSSDisplayObject(HSSObjectTypeTextBlock)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSTextBlock: creating text block object");

    this->observedTextAlign = this->observedTransform = this->observedText
            = NULL;

    this->registerProperty(HSSObservablePropertyText, & this->text);
    this->registerProperty(HSSObservablePropertyTransform, & this->transform);
    this->registerProperty(HSSObservablePropertyTextAlign, & this->textAlign);
    //    this->registerProperty(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
    //    this->registerProperty(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);

    std::vector<std::string> shorthandProperties;
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
    this->observedTextAlign = this->observedTransform = this->observedText
            = NULL;

    this->registerProperty(HSSObservablePropertyText, & this->text);
    this->registerProperty(HSSObservablePropertyTransform, & this->transform);
    this->registerProperty(HSSObservablePropertyTextAlign, & this->textAlign);
    //    this->registerProperty(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
    //    this->registerProperty(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);

    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("text");
    shorthandProperties.push_back("textAlign");
    shorthandProperties.push_back("transform");
    shorthandProperties.push_back("direction");
    shorthandProperties.push_back("wrapDirection");
    this->setShorthandProperties(shorthandProperties);
}

HSSTextBlock::p HSSTextBlock::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSTextBlock: cloning text block object");
    return boost::static_pointer_cast<HSSTextBlock, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSTextBlock::cloneImpl() const
{
    return HSSClonable::p(new HSSTextBlock(*this));
}

HSSTextBlock::~HSSTextBlock()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSTextBlock: destructing text block object");
}

std::string HSSTextBlock::defaultObjectType()
{
    return "textBlock";
}

bool HSSTextBlock::isKeyword(std::string value, std::string property)
{
    if (property == "transform")
    {
        if (value == "lowercase"
                || value == "uppercase"
                || value == "capitalize"
                || value == "humanize")
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

std::string HSSTextBlock::toString()
{
    return "Text block with content:\n" + this->text;
}

void HSSTextBlock::setDefaults()
{
    HSSDisplayObject::setDefaults();

    //transform
    HSSKeywordConstant::p newDTransform(new HSSKeywordConstant("none"));
    this->setDTransform(newDTransform);
    //textAlign
    HSSKeywordConstant::p newDTextAlign(new HSSKeywordConstant("inherit"));
    this->setDTextAlign(newDTextAlign);
    //font
    HSSKeywordConstant::p newDFont(new HSSKeywordConstant("inherit"));
    this->setDFont(newDFont);
    //height
    this->setDHeight(HSSKeywordConstant::p(new HSSKeywordConstant("content")));
}

void HSSTextBlock::setProperty(HSSObservableProperty name, HSSParserNode::p value)
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
    HSSFont::p theFont;
    if (font.size() > 0)
        theFont = *font.begin();

    if (theFont && !theFont->getFace().empty())
        font_description.setFamily(QString::fromStdString(theFont->getFace()));
    else
        font_description.setFamily("monospace");

    // Set the weight of the font (bold, italic, etc.) if available
    if (theFont && theFont->getWeight())
        font_description.setWeight(getQtWeight(theFont->getWeight()->getValue()));
    else
        font_description.setWeight(QFont::Normal);

    font_description.setPointSize(theFont ? theFont->getSize() : HSSFont::DEFAULT_SIZE);

    return font_description;
}

void HSSTextBlock::drawForeground()
{
    QPainter painter(this->foregroundSurface);
    painter.setRenderHint(QPainter::Antialiasing);

    HSSFont::p theFont;
    if (font.size() > 0)
        theFont = *font.begin();

    QPen pen;
    if (theFont && theFont->getColor())
    {
        HSSRgb::p textColor = boost::static_pointer_cast<HSSRgb>(theFont->getColor());
        pen.setColor(QColor(textColor->getRed(), textColor->getGreen(), textColor->getBlue(), textColor->getAlpha()));
    }
    else
    {
        pen.setColor(QColor(0, 0, 0));
    }

    painter.setPen(pen);

    painter.setFont(getFont());

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

    std::string text = this->getText();
    painter.drawText(0, 0, this->width, this->height, flags, QString::fromUtf8(text.data(), text.length()));
}

void HSSTextBlock::layout()
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
    QRect bounds = fontMetrics.boundingRect(0, 0, this->width, std::numeric_limits<int>::max(), flags, QString::fromStdString(this->getText()));

    this->height = bounds.height();
    this->_setInnerHeight();
    this->_setOuterHeight();
    this->notifyObservers(HSSObservablePropertyHeight, &this->height);
    this->setNeedsSurface(true);
}

HSSTextTransformType HSSTextBlock::getTransform()
{
    return this->transform;
}

HSSParserNode::p HSSTextBlock::getDTransform()
{
    return this->dTransform;
}

void HSSTextBlock::setDTransform(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dTransform = value;
        try
        {
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            this->setDTransform(objdef);
            valid = true;

        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dTransform = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->transform = boost::any_cast<HSSTextTransformType > (remoteValue);
                valid = true;
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTransform, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::transformChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->dTransform = value;
        this->transform = HSSTextBlock::textTransformTypeFromString(boost::static_pointer_cast<HSSKeywordConstant > (value)->getValue());
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
        HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getParent());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->transform = HSSTextBlock::textTransformTypeFromString(boost::static_pointer_cast<HSSValue>(theobj)->getStringValue());
            std_log("######## FIXME ################");
            valid = true;
        }

        break;
    }
    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for transform of " + this->name));

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

HSSParserNode::p HSSTextBlock::getDTextAlign()
{
    return this->dTextAlign;
}

void HSSTextBlock::setDTextAlign(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dTextAlign = value;
        try
        {
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            this->setDTextAlign(objdef);
            valid = true;
        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dTextAlign = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->textAlign = boost::any_cast<HSSTextAlignType > (remoteValue);
                valid = true;
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTextAlign, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::textAlignChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->dTextAlign = value;
        std::string kwValue = boost::static_pointer_cast<HSSKeywordConstant > (value)->getValue();
        if (kwValue == "inherit")
        {
            if (this->observedTextAlign)
            {
                this->observedTextAlign->removeObserver(this->observedTextAlignProperty, HSSObservablePropertyTextAlign, this);
            }
            HSSContainer::p parent = this->getParent();
            boost::any remoteValue = parent->getProperty(HSSObservablePropertyTextAlign);
            try {
                this->textAlign = * boost::any_cast<HSSTextAlignType *>(remoteValue);
                parent->observe(HSSObservablePropertyTextAlign, HSSObservablePropertyTextAlign, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::textAlignChanged));
                valid = true;
            } catch (boost::bad_any_cast &) {
                //do nothing
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
        HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getParent());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->textAlign = HSSTextBlock::textAlignTypeFromString(boost::static_pointer_cast<HSSValue>(theobj)->getStringValue());
            std_log("######## FIXME ################");
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for textAlign of " + this->name));

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

std::string HSSTextBlock::getText()
{
    return this->text;
}

HSSParserNode::p HSSTextBlock::getDText()
{
    return this->dText;
}

void HSSTextBlock::setDText(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeStringConstant:
    {
        this->dText = value;
        this->text = boost::static_pointer_cast<HSSStringConstant > (value)->getValue();
        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            this->setDText(objdef);
            valid = true;

        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dText = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->text = boost::any_cast<std::string > (remoteValue);
                valid = true;
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyText, this, new HSSValueChangedCallback<HSSTextBlock > (this, &HSSTextBlock::textChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->dText = value;
        HSSKeywordConstant::p kwd = boost::static_pointer_cast<HSSKeywordConstant > (value);
        if (kwd->getValue() == "none")
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
        HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getParent());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->text = boost::static_pointer_cast<HSSValue>(theobj)->getStringValue();
            std_log("######## FIXME ################");
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for text of " + this->name));

    this->notifyObservers(HSSObservablePropertyText, &this->text);
}

void HSSTextBlock::textChanged(HSSObservableProperty source, void *data)
{
    switch (this->dText->getType())
    {
    case HSSParserNodeTypeFunctionCall:
        this->text = *(std::string *)data;
        break;

    default:
        break;
    }

    switch (this->dText->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
        this->text = *(std::string *)data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyText, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSTextBlock::trimContentText()
{
    this->text = QString::fromStdString(this->text).trimmed().toStdString();
}

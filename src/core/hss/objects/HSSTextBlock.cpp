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
#include "axr.h"
#include "HSSTextBlock.h"

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
    this->_initialize();

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
    this->_initialize();
}

void HSSTextBlock::_initialize()
{
    this->addNotifyCallback("font", new HSSObserveCallback<HSSTextBlock>(this, &HSSTextBlock::notifyFont));
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
    this->cleanTrackedObservers();
}

void HSSTextBlock::setDefaults()
{
    HSSDisplayObject::setDefaults();
    //the 'text' property is intentionally left out here
    this->setDefaultKw("font", "inherit");
    this->setDefaultKw("textAlign", "inherit");
    this->setDefaultKw("transform", "no");
    this->setDefaultKw("direction", "ltr");
    this->setDefaultKw("wrapDirection", "ttb");
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
    return "Text block with content:\n" + this->getText();
}

QFont HSSTextBlock::getQFont() const
{
    QFont font_description;

    // Get the first font available
    QSharedPointer<HSSFont> theFont;
    QSharedPointer<HSSObject> fontObj = this->getFont();
    if (fontObj && fontObj->isA(HSSObjectTypeFont))
    {
        theFont = qSharedPointerCast<HSSFont>(fontObj);
    }

    if (theFont && !theFont->getFace().isEmpty())
        font_description.setFamily(theFont->getFace());
    else
        font_description.setFamily("monospace");

    // Set the weight of the font (bold, italic, etc.) if available
    if (theFont && theFont->getWeight() != "")
        font_description.setWeight(getQtWeight(theFont->getWeight()));
    else
        font_description.setWeight(QFont::Normal);

    font_description.setPointSize(theFont ? theFont->getSize() : HSSFont::DEFAULT_SIZE);

    return font_description;
}

void HSSTextBlock::accept(HSSAbstractVisitor* visitor, HSSVisitorFilterFlags filterFlags)
{
    visitor->visit(*this);
}

HSSTextTransformType HSSTextBlock::getTransform() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("transform");
    if (value && value->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(value)->getValue();
        if (parserNode && parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            return HSSTextBlock::textTransformTypeFromString(qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue());
        }
    }
    return HSSTextTransformTypeNone;
}

HSSTextAlignType HSSTextBlock::getTextAlign() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("textAlign");
    return this->_getTextAlign(value);
}

HSSTextAlignType HSSTextBlock::_getTextAlign(QSharedPointer<HSSObject> theObj) const
{
    if (theObj && theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
        if (parserNode)
        {
            switch (parserNode->getType()) {
                case HSSParserNodeTypeKeywordConstant:
                {
                    return HSSTextBlock::textAlignTypeFromString(qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue());
                }
                case HSSParserNodeTypeFunctionCall:
                {
                    if (parserNode->isA(HSSFunctionTypeRef))
                    {
                        QSharedPointer<HSSRefFunction> refFunction = qSharedPointerCast<HSSRefFunction>(parserNode);
                        QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSRefFunction>(refFunction)->evaluate();
                        if (remoteObj)
                        {
                            return this->_getTextAlign(remoteObj);
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    return HSSTextAlignTypeNone;
}

AXRString HSSTextBlock::getText() const
{
    return this->getComputedString("text");
}

void HSSTextBlock::setText(AXRString value, HSSUnit specificity)
{
    this->setComputedValue("text", value, specificity);
}

void HSSTextBlock::notifyFont(QSharedPointer<HSSObject> theObj)
{
    this->setNeedsLayout(true);
    this->notifyObservers("font", theObj);
}

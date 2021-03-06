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

HSSTextTransformType HSSTextBlock::textTransformTypeFromString(AXRString value)
{
    static std::map<AXRString, HSSTextTransformType> types;
    if (types.empty())
    {
        types["lowercase"] = HSSTextTransformTypeLowercase;
        types["uppercase"] = HSSTextTransformTypeUppercase;
        types["capitalize"] = HSSTextTransformTypeCapitalize;
        types["humanize"] = HSSTextTransformTypeHumanize;
    }

    HSSTextTransformType type = HSSTextTransformTypeNone;
    if (types.count(value))
    {
        type = types[value];
    }

    return type;
}

HSSTextAlignType HSSTextBlock::textAlignTypeFromString(AXRString value)
{
    static std::map<AXRString, HSSTextAlignType>types;
    if (types.empty())
    {
        types["left"] = HSSTextAlignTypeLeft;
        types["right"] = HSSTextAlignTypeRight;
        types["center"] = HSSTextAlignTypeCenter;
        types["justify"] = HSSTextAlignTypeJustify;
    }

    HSSTextAlignType type = HSSTextAlignTypeNone;
    if (types.count(value))
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

AXRString HSSTextBlock::toString()
{
    return "Text block with content:\n" + this->getText();
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
                case HSSParserNodeTypeFunction:
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

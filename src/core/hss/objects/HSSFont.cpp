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
#include "HSSFont.h"

using namespace AXR;

const HSSUnit HSSFont::DEFAULT_SIZE = 16;

QSharedPointer<HSSFont> HSSFont::defaultFont(AXRController * controller)
{
    QSharedPointer<HSSFont> ret(new HSSFont(controller));
    ret->setDefaults();
    ret->fillWithDefaults();
    return ret;
}

HSSFont::HSSFont(AXRController * controller)
: HSSObject(HSSObjectTypeFont, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFont: creating font object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("face");
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("weight");
    this->setShorthandProperties(shorthandProperties);
}

HSSFont::HSSFont(const HSSFont & orig)
: HSSObject(orig)
{
    this->_initialize();
}

void HSSFont::_initialize()
{
    this->addCallback("color", new HSSComputeCallback<HSSFont>(this, &HSSFont::computeColor));
}

QSharedPointer<HSSFont> HSSFont::clone() const
{
    return qSharedPointerCast<HSSFont> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSFont::cloneImpl() const
{
    return QSharedPointer<HSSFont>(new HSSFont(*this));
}

HSSFont::~HSSFont()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFont: destructing font object");
    this->cleanTrackedObservers();
}

void HSSFont::setDefaults()
{
    this->setDefault("size", DEFAULT_SIZE);
    this->setDefault("face", "Helvetica");
    this->setDefaultKw("color", "black");
    this->setDefaultKw("weight", "regular");
}

AXRString HSSFont::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSFont: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSFont";
    }
}

AXRString HSSFont::defaultObjectType()
{
    return "font";
}

AXRString HSSFont::defaultObjectType(AXRString property)
{
    if (property == "color")
    {
        return "rgb";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

HSSUnit HSSFont::getSize() const
{
    return this->getComputedNumber("size");
}

AXRString HSSFont::getFace() const
{
    return this->getComputedString("face");
}

QSharedPointer<HSSRgb> HSSFont::getColor() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("color");
    if (value && value->isA(HSSObjectTypeRgb))
    {
        return qSharedPointerCast<HSSRgb>(value);
    }
    return HSSRgb::blackColor(this->getController());
}

QSharedPointer<HSSObject> HSSFont::computeColor(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString theKw = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (theKw == "black")
            {
                return HSSRgb::blackColor(this->getController());
            }
            else if (theKw == "white")
            {
                return HSSRgb::whiteColor(this->getController());
            }
            else if (theKw == "transparent")
            {
                return this->computeValueObject(parserNode);
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "color");
}


AXRString HSSFont::getWeight() const
{
    return this->getComputedString("weight");
}

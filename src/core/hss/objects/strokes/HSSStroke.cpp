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
#include "HSSStroke.h"

using namespace AXR;

HSSStroke::HSSStroke(AXRController * controller)
: HSSAbstractStroke(HSSStrokeTypeLine, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSStroke: creating stroke object");

    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("segments");
    shorthandProperties.push_back("position");
    this->setShorthandProperties(shorthandProperties);
}

HSSStroke::HSSStroke(const HSSStroke & orig)
: HSSAbstractStroke(orig)
{
    this->_initialize();
}

void HSSStroke::_initialize()
{
    this->addCallback("color", new HSSComputeCallback<HSSStroke>(this, &HSSStroke::computeColor));
}

QSharedPointer<HSSStroke> HSSStroke::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSStroke: cloning stroke object");
    return qSharedPointerCast<HSSStroke> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSStroke::cloneImpl() const
{
    return QSharedPointer<HSSStroke>(new HSSStroke(*this));
}

HSSStroke::~HSSStroke()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSStroke: destructing stroke object");
    this->cleanTrackedObservers();
}

void HSSStroke::setDefaults()
{
    HSSAbstractStroke::setDefaults();
    this->setDefaultKw("color", "black");
}

AXRString HSSStroke::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSStroke: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSStroke";
    }
}

AXRString HSSStroke::defaultObjectType()
{
    return "stroke";
}

AXRString HSSStroke::defaultObjectType(AXRString property)
{
    if (property == "color")
    {
        return "rgb";
    }
    else if (property == "joins")
    {
        return "mitered";
    }
    else if (property == "caps")
    {
        return "mitered";
    }
    else
    {
        return HSSAbstractStroke::defaultObjectType(property);
    }
}

QSharedPointer<HSSObject> HSSStroke::getColor()
{
    return this->getComputedValue("color");
}

QSharedPointer<HSSObject> HSSStroke::computeColor(QSharedPointer<HSSParserNode> parserNode)
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
                return HSSRgb::transparentColor(this->getController());
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "color");
}

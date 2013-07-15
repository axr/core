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

#include <QPainter>
#include <QPainterPathStroker>
#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSLineBorder.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSLineBorder::HSSLineBorder(AXRController * controller)
: HSSBorder(controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLineBorder: creating line border object");

    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("segments");
    shorthandProperties.push_back("position");

    this->setShorthandProperties(shorthandProperties);
}

HSSLineBorder::HSSLineBorder(const HSSLineBorder & orig)
: HSSBorder(orig)
{
    this->_initialize();
}

void HSSLineBorder::_initialize()
{
    this->addCallback("color", new HSSComputeCallback<HSSLineBorder>(this, &HSSLineBorder::computeColor));
}

QSharedPointer<HSSLineBorder> HSSLineBorder::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLineBorder: cloning line border object");
    return qSharedPointerCast<HSSLineBorder> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLineBorder::cloneImpl() const
{
    return QSharedPointer<HSSLineBorder>(new HSSLineBorder(*this));
}

HSSLineBorder::~HSSLineBorder()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLineBorder: destructing line border object");
}

void HSSLineBorder::setDefaults()
{
    HSSBorder::setDefaults();
    this->setDefaultKw("color", "black");
}

AXRString HSSLineBorder::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSLineBorder: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSLineBorder";
    }
}

AXRString HSSLineBorder::defaultObjectType()
{
    return "lineBorder";
}

AXRString HSSLineBorder::defaultObjectType(AXRString property)
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
        return HSSBorder::defaultObjectType(property);
    }
}

bool HSSLineBorder::isKeyword(AXRString value, AXRString property)
{
    if ((value == "rounded" || value == "projected") && (property == "caps"))
    {
        return true;
    }
    else if ( value == "black" || value == "white" || value == "transparent")
    {
        if (property == "color")
        {
            return true;
        }
    }

    return HSSBorder::isKeyword(value, property);
}

QSharedPointer<HSSObject> HSSLineBorder::getColor()
{
    return this->getComputedValue("color");
}

QSharedPointer<HSSObject> HSSLineBorder::computeColor(QSharedPointer<HSSParserNode> parserNode)
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
    return this->computeValueObject(parserNode, "color");
}

void HSSLineBorder::draw(QPainter &painter, const QPainterPath &path)
{
    QPainterPathStroker stroker;
    stroker.setWidth(this->getSize());
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::FlatCap);
    QPainterPath borderPath = stroker.createStroke(path);

    QSharedPointer<HSSObject> colorObj = this->getColor();
    QColor usedColor;
    if (colorObj && colorObj->isA(HSSObjectTypeRgb))
    {
        usedColor = qSharedPointerCast<HSSRgb>(colorObj)->toQColor();
    }
    else
    {
        usedColor = QColor(Qt::black);
    }
    painter.fillPath(borderPath, usedColor);

}

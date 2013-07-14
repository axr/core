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
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("segments");
    shorthandProperties.push_back("position");

    this->setShorthandProperties(shorthandProperties);
    this->registerProperty(HSSObservablePropertyColor, QVariant::fromValue(&this->color));
}

HSSLineBorder::HSSLineBorder(const HSSLineBorder & orig)
: HSSBorder(orig)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("segments");
    shorthandProperties.push_back("position");

    this->setShorthandProperties(shorthandProperties);
    this->registerProperty(HSSObservablePropertyColor, QVariant::fromValue(&this->color));
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

void HSSLineBorder::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyColor:
        this->setDColor(value);
        break;
    default:
        HSSBorder::setProperty(name, value);
        break;
    }
}

QSharedPointer<HSSObject> HSSLineBorder::getColor()
{
    return this->color;
}

void HSSLineBorder::setDColor(QSharedPointer<HSSParserNode> value)
{

    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dColor = value;
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
            this->setDColor(objdef);
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
        this->dColor = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<QSharedPointer<HSSRgb> >())
            {
                this->color = remoteValue.value<QSharedPointer<HSSRgb> >();
            }
            else
            {
                this->color = QSharedPointer<HSSRgb>(new HSSRgb(this->getController()));
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyColor, this, new HSSValueChangedCallback<HSSLineBorder > (this, &HSSLineBorder::colorChanged));
            valid = true;

        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(value);
        if (theKW->getValue() == "black")
        {
            this->color = HSSRgb::blackColor(this->getController());
            valid = true;
        }
        else if (theKW->getValue() == "white")
        {
            this->color = HSSRgb::whiteColor(this->getController());
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            QSharedPointer<HSSRgb> transColor = HSSRgb::blackColor(this->getController());
            transColor->setDAlpha(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(0.0, this->getController())));
            this->color = transColor;
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
        this->dColor = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getThisObj());
        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeRgb))
        {
            this->color = qSharedPointerCast<HSSRgb > (theobj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSLineBorder", "Invalid value for color of " + this->name);

    this->notifyObservers(HSSObservablePropertyStartColor, &this->color);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSLineBorder::colorChanged(AXR::HSSObservableProperty source, void *data)
{
    axr_log(LoggerChannelObsolete1, "********************** colorChanged unimplemented ****************************");
}

void HSSLineBorder::draw(QPainter &painter, const QPainterPath &path)
{
    QPainterPathStroker stroker;
    stroker.setWidth(this->size);
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::FlatCap);
    QPainterPath borderPath = stroker.createStroke(path);
    painter.fillPath(borderPath, this->color ? this->color->toQColor() : QColor(Qt::black));
}

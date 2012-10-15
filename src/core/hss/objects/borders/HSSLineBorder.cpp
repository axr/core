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
#include "AXRWarning.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSLineBorder.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"

using namespace AXR;

HSSLineBorder::HSSLineBorder()
: HSSBorder()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLineBorder: creating line border object");
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");

    this->setShorthandProperties(shorthandProperties);
    this->registerProperty(HSSObservablePropertyColor, & this->color);
}

HSSLineBorder::HSSLineBorder(const HSSLineBorder & orig)
: HSSBorder(orig)
{
    this->observedColor = NULL;
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");

    this->setShorthandProperties(shorthandProperties);
    this->registerProperty(HSSObservablePropertyColor, & this->color);
}

HSSLineBorder::p HSSLineBorder::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLineBorder: cloning line border object");
    return boost::static_pointer_cast<HSSLineBorder, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSLineBorder::cloneImpl() const
{
    return HSSClonable::p(new HSSLineBorder(*this));
}

HSSLineBorder::~HSSLineBorder()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLineBorder: destructing line border object");
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

void HSSLineBorder::setProperty(HSSObservableProperty name, HSSParserNode::p value)
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

HSSObject::p HSSLineBorder::getColor()
{
    return this->color;
}

void HSSLineBorder::setDColor(HSSParserNode::p value)
{

    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dColor = value;
        try
        {
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            this->setDColor(objdef);
            valid = true;
        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }
        catch (const AXRWarning::p &e)
        {
            e->raise();
        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dColor = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->color = boost::any_cast<HSSRgb::p > (remoteValue);
            }
            catch (boost::bad_any_cast &)
            {
                this->color = HSSRgb::p(new HSSRgb());
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyColor, this, new HSSValueChangedCallback<HSSLineBorder > (this, &HSSLineBorder::colorChanged));
            valid = true;

        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = boost::static_pointer_cast<HSSKeywordConstant>(value);
        if (theKW->getValue() == "no")
        {
            valid = true;
        }
        else if (theKW->getValue() == "black")
        {
            this->color = HSSRgb::blackColor();
            valid = true;
        }
        else if (theKW->getValue() == "white")
        {
            this->color = HSSRgb::whiteColor();
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            HSSRgb::p transColor = HSSRgb::blackColor();
            transColor->setDAlpha(HSSNumberConstant::p(new HSSNumberConstant(0.0)));
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
        HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getThisObj());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeRgb))
        {
            this->color = boost::static_pointer_cast<HSSRgb > (theobj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSLineBorder", "Invalid value for color of " + this->name));

    this->notifyObservers(HSSObservablePropertyStartColor, &this->color);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSLineBorder::colorChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** colorChanged unimplemented ****************************");
}

void HSSLineBorder::draw(QPainter &painter, const QPainterPath &path)
{
    HSSUnit r = 0., g = 0., b = 0., a = 255.;
    if (this->color)
    {
        r = this->color->getRed();
        g = this->color->getGreen();
        b = this->color->getBlue();
        a = this->color->getAlpha();
    }

    QPainterPathStroker stroker;
    stroker.setWidth(this->size);
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::RoundCap);
    QPainterPath borderPath = stroker.createStroke(path);
    painter.fillPath(borderPath, QColor(r, g, b, a));
}

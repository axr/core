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
#include "errors.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSConstants.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSObjectDefinition.h"
#include "HSSPolygon.h"

using namespace AXR;

HSSPolygon::HSSPolygon()
: HSSShape(HSSShapeTypePolygon)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSPolygon: creating polygon object");
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("sides");
    shorthandProperties.push_back("angle");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertySides, & this->sides);
    this->registerProperty(HSSObservablePropertyAngle, & this->angle);
    this->sides = 3;
    this->angle = 0;

    this->observedSides = NULL;
    this->observedAngle = NULL;
}

HSSPolygon::HSSPolygon(const HSSPolygon & orig)
: HSSShape(orig)
{
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("sides");
    shorthandProperties.push_back("angle");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertySides, & this->sides);
    this->registerProperty(HSSObservablePropertyAngle, & this->angle);
    this->sides = 3;
    this->angle = 0;

    this->observedSides = NULL;
    this->observedAngle = NULL;
}

HSSPolygon::p HSSPolygon::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSPolygon: cloning polygon object");
    return boost::static_pointer_cast<HSSPolygon, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSPolygon::cloneImpl() const
{
    return HSSClonable::p(new HSSPolygon(*this));
}

HSSPolygon::~HSSPolygon()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSPolygon: destructing polygon object");
}

std::string HSSPolygon::toString()
{
    if (this->isNamed())
    {
        return std::string("HSSPolygon: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSPolygon";
    }
}

std::string HSSPolygon::defaultObjectType()
{
    return "polygon";
}

bool HSSPolygon::isKeyword(std::string value, std::string property)
{
    if (property == "angle")
    {
        if (value == "top" || value == "topRight" || value == "right" || value == "bottomRight" || value == "bottom" || value == "bottomLeft" || value == "left" || value == "topLeft")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSShape::isKeyword(value, property);
}

void HSSPolygon::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertySides:
        this->setDSides(value);
        break;
    case HSSObservablePropertyAngle:
        this->setDAngle(value);
        break;
    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

void HSSPolygon::draw(cairo_t * cairo, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height)
{
    cairo_save(cairo);
    cairo_translate(cairo, x + width / 2., y + height / 2.);
    cairo_scale(cairo, 1. * (width / 2.), 1. * (height / 2.));

    HSSUnit radius = 1;
    unsigned int sides = this->sides < 3 ? 3 : this->sides;
    long double step = 360.0f / sides;
    long double i;
    long double l_angle = this->angle;
    for (i = this->angle; i < this->angle + 360.0; i += step) //go in a full circle
    {
        double radians = l_angle * M_PI / 180.0;
        HSSUnit p_x = cosl(radians) * radius;
        HSSUnit p_y = sinl(radians) * radius;
        if (i == this->angle)
        {
            cairo_move_to(cairo, p_x, p_y);
        }
        else
        {
            cairo_line_to(cairo, p_x, p_y);
        }

        l_angle += step;
    }
    cairo_close_path(cairo);

    cairo_restore(cairo);
    return;




    cairo_arc(cairo, 0., 0., 1., 0., 2 * M_PI);

}

unsigned int HSSPolygon::getSides()
{
    return this->sides;
}

HSSParserNode::p HSSPolygon::getDSides()
{
    return this->dSides;
}

void HSSPolygon::setDSides(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dSides = value;
        this->sides = floor(this->_setLDProperty(
                                           &HSSPolygon::sidesChanged,
                                           value,
                                           1.,
                                           HSSObservablePropertySides,
                                           this->observedSides,
                                           this->observedSidesProperty
                                           ));

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dSides = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->thisObj);
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->sides = floor(boost::any_cast<long double>(remoteValue));
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }
            try
            {
                this->sides = boost::any_cast<unsigned int>(remoteValue);
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertySides, this, new HSSValueChangedCallback<HSSPolygon > (this, &HSSPolygon::sidesChanged));

        }
        else
        {
            throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type for sides of @polygon " + this->name));
        }

        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSPolygon", "Invalid value for sides of @polygon " + this->name));
    }
    this->notifyObservers(HSSObservablePropertySides, &this->sides);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSPolygon::sidesChanged(HSSObservableProperty source, void*data)
{
    switch (this->dSides->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->sides = floor(*(long double*) data);
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dSides);
        this->sides = floor(percentageValue->getValue(*(long double*) data));
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertySides, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSPolygon::getAngle()
{
    return this->angle;
}

HSSParserNode::p HSSPolygon::getDAngle()
{
    return this->dAngle;
}

void HSSPolygon::setDAngle(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dAngle = value;
        this->angle = this->_setLDProperty(
                                           &HSSPolygon::angleChanged,
                                           value,
                                           1.,
                                           HSSObservablePropertyAngle,
                                           this->observedAngle,
                                           this->observedAngleProperty
                                           );

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dAngle = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->thisObj);
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->angle = boost::any_cast<long double>(remoteValue);
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyAngle, this, new HSSValueChangedCallback<HSSPolygon > (this, &HSSPolygon::angleChanged));

        }
        else
        {
            throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type for angle of @polygon " + this->name));
        }

        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSPolygon", "Invalid value for angle of @polygon " + this->name));
    }
    this->notifyObservers(HSSObservablePropertyAngle, &this->angle);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSPolygon::angleChanged(HSSObservableProperty source, void*data)
{
    switch (this->dAngle->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->angle = *(long double*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dAngle);
        this->angle = percentageValue->getValue(*(long double*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertySides, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSPolygon::_setLDProperty(
                                       void(HSSPolygon::*callback)(HSSObservableProperty property, void* data),
                                       HSSParserNode::p value,
                                       long double percentageBase,
                                       HSSObservableProperty observedSourceProperty,
                                       HSSObservable * &observedStore,
                                       HSSObservableProperty &observedStoreProperty
                                       )
{
    long double ret;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        HSSNumberConstant::p numberValue = boost::static_pointer_cast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setScope(this->scope);
        expressionValue->setThisObj(this->thisObj);
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSPolygon > (this, callback));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        AXRWarning::p(new AXRWarning("HSSPolygon", "Unknown parser node type while setting value for @polygon property"))->raise();
        break;
    }

    return ret;
}

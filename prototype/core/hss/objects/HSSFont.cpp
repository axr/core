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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
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

#include "errors.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSConstants.h"
#include "HSSExpression.h"
#include "HSSFont.h"
#include "HSSFunction.h"
#include "HSSObjectDefinition.h"

using namespace AXR;

HSSFont::HSSFont()
: HSSObject(HSSObjectTypeFont)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFont: creating font object");

    this->observedSize = this->observedFace = this->observedColor = this->observedWeight = NULL;

    this->size = 18.;
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("face");
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("weight");
    this->setShorthandProperties(shorthandProperties);
}

HSSFont::HSSFont(const HSSFont & orig)
: HSSObject(orig)
{
    this->observedSize = this->observedFace = this->observedColor = this->observedWeight = NULL;
    this->size = 18.;
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("face");
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("weight");
    this->setShorthandProperties(shorthandProperties);
}

HSSFont::p HSSFont::clone() const
{
    return boost::static_pointer_cast<HSSFont, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSFont::cloneImpl() const
{
    return HSSClonable::p(new HSSFont(*this));
}

HSSFont::~HSSFont()
{

}

std::string HSSFont::toString()
{
    if (this->isNamed())
    {
        return std::string("HSSFont: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSFont";
    }
}

std::string HSSFont::defaultObjectType()
{
    return "font";
}

std::string HSSFont::defaultObjectType(std::string property)
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

bool HSSFont::isKeyword(std::string value, std::string property)
{
    if (property == "weight")
    {
        if (
                value == "normal"
                || value == "bold"
                || value == "medium"
                || value == "thin"
                || value == "light"
                || value == "book"
                || value == "heavy"
                || value == "ultralight"
                || value == "semibold"
                || value == "ultrabold"
                || value == "ultraheavy"
                )
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSFont::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertySize:
        this->setDSize(value);
        break;
    case HSSObservablePropertyFace:
        this->setDFace(value);
        break;
    case HSSObservablePropertyColor:
        this->setDColor(value);
        break;
    case HSSObservablePropertyWeight:
        this->setDWeight(value);
        break;

    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

long double HSSFont::getSize()
{
    return this->size;
}

void HSSFont::setDSize(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        break;
    default:
        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for size of @font object " + this->name));
    }

    this->dSize = value;
    this->size = this->_setLDProperty(
                                      &HSSFont::sizeChanged,
                                      value,
                                      18.,
                                      HSSObservablePropertySize,
                                      this->observedSize,
                                      this->observedSizeProperty
                                      );

    this->notifyObservers(HSSObservablePropertySize, &this->size);
}

void HSSFont::sizeChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** sizeChanged unimplemented ****************************");
}

std::string HSSFont::getFace()
{
    return this->face;
}

void HSSFont::setDFace(HSSParserNode::p value)
{

    if (value->isA(HSSParserNodeTypeStringConstant))
    {
        this->dFace = value;
        HSSStringConstant::p theString = boost::static_pointer_cast<HSSStringConstant > (value);
        this->face = theString->getValue();
    }
    else
    {
        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for face of @font object " + this->name));
    }
}

void HSSFont::faceChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** faceChanged unimplemented ****************************");
}

HSSRgb::p HSSFont::getColor()
{
    return this->color;
}

HSSParserNode::p HSSFont::getDColor()
{
    return this->dColor;
}

void HSSFont::setDColor(HSSParserNode::p value)
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
        catch (AXRError::p e)
        {
            e->raise();
        }
        catch (AXRWarning::p e)
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
            catch (...)
            {
                this->color = HSSRgb::p(new HSSRgb());
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyColor, this, new HSSValueChangedCallback<HSSFont > (this, &HSSFont::colorChanged));
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
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for color of " + this->name));

    this->notifyObservers(HSSObservablePropertyColor, &this->color);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSFont::colorChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** colorChanged unimplemented ****************************");
}

HSSKeywordConstant::p HSSFont::getWeight()
{
    return this->weight;
}

void HSSFont::setDWeight(HSSParserNode::p value)
{

    bool valid = false;

    switch (value->getType())
    {

    case HSSParserNodeTypeFunctionCall:
    {
        this->dWeight = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->weight = boost::any_cast<HSSKeywordConstant::p > (remoteValue);
                valid = true;
            }
            catch (...)
            {

            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyWeight, this, new HSSValueChangedCallback<HSSFont > (this, &HSSFont::weightChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant > (value);
        this->weight = keywordValue;
        valid = true;
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSFont", "Invalid value for weight of " + this->name));

    this->notifyObservers(HSSObservablePropertyWeight, &this->weight);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSFont::weightChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** weightChanged unimplemented ****************************");
}

long double HSSFont::_setLDProperty(
                                    void(HSSFont::*callback)(HSSObservableProperty property, void* data),
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
        //expressionValue->setScope(scope);
        ret = expressionValue->evaluate();
        if (callback != NULL)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSFont > (this, callback));
            observedStore = expressionValue.get();
            observedStoreProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        AXRWarning::p(new AXRWarning("HSSFont", "Unknown parser node type while setting value for HSSFont property"))->raise();
        break;
    }

    return ret;
}

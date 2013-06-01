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

#include "AXRController.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSBorder::HSSBorder(AXRController * controller)
: HSSObject(HSSObjectTypeBorder, controller)
{
    this->observedSize = NULL;
    this->observedPosition = NULL;
    this->observedSegments = NULL;
    this->position = HSSBorderPositionCenter;
    this->registerProperty(HSSObservablePropertySize, QVariant::fromValue(&this->size));
}

HSSBorder::HSSBorder(const HSSBorder & orig)
: HSSObject(orig)
{
    this->observedSize = NULL;
    this->observedPosition = NULL;
    this->observedSegments = NULL;
    this->size = orig.size;
    this->position = orig.position;
    this->segments = orig.segments;
    this->registerProperty(HSSObservablePropertySize, QVariant::fromValue(&this->size));
}

HSSBorder::~HSSBorder()
{

}

AXRString HSSBorder::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSBorder: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSBorder";
    }
}

AXRString HSSBorder::defaultObjectType()
{
    return "border";
}

AXRString HSSBorder::defaultObjectType(AXRString property)
{
    if (property == "effects")
    {
        return "shadow";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSBorder::isKeyword(AXRString value, AXRString property)
{
    if (property == "position")
    {
        if (value == "inside" || value == "center" || value == "outside")
        {
            return true;
        }
    }
    else if (property == "segments")
    {
        if (value == "left" || value == "top" || value == "right" || value == "bottom" || value == "all")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSBorder::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertySize:
        this->setDSize(value);
        break;
    case HSSObservablePropertyPosition:
        this->setDPosition(value);
        break;
    case HSSObservablePropertySegments:
        this->setDSegments(value);
        break;
    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

HSSUnit HSSBorder::getSize()
{
    return this->size;
}

void HSSBorder::setDSize(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dSize = value;
        this->size = this->_evaluatePropertyValue(
                                          &HSSBorder::sizeChanged,
                                          value,
                                          18.,
                                          HSSObservablePropertySize,
                                          this->observedSize,
                                          this->observedSizeProperty
                                          );

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dSize = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSUnit>())
            {
                this->size = remoteValue.value<HSSUnit>();
            }
            else
            {
                this->size = 1.;
            }
            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertySize, this, new HSSValueChangedCallback<HSSBorder > (this, &HSSBorder::sizeChanged));


        }
        else
        {
            throw AXRWarning("HSSDBorder", "Invalid function type size of " + this->name);
        }

        break;
    }

    default:
        throw AXRWarning("HSSBorder", "Invalid value for size of " + this->name);
    }
    this->notifyObservers(HSSObservablePropertySize, &this->size);
}

void HSSBorder::sizeChanged(AXR::HSSObservableProperty source, void *data)
{
    switch (this->dSize->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->size = *(HSSUnit*) data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertySize, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSBorderPosition HSSBorder::getPosition()
{
    return this->position;
}

QSharedPointer<HSSParserNode> HSSBorder::getDPosition()
{
    return this->dPosition;
}

void HSSBorder::setDPosition(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;
    switch (value->getType())
    {
        case HSSParserNodeTypeObjectNameConstant:
        {
            try
            {
                QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
                QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
                this->setDPosition(objdef);
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
            if (this->observedPosition)
            {
                this->observedPosition->removeObserver(this->observedPositionProperty, HSSObservablePropertyPosition, this);
                this->observedPosition = NULL;
            }
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
            fnct->setScope(this->getScope());
            fnct->setThisObj(this->getThisObj());

            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSBorderPosition*>())
            {
                this->position = *remoteValue.value<HSSBorderPosition*>();
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyPosition, this, new HSSValueChangedCallback<HSSBorder > (this, &HSSBorder::positionChanged));
            this->observedPosition = fnct.data();
            this->observedPositionProperty = HSSObservablePropertyValue;
            valid = true;

            break;
        }

        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
            if (kwValue == "inside" || kwValue == "center" || kwValue == "outside")
            {
                if (this->observedPosition)
                {
                    this->observedPosition->removeObserver(this->observedPositionProperty, HSSObservablePropertyPosition, this);
                    this->observedPosition = NULL;
                }
                if(kwValue == "inside")
                {
                    this->position = HSSBorderPositionInside;
                }
                else if(kwValue == "center"){
                    this->position = HSSBorderPositionCenter;
                }
                else if(kwValue == "outside"){
                    this->position = HSSBorderPositionOutside;
                }
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
            QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
            objdef->setScope(this->getScope());
            objdef->setThisObj(this->getThisObj());
            objdef->apply();
            QSharedPointer<HSSObject> theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> remoteValue = qSharedPointerCast<HSSValue > (theobj)->getDValue();
                try
                {
                    this->setDPosition(remoteValue);
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }
            break;
        }

        default:
            break;
    }

    if (!valid)
    {
        throw AXRWarning("HSSBorder", "Invalid value for position of " + this->getName());
    }
    this->notifyObservers(HSSObservablePropertyPosition, &this->position);
}

void HSSBorder::positionChanged(HSSObservableProperty source, void*data)
{
    HSSParserNodeType nodeType = this->dPosition->getType();
    switch (nodeType)
    {
        case HSSParserNodeTypeFunctionCall:
        {
            this->position = *(HSSBorderPosition*) data;
        }

        default:
            break;
    }

    this->notifyObservers(HSSObservablePropertyPosition, &this->position);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

std::vector<QSharedPointer<HSSParserNode> > HSSBorder::getSegments()
{
    return this->segments;
}

QSharedPointer<HSSParserNode> HSSBorder::getDSegments()
{
    return this->dSegments;
}

void HSSBorder::setDSegments(QSharedPointer<HSSParserNode> value)
{
    this->segments.clear();
    this->dSegments = value;
    this->addDSegments(value);
}

void HSSBorder::addDSegments(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
            std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
            for (HSSParserNode::it iterator = values.begin(); iterator != values.end(); ++iterator)
            {
                this->addDSegments(*iterator);
            }

            valid = true;
            break;
        }

        case HSSParserNodeTypeObjectNameConstant:
        {
            try
            {
                QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
                this->addDSegments(this->getController()->objectTreeNodeNamed(objname->getValue()));
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
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
            if (fnct && fnct->isA(HSSFunctionTypeRef))
            {
                fnct->setScope(this->getScope());
                fnct->setThisObj(this->getThisObj());
                QVariant remoteValue = fnct->evaluate();
                if (remoteValue.canConvert<QSharedPointer<HSSParserNode> >())
                {
                    try
                    {
                        QSharedPointer<HSSParserNode> theVal = remoteValue.value<QSharedPointer<HSSParserNode> >();
                        this->addDSegments(theVal);
                        valid = true;
                    }
                    catch (const AXRError &e)
                    {
                        e.raise();
                    }
                }
            }
            break;
        }

        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeStringConstant:
        {
            this->segments.push_back(value->clone());
            valid = true;
            break;
        }

        default:
            valid = false;
            break;
    }

    switch (value->getStatementType())
    {
        case HSSStatementTypeObjectDefinition:
        {
            QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value)->clone();
            objdef->setScope(this->getScope());
            objdef->setThisObj(this->getThisObj());
            objdef->apply();
            QSharedPointer<HSSObject> theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> remoteValue = qSharedPointerCast<HSSValue > (theobj)->getDValue();
                try
                {
                    this->addDSegments(remoteValue);
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }

            break;
        }

        default:
            break;
    }

    if (!valid)
        throw AXRWarning("HSSBorder", "Invalid value for segments of " + this->getName());

    this->notifyObservers(HSSObservablePropertySegments, &this->segments);
}

void HSSBorder::segmentsChanged(HSSObservableProperty source, void*data)
{

}

HSSUnit HSSBorder::_evaluatePropertyValue(
                                      void(HSSBorder::*callback)(HSSObservableProperty property, void* data),
                                      QSharedPointer<HSSParserNode> value,
                                      HSSUnit percentageBase,
                                      HSSObservableProperty observedSourceProperty,
                                      HSSObservable * &observedStore,
                                      HSSObservableProperty &observedStoreProperty
                                      )
{
    HSSUnit ret = 0;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        QSharedPointer<HSSNumberConstant> numberValue = qSharedPointerCast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setScope(this->scope);
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSBorder > (this, callback));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        AXRWarning("HSSLineBorder", "Unknown parser node type while setting value for HSSLineBorder property").raise();
        break;
    }

    return ret;
}

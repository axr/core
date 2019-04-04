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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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
#include "HSSInsertFunction.h"

using namespace AXR;

HSSInsertFunction::HSSInsertFunction(AXRController * controller)
: HSSTargetedFunction(HSSFunctionTypeInsert, controller)
{

}

HSSInsertFunction::~HSSInsertFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSInsertFunction: destructing insert function");
    this->cleanTrackedObservers();
}

HSSInsertFunction::HSSInsertFunction(const HSSInsertFunction & orig)
: HSSTargetedFunction(orig)
{
    this->value = orig.value;
}

QSharedPointer<HSSFunction> HSSInsertFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSInsertFunction::cloneImpl() const
{
    QSharedPointer<HSSInsertFunction> clone = QSharedPointer<HSSInsertFunction>(new HSSInsertFunction(*this));
    this->cloneSelectorChains(clone);
    return clone;
}

AXRString HSSInsertFunction::toString()
{
    AXRString tempstr = AXRString("HSSInsertFunction\n");
    return tempstr;
}

QSharedPointer<HSSObject> HSSInsertFunction::_evaluate()
{
    QSharedPointer<HSSObject> errorState;
    
    AXRString appendText = this->_getTextFromValue(this->getValue());
    
    QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
    QSharedPointer<HSSSelection> selection = this->getController()->select(this->getSelectorChains(), this->scope, thisObj);
    QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();

    for (HSSSimpleSelection::iterator innerIt = inner->begin(); innerIt != inner->end(); ++innerIt)
    {
        QSharedPointer<HSSContainer> container = HSSContainer::asContainer(*innerIt);
        container->appendContentText(appendText);
        container->setNeedsLayout(true);
        this->getController()->document()->setNeedsDisplay(true);
    }

    this->setDirty(true);
    return this->_value;
}

AXRString HSSInsertFunction::_getTextFromValue(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theString = qSharedPointerCast<HSSStringConstant>(value);
            return theString->getValue().stripQuotes();
        }
        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction>(value);
            if (fnct->isA(HSSFunctionTypeRef))
            {
                QSharedPointer<HSSRefFunction> refFnct = qSharedPointerCast<HSSRefFunction > (fnct);
                refFnct->setScope(this->scope);
                refFnct->setThisObj(this->getThisObj());
                QSharedPointer<HSSObject> remoteObj = refFnct->evaluate();
                if (remoteObj)
                {
                    if (remoteObj->isA(HSSObjectTypeValue))
                    {
                        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                        return this->_getTextFromValue(parserNode);
                    }
                    else
                    {
                        
                    }
                }
            }
            break;
        }
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kwValue("`");
            kwValue.append(qSharedPointerCast<HSSStringConstant>(value)->getValue());
            kwValue.append("`");
            return kwValue;
        }
        case HSSParserNodeTypeNumberConstant:
        {
            AXRString kwValue = HSSString::number(qSharedPointerCast<HSSNumberConstant>(value)->getValue());
            return kwValue;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSObjectNameConstant > (value)->getValue();
            return kwValue;
        }

        default:
            break;
    }
    return AXRString("");
}

QSharedPointer<HSSParserNode> HSSInsertFunction::getValue() const
{
    return this->value;
}

void HSSInsertFunction::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    this->value = parserNode;
}

void HSSInsertFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSTargetedFunction::setScope(newScope);
    //propagate values
    if (this->value)
    {
        switch (this->value->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(this->value)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunctionCall:
                qSharedPointerCast<HSSFunction>(this->value)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSInsertFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSTargetedFunction::setThisObj(newThis);
    if (this->value)
    {
        this->value->setThisObj(newThis);
    }
}

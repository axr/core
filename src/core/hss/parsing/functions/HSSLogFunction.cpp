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
#include "AXRLoggerManager.h"
#include "HSSDisplayObject.h"
#include "HSSLogFunction.h"
#include "HSSNumberConstant.h"
#include "HSSObjectNameConstant.h"
#include "HSSRefFunction.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSLogFunction::HSSLogFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeLog, controller)
{

}

HSSLogFunction::~HSSLogFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLogFunction: destructing log function");
    this->cleanTrackedObservers();
}

HSSLogFunction::HSSLogFunction(const HSSLogFunction & orig)
: HSSFunction(orig)
{
    Q_FOREACH(QSharedPointer<HSSParserNode> parserNode, orig.values)
    {
        this->add(parserNode);
    }
}

QSharedPointer<HSSFunction> HSSLogFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

AXRString HSSLogFunction::toString()
{
    AXRString tempstr = AXRString("HSSLogFunction\n");
    return tempstr;
}

std::vector<QSharedPointer<HSSParserNode> > HSSLogFunction::getValues()
{
    return this->values;
}

void HSSLogFunction::setValues(std::vector<QSharedPointer<HSSParserNode> > newValues)
{
    this->values = newValues;
}

void HSSLogFunction::add(QSharedPointer<HSSParserNode> newValue)
{
    this->values.push_back(newValue);
}

bool HSSLogFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSFunction::equalTo(otherNode)) return false;
    return true;
}

QSharedPointer<HSSObject> HSSLogFunction::_evaluate()
{
    Q_FOREACH(QSharedPointer<HSSParserNode> parserNode, this->values){
        this->_logParserNode(parserNode);
    }
    this->setDirty(true);
    return this->_value;
}

void HSSLogFunction::_logParserNode(QSharedPointer<HSSParserNode> parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (parserNode)->clone();
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
                        this->_logParserNode(parserNode);
                    }
                    else
                    {
                        this->_logObject(remoteObj);
                    }
                }
            }
            break;
        }

        case HSSParserNodeTypeStringConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            axr_log(LoggerChannelLogFunction, kwValue);
            break;
        }

        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kwValue("`");
            kwValue.append(qSharedPointerCast<HSSStringConstant>(parserNode)->getValue());
            kwValue.append("`");
            axr_log(LoggerChannelLogFunction, kwValue);
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        {
            AXRString kwValue = QString::number(qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue());
            axr_log(LoggerChannelLogFunction, kwValue);
            break;
        }

        case HSSParserNodeTypeObjectNameConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSObjectNameConstant > (parserNode)->getValue();
            axr_log(LoggerChannelLogFunction, kwValue);
        }

        default:
            break;
    }
}

void HSSLogFunction::_logObject(QSharedPointer<HSSObject> theObj) const
{
    axr_log(LoggerChannelGeneral, "not implemented yet");
}

QSharedPointer<HSSClonable> HSSLogFunction::cloneImpl() const
{
    QSharedPointer<HSSLogFunction> clone = QSharedPointer<HSSLogFunction>(new HSSLogFunction(*this));

    return clone;
}

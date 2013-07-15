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
#include "AXRWarning.h"
#include "HSSDisplayObject.h"
#include "HSSFunction.h"
#include "HSSLog.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSRefFunction.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSLog::HSSLog(AXRController * controller)
: HSSAction(HSSActionTypeLog, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLog: creating log object");
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

HSSLog::HSSLog(const HSSLog & orig)
: HSSAction(orig)
{
}

QSharedPointer<HSSLog> HSSLog::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLog: cloning log object");
    return qSharedPointerCast<HSSLog> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLog::cloneImpl() const
{
    return QSharedPointer<HSSLog>(new HSSLog(*this));
}

HSSLog::~HSSLog()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLog: destructing log object");
}

AXRString HSSLog::toString()
{
    return "HSSLog";
}

AXRString HSSLog::defaultObjectType()
{
    return "log";
}

void HSSLog::fire()
{
    QSharedPointer<HSSObject> valueObj = this->getValue();
    if (valueObj)
    {
        if (valueObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(valueObj)->getValue();
            this->_logParserNode(parserNode);
        }
        else
        {
            this->_logObject(valueObj);
        }
    }
}

void HSSLog::_logParserNode(QSharedPointer<HSSParserNode> parserNode) const
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
                }
            }
            break;
        }

        case HSSParserNodeTypeStringConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            axr_log(LoggerChannelGeneral, kwValue);
            break;
        }

        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kwValue("`");
            kwValue.append(qSharedPointerCast<HSSStringConstant>(parserNode)->getValue());
            kwValue.append("`");
            axr_log(LoggerChannelGeneral, kwValue);
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        {
            AXRString kwValue = QString::number(qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue());
            axr_log(LoggerChannelGeneral, kwValue);
            break;
        }

        case HSSParserNodeTypeObjectNameConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSObjectNameConstant > (parserNode)->getValue();
            axr_log(LoggerChannelGeneral, kwValue);
        }

        default:
            break;
    }
}

void HSSLog::_logObject(QSharedPointer<HSSObject> theObj) const
{
    axr_log(LoggerChannelGeneral, "not implemented yet");
}

const QSharedPointer<HSSObject> HSSLog::getValue() const
{
    return this->getComputedValue("value");
}

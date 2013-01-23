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
#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "HSSDisplayObject.h"
#include "HSSFunction.h"
#include "HSSLog.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSRefFunction.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"

using namespace AXR;

HSSLog::HSSLog(AXRController * controller)
: HSSAction(HSSActionTypeLog, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLog: creating log object");
    this->observedValue = NULL;
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

HSSLog::HSSLog(const HSSLog & orig)
: HSSAction(orig)
{
    this->observedValue = NULL;
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

QSharedPointer<HSSLog> HSSLog::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLog: cloning log object");
    return qSharedPointerCast<HSSLog> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLog::cloneImpl() const
{
    return QSharedPointer<HSSLog>(new HSSLog(*this));
}

HSSLog::~HSSLog()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLog: destructing log object");
}

AXRString HSSLog::toString()
{
    return "HSSLog";
}

AXRString HSSLog::defaultObjectType()
{
    return "log";
}

void HSSLog::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyValue:
        this->setDValue(value);
        break;

    default:
        HSSAction::setProperty(name, value);
        break;
    }
}

void HSSLog::fire()
{
    bool done = false;
    switch (this->dValue->getType())
    {
        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (this->dValue)->clone();
            if (fnct->isA(HSSFunctionTypeRef))
            {
                QSharedPointer<HSSRefFunction> refFnct = qSharedPointerCast<HSSRefFunction > (fnct);
                refFnct->setScope(this->scope);
                refFnct->setThisObj(this->getThisObj());
                QVariant remoteValue = refFnct->evaluate();
                if (remoteValue.canConvert<AXRString>())
                {
                    AXRString theVal = remoteValue.value<AXRString>();
                    std_log(theVal);
                    done = true;
                }

                if (remoteValue.canConvert<HSSUnit>())
                {
                    HSSUnit theVal = remoteValue.value<HSSUnit>();
                    std_log(AXRString::number(theVal));
                    done = true;
                }

                if (remoteValue.canConvert<QSharedPointer<HSSObject> >())
                {
                    QSharedPointer<HSSObject> theVal = remoteValue.value<QSharedPointer<HSSObject> >();
                    std_log(theVal->toString());
                    done = true;
                }

                if (remoteValue.canConvert< std::vector<QSharedPointer<HSSObject> >  >())
                {
                    std::vector<QSharedPointer<HSSObject> > v_data = remoteValue.value< std::vector<QSharedPointer<HSSObject> >  >();
                    if (v_data.empty())
                    {
                        std_log("empty");
                    }
                    else
                    {
                        for (std::vector<QSharedPointer<HSSObject> >::iterator it = v_data.begin(); it != v_data.end(); ++it)
                        {
                            std_log((*it)->toString());
                        }
                    }

                    done = true;
                }

                if (remoteValue.canConvert< QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> >  > >())
                {
                    QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> >  > m_data = remoteValue.value< QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> >  > >();
                    if (m_data.empty())
                    {
                        std_log("empty");
                    }
                    else
                    {
                        for (QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> >  >::iterator it = m_data.begin(); it != m_data.end(); ++it)
                        {
                            std::vector<QSharedPointer<HSSObject> > v_data = it.value();
                            for (std::vector<QSharedPointer<HSSObject> >::iterator it2 = v_data.begin(); it2 != v_data.end(); ++it2)
                            {
                                std_log((*it2)->toString());
                            }
                        }
                    }

                    done = true;
                }
            }
            break;
        }

        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> str = qSharedPointerCast<HSSStringConstant > (this->dValue);
            std_log(str->getValue());
            done = true;
            break;
        }

        case HSSParserNodeTypeObjectNameConstant:
        {
            try
            {
                QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (this->dValue);
                QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
                objdef->setThisObj(this->getThisObj());
                objdef->setScope(this->scope);
                objdef->apply();
                QSharedPointer<HSSObject> theObject = objdef->getObject();
                std_log(theObject->toString());
                done = true;
            }
            catch (const AXRError &e)
            {
                e.raise();

            }

            break;
        }

        default:
        {
            break;
        }
    }

    if (!done)
    {
        std_log(this->dValue->toString());
    }
}

QSharedPointer<HSSParserNode> HSSLog::getDValue()
{
    return this->dValue;
}

void HSSLog::setDValue(QSharedPointer<HSSParserNode> value)
{
    this->dValue = value;
    this->dValue->setThisObj(this->getThisObj());
    if (this->observedValue)
    {
        this->observedValue->removeObserver(this->observedValueProperty, HSSObservablePropertyValue, this);
    }

    this->notifyObservers(HSSObservablePropertyValue, &this->dValue);
}

void HSSLog::valueChanged(AXR::HSSObservableProperty source, void *data)
{
    this->dValue = *(QSharedPointer<HSSParserNode> *)data;

    this->notifyObservers(HSSObservablePropertyValue, &this->dValue);
}

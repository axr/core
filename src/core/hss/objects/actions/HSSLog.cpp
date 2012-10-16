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
#include "HSSFunction.h"
#include "HSSLog.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSRefFunction.h"
#include "HSSStringConstant.h"

using namespace AXR;

HSSLog::HSSLog()
: HSSAction(HSSActionTypeLog)
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

HSSLog::p HSSLog::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLog: cloning log object");
    return qSharedPointerCast<HSSLog, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSLog::cloneImpl() const
{
    return HSSClonable::p(new HSSLog(*this));
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

void HSSLog::setProperty(HSSObservableProperty name, HSSParserNode::p value)
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
            HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (this->dValue)->clone();
            if (fnct->isA(HSSFunctionTypeRef))
            {
                HSSRefFunction::p refFnct = qSharedPointerCast<HSSRefFunction > (fnct);
                refFnct->setScope(this->scope);
                refFnct->setThisObj(this->getThisObj());
                boost::any remoteValue = refFnct->evaluate();
                try
                {
                    AXRString theVal = boost::any_cast<AXRString > (remoteValue);
                    std_log(theVal);
                    done = true;
                }
                catch (boost::bad_any_cast &)
                {
                    //do nothing
                }
                try
                {
                    HSSUnit theVal = boost::any_cast<HSSUnit > (remoteValue);
                    std_log(AXRString::number(theVal));
                    done = true;
                }
                catch (boost::bad_any_cast &)
                {
                    //do nothing
                }
                try
                {
                    HSSObject::p theVal = boost::any_cast<HSSObject::p > (remoteValue);
                    std_log(theVal->toString());
                    done = true;
                }
                catch (boost::bad_any_cast &)
                {
                    //do nothing
                }
                try
                {
                    std::vector<HSSObject::p> v_data = boost::any_cast< std::vector<HSSObject::p> >(remoteValue);
                    if (v_data.empty())
                    {
                        std_log("empty");
                    }
                    else
                    {
                        for (std::vector<HSSObject::p>::iterator it = v_data.begin(); it != v_data.end(); ++it)
                        {
                            std_log((*it)->toString());
                        }
                    }

                    done = true;
                }
                catch (boost::bad_any_cast &)
                {
                    //do nothing
                }
                try
                {
                    QMap<HSSEventType, std::vector<HSSObject::p> > m_data = boost::any_cast< QMap<HSSEventType, std::vector<HSSObject::p> > >(remoteValue);
                    if (m_data.empty())
                    {
                        std_log("empty");
                    }
                    else
                    {
                        for (QMap<HSSEventType, std::vector<HSSObject::p> >::iterator it = m_data.begin(); it != m_data.end(); ++it)
                        {
                            std::vector<HSSObject::p> v_data = it.value();
                            for (std::vector<HSSObject::p>::iterator it2 = v_data.begin(); it2 != v_data.end(); ++it2)
                            {
                                std_log((*it2)->toString());
                            }
                        }
                    }

                    done = true;
                }
                catch (boost::bad_any_cast &)
                {
                    //do nothing
                }
            }
            break;
        }

        case HSSParserNodeTypeStringConstant:
        {
            HSSStringConstant::p str = qSharedPointerCast<HSSStringConstant > (this->dValue);
            std_log(str->getValue());
            done = true;
            break;
        }

        case HSSParserNodeTypeObjectNameConstant:
        {
            try
            {
                HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (this->dValue);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                objdef->setThisObj(this->getThisObj());
                objdef->setScope(this->scope);
                objdef->apply();
                HSSObject::p theObject = objdef->getObject();
                std_log(theObject->toString());
                done = true;
            }
            catch (const AXRError &e)
            {
                e.raise();

            }
            catch (const AXRWarning &e)
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

HSSParserNode::p HSSLog::getDValue()
{
    return this->dValue;
}

void HSSLog::setDValue(HSSParserNode::p value)
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
    this->dValue = *(HSSParserNode::p *)data;

    this->notifyObservers(HSSObservablePropertyValue, &this->dValue);
}

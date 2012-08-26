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

#include <boost/lexical_cast.hpp>
#include <boost/pointer_cast.hpp>
#include "AXRController.h"
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
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

HSSLog::HSSLog(const HSSLog & orig)
: HSSAction(orig)
{
    this->observedValue = NULL;
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

HSSLog::p HSSLog::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLog: cloning log object");
    return boost::static_pointer_cast<HSSLog, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSLog::cloneImpl() const
{
    return HSSClonable::p(new HSSLog(*this));
}

HSSLog::~HSSLog()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLog: destructing log object");
}

std::string HSSLog::toString()
{
    return "HSSLog";
}

std::string HSSLog::defaultObjectType()
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
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (this->dValue)->clone();
        if (fnct->isA(HSSFunctionTypeRef))
        {
            HSSRefFunction::p refFnct = boost::static_pointer_cast<HSSRefFunction > (fnct);
            refFnct->setScope(this->scope);
            refFnct->setThisObj(this->getThisObj());
            boost::any remoteValue = refFnct->evaluate();
            try
            {
                std::string theVal = boost::any_cast<std::string > (remoteValue);
                std_log(theVal);
                done = true;
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }
            try
            {
                HSSUnit theVal = boost::any_cast<HSSUnit > (remoteValue);
                std_log(boost::lexical_cast<std::string > (theVal));
                done = true;
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }
            try
            {
                HSSObject::p theVal = boost::any_cast<HSSObject::p > (remoteValue);
                std_log(theVal->toString());
                done = true;
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }
            try
            {
                std::vector<HSSObject::p> v_data = boost::any_cast< std::vector<HSSObject::p> >(remoteValue);
                std::vector<HSSObject::p>::iterator it;
                if (v_data.size() == 0)
                {
                    std_log("empty");
                }
                else
                {
                    for (it = v_data.begin(); it != v_data.end(); it++)
                    {
                        std_log((*it)->toString());
                    }
                }
                done = true;
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }
            try
            {
                boost::unordered_map<HSSEventType, std::vector<HSSObject::p> > m_data = boost::any_cast< boost::unordered_map<HSSEventType, std::vector<HSSObject::p> > >(remoteValue);
                boost::unordered_map<HSSEventType, std::vector<HSSObject::p> >::iterator it;
                if (m_data.size() == 0)
                {
                    std_log("empty");
                }
                else
                {
                    for (it = m_data.begin(); it != m_data.end(); it++)
                    {
                        std::vector<HSSObject::p> v_data = (*it).second;
                        std::vector<HSSObject::p>::iterator it2;
                        for (it2 = v_data.begin(); it2 != v_data.end(); it2++)
                        {
                            std_log((*it2)->toString());
                        }
                    }
                }
                done = true;
            }
            catch (boost::bad_any_cast & e)
            {
                //do nothing
            }
        }
        break;
    }

    case HSSParserNodeTypeStringConstant:
    {
        HSSStringConstant::p str = boost::static_pointer_cast<HSSStringConstant > (this->dValue);
        std_log(str->getValue());
        done = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (this->dValue);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            objdef->setThisObj(this->getThisObj());
            objdef->setScope(this->scope);
            objdef->apply();
            HSSObject::p theObject = objdef->getObject();
            std_log(theObject->toString());
            done = true;
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
    if (this->observedValue != NULL)
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

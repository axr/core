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
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSValue::HSSValue(AXRController * controller)
: HSSObject(HSSObjectTypeValue, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSValue: creating value object");

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

HSSValue::HSSValue(const HSSValue & orig)
: HSSObject(orig)
{
}

QSharedPointer<HSSValue> HSSValue::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSValue: cloning value object");
    return qSharedPointerCast<HSSValue>(this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSValue::cloneImpl() const
{
    return QSharedPointer<HSSValue>(new HSSValue(*this));
}

HSSValue::~HSSValue()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSValue: destructing value object");
}

AXRString HSSValue::toString()
{
    return "HSSValue: " + (dValue ? dValue->toString() : "(null)");
}

AXRString HSSValue::defaultObjectType()
{
    return "value";
}

AXRString HSSValue::defaultObjectType(AXRString property)
{
    if (property == "value")
    {
        return "request";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}


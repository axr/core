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

/**
 *  @todo set defaults
 */

#include <cmath>
#include <QVariant>
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSColorStop.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSLinearGradient.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSLinearGradient::HSSLinearGradient(AXRController * controller)
: HSSGradient(HSSGradientTypeLinear, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: creating linear gradient object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("endY");
    this->setShorthandProperties(shorthandProperties);
}

HSSLinearGradient::HSSLinearGradient(const HSSLinearGradient & orig)
: HSSGradient(orig)
{
    this->_initialize();
}

void HSSLinearGradient::_initialize()
{
}

QSharedPointer<HSSLinearGradient> HSSLinearGradient::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: cloning linear gradient object");
    return qSharedPointerCast<HSSLinearGradient> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLinearGradient::cloneImpl() const
{
    return QSharedPointer<HSSLinearGradient>(new HSSLinearGradient(*this));
}

HSSLinearGradient::~HSSLinearGradient()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: destructing linear gradient object");
}

AXRString HSSLinearGradient::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSLinearGradient: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSLinearGradient";
    }
}

AXRString HSSLinearGradient::defaultObjectType()
{
    return "linearGradient";
}

AXRString HSSLinearGradient::defaultObjectType(AXRString property)
{
    //    if (property == "bla"){
    //
    //    } else {
    return HSSGradient::defaultObjectType(property);
    //    }
}

bool HSSLinearGradient::isKeyword(AXRString value, AXRString property)
{
    if (value == "top" || value == "bottom")
    {
        if (property == "startX")
        {
            return true;
        }
    }
    else if (value == "left" || value == "right")
    {
        if (property == "startY")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSGradient::isKeyword(value, property);
}


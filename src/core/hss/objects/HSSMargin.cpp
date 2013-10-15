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

#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSMargin.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSSimpleSelection.h"
#include "HSSValue.h"

using namespace AXR;

HSSMargin::HSSMargin(AXRController * controller)
: HSSObject(HSSObjectTypeMargin, controller)
{
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("top");
    shorthandProperties.push_back("right");
    shorthandProperties.push_back("bottom");
    shorthandProperties.push_back("left");
    this->setShorthandProperties(shorthandProperties);
}

HSSMargin::HSSMargin(const HSSMargin & orig)
: HSSObject(orig)
{
    this->_initialize();
}

void HSSMargin::_initialize()
{
    this->addCallback("size", new HSSObserveCallback<HSSMargin>(this, &HSSMargin::setSize));
}

QSharedPointer<HSSMargin> HSSMargin::clone() const
{
    return qSharedPointerCast<HSSMargin> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSMargin::cloneImpl() const
{
    return QSharedPointer<HSSMargin>(new HSSMargin(*this));
}

HSSMargin::~HSSMargin()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSMargin: destructing margin object");
    this->cleanTrackedObservers();
}

void HSSMargin::setDefaults()
{
    this->setDefault("size", 0.);
    this->setDefault("top", 0.);
    this->setDefault("right", 0.);
    this->setDefault("bottom", 0.);
    this->setDefault("left", 0.);
}

AXRString HSSMargin::toString()
{
    return "HSSMargin";
}

AXRString HSSMargin::defaultObjectType()
{
    return "margin";
}

AXRString HSSMargin::defaultObjectType(AXRString property)
{
    return HSSObject::defaultObjectType(property);
}

bool HSSMargin::isKeyword(AXRString value, AXRString property)
{
    return false;
}

HSSUnit HSSMargin::getSize() const
{
    return this->getComputedNumber("size");
}

void HSSMargin::setSize(HSSUnit value)
{
    this->setLeft(value);
    this->setRight(value);
    this->setTop(value);
    this->setBottom(value);
}

void HSSMargin::setSize(QSharedPointer<HSSObject> theObj)
{
    this->setComputed("left", theObj);
    this->setComputed("right", theObj);
    this->setComputed("top", theObj);
    this->setComputed("bottom", theObj);
}


HSSUnit HSSMargin::getTop() const
{
    return this->getComputedNumber("top");
}

void HSSMargin::setTop(HSSUnit value)
{
    this->setComputedValue("top", value);
}

HSSUnit HSSMargin::getRight() const
{
    return this->getComputedNumber("right");
}

void HSSMargin::setRight(HSSUnit value)
{
    this->setComputedValue("right", value);
}

HSSUnit HSSMargin::getBottom() const
{
    return this->getComputedNumber("bottom");
}

void HSSMargin::setBottom(HSSUnit value)
{
    this->setComputedValue("bottom", value);
}

HSSUnit HSSMargin::getLeft() const
{
    return this->getComputedNumber("left");
}

void HSSMargin::setLeft(HSSUnit value)
{
    this->setComputedValue("left", value);
}

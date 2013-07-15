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

#include "HSSContainer.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSPercentageConstant::HSSPercentageConstant(HSSUnit number, AXRController * controller)
: HSSParserNode(HSSParserNodeTypePercentageConstant, controller)
{
    this->_number = number / 100.;
    this->_isDirty = true;
    this->_percentageBase = this->_value = 0.;
}

QSharedPointer<HSSPercentageConstant> HSSPercentageConstant::clone() const
{
    return qSharedPointerCast<HSSPercentageConstant> (this->cloneImpl());
}

HSSPercentageConstant::~HSSPercentageConstant()
{

}

AXRString HSSPercentageConstant::toString()
{
    return AXRString("HSSPercentageConstant with value %1%").arg(this->_number);
}

HSSUnit HSSPercentageConstant::evaluate()
{
    if (this->isDirty())
    {
        this->setDirty(false);
        this->setValue(this->_number * this->_percentageBase);
    }
    return this->getValue();
}

void HSSPercentageConstant::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSPercentageConstant::isDirty()
{
    return this->_isDirty;
}

void HSSPercentageConstant::setNumber(HSSUnit newValue)
{
    this->_number = newValue;
}

HSSUnit HSSPercentageConstant::getNumber()
{
    return this->_number;
}

void HSSPercentageConstant::setValue(HSSUnit newValue)
{
    this->_value = newValue;
}

HSSUnit HSSPercentageConstant::getValue()
{
    return this->_value;
}

void HSSPercentageConstant::setPercentageBase(HSSUnit value)
{
    this->_percentageBase = value;
    this->setDirty(true);
}

QSharedPointer<HSSClonable> HSSPercentageConstant::cloneImpl() const
{
    return QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(*this));
}

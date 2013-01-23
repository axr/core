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

#include "HSSContainer.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSPercentageConstant::HSSPercentageConstant(HSSUnit value, AXRController * controller)
: HSSParserNode(HSSParserNodeTypePercentageConstant, controller)
{
    this->value = value / 100.;
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
    return AXRString("HSSPercentageConstant with value %1%").arg(this->value);
}

void HSSPercentageConstant::setValue(HSSUnit newValue)
{
    this->value = value / 100.;
}

HSSUnit HSSPercentageConstant::getValue(HSSUnit baseValue)
{
    return this->value * baseValue;
}

QSharedPointer<HSSClonable> HSSPercentageConstant::cloneImpl() const
{
    return QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(*this));
}

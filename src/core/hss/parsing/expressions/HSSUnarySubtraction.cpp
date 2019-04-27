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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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

#include "axr.h"
#include "HSSUnarySubtraction.h"

using namespace AXR;

HSSUnarySubtraction::HSSUnarySubtraction(AXRController * controller)
: HSSUnaryExpression(HSSUnaryExpressionTypeSubtraction, controller)
{
    
}

HSSUnarySubtraction::HSSUnarySubtraction(const HSSUnarySubtraction &orig)
: HSSUnaryExpression(orig)
{
    
}

QSharedPointer<HSSUnarySubtraction> HSSUnarySubtraction::clone() const
{
    return qSharedPointerCast<HSSUnarySubtraction> (this->cloneImpl());
}

HSSUnarySubtraction::~HSSUnarySubtraction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSUnarySubtraction: destructing unary sum expression");
    this->cleanTrackedObservers();
}

HSSUnit HSSUnarySubtraction::calculate(HSSUnit leftval, HSSUnit rightval)
{
    return leftval - rightval;
}

QSharedPointer<HSSClonable> HSSUnarySubtraction::cloneImpl() const
{
    return QSharedPointer<HSSUnarySubtraction>(new HSSUnarySubtraction(*this));
}

void HSSUnarySubtraction::addValue(QSharedPointer<AXR::HSSParserNode> value)
{
    this->_rightNode = value;
}

void HSSUnarySubtraction::mergeNumber(QSharedPointer<HSSNumberConstant> numConst, QSharedPointer<HSSUnaryExpression> other)
{
    QSharedPointer<HSSNumberConstant> thisNumConst = qSharedPointerCast<HSSNumberConstant>(this->_rightNode);
    HSSUnit thisNumber = thisNumConst->getValue();
    
    switch (other->getUnaryExpressionType()) {
        case HSSUnaryExpressionTypeSum:
        {
            thisNumConst->setValue(thisNumber - numConst->getValue());
            break;
        }
        case HSSUnaryExpressionTypeSubtraction:
        {
            thisNumConst->setValue(thisNumber + numConst->getValue());
            break;
        }

        default:
            break;
    }
}

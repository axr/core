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
#include "HSSDisplayObject.h"
#include "HSSSubtraction.h"

using namespace AXR;

HSSSubtraction::HSSSubtraction(QSharedPointer<HSSParserNode> _left, QSharedPointer<HSSParserNode> _right, AXRController * controller)
: HSSExpression(HSSExpressionTypeSubtraction, _left, _right, controller)
{

}

HSSSubtraction::HSSSubtraction(const HSSSubtraction &orig)
: HSSExpression(orig)
{

}

QSharedPointer<HSSSubtraction> HSSSubtraction::clone() const
{
    return qSharedPointerCast<HSSSubtraction> (this->cloneImpl());
}

HSSSubtraction::~HSSSubtraction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSSubtraction: destructing subtraction expression");
    this->cleanTrackedObservers();
}

HSSUnit HSSSubtraction::calculate(HSSUnit leftval, HSSUnit rightval)
{
    return leftval - rightval;
}

QSharedPointer<HSSClonable> HSSSubtraction::cloneImpl() const
{
    return QSharedPointer<HSSSubtraction>(new HSSSubtraction(*this));
}

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

#ifndef HSSDIVISION_H
#define HSSDIVISION_H

#include "HSSExpression.h"

namespace AXR
{
    /**
     *  @brief A type of expression that uses the division operator "/".
     */
    class AXR_API HSSDivision : public HSSExpression
    {
    public:
        /**
         *  Creates a new instance of a division expression, with two
         *  parser nodes as left and right terms.
         *
         *  @param left     The value on the left hand side of the operator.
         *  @param right    The value on the right hand side of the operator.
         */
        HSSDivision(QSharedPointer<HSSParserNode> left, QSharedPointer<HSSParserNode> right, AXRController * controller);

        /**
         *  Copy constructor. Do not use directly, but use clone() instead.
         *
         *  @param orig     The other division expression to clone.
         */
        HSSDivision(const HSSDivision &orig);

        /**
         *  Creates new clone of the current instance.
         *  @return A shared pointer to the new division expression.
         */
        QSharedPointer<HSSDivision> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSDivision();

        /**
         *  The actual implementation of the operation.
         *
         *  @param leftval  A HSSUnit containing the dividend.
         *  @param rightval A HSSUnit containing the divisor.
         *  @return A HSSUnit containing the quotient of the division.
         */
        HSSUnit calculate(HSSUnit leftval, HSSUnit rightval);
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif //HSSDIVISION_H

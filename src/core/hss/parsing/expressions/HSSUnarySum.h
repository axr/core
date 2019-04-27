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

#ifndef HSSUNARYSUM_H
#define HSSUNARYSUM_H

#include "HSSUnaryExpression.h"

namespace AXR
{
    /**
     *  @brief A type of unary expression (only one operand) that uses the plus operator "+".
     */
    class AXR_API HSSUnarySum : public HSSUnaryExpression
    {
    public:
        /**
         *  Creates a new instance of a unary sum expression
         */
        HSSUnarySum(AXRController * controller);
        
        /**
         *  Copy constructor. Do not use directly, but use clone() instead.
         *
         *  @param orig     The other sum expression to clone.
         */
        HSSUnarySum(const HSSUnarySum &orig);
        
        /**
         *  Creates new clone of the current instance.
         *  @return A shared pointer to the new sum expression.
         */
        QSharedPointer<HSSUnarySum> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSUnarySum();
        
        /**
         *  The actual implementation of the operation.
         *
         *  @param leftval  A HSSUnit containing the left member.
         *  @param rightval A HSSUnit containing the right member.
         *  @return A HSSUnit containing the addition of both members.
         */
        HSSUnit calculate(HSSUnit leftval, HSSUnit rightval);

        void addValue(QSharedPointer<HSSParserNode> value);
        
        virtual void mergeNumber(QSharedPointer<HSSNumberConstant> numConst, QSharedPointer<HSSUnaryExpression> other);
        
        QSharedPointer<HSSStringConstant> appendString(QSharedPointer<HSSObject> currentValue);
        
        bool isStringOperation() const;

    private:
        bool _stringMode;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        HSSString _getRightString(QSharedPointer<HSSParserNode> rightNode);
    };
}

#endif

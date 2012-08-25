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
 *      LICENSE: see License.txt file
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

#ifndef HSSSUBTRACTION_H
#define HSSSUBTRACTION_H

#include <boost/shared_ptr.hpp>
#include "HSSExpression.h"

namespace AXR {

    /**
     *  @brief A type of expression that uses the subtraction operator "-".
     *
     *  The right member of this expression is subtracted from the left one.
     */
    class HSSSubtraction : public HSSExpression {

    public:
        typedef boost::shared_ptr<HSSSubtraction> p;

        /**
         *  Creates a new instance of a subtraction expression, with two
         *  parser nodes as left and right terms.
         *
         *  @param left     The value on the left hand side of the operator.
         *  @param right    The value on the right hand side of the operator.
         */
        HSSSubtraction(HSSParserNode::p left, HSSParserNode::p right);

        /**
         *  Copy constructor. Do not use directly, but use clone() instead.
         *
         *  @param orig     The other subtraction expression to clone.
         */
        HSSSubtraction(const HSSSubtraction &orig);

        /**
         *  Creates new clone of the current instance.
         *  @return A shared pointer to the new subtraction expression.
         */
        p clone() const;

        virtual ~HSSSubtraction();

        /**
         *  The actual implementation of the operation.
         *
         *  @param leftval  A long double containing the value from which should be subtracted.
         *  @param rightval A long double containing the value to be subtracted.
         *  @return A long double containing result of the subtraction.
         */
        long double calculate(long double leftval, long double rightval);

    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif //HSSSUBTRACTION_H

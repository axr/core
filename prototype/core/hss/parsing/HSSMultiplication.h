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

#ifndef HSSMULTIPLICATION_H
#define HSSMULTIPLICATION_H

#include <boost/shared_ptr.hpp>
#include "HSSExpression.h"

namespace AXR {

    /**
     *  @brief Type of expression that multiplies the left value by the right value.
     */
    class HSSMultiplication : public HSSExpression {

    public:
        typedef boost::shared_ptr<HSSMultiplication> p;

        /**
         *  Creates a new instance of a multiplication expression with the two
         *  members you give.
         *
         *  @param left     A shared parser node to the left member of the expression.
         *  @param right    A shared parser node to the right member of the expression.
         */
        HSSMultiplication(HSSParserNode::p left, HSSParserNode::p right);

        /**
         *  Copy constructor for HSSContainer objects. Do not call directly, use clone() instead.
         *  @param orig     The original multiplication to clone.
         */
        HSSMultiplication(const HSSMultiplication &orig);

        /**
         *  Clones an instance of a multiplication expression and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSMultiplication
         */
        p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSMultiplication();

        long double calculate(long double leftval, long double rightval);

    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif //HSSMULTIPLICATION_H

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

#ifndef HSSIFFUNCTION_H
#define HSSIFFUNCTION_H

#include "HSSEvaluableFunction.h"

namespace AXR
{
    /**
     *  @brief Implementation of the if() { } function in HSS.
     */
    class AXR_API HSSIfFunction : public HSSEvaluableFunction
    {
    public:
        static HSSComparisonType comparisonTypeForTokenType(HSSTokenType tokenType);
        
        /**
         *  Creates a new instance of an if function.
         */
        HSSIfFunction(AXRController * controller);
        
        /**
         *  Copy constructor for HSSIfFunction objects. Do not call directly, use clone() instead.
         */
        HSSIfFunction(const HSSIfFunction & orig);
        
        /**
         *  Clones an instance of HSSIfFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSIfFunction
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSIfFunction();
        
        virtual AXRString toString();
        
        void addValue(QSharedPointer<HSSParserNode> value);
        void setReadEvaluables(bool newVal);
        const bool readEvaluables() const;

        bool evaluateCondition();
        void fire();
        
        void setComparisonType(HSSComparisonType comparisonType);
        const HSSComparisonType comparisonType() const;
        
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);
        
        void setElse(QSharedPointer<HSSParserNode> newElse);
        QSharedPointer<HSSParserNode> getElse();
        
    protected:
        HSSComparisonType _comparisonType;
        QSharedPointer<HSSParserNode> _condition;
        bool _readEvaluables;
        QSharedPointer<HSSParserNode> _else;

        /**
         *  This is the actual implementation of what the function does.
         *  @return An empty pointer.
         */
        virtual QSharedPointer<HSSObject> _evaluate();
        bool _evaluateCondition(QSharedPointer<HSSParserNode> parserNode);
        
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

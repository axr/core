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

#ifndef HSSFUNCTIONFUNCTION_H
#define HSSFUNCTIONFUNCTION_H

#include "HSSFunction.h"
#include "HSSEvaluableFunction.h"

namespace AXR
{
    class HSSObjectNameConstant;

    /**
     *  @brief Implementation of user defined functions in HSS.
     */
    class AXR_API HSSFunctionFunction : public HSSEvaluableFunction
    {
    public:
        static HSSComparisonType comparisonTypeForTokenType(HSSTokenType tokenType);
        
        /**
         *  Creates a new instance of an this class.
         */
        HSSFunctionFunction(AXRController * controller);
        
        /**
         *  Copy constructor for this class. Do not call directly, use clone() instead.
         */
        HSSFunctionFunction(const HSSFunctionFunction & orig);
        
        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSFunctionFunction();
        
        virtual AXRString toString();
        
        void addValue(QSharedPointer<HSSParserNode> value);

        void setReturnValue(QSharedPointer<HSSObject> theObj);
        QSharedPointer<HSSObject> evaluate(const std::vector<QSharedPointer<HSSArgument> > & arguments);

        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);

        std::vector<QSharedPointer<HSSObjectNameConstant> > getArguments();
        void setArguments(std::vector<QSharedPointer<HSSObjectNameConstant> > arguments);
        void addArgument(QSharedPointer<HSSObjectNameConstant> argument);
        
    protected:
        HSSComparisonType _comparisonType;
        
        /**
         *  This is the actual implementation of what the function does.
         *  @return An empty pointer.
         */
        virtual QSharedPointer<HSSObject> _evaluate();
        
    private:
        std::vector<QSharedPointer<HSSObjectNameConstant> > _arguments;
        

        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

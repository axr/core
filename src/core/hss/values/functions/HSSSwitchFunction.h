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

#ifndef HSSSWITCHFUNCTION_H
#define HSSSWITCHFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    class HSSCase;

    /**
     *  @brief Implementation of the if() { } function in HSS.
     */
    class AXR_API HSSSwitchFunction : public HSSFunction
    {
    public:
        static HSSComparisonType comparisonTypeForTokenType(HSSTokenType tokenType);
        
        /**
         *  Creates a new instance of an if function.
         */
        HSSSwitchFunction(AXRController * controller);
        
        /**
         *  Copy constructor for HSSSwitchFunction objects. Do not call directly, use clone() instead.
         */
        HSSSwitchFunction(const HSSSwitchFunction & orig);
        
        /**
         *  Clones an instance of HSSSwitchFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSSwitchFunction
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSSwitchFunction();
        
        virtual AXRString toString();
        
        void addValue(QSharedPointer<HSSParserNode> value);
        void addCase(QSharedPointer<HSSCase> newCase);

        bool evaluateEvaluables();
        QSharedPointer<HSSParserNode> evaluateCondition();
        
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);

    protected:
        HSSComparisonType _comparisonType;
        QSharedPointer<HSSParserNode> _condition;
        std::vector<QSharedPointer<HSSCase> > _cases;

        QSharedPointer<HSSParserNode> _evaluateCondition(QSharedPointer<HSSParserNode> parserNode);
        
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

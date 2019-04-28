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

#ifndef HSSEVALUABLEFUNCTION_H
#define HSSEVALUABLEFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    /**
     *  @brief Implementation of functions that have blocks
     */
    class AXR_API HSSEvaluableFunction : public HSSFunction
    {
    public:
        /**
         *  Destructor for this class.
         */
        virtual ~HSSEvaluableFunction();
        
        virtual AXRString toString();
        
        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        /**
         *  Evaluate all the evaluables in the block of this function
         *  @return Wether or not the current loop needs to be stopped.
         */
        bool evaluateEvaluables();
        
        void addEvaluable(QSharedPointer<HSSParserNode> parserNode);
        const std::vector<QSharedPointer<HSSParserNode> > getEvaluables();

        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);
        void cloneEvaluables(QSharedPointer<HSSEvaluableFunction> clone) const;

        void setLocalVariable(AXRString name, QSharedPointer<HSSObject> obj);
        QSharedPointer<HSSObject> getLocalVariable(AXRString name);
        QSharedPointer<HSSObject> getVariable(HSSString name);
        bool hasLocalVariable(AXR::HSSString name);

    protected:
        /**
         *  Creates a new instance of this class
         *  @param controller   A pointer to the controller.
         */
        HSSEvaluableFunction(HSSFunctionType type, AXRController * controller);
        
        /**
         *  Copy constructor for HSSEvaluableFunction objects. Do not call directly, use clone() on
         *  on of the subclasses instead.
         */
        HSSEvaluableFunction(const HSSEvaluableFunction & orig);

        std::vector<QSharedPointer<HSSParserNode> > _evaluables;
        std::map<AXRString, QSharedPointer<HSSObject> > _locals;
    };
}

#endif

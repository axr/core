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

#ifndef HSSFUNCTIONCALL_H
#define HSSFUNCTIONCALL_H

#include "HSSFunction.h"

namespace AXR
{
    class HSSArgument;
    class HSSRefFunction;

    /**
     *  @brief Implementation of a call to a user defined function in HSS.
     */
    class AXR_API HSSFunctionCall : public HSSFunction
    {
    public:
        /**
         *  Creates a new instance of an this class.
         */
        HSSFunctionCall(AXRController * controller);
        
        /**
         *  Copy constructor for this class. Do not call directly, use clone() instead.
         */
        HSSFunctionCall(const HSSFunctionCall & orig);
        
        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSFunctionCall();
        
        virtual AXRString toString();
        virtual AXRString stringRep();
        
        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        /**
         *  Each function overrides this method to provide the actual implementation
         *  of what the function does.
         */
        virtual QSharedPointer<HSSObject> _evaluate();

        std::vector<QSharedPointer<HSSArgument> > getArguments();
        void setArguments(std::vector<QSharedPointer<HSSArgument> > arguments);
        void addArgument(QSharedPointer<HSSArgument> argument);

        void setSubject(QSharedPointer<HSSParserNode> parserNode);
        const QSharedPointer<HSSParserNode> & subject() const;

        void fire();

        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);
        
        QSharedPointer<HSSObject> evaluate(QSharedPointer<HSSObject> theObj);

    protected:
        QSharedPointer<HSSSimpleSelection> scope;

    private:
        HSSFunctionType functionType;
        std::vector<QSharedPointer<HSSArgument> > _arguments;
        QSharedPointer<HSSParserNode> _subject;

        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        QSharedPointer<HSSObject> __evaluate(QSharedPointer<HSSParserNode> parserNode);
    };
}

#endif

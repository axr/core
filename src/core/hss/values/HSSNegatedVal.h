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

#ifndef HSSNEGATEDVAL_H
#define HSSNEGATEDVAL_H

#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief A value that has been neegated with ! in front of it.
     */
    class AXR_API HSSNegatedVal : public HSSParserNode
    {
    public:
        /**
         *  Creates a new negated value.
         *  @param value A parser node containing the negated value.
         */
        HSSNegatedVal(AXRController * controller);
        
        /**
         *  Copy constructor for this class
         */
        HSSNegatedVal(const HSSNegatedVal & orig);
        
        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        QSharedPointer<HSSNegatedVal> clone() const;
        
        /**
         *  Destructor
         */
        virtual ~HSSNegatedVal();
        
        virtual AXRString toString();
        virtual AXRString stringRep();
        
        /**
         *  Each node overrides this method to compare against another node.
         *  @param otherNode    The other object to compare to.
         *  @return Wether the node is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        /**
         *  Setter for the node value.
         *  @param value A parser node containing the node value.
         */
        void setValue(QSharedPointer<HSSParserNode> value);
        
        /**
         *  Getter for the node value.
         *  @return A parser node containing the node value.
         */
        const QSharedPointer<HSSParserNode> & getValue() const;
        
        QSharedPointer<HSSParserNode> evaluate();
        
        void setThisObj(QSharedPointer<AXR::HSSDisplayObject> value);
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        
    private:
        bool _hasName;
        QSharedPointer<HSSParserNode> _value;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        QSharedPointer<HSSParserNode> _evaluate(QSharedPointer<HSSParserNode> parserNode);
    };
}

#endif

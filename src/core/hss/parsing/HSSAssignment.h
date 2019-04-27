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

#ifndef HSSASSIGNMENT_H
#define HSSASSIGNMENT_H

#include "HSSStatement.h"

namespace AXR
{
    class HSSPropertyPath;
    
    /**
     *  @brief A property path contains multiple property path nodes.
     */
    class AXR_API HSSAssignment : public HSSStatement
    {
    public:
        /**
         *  Creates a new instance of this class
         */
        HSSAssignment(AXRController * controller);
        
        /**
         *  Copy constructor for this class
         */
        HSSAssignment(const HSSAssignment & orig);
        
        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        QSharedPointer<HSSAssignment> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSAssignment();
        
        virtual AXRString toString();
        
        QSharedPointer<HSSObject> evaluate();
        QSharedPointer<HSSObject> evaluate(QSharedPointer<HSSParserNode> node);
        
        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        void setPropertyPath(QSharedPointer<HSSPropertyPath> newPath);
        QSharedPointer<HSSPropertyPath> propertyPath();
        void setValue(QSharedPointer<HSSParserNode> parserNode);
        QSharedPointer<HSSParserNode> value();
        void addValue(QSharedPointer<AXR::HSSParserNode> value);
        
        
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);
        
    private:
        QSharedPointer<HSSPropertyPath> _propertyPath;
        QSharedPointer<HSSParserNode> _value;
        
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

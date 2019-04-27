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

#ifndef HSSTONUMFUNCTION_H
#define HSSTONUMFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    /**
     *  @brief Implementation of a call to a user defined function in HSS.
     */
    class AXR_API HSSToNumFunction : public HSSFunction
    {
    public:
        /**
         *  Creates a new instance of an this class.
         */
        HSSToNumFunction(AXRController * controller);
        
        /**
         *  Copy constructor for this class. Do not call directly, use clone() instead.
         */
        HSSToNumFunction(const HSSToNumFunction & orig);
        
        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSToNumFunction();
        
        virtual AXRString toString();
        
        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        HSSUnit evaluate();
        
        void addValue(QSharedPointer<HSSParserNode> parserNode);
        void setThisObj(QSharedPointer<HSSDisplayObject> newThis);
        void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        
    private:
        HSSUnit _evaluateNode(QSharedPointer<HSSParserNode> node);
        QSharedPointer<HSSParserNode> _node;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

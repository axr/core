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

#ifndef HSSCUSTOMFUNCTION_H
#define HSSCUSTOMFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    class HSSSimpleSelection;
    
    /**
     *  @brief Represents custom functions defined by the hosting app.
     */
    class AXR_API HSSCustomFunction : public HSSFunction
    {
    public:
        /**
         *  Creates a new instance of this class.
         */
        HSSCustomFunction(AXRController * controller);
        
        /**
         *  Copy constructor for this class. Do not call directly, use clone() instead.
         */
        HSSCustomFunction(const HSSCustomFunction & orig);
        
        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        virtual QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSCustomFunction();
        
        virtual AXRString toString();
        
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
        
        /**
         *  Setter for the scope which to pass to members like references or selections.
         *  @param newScope     The new scope, a shared pointer to a simple selection.
         */
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        
        /**
         *  Setter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @param value        A shared pointer to the nearest display object.
         */
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);

        std::vector<QSharedPointer<HSSArgument> > getArguments();
        void setArguments(std::vector<QSharedPointer<HSSArgument> > arguments);
        void addArgument(QSharedPointer<HSSArgument> argument);
        
    private:
        std::vector<QSharedPointer<HSSArgument> > _arguments;
        
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

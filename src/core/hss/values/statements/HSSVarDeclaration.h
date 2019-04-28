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

#ifndef HSSVARDECLARATION_H
#define HSSVARDECLARATION_H

#include "HSSStatement.h"

namespace AXR
{
    class HSSAssignment;

    /**
     *  @brief Makes a global or local variable
     */
    class AXR_API HSSVarDeclaration : public HSSStatement
    {
    public:
        /**
         *  Creates a new instance of an object definition, storing the given object, to which
         *   the properties will be applied by calling the apply() method.
         */
        HSSVarDeclaration(HSSString name, AXRController * controller);
        
        /**
         *  Copy constructor for this class. Do not call directly, use clone() instead.
         */
        HSSVarDeclaration(const HSSVarDeclaration & orig);
        
        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        QSharedPointer<HSSVarDeclaration> clone() const;
        
        /**
         *  Destructor for this class
         */
        ~HSSVarDeclaration();
        
        virtual HSSString toString();
        
        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        /**
         *  Setter for the scope which to pass to members like references or selections.
         *  @param newScope     The new scope, a shared pointer to a simple selection.
         */
        void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        
        /**
         *  Setter for the "\@this object" which to pass to members like references or selections.
         *  @param value    A shared pointer to a display object representing the \@this object.
         */
        void setThisObj(QSharedPointer<HSSDisplayObject> value);
        
        /**
         *  Getter for the "\@this object" which to pass to members like references or selections.
         *  @return A shared pointer to a display object representing the \@this object.
         */
        QSharedPointer<HSSDisplayObject> getThisObj();

        void setName(HSSString newName);
        const HSSString name() const;
        
        void setAssignment(QSharedPointer<HSSAssignment> value);
        QSharedPointer<HSSAssignment> assignment() const;
        
    protected:
        QSharedPointer<HSSVarDeclaration> shared_from_this();
        
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;

        HSSString _name;
        QSharedPointer<HSSAssignment> _assignment;
    };
}

#endif

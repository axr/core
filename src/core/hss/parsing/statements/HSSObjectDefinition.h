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

#ifndef HSSOBJECTDEFINITION_H
#define HSSOBJECTDEFINITION_H

#include "HSSStatement.h"

namespace AXR
{
    class HSSObject;
    class HSSPropertyDefinition;
    class HSSRule;
    class HSSSimpleSelection;

    /**
     *  @brief Object definitions in HSS are represented by this class.
     *
     *  Object definitions hold property definitions, rules and other object definitions.
     */
    class AXR_API HSSObjectDefinition : public HSSStatement
    {
    public:
        /**
         *  Creates a new instance of an object definition, storing the given object, to which
         *   the properties will be applied by calling the apply() method.
         */
        HSSObjectDefinition(QSharedPointer<HSSObject> prototype, AXRController * controller);

        /**
         *  Copy constructor for HSSObjectDefinition. Do not call directly, use clone() instead.
         */
        HSSObjectDefinition(const HSSObjectDefinition & orig);

        /**
         *  Clones an instance of HSSObjectDefinition and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSObjectDefinition
         */
        QSharedPointer<HSSObjectDefinition> clone() const;

        /**
         *  Destructor for this class
         */
        ~HSSObjectDefinition();

        virtual AXRString toString();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        void applyStack();
        void applyRules();

        /**
         *  Adds a new property definition to the end of the properties list.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesAdd(QSharedPointer<HSSPropertyDefinition> &newProperty);

        /**
         *  Adds a new property definition to the end of the properties list, const version.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesAdd(const QSharedPointer<HSSPropertyDefinition> &newProperty);

        /**
         *  Adds a new property definition to the beginning of the properties list.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesPrepend(QSharedPointer<HSSPropertyDefinition> &newProperty);

        /**
         *  Adds a new property definition to the beginning of the properties list, const version.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesPrepend(const QSharedPointer<HSSPropertyDefinition> &newProperty);

        /**
         *  Removes last statement from the list and then deletes it.
         */
        void propertiesRemoveLast();

        /**
         *  @return A shared pointer to the last statement in the list, passed by reference [why?].
         */
        QSharedPointer<HSSPropertyDefinition> &propertiesLast();

        /**
         *  @return How many statements there are in the properties list.
         */
        size_t propertiesSize() const;

        /**
         *  @return All the properties in the list, contained in a double ended queue holding shared
         *  pointers to property definitions.
         */
        std::deque<QSharedPointer<HSSPropertyDefinition> > getProperties();

        /**
         *  Adds a new object definition to the end of the children list.
         *  @param child    A shared pointer (passed by reference [why?]) to an object definition.
         */
        void childrenAdd(QSharedPointer<HSSObjectDefinition> &child);

        /**
         *  Adds a new object definition to the end of the children list, const version.
         *  @param child    A shared pointer (passed by reference [why?]) to an object definition.
         */
        void childrenAdd(const QSharedPointer<HSSObjectDefinition> &child);

        /**
         *  Removes last statement from the list and then deletes it.
         */
        void childrenRemoveLast();

        /**
         *  @return A shared pointer (passed by reference [why?]) to the last statement in the list.
         */
        QSharedPointer<HSSObjectDefinition> &childrenLast();

        /**
         *  @return How many statements there are in the children list.
         */
        size_t childrenSize() const;

        /**
         *  @return A vector of shared pointers to object defnitions, the children.
         */
        const std::vector<QSharedPointer<HSSObjectDefinition> > getChildren() const;

        /**
         *  @return A shared pointer to the object that corresponds to the object type of the
         *  object definition. Call apply() before using.
         */
        QSharedPointer<HSSObject> getObject();

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

        /**
         *  Setter for the list of rules. Replaces the whole list with the given one.
         *  @param newRules     A double ended queue holding shared pointers to rules.
         */
        void setRules(std::deque<QSharedPointer<HSSRule> > newRules);

        /**
         *  Getter for the list of rules.
         *  @return A double ended queue holding shared pointers to rules.
         *  @warning The returned deque is read-only.
         */
        const std::deque<QSharedPointer<HSSRule> > getRules() const;

        /**
         *  Add a rule to the end of the list of rules.
         *  @param rule     A shared pointer to a rule, to be added to the list of rules.
         */
        void rulesAdd(QSharedPointer<HSSRule> rule);

        /**
         *  Add a rule to the beginning of the list of rules.
         *  @param rule     A shared pointer to a rule, to be added to the list of rules.
         */
        void rulesPrepend(QSharedPointer<HSSRule> rule);

        /**
         *  Remove a rule from the list of rules.
         *  @param rule     A shared pointer to a rule, to be removed from the list of rules.
         */
        void rulesRemove(QSharedPointer<HSSRule> rule);

    protected:
        QSharedPointer<HSSObjectDefinition> shared_from_this();

        std::deque<QSharedPointer<HSSPropertyDefinition> > properties;
        std::vector<QSharedPointer<HSSObjectDefinition> > children;

        QSharedPointer<HSSDisplayObject> thisObj;
        QSharedPointer<HSSSimpleSelection> scope;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        QSharedPointer<HSSObject> prototype;
        std::deque<QSharedPointer<HSSRule> > _rules;
    };
}

#endif

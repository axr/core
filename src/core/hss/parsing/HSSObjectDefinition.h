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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
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

#include <deque>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "HSSDisplayObject.h"
#include "HSSObject.h"
#include "HSSParserNodes.h"
#include "HSSRule.h"
#include "HSSStatement.h"

namespace AXR
{
    /**
     *  @brief Object definitions in HSS are represented by this class.
     *
     *  Object definitions hold property definitions, rules and other object definitions.
     */
    class AXR_API HSSObjectDefinition : public HSSStatement
    {
    public:
        typedef boost::shared_ptr<HSSObjectDefinition> p;

        /**
         *  Creates a new instance of an object definition, storing the given object, to which
         *   the properties will be applied by calling the apply() method.
         */
        HSSObjectDefinition(HSSObject::p prototype);

        /**
         *  Copy constructor for HSSObjectDefinition. Do not call directly, use clone() instead.
         */
        HSSObjectDefinition(const HSSObjectDefinition & orig);

        /**
         *  Clones an instance of HSSObjectDefinition and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSObjectDefinition
         */
        p clone() const;

        /**
         *  Destructor for this class
         */
        ~HSSObjectDefinition();

        virtual std::string toString();

        /**
         *  Call this to apply the property definitions to the prototype object. You can
         *  call getObject() afterwards, to access the object itself.
         */
        void apply();

        /**
         *  Adds a new property definition to the end of the properties list.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesAdd(HSSPropertyDefinition::p &newProperty);

        /**
         *  Adds a new property definition to the end of the properties list, const version.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesAdd(const HSSPropertyDefinition::p &newProperty);

        /**
         *  Adds a new property definition to the beginning of the properties list.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesPrepend(HSSPropertyDefinition::p &newProperty);

        /**
         *  Adds a new property definition to the beginning of the properties list, const version.
         *  @param newProperty      A shared pointer (passed by reference [why?]) to the new
         *                          property definition to be added.
         */
        void propertiesPrepend(const HSSPropertyDefinition::p &newProperty);

        /**
         *  Removes last statement from the list and then deletes it.
         */
        void propertiesRemoveLast();

        /**
         *  @return A shared pointer to the last statement in the list, passed by reference [why?].
         */
        HSSPropertyDefinition::p &propertiesLast();

        /**
         *  @return How many statements there are in the properties list.
         */
        int propertiesSize();

        /**
         *  @return All the properties in the list, contained in a double ended queue holding shared
         *  pointers to property definitions.
         */
        std::deque<HSSPropertyDefinition::p> getProperties();

        /**
         *  Adds a new object definition to the end of the children list.
         *  @param child    A shared pointer (passed by reference [why?]) to an object definition.
         */
        void childrenAdd(HSSObjectDefinition::p &child);

        /**
         *  Adds a new object definition to the end of the children list, const version.
         *  @param child    A shared pointer (passed by reference [why?]) to an object definition.
         */
        void childrenAdd(const HSSObjectDefinition::p &child);

        /**
         *  Removes last statement from the list and then deletes it.
         */
        void childrenRemoveLast();

        /**
         *  @return A shared pointer (passed by reference [why?]) to the last statement in the list.
         */
        HSSObjectDefinition::p &childrenLast();

        /**
         *  @return How many statements there are in the children list.
         */
        int childrenSize();

        /**
         *  @return A vector of shared pointers to object defnitions, the children.
         */
        const std::vector<HSSObjectDefinition::p> getChildren() const;

        /**
         *  @return A shared pointer to the object that corresponds to the object type of the
         *  object definition. Call apply() before using.
         */
        HSSObject::p getObject();

        /**
         *  Setter for the scope which to pass to members like references or selections.
         *  @param newScope     The new scope, a regular pointer to a vector of shared poninters
         *  to display obects.
         *
         *  @todo how is memory handled for the scopes?
         */
        void setScope(const std::vector<HSSDisplayObject::p> * newScope);

        /**
         *  Setter for the "\@this object" which to pass to members like references or selections.
         *  @param value    A shared pointer to a display object representing the \@this object.
         */
        void setThisObj(HSSDisplayObject::p value);

        /**
         *  Getter for the "\@this object" which to pass to members like references or selections.
         *  @return A shared pointer to a display object representing the \@this object.
         */
        HSSDisplayObject::p getThisObj();

        /**
         *  Setter for the list of rules. Replaces the whole list with the given one.
         *  @param newRules     A double ended queue holding shared pointers to rules.
         */
        void setRules(std::deque<HSSRule::p> newRules);

        /**
         *  Getter for the list of rules.
         *  @return A double ended queue holding shared pointers to rules.
         *  @warning The returned deque is read-only.
         */
        const std::deque<HSSRule::p> getRules() const;

        /**
         *  Add a rule to the end of the list of rules.
         *  @param rule     A shared pointer to a rule, to be added to the list of rules.
         */
        void rulesAdd(HSSRule::p rule);

        /**
         *  Add a rule to the beginning of the list of rules.
         *  @param rule     A shared pointer to a rule, to be added to the list of rules.
         */
        void rulesPrepend(HSSRule::p rule);

        /**
         *  Remove a rule from the list of rules.
         *  @param rule     A shared pointer to a rule, to be removed from the list of rules.
         */
        void rulesRemove(HSSRule::p rule);

    protected:
        HSSObjectDefinition::p shared_from_this();

        std::deque<HSSPropertyDefinition::p> properties;
        std::vector<HSSObjectDefinition::p> children;


        HSSDisplayObject::p thisObj;
        const std::vector<HSSDisplayObject::p> * scope;

    private:
        virtual HSSClonable::p cloneImpl() const;
        HSSObject::p prototype;
        std::deque<HSSRule::p> _rules;
    };
}

#endif

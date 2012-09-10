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

#ifndef HSSRULE_H
#define HSSRULE_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "HSSFilterType.h"
#include "HSSInstruction.h"
#include "HSSPropertyDefinition.h"
#include "HSSSelectorChain.h"
#include "HSSStatement.h"

namespace AXR
{

    class HSSDisplayObject;
    class HSSFilter;

    class HSSRule;

    /**
     *  @brief This class encapsulates a relation between a rule a its state.
     *
     *  Display objects use these to determine if the rules they hold pointers to
     *  are to be applied or not.
     */
    class HSSRuleStatus
    {
    public:

        /**
         *  The shared pointer to this class.
         */
        typedef boost::shared_ptr<HSSRuleStatus> p;

        /**
         *  The current state the rule.
         */
        HSSRuleState state;

        /**
         *  A shared pointer to the rule.
         */
        boost::shared_ptr<HSSRule> rule;
    };

    /**
     *  @brief Rules are made up of selector chain + block, which can contain property definitions and
     *  other rules.
     *
     *  Rules are read by the parser, then applied on display objects, where the property definitions
     *  are used to determine how the elements are going to be displayed on the page.
     */
    class HSSRule : public HSSStatement
    {
    public:
        typedef boost::shared_ptr<HSSRule> p;
        typedef std::deque<p>::iterator it;
        typedef std::deque<p>::const_iterator const_it;

        /**
         *  Creates a new instance of a request action.
         */
        HSSRule();

        /**
         *  Copy constructor for HSSRequest objects. Do not call directly,
         *  use clone() instead.
         */
        HSSRule(const HSSRule &orig);

        /**
         *  Clones an instance of HSSRequest and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRequest
         */
        p clone() const;

        /**
         *  Destructor for this class.
         */
        ~HSSRule();

        std::string toString();

        /**
         *  Setter fo the selector chains, which are used to select elements from the
         *  content tree.
         *  @param newChain     A vector of shared pointers to selector chains.
         */
        void setSelectorChains(std::vector<HSSSelectorChain::p> newChains);

        /**
         *  Getter fo the selector chains, which are used to select elements from the
         *  content tree.
         *  @return A vector of shared pointers to selector chains.
         */
        const std::vector<HSSSelectorChain::p> & getSelectorChains() const;

        /**
         *  Add a selector chain to the selector chains vector.
         *  @param newValue A shared pointer to the selector chain to be added.
         */
        void selectorChainsAdd(HSSSelectorChain::p & newSelectorChain);

        /**
         *  Get a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain.
         *  @return A shared pointer to the element at that index.
         */
        HSSSelectorChain::p &selectorChainsGet(unsigned index);

        /**
         *  Removes a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain to be deleted.
         */
        void selectorChainsRemove(unsigned index);

        /**
         *  Removes the last element in the selector chains vector.
         */
        void selectorChainsRemoveLast();

        /**
         *  @return the last element of the selector chains vector.
         */
        HSSSelectorChain::p &selectorChainsLast();

        /**
         *  @return the size of the selector chains vector
         */
        const int selectorChainsSize();

        /**
         *  @todo Add setter for properties vector.
         *  Setter fo the property definitions that this rules holds.
         *  //@param newChain     A vector of shared pointers to property definitions.
         */

        /**
         *  Getter fo the property definitions vector.
         *  @return A vector of shared pointers to property definitions.
         */
        const std::vector<HSSPropertyDefinition::p> & getProperties() const;

        /**
         *  Add a property definition to the properties vector.
         *  @param newProperty  A shared pointer to the property definition to be added.
         */
        void propertiesAdd(HSSPropertyDefinition::p & newProperty);

        /**
         *  Get a property definition by index.
         *  @param index    An unsigned integer with the index of the property definition.
         *  @return A shared pointer to the element at that index.
         */
        HSSPropertyDefinition::p &propertiesGet(unsigned index);

        /**
         *  Removes a property definition by index.
         *  @param index    An unsigned integer with the index of the property definition to be deleted.
         */
        void propertiesRemove(unsigned index);

        /**
         *  Removes the last element in the properties vector.
         */
        void propertiesRemoveLast();

        /**
         *  @return the last element of the properties vector.
         */
        HSSPropertyDefinition::p &propertiesLast();

        /**
         *  @return the size of the properties vector
         */
        const unsigned int propertiesSize();

        /**
         *  Add a child rule.
         *  @param newProperty  A shared pointer to the rule to be added.
         */
        void childrenAdd(HSSRule::p newRule);

        /**
         *  Get a child rule by index.
         *  @param index    An unsigned integer with the index of the child rule.
         *  @return A shared pointer to the element at that index.
         */
        HSSRule::p childrenGet(unsigned index);

        /**
         *  Removes a child rule by index.
         *  @param index    An unsigned integer with the index of the child rule to be deleted.
         */
        void childrenRemove(unsigned index);

        /**
         *  Removes the last element in the children vector.
         */
        void childrenRemoveLast();

        /**
         *  @return the size of the children vector
         */
        const int childrenSize();

        void setInstruction(HSSInstruction::p newInstruction);

        HSSInstruction::p getInstruction();

        virtual void setThisObj(boost::shared_ptr<HSSDisplayObject> value);

        void treeChanged(HSSObservableProperty source, void*data);

        bool getActiveByDefault();

        void setActiveByDefault(bool newValue);

        const std::vector<boost::weak_ptr<HSSDisplayObject> > getAppliedTo() const;

        void setAppliedTo(std::vector<boost::weak_ptr<HSSDisplayObject> > newObjects);

        void appliedToAdd(boost::shared_ptr<HSSDisplayObject> displayObject);

        const std::vector<boost::shared_ptr<HSSDisplayObject> > getOriginalScope() const;

        void setOriginalScope(const std::vector<boost::shared_ptr<HSSDisplayObject> > & scope);

        void setObservedTreeChanger(HSSObservable * newValue);

        HSSObservable * getObservedTreeChanger();

    protected:
        HSSRule::p shared_from_this();

        std::vector<HSSPropertyDefinition::p> properties;
        std::vector<HSSRule::p>children;
        HSSInstruction::p instruction;

        bool _activeByDefault;

        std::vector<boost::weak_ptr<HSSDisplayObject> > appliedTo;

        std::vector<boost::shared_ptr<HSSDisplayObject> > _originalScope;

    private:
        virtual HSSClonable::p cloneImpl() const;

        HSSObservable * observedTreeChanger;

        std::vector<HSSSelectorChain::p> selectorChains;
    };
}

#endif

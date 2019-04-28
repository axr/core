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

#ifndef HSSRULE_H
#define HSSRULE_H

#include "HSSStatement.h"

namespace AXR
{
    class HSSDisplayObject;
    class HSSFilter;
    class HSSInstruction;
    class HSSPropertyDefinition;
    class HSSRule;
    class HSSSelector;
    class HSSSelectorChain;

    /**
     *  @brief This class encapsulates a relation between a rule a its state.
     *
     *  Display objects use these to determine if the rules they hold pointers to
     *  are to be applied or not.
     */
    class AXR_API HSSRuleStatus
    {
    public:
        /**
         *  The shared pointer to this class.
         */
        typedef QSharedPointer<HSSRuleStatus> p;

        /**
         *  The current state the rule.
         */
        HSSRuleState state;

        /**
         *  A shared pointer to the rule.
         */
        QSharedPointer<HSSRule> rule;
    };

    /**
     *  @brief Rules are made up of selector chain + block, which can contain property definitions and
     *  other rules.
     *
     *  Rules are read by the parser, then applied on display objects, where the property definitions
     *  are used to determine how the elements are going to be displayed on the page.
     */
    class AXR_API HSSRule : public HSSStatement
    {
    public:
        typedef std::deque<QSharedPointer<HSSRule> >::iterator it;
        typedef std::deque<QSharedPointer<HSSRule> >::const_iterator const_it;

        /**
         *  Creates a new instance of a request action.
         */
        HSSRule(AXRController * controller);

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
        QSharedPointer<HSSRule> clone() const;

        /**
         *  Destructor for this class.
         */
        ~HSSRule();

        AXRString toString();

        /**
         *  Setter fo the selector chains, which are used to select elements from the
         *  content tree.
         *  @param newChains A vector of shared pointers to selector chains.
         */
        void setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > newChains);

        /**
         *  Getter fo the selector chains, which are used to select elements from the
         *  content tree.
         *  @return A vector of shared pointers to selector chains.
         */
        const std::vector<QSharedPointer<HSSSelectorChain> > & getSelectorChains() const;

        /**
         *  Add a selector chain to the selector chains vector.
         *  @param newSelectorChain A shared pointer to the selector chain to be added.
         */
        void selectorChainsAdd(QSharedPointer<HSSSelectorChain> & newSelectorChain);

        /**
         *  Get a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain.
         *  @return A shared pointer to the element at that index.
         */
        QSharedPointer<HSSSelectorChain> &selectorChainsGet(size_t index);

        /**
         *  Removes a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain to be deleted.
         */
        void selectorChainsRemove(off_t index);

        /**
         *  Removes the last element in the selector chains vector.
         */
        void selectorChainsRemoveLast();

        /**
         *  @return the last element of the selector chains vector.
         */
        QSharedPointer<HSSSelectorChain> &selectorChainsLast();

        /**
         *  @return the size of the selector chains vector
         */
        size_t selectorChainsSize() const;

        /*
         *  @todo Add setter for properties vector.
         *  Setter for the property definitions that this rules holds.
         *  @param newChain A vector of shared pointers to property definitions.
         */

        /**
         *  Getter for the property definitions vector.
         *  @return A vector of shared pointers to property definitions.
         */
       const std::vector<QSharedPointer<HSSPropertyDefinition> > & getProperties() const;

        /**
         *  Add a property definition to the properties vector.
         *  @param newProperty  A shared pointer to the property definition to be added.
         */
        void propertiesAdd(QSharedPointer<HSSPropertyDefinition> & newProperty);

        /**
         *  Get a property definition by index.
         *  @param index    An unsigned integer with the index of the property definition.
         *  @return A shared pointer to the element at that index.
         */
        QSharedPointer<HSSPropertyDefinition> &propertiesGet(size_t index);

        /**
         *  Removes a property definition by index.
         *  @param index    An unsigned integer with the index of the property definition to be deleted.
         */
        void propertiesRemove(off_t index);

        /**
         *  Removes the last element in the properties vector.
         */
        void propertiesRemoveLast();

        /**
         *  @return the last element of the properties vector.
         */
        QSharedPointer<HSSPropertyDefinition> &propertiesLast();

        /**
         *  @return the size of the properties vector
         */
        size_t propertiesSize() const;

        /**
         *  Add a child rule.
         *  @param newRule A shared pointer to the rule to be added.
         */
        void childrenAdd(QSharedPointer<HSSRule> newRule);

        /**
         *  Add a child rule to the front of the rules list
         *  @param newRule A shared pointer to the rule to be prepended.
         */
        void childrenPrepend(QSharedPointer<HSSRule> newRule);

        /**
         *  Add a child rule to the front of the rules list, but only if hasn't been
         *  added before already.
         *  @param newRule A shared pointer to the rule to be prepended.
         */
        void childrenPrependOnce(QSharedPointer<HSSRule> newRule);

        /**
         *  Get a child rule by index.
         *  @param index    An unsigned integer with the index of the child rule.
         *  @return A shared pointer to the element at that index.
         */
        QSharedPointer<HSSRule> childrenGet(size_t index);

        /**
         *  Removes a child rule by index.
         *  @param index    An unsigned integer with the index of the child rule to be deleted.
         */
        void childrenRemove(off_t index);

        /**
         *  Removes the last element in the children vector.
         */
        void childrenRemoveLast();

        /**
         *  @return the size of the children vector
         */
        size_t childrenSize() const;

        void setInstruction(QSharedPointer<HSSInstruction> newInstruction);

        QSharedPointer<HSSInstruction> getInstruction();

        virtual void setThisObj(QSharedPointer<HSSDisplayObject> value);

        void treeChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj);

        std::vector<QWeakPointer<HSSDisplayObject> > getAppliedTo() const;

        void setAppliedTo(std::vector<QWeakPointer<HSSDisplayObject> > newObjects);

        void appliedToAdd(QSharedPointer<HSSDisplayObject> displayObject);

        QSharedPointer<HSSSimpleSelection> getOriginalScope() const;

        void setOriginalScope(QSharedPointer<HSSSimpleSelection> scope);

        void addOriginalScope(QSharedPointer<HSSSimpleSelection> scope);
        
        void contentTreeChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj);

        bool hasParent();
        QSharedPointer<HSSRule> getParent();

        bool clonedFromSameRule(const QSharedPointer<HSSRule> & otherRule) const;

        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        void removeFromDisplayObjects();
        void removeFromDisplayObject(QSharedPointer<HSSDisplayObject> theDO);

        void setSpecificity(HSSUnit newValue);

        void setIsConditional(const bool newValue);
        const bool isConditional();
        void recursiveSetIsConditional(const bool newValue);

        void fastForwardSelectorChain(HSSSelector * theSelector);
        void resetFastForwardSelectorChain();
        

    protected:
        QSharedPointer<HSSRule> shared_from_this();

        std::vector<QSharedPointer<HSSPropertyDefinition> > properties;
        std::deque<QSharedPointer<HSSRule> >children;
        QSharedPointer<HSSInstruction> instruction;

        std::vector<QWeakPointer<HSSDisplayObject> > appliedTo;

        QSharedPointer<HSSSimpleSelection> _originalScope;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;

        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
        const HSSRule * _clonedFromRule;
        bool _isConditional;
    };
}

#endif

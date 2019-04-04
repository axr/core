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

#ifndef AXRCONTROLLER_H
#define AXRCONTROLLER_H

#include "AXRGlobal.h"
#include "HSSTypeEnums.h"

template <class T> class QSharedPointer;

namespace AXR
{
    class HSSVisitorManager;
    class AXRControllerPrivate;
    class AXRDocument;
    class HSSContainer;
    class HSSDisplayObject;
    class HSSEvent;
    class HSSInstruction;
    class HSSObjectDefinition;
    class HSSObject;
    class HSSParserNode;
    class HSSRule;
    class HSSSelection;
    class HSSSelectorChain;
    class HSSSimpleSelection;

    /**
     *  @brief  This class holds trees of objects, sheets, rules and the parser tree. Also,
     *  it has methods to select object in those trees.
     */
    class AXR_API AXRController
    {
    public:
        /**
         *  Creates a new instance of a controller.
         *
         */
        AXRController(AXRDocument *document);

        /**
         *  Creates a new instance of a controller, without a document
         *
         */
        AXRController();

        /**
         *  Destructs the controller and clears all the trees.
         */
        virtual ~AXRController();

        AXRDocument* document() const;
        void setDocument(AXRDocument * document) const;

        void setUpTreeChangeObservers();
        void recursiveSetUpTreeChangeObservers(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool subscribingToParent);
        void matchRulesToContentTree();
        void activateRules();
        void recursiveSetRuleState(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, HSSRuleState state);
        void setRuleStateOnSelection(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSimpleSelection> selection, HSSRuleState state);

        /**
         *  @return A textual representation of itself, as a string.
         */
        AXRString toString() const;

        /**
         *  Use this to clean up and start from fresh (e.g. when reloading a file).
         */
        void reset();

        /**
         *  @return A shared pointer to the root element.
         */
        QSharedPointer<HSSContainer>& root() const;

        /**
         *  Sets the element you pass in as the new root element.
         *  If the current object context is empty, it sets it automatically to the new root.
         *
         *  @param newRoot A shared pointer to the new element
         */
        void setRoot(QSharedPointer<HSSContainer> newRoot);

        /**
         *  This method is called by the XML parser when it reads an opening tag.
         *
         *  @param elementName  A string containing the name of the element we just entered.
         */
        void enterElement(const AXRString &elementName);

        /**
         *  This method is called by the XML parser when it reads an attribute of the current element.
         *
         *  @param  name    A string containing the name of the attribute.
         *  @param  value   A string containing the value of the attribute.
         */
        void addAttribute(const AXRString &name, const AXRString &value);

        /**
         *  This method is called by the XML parser when it reads the content text of the
         *  current element.
         *
         *  @param  text    A string containing the content text for the current element.
         */
        void setContentText(const AXRString &text);

        /**
         *  When content text is read in chunks, this allows to append the text to the
         *  current element.
         *
         *  @param text     A string containing content text for the current element.
         */
        void appendContentText(const AXRString &text);

        /**
         *  This method is called by the XML parser when it reads a closing tag.
         *
         */
        void exitElement();

        /**
         *  Add a display object as a children of the current context, or as root if there is none.
         *
         *  @param newContainer A shared pointer to a display object.
         *
         *  @todo change name to better reflect the type of the object
         */
        void add(QSharedPointer<HSSDisplayObject> newContainer);

        /**
         *  Add a display object as an offscreen children of the current context.
         *
         *  @param newContainer A shared pointer to a display object.
         */
        void addOffscreen(QSharedPointer<HSSDisplayObject> newContainer);

        /**
         *  Append an object definition to the object tree.
         *
         *  @param newObject A shared pointer to an object definition.
         */
        void addObjectTreeNode(const QSharedPointer<HSSObjectDefinition> &newObject);

        /**
         *  Remove the object at given index from the object tree.
         *
         *  @param index    An unsigned int containing the index of the object to be removed.
         */
        void removeObjectTreeNodeAt(int index);

        /**
         *  Returns an object from the object tree by index.
         *
         *  @param index    An unsigned int containing the index of the object to be returned.
         *  @return         The object definitionat the index. May be an empty pointer if not found.
         */
        QSharedPointer<HSSObjectDefinition> objectTreeNodeAt(int index);

        /**
         *  Returns an object from the object tree by name.
         *
         *  @param name     A string containing the name of the object to be returned.
         *  @return         The object definitionat with that name. May be an empty pointer if not found.
         */
        QSharedPointer<HSSObjectDefinition> objectTreeNodeNamed(const AXRString &name);

        /**
         *  Adds an entry in the list of sheets to be loaded.
         *
         *  @param sheet    A string containing the file name of the stylesheet
         */
        void addStyleSheetUrl(const AXRString &url);

        /**
         *  Removes an entry in the list of sheets to be loaded.
         *
         *  @param index    An unsigned int containing the index of the sheet to be removed.
         */
        void removeStyleSheetUrlAt(int index);

        /**
         *  Returns an entry from the list of sheets to be loaded.
         *
         *  @param index    An unsigned int containing the index of the sheet to be removed.
         *  @return         The sheet at that index.
         */
        AXRString styleSheetUrlAt(int index) const;

        /**
         *  Returns all entries from the list of sheets to be loaded.
         *
         *  @return         The list of sheets to be loaded.
         */
        const std::vector<AXRString>& styleSheetUrls() const;

        /**
         *  Returns the whole parser tree.
         *
         *  @return         A vector of shared pointers to parser nodes, representing the tree.
         */
        const std::list<QSharedPointer<HSSParserNode> >& parserTree() const;

        /**
         *  Replaces the whole parser tree with what you give.
         *
         *  @param newTree  A vector of shared pointers to parser nodes, representing the new tree.
         */
        void setParserTree(const std::list<QSharedPointer<HSSParserNode> > &newTree);

        /**
         *  Appends the parser node to the parser tree.
         *
         *  @param node     A shared pointer to node to be added.
         */
        void addParserTreeNode(QSharedPointer<HSSParserNode> node);

        /**
         *  Finds the node in the parser tree and removes it.
         *
         *  @param node     A shared pointer to node to be removed.
         */
        void removeParserTreeNode(QSharedPointer<HSSParserNode> node);

        /**
         *  Returns the list of rules.
         *
         *  @return         A vector of shared pointers to rules.
         */
        const std::vector<QSharedPointer<HSSRule> >& rules() const;

        /**
         *  Adds an entry to the list of rules.
         *
         *  @param rule     A shared pointer to the rule.
         */
        void addRule(QSharedPointer<HSSRule> &rule);

        /**
         *  Removes an entry from the list of rules.
         *
         *  @param index    An unsigned int containing the index of the rule to be removed.
         */
        void removeRuleAt(int index);

        /**
         *  Returns the rule at given index.
         *
         *  @param index    An unsigned int containing the index of the rule to be returned.
         *  @return         A shared pointer to the rule. May be an empty pointer if not found.
         */
        QSharedPointer<HSSRule> ruleAt(int index);

        /**
         *  Gives the size of the list of rules.
         *
         *  @return         The number of entries in the list of rules.
         */
        int ruleCount() const;

        /**
         *  Set the given selector chain as the current one. It will set the new current chain size,
         *  reset the current chain count to 0 and make the current selector node the first node in the chain.
         *  @param selectorChain    A shared pointer to the selector chain to make the current one.
         */
        void initializeSelectorChain(QSharedPointer<HSSSelectorChain> selectorChain);

        /**
         *  Shorthand for selecting elements. Calls select(selectorChains, scope, thisObj, processing) with
         *  processing set to to false.
         *  See select(std::vector<QSharedPointer<HSSSelectorChain>>, QSharedPointer<HSSSelection>, QSharedPointer<HSSDisplayObject>, bool)
         *  for documentation on how to use it.
         */
        QSharedPointer<HSSSelection> select(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj);

        /**
         *  This is the method that should be called when creating a new selection. It will use the other types
         *  of selection methods as needed, according to the given selector chains. It will loop over them, initializing and then
         *  selecting elements for each one. The outer vectors of each selection are then appended one after another.
         *
         *  @param  selectorChains  A vector of shared pointers to selector chains, that will determine what will be selected
         *  @param scope            A reference to the current scope, a vector containing shared pointers
         *                          to display objects.
         *  @param thisObj          A shared pointer to the display object that will be selected when using
         *                          \@this in HSS.
         *  @param processing       The first time the rules are matched to the display objects, we want to
         *                          process stuff like flags, instead of just plain selecting.
         *  @param subscribingToNotifications       We can disable subscribing to dom notifications with this argument.
         *  @return         A vector of selections of the elements that were selected, each of these
         *                  is a vector containing shared pointers to display objects.
         */
        QSharedPointer<HSSSelection> select(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent);

        /**
         *  Selects elements hierarchically from the content tree.
         *
         *  Do not call directly, use select() instead.
         *
         *  @param scope        A reference to the current scope, a vector containing shared pointers
         *                      to display objects.
         *  @param thisObj      A shared pointer to the display object that will be selected when using
         *                      \@this in HSS.
         *  @param processing   The first time the rules are matched to the display objects, we want to
         *                      process stuff like flags, instead of just plain selecting.
         *  @param subscribingToNotifications       We can disable subscribing to dom notifications with this argument.
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        QSharedPointer<HSSSelection> selectHierarchical(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent);

        /**
         *  Selects descendants according to the current selector chain. It will call selectOnLevel()
         *  automatically.
         *
         *  Do not call directly, use select() instead.
         *
         *  @param scope        A reference to the current scope, a vector containing shared pointers
         *                      to display objects.
         *  @param thisObj      A shared pointer to the display object that will be selected when using
         *                      \@this in HSS.
         *  @param processing   The first time the rules are matched to the display objects, we want to
         *                      process stuff like flags, instead of just plain selecting.
         *  @param subscribingToNotifications       We can disable subscribing to dom notifications with this argument.
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        QSharedPointer<HSSSelection> selectAllHierarchical(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent);

        /**
         *  Selects siblings according to the current selector chain. It will call selectSimple()
         *  automatically.
         *
         *  Do not call directly, use select() instead.
         *
         *  @param scope        A reference to the current scope, a vector containing shared pointers
         *                      to display objects.
         *  @param thisObj      A shared pointer to the display object that will be selected when using
         *                      \@this in HSS.
         *  @param processing   The first time the rules are matched to the display objects, we want to
         *                      process stuff like flags, instead of just plain selecting.
         *  @param subscribingToNotifications       We can disable subscribing to dom notifications with this argument.
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        QSharedPointer<HSSSelection> selectOnLevel(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent);

        /**
         *  Selects elements according to the current selector chain. It will automatically call
         *  filterSelection() after selecting.
         *
         *  Do not call directly, use select() instead.
         *
         *  @param scope        A reference to the current scope, a vector containing shared pointers
         *                      to display objects.
         *  @param thisObj      A shared pointer to the display object that will be selected when using
         *                      \@this in HSS.
         *  @param processing   The first time the rules are matched to the display objects, we want to
         *                      process stuff like flags, instead of just plain selecting.
         *  @param subscribingToNotifications       We can disable subscribing to dom notifications with this argument.
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        QSharedPointer<HSSSelection> selectSimple(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent);

        /**
         *  Walks up the tree until it finds the top level rule, then selects downwards.
         *
         *  @param theRule      The rule where the walking to the top should begin
         *  @param finalThisObj The object that should be used as the @this object on the last selection level
         *  @return             A simple selection of display objects.
         */
        QSharedPointer<HSSSimpleSelection>  selectFromTop(QSharedPointer<HSSRule> theRule, QSharedPointer<HSSDisplayObject> finalThisObj);

        /**
         *  After reading the XML and HSS documents, this method is used to match the rules to the
         *  display objects in the content tree. You give the rule and the scope where to apply it,
         *  and it will select the elements from the scope according to the selector chain that is
         *  stored in the rule, assigning them the rule if they are selected.
         *
         *  It will look at child rules and match them to the descendants of the selected elements too.
         *
         *  @param rule         A shared pointer to the rule to be matched.
         *  @param scope        The list of display objects to which the rule should be matched.
         *  @param applyingInstructions   Wether to apply instruction rules
         */
        void recursiveMatchRulesToDisplayObjects(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSContainer> container, bool applyingInstructions);
        void recursiveApplyRulesToDisplayObjects(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSimpleSelection> scope, QSharedPointer<HSSContainer> container, bool applyingInstructions);

        std::vector<QSharedPointer<HSSContainer> > addContainerFromNewInstruction(const QSharedPointer<HSSInstruction> & instruction, const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSContainer> container, bool activeByDefault);

        // TODO: All we ever do is push and pop things to and from this... should we provide
        // methods for doing that rather than providing a list reference?
        std::vector<QSharedPointer<HSSContainer> >& currentContext() const;

        void currentContextPush(QSharedPointer<HSSContainer> container);
        const QSharedPointer<HSSContainer> currentContextPop() const;
        void setLogsSelections(bool newValue);
        const bool logsSelections() const;
        const QSharedPointer<HSSSimpleSelection> getLastSelection() const;
        void setCurrentEvent(QSharedPointer<HSSEvent> newEvent) const;
        const QSharedPointer<HSSEvent> getCurrentEvent() const;

    protected:
        void readNextSelectorNode();
        bool isAtEndOfSelector() const;

    private:
        AXRControllerPrivate *const d;
        inline void _setUpTreeChangeObservers(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSimpleSelection> scope, QSharedPointer<HSSContainer> thisObj, bool subscribingToParent);
        inline void _matchRuleToSelection(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSimpleSelection> selection);
        inline void _selectOnLevelSimple(QSharedPointer<HSSSimpleSelection> & ret, HSSCombinatorType combinatorType, QSharedPointer<HSSSimpleSelection> simpleSel, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent);
        void _recursiveGetDescendants(QSharedPointer<HSSSimpleSelection> & ret, QSharedPointer<HSSSimpleSelection> scope);
    };
}

#endif //AXRCONTROLLER_H

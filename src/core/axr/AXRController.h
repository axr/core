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

#ifndef AXRCONTROLLER_H
#define AXRCONTROLLER_H

#include <stack>
#include <vector>
#include <QSharedPointer>
#include "HSSContainer.h"
#include "HSSObject.h"
#include "HSSParser.h"

namespace AXR
{
    class AXRRender;
    class AXRCore;

    /**
     *  @brief  This class holds trees of objects, sheets, rules and the parser tree. Also,
     *  it has methods to select object in those trees.
     */
    class AXR_API AXRController
    {
    public:
        /**
         *  The shared pointer to the controller.
         */
        typedef QSharedPointer<AXRController> p;

        /**
         *  Creates a new instance of a controller.
         *
         */
        AXRController();

        /**
         *  Destructs the controller and clears all the trees.
         */
        virtual ~AXRController();

        void matchRulesToContentTree();

        /**
         *  @return A textual representation of itself, as a string.
         */
        virtual AXRString toString();

        /**
         *  Use this to clean up and start from fresh (e.g. when reloading a file).
         */
        void reset();

        /**
         *  @return A shared pointer to the root element.
         */
        HSSContainer::p & getRoot();

        /**
         *  Sets the element you pass in as the new root element.
         *  If the current object context is empty, it sets it automatically to the new root.
         *
         *  @param newRoot A shared pointer to the new element
         */
        void setRoot(HSSContainer::p newRoot);

        /**
         *  This method is called by the XML parser when it reads an opening tag.
         *
         *  @param elementName  A string containing the name of the element we just entered.
         */
        void enterElement(AXRString elementName);

        /**
         *  This method is called by the XML parser when it reads an attribute of the current element.
         *
         *  @param  name    A string containing the name of the attribute.
         *  @param  value   A string containing the value of the attribute.
         */
        void addAttribute(AXRString name, AXRString value);

        /**
         *  This method is called by the XML parser when it reads the content text of the
         *  current element.
         *
         *  @param  text    A string containing the content text for the current element.
         */
        void setContentText(AXRString text);

        /**
         *  When content text is read in chunks, this allows to append the text to the
         *  current element.
         *
         *  @param text     A string containing content text for the current element.
         */
        void appendContentText(AXRString text);

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
        void add(HSSDisplayObject::p newContainer);

        /**
         *  Append an object definition to the object tree.
         *
         *  @param newObject A shared pointer to an object definition.
         */
        void objectTreeAdd(HSSObjectDefinition::p & newObject);

        /**
         *  Remove the object at given index from the object tree.
         *
         *  @param index    An unsigned int containing the index of the object to be removed.
         */
        void objectTreeRemove(unsigned index);

        /**
         *  Returns an object from the object tree by index.
         *
         *  @param index    An unsigned int containing the index of the object to be returned.
         *  @return         The object definitionat the index. May be an empty pointer if not found.
         */
        HSSObjectDefinition::p & objectTreeGet(unsigned index);

        /**
         *  Returns an object from the object tree by name.
         *
         *  @param name     A string containing the name of the object to be returned.
         *  @return         The object definitionat with that name. May be an empty pointer if not found.
         */
        HSSObjectDefinition::p & objectTreeGet(AXRString name);

        /**
         *  Adds an entry in the list of sheets to be loaded.
         *
         *  @param sheet    A string containing the file name of the stylesheet
         */
        void loadSheetsAdd(QUrl url);

        /**
         *  Removes an entry in the list of sheets to be loaded.
         *
         *  @param index    An unsigned int containing the index of the sheet to be removed.
         */
        void loadSheetsRemove(unsigned index);

        /**
         *  Returns an entry from the list of sheets to be loaded.
         *
         *  @param index    An unsigned int containing the index of the sheet to be removed.
         *  @return         The sheet at that index.
         */
        QUrl loadSheetsGet(unsigned index);

        /**
         *  Returns all entries from the list of sheets to be loaded.
         *
         *  @return         The list of sheets to be loaded.
         */
        const std::vector<QUrl> loadSheetsGet() const;

        /**
         *  Replaces the whole parser tree with what you give.
         *
         *  @param newTree  A vector of shared pointers to parser nodes, representing the new tree.
         */
        void setParserTree(std::vector<HSSParserNode::p> newTree);

        /**
         *  Returns the whole parser tree.
         *
         *  @return         A vector of shared pointers to parser nodes, representing the tree.
         */
        const std::vector<HSSParserNode::p> getParserTree() const;

        /**
         *  Appends the parser node to the parser tree.
         *
         *  @param node     A shared pointer to node to be added.
         */
        void parserTreeAdd(HSSParserNode::p node);

        /**
         *  Finds the node in the parser tree and removes it.
         *
         *  @param node     A shared pointer to node to be removed.
         */
        void parserTreeRemove(HSSParserNode::p node);

        /**
         *  Returns the list of rules.
         *
         *  @return         A vector of shared pointers to rules.
         */
        const std::vector<HSSRule::p>& getRules() const;

        /**
         *  Adds an entry to the list of rules.
         *
         *  @param rule     A shared pointer to the rule.
         */
        void rulesAdd(HSSRule::p & rule);

        /**
         *  Removes an entry from the list of rules.
         *
         *  @param index    An unsigned int containing the index of the rule to be removed.
         */
        void rulesRemove(unsigned index);

        /**
         *  Returns the rule at given index.
         *
         *  @param index    An unsigned int containing the index of the rule to be returned.
         *  @return         A shared pointer to the rule. May be an empty pointer if not found.
         */
        HSSRule::p & rulesGet(unsigned index);

        /**
         *  Gives the size of the list of rules.
         *
         *  @return         The number of entries in the list of rules.
         */
        unsigned rulesSize();

        /**
         *  Set the given selector chain as the current one. It will set the new current chain size,
         *  reset the current chain count to 0 and make the current selector node the first node in the chain.
         *  @param selectorChain    A shared pointer to the selector chain to make the current one.
         */
        void initializeSelectorChain(HSSSelectorChain::p selectorChain);

        /**
         *  Shorthand for selecting elements. Calls select(selectorChains, scope, thisObj, processing) with
         *  processing set to to true.
         *  See select(std::vector<HSSSelectorChain::p>, const std::vector<HSSDisplayObject::p> &, HSSDisplayObject::p, bool)
         *  for documentation on how to use it.
         */
        std::vector< std::vector<HSSDisplayObject::p> > select(std::vector<HSSSelectorChain::p> selectorChains, const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj);

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
         *  @return         A vector of selections of the elements that were selected, each of these
         *                  is a vector containing shared pointers to display objects.
         */
        std::vector< std::vector<HSSDisplayObject::p> > select(std::vector<HSSSelectorChain::p> selectorChains, const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing);

        /**
         *  Shortand for selecting elements hierarchically from the content tree. See
         *  selectHierarchical(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj) for
         *  documentation on how to use it.
         *
         *  Do not call directly, use select() instead.
         *
         *  @param scope    A reference to the current scope, a vector containing shared pointers
         *                  to display objects.
         *  @param thisObj  A shared pointer to the display object that will be selected when using
         *                  \@this in HSS.
         *  @return         A vector of selections of the elements that were selected, each of these
         *                  is a vector containing shared pointers to display objects.
         */
        std::vector< std::vector<HSSDisplayObject::p> > selectHierarchical(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj);

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
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        std::vector< std::vector<HSSDisplayObject::p> > selectHierarchical(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing);

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
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        std::vector< std::vector<HSSDisplayObject::p> > selectAllHierarchical(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing);

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
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        std::vector< std::vector<HSSDisplayObject::p> > selectOnLevel(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing);

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
         *  @return             A vector of selections of the elements that were selected, each of these
         *                      is a vector containing shared pointers to display objects.
         */
        std::vector< std::vector<HSSDisplayObject::p> > selectSimple(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing);

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
        void recursiveMatchRulesToDisplayObjects(const HSSRule::p & rule, const std::vector<HSSDisplayObject::p> & scope, HSSContainer::p container, bool applyingInstructions);

        std::stack<HSSContainer::p>currentContext;

    protected:
        HSSContainer::p root;

        std::vector<HSSObjectDefinition::p>objectTree;
        std::vector<QUrl>loadSheets;
        std::vector<HSSParserNode::p>parserTree;
        std::vector<HSSRule::p>rules;

        HSSSelectorChain::p currentChain;
        HSSParserNode::p currentSelectorNode;
        unsigned currentChainCount;
        unsigned currentChainSize;
        void readNextSelectorNode();
        bool isAtEndOfSelector();

    private:
        void _recursiveGetDescendants(std::vector<HSSDisplayObject::p> &ret, const std::vector<HSSDisplayObject::p> & scope);
    };
}

#endif //AXRCONTROLLER_H

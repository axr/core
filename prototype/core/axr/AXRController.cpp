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
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/12/22
 *      HSS version: 1.0
 *      Core version: 0.43
 *      Revision: 19
 *
 ********************************************************************/

#include "AXRController.h"
#include "AXRRender.h"
#include "AXRExceptions.h"
#include <sstream>
#include "../axr/AXRDebugging.h"
#include "../axr/errors/errors.h"
#include "../hss/objects/HSSDisplayObject.h"
#include <boost/pointer_cast.hpp>
#include "../hss/parsing/HSSSelectorChain.h"
#include "../hss/parsing/HSSFilters.h"
#include <boost/algorithm/string.hpp>

using namespace AXR;


AXRController::AXRController()
{
    this->osHelper = OSHelper::p(new OSHelper());
    this->parserXML = XMLParser::p(new XMLParser(this));
    this->parserHSS = HSSParser::p(new HSSParser(this));
    this->_hasLoadedFile = false;
    this->_isHSSOnly = false;
    this->_render = NULL;
    this->currentContext = std::stack<HSSContainer::p>();
}

AXRController::~AXRController()
{
    this->objectTree.clear();
    this->loadSheets.clear();
    this->rules.clear();
    this->statements.clear();
}

void AXRController::setRender(AXRRender * render)
{
    this->_render = render;
}

bool AXRController::loadFile()
{
    std::string filepath = std::string();
    std::string filename = std::string();
    std::string fileextension = std::string();
    bool success = this->osHelper->openFileDialog(filepath, filename, this->basepath);
    if(success){
        fileextension = filename.substr(filename.rfind(".") + 1, filename.length());
        if(fileextension == "xml"){
            this->_isHSSOnly = false;
            return this->loadFile(filepath, filename);
        } else if (fileextension == "hss") {
            this->_isHSSOnly = true;
            return this->loadFileHSS(filepath, filename);
        } else {
            AXRError::p(new AXRError("AXRController", "Unknown file extension"))->raise();
            return false;
        }
        
    } else {
        return false;
    }
}

bool AXRController::loadFile(std::string xmlfilepath, std::string xmlfilename)
{
    //we assume success unless noted otherwise
    bool loadingSuccess = true;
    std::string hssfilepath = std::string();
    std::string hssfolder = std::string();
    std::string hssfilename = std::string();
    unsigned i, j;
    
    if(this->_hasLoadedFile){
        //prepare itself for loading a new file
        this->reset();
    }
    
    //the xml parser will call the enterElement, addAttribute and exitElement functions to construct the initial tree
    loadingSuccess = this->loadXMLFile(xmlfilepath, xmlfilename);
    if(!loadingSuccess){
        AXRError::p(new AXRError("AXRController", "Could not load the XML file"))->raise();
    } else {
        //needs reset on next load
        this->_hasLoadedFile = true;
        
        HSSDisplayObject::p rootObj;
        
        //load the obtained stylesheets
        // if(this->loadSheets.size() == 0){
        //    AXRWarning::p(new AXRWarning("AXRController", "There was no stylesheet linked in your XML file"))->raise();
        //    loadingSuccess = false;
        //} else {
            for (i=0; i<this->loadSheets.size(); i++) {
                
                hssfilename = this->loadSheets[i];
                
                if(hssfilename.substr(0,1) == "/"){
                    //FIXME: add support for absolute paths
                } else {
                    //FIXME: why is doing exactly the opposite of what I expect? that's why I put the ! in there
                    //                    size_t hasFolder = hssfilename.rfind("/", hssfilename.size());
                    //                    if(hasFolder != -1){
                    //                        //construct the new path
                    //                        hssfolder = hssfilename.substr(0, hssfilename.rfind("/", hssfilename.length()));
                    //                        std_log1("has folder: ");std_log1(hssfolder);
                    //                    } else {
                    hssfolder = xmlfilepath.substr(0, xmlfilepath.rfind("/", xmlfilepath.length())+1);
                    hssfilepath = hssfolder.append(hssfilename);
                    //                        std_log1("no folder: ");std_log1(hssfilepath);
                    //                    }
                    
                    if(! this->loadHSSFile(hssfilepath, hssfilename.substr(hssfilename.rfind("/", hssfilename.length())+1))){
                        //FIXME: handle the error
                        loadingSuccess = false;
                        std_log1("error loading hss file");
                    } else {
                        //assign the rules to the objects
                        rootObj = boost::static_pointer_cast<HSSDisplayObject>(this->root);
                        for (j=0; j<this->rules.size(); j++) {
                            HSSRule::p& rule = this->rules[j];
                            const HSSDisplayObject::p rootScopeArr[1] = {root};
                            const std::vector<HSSDisplayObject::p>rootScope(rootScopeArr, rootScopeArr+1);
                            this->recursiveMatchRulesToDisplayObjects(rule, rootScope, root);
                        }
                        rootObj->setNeedsRereadRules(true);
                    }
                }
            }
        //}
        
        if (rootObj) {
            rootObj->recursiveReadDefinitionObjects();
            rootObj->handleEvent(HSSEventTypeLoad, NULL);
        }
    }
    
    return loadingSuccess;
}


bool AXRController::loadFileHSS(std::string hssfilepath, std::string hssfilename)
{
    //we assume success unless noted otherwise
    bool loadingSuccess = true;
    unsigned i;
    
    if(this->_hasLoadedFile){
        //prepare itself for loading a new file
        this->reset();
    }
    
    this->enterElement("root");
    this->exitElement();
    
    loadingSuccess = this->loadHSSFile(hssfilepath, hssfilename.substr(hssfilename.rfind("/", hssfilename.length())+1));
    if(!loadingSuccess){
        AXRError::p(new AXRError("AXRController", "Could not load the HSS file"))->raise();
    } else {
        //needs reset on next load
        this->_hasLoadedFile = true;
        
        HSSDisplayObject::p rootObj = boost::static_pointer_cast<HSSDisplayObject>(this->root);
        //assign the rules to the object
        for (i=0; i<this->rules.size(); i++) {
            HSSRule::p& rule = this->rules[i];
            const HSSDisplayObject::p rootScopeArr[1] = {root};
            const std::vector<HSSDisplayObject::p>rootScope(rootScopeArr, rootScopeArr+1);
            this->recursiveMatchRulesToDisplayObjects(rule, rootScope, root);
        }
        //read the rules into values
        rootObj->recursiveReadDefinitionObjects();
        rootObj->fireEvent(HSSEventTypeLoad);
    }
    
    return loadingSuccess;
}

void AXRController::recursiveMatchRulesToDisplayObjects(HSSRule::p & rule, const std::vector<HSSDisplayObject::p> & scope, HSSContainer::p container)
{
    HSSInstruction::p instruction = rule->getInstruction();
    if (instruction) {
        switch (instruction->getInstructionType()) {
            case HSSNewInstruction:
            {
                std::string elementName = rule->getSelectorChain()->subject()->getElementName();
                HSSContainer::p newContainer = HSSContainer::p(new HSSContainer(elementName));
                newContainer->setElementName(elementName);
                this->add(newContainer);
                std_log1("created "+newContainer->getElementName());
                newContainer->rulesAdd(rule);
                newContainer->setNeedsRereadRules(true);
                newContainer->setNeedsSurface(true);
                newContainer->setDirty(true);
                unsigned i, size;
                this->currentContext.push(newContainer);
                for (i=0, size=rule->childrenSize(); i<size; i++) {
                    HSSRule::p childRule = rule->childrenGet(i);
                    this->recursiveMatchRulesToDisplayObjects(childRule, newContainer->getChildren(), newContainer);
                }
                newContainer->setNeedsRereadRules(true);
                //newContainer->fireEvent(HSSEventTypeLoad);
                this->currentContext.pop();
                break;
            }
                
            case HSSMoveInstruction:
            {
                HSSContainer::p parent = container->getParent();
                if (parent) {
                    std::vector<HSSDisplayObject::p> moveScope = parent->getChildren();
                    this->setSelectorChain(rule->getSelectorChain());
                    std::vector< std::vector<HSSDisplayObject::p> > selection = this->selectHierarchical(moveScope);
                    unsigned i, j, k, size, size2, size3;
                    
                    this->currentContext.push(container);
                    //move the children over
                    for (i=0, size=selection.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> inner = selection[i];
                        for (j=0, size2=inner.size(); j<size2; j++) {
                            HSSDisplayObject::p theDO = inner[j];
                            if(theDO != container){
                                theDO->removeFromParent();
                                this->add(theDO);
                                std_log1("moved "+theDO->getElementName());
                                theDO->rulesAdd(rule);
                                theDO->setNeedsRereadRules(true);
                                theDO->setNeedsSurface(true);
                                theDO->setDirty(true);
                                
                                if(theDO->isA(HSSObjectTypeContainer)){
                                    HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(theDO);
                                    //assign more rules
                                    for (k=0, size3=rule->childrenSize(); k<size3; k++) {
                                        HSSRule::p childRule = rule->childrenGet(k);
                                        this->recursiveMatchRulesToDisplayObjects(childRule, theContainer->getChildren(), theContainer);
                                    } 
                                }
                                
                                theDO->setNeedsRereadRules(true);
                                //theDO->fireEvent(HSSEventTypeLoad);
                            }
                            
                        }
                    }
                    
                    
                    
                    
                    this->currentContext.pop();
                }
                
                break;
            }
                
            case HSSDeleteInstruction:
            {
                //select the items to be deleted
                this->setSelectorChain(rule->getSelectorChain());
                std::vector< std::vector<HSSDisplayObject::p> > selection = this->selectHierarchical(scope);
                unsigned i, j, size, size2;
                for (i=0, size=selection.size(); i<size; i++) {
                    std::vector<HSSDisplayObject::p> inner = selection[i];
                    for (j=0, size2=inner.size(); j<size2; j++) {
                        inner[j]->removeFromParent();
                    }
                }
                break;
            }
                                
            default:
                break;
        }
        
    } else {
        this->setSelectorChain(rule->getSelectorChain());
        
        //if it starts with a combinator, adjust the scope and selector chain
        bool useAdjustedScope = false;
        std::vector<HSSDisplayObject::p> adjustedScope;
        if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator)){
            useAdjustedScope = true;
            adjustedScope.push_back(container);
            this->currentChain->prepend(HSSSelector::p(new HSSSelector(container->getElementName())));
            this->currentChainSize ++;
            this->currentSelectorNode = this->currentChain->get(0);
        }
        
        std::vector< std::vector<HSSDisplayObject::p> > selection = this->selectHierarchical((useAdjustedScope ? adjustedScope : scope));
        unsigned i, j, k, size, size2, size3;
        for (i=0, size=selection.size(); i<size; i++) {
            std::vector<HSSDisplayObject::p> inner = selection[i];
            for (j=0, size2=inner.size(); j<size2; j++) {
                const HSSDisplayObject::p & displayObject = inner[j];
                std_log1("match "+displayObject->getElementName());
                displayObject->rulesAdd(rule);
                displayObject->setNeedsRereadRules(true);
                displayObject->setNeedsSurface(true);
                displayObject->setDirty(true);
                
                //if it is a container it may have children
                if(displayObject->isA(HSSObjectTypeContainer)){
                    HSSContainer::p selectedContainer = boost::static_pointer_cast<HSSContainer>(displayObject);
                    this->currentContext.push(selectedContainer);
                    for (k=0, size3=rule->childrenSize(); k<size3; k++) {
                        HSSRule::p childRule = rule->childrenGet(k);
                        this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer);
                    }
                    this->currentContext.pop();
                }
                
                displayObject->setNeedsRereadRules(true);
            }
        }
    }
}

void AXRController::setSelectorChain(HSSSelectorChain::p selectorChain)
{
    this->currentChain = selectorChain;
    this->currentChainSize = selectorChain->size();
    this->currentChainCount = 0;
    this->currentSelectorNode = selectorChain->get(0);
    
}

void AXRController::readNextSelectorNode()
{
    if(this->currentChainCount < this->currentChainSize-1){
        this->currentChainCount++;
        this->currentSelectorNode = this->currentChain->get(this->currentChainCount);
    }
}

bool AXRController::isAtEndOfSelector()
{
    return this->currentChainCount >= this->currentChainSize-1;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectHierarchical(const std::vector<HSSDisplayObject::p> & scope)
{
    std::vector< std::vector<HSSDisplayObject::p> > selections = this->selectOnLevel(scope);
    unsigned i, size;
    for (i=0, size=selections.size(); i<size; i++) {
        std::vector<HSSDisplayObject::p> selection = selections[i];
        if(selection.size() > 0){
            if(!this->isAtEndOfSelector()){
                if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator)){
                    HSSCombinator::p combinator = boost::static_pointer_cast<HSSCombinator>(this->currentSelectorNode);
                    HSSCombinatorType combinatorType = combinator->getCombinatorType();
                    switch (combinatorType) {
                        case HSSCombinatorTypeChildren:
                        {
                            std::vector<HSSDisplayObject::p> newScope;
                            unsigned j, size2, k, size3;
                            for(j=0, size2 = selection.size(); j<size2; j++){
                                if(selection[j]->isA(HSSObjectTypeContainer)){
                                    HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(selection[j]);
                                    const std::vector<HSSDisplayObject::p> children = theContainer->getChildren();
                                    for(k=0, size3 = children.size(); k<size3; k++){
                                        newScope.push_back(children[k]);
                                    }
                                }
                            }
                            this->readNextSelectorNode();
                            return this->selectHierarchical(newScope);
                            break;
                        }
                            
                        case HSSCombinatorTypeDescendants:
                        {
                            std::vector<HSSDisplayObject::p> newScope;
                            unsigned j, size2, k, size3;
                            for(j=0, size2 = selection.size(); j<size2; j++){
                                if(selection[j]->isA(HSSObjectTypeContainer)){
                                    HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(selection[j]);
                                    const std::vector<HSSDisplayObject::p> children = theContainer->getChildren();
                                    for(k=0, size3 = children.size(); k<size3; k++){
                                        newScope.push_back(children[k]);
                                    }
                                }
                            }
                            
                            this->readNextSelectorNode();
                            return this->selectAllHierarchical(newScope);
                            break;
                        }
                            
                        default:
                            break;
                    }
                }
            }
        }
    }
    
    return selections;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectAllHierarchical(const std::vector<HSSDisplayObject::p> & scope)
{
    std::vector< std::vector<HSSDisplayObject::p> > selections = this->selectOnLevel(scope);
    
    unsigned i, j, k, size, size2, size3;
    for (i=0, size=selections.size(); i<size; i++) {
        
        std::vector<HSSDisplayObject::p> newSearch;
        for(j=0, size2=scope.size(); j<size2; j++){
            if(scope[j]->isA(HSSObjectTypeContainer)){
                HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(scope[j]);
                const std::vector<HSSDisplayObject::p> children = theContainer->getChildren();
                for(k=0, size3 = children.size(); k<size3; k++){
                    newSearch.push_back(children[k]);
                }
            }
        }
        
        std::vector< std::vector<HSSDisplayObject::p> > childSelection;
        if(newSearch.size() > 0){
            childSelection = this->selectAllHierarchical(newSearch);
            for (j=0, size2=selections.size(); j<size2; j++) {
                //std::vector<HSSDisplayObject::p> & selection = selections[j];
                for (k=0, size3=childSelection.size(); k<size3; k++) {
                    selections[j].insert(selections[j].end(), childSelection[k].begin(), childSelection[k].end());
                }
            }
        }
    }
    
    return selections;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectOnLevel(const std::vector<HSSDisplayObject::p> & scope)
{
    std::vector< std::vector<HSSDisplayObject::p> > selections = this->selectSimple(scope);
    if(!this->isAtEndOfSelector()){
        if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator))
        {
            std::vector< std::vector<HSSDisplayObject::p> > ret;
            HSSCombinator::p combinator = boost::static_pointer_cast<HSSCombinator>(this->currentSelectorNode);
            HSSCombinatorType combinatorType = combinator->getCombinatorType();
            switch (combinatorType) {
                case HSSCombinatorTypeSiblings:
                {
                    //if found, just select the right part
                    this->readNextSelectorNode();
                    unsigned i, size;
                    for (i=0, size=selections.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> selection = selections[i];
                        std::vector< std::vector<HSSDisplayObject::p> > newSelection = this->selectSimple(scope);
                        ret.insert(ret.end(), newSelection.begin(), newSelection.end());
                    }
                    return ret;
                }
                    
                case HSSCombinatorTypeNextSiblings:
                {
                    //find the selected ones on the left, and select all the following ones that match
                    unsigned i, size;
                    for (i=0, size=selections.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> selection = selections[i];
                        std::vector<HSSDisplayObject::p> vect(scope);
                        std::vector<HSSDisplayObject::p>::iterator it;
                        it = std::find(vect.begin(), vect.end(), selection.front());
                        std::vector<HSSDisplayObject::p> right(it+1, vect.end());
                        this->readNextSelectorNode();
                        std::vector< std::vector<HSSDisplayObject::p> > newSelection = this->selectSimple(right);
                        ret.insert(ret.end(), newSelection.begin(), newSelection.end());
                    }
                    return ret;
                }
                    
                case HSSCombinatorTypePreviousSiblings:
                {
                    //find the selected ones on the left, and select all the previous ones that match
                    this->readNextSelectorNode();
                    unsigned i, size;
                    for (i=0, size=selections.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> selection = selections[i];
                        std::vector<HSSDisplayObject::p> vect(scope);
                        std::vector<HSSDisplayObject::p>::iterator it;
                        it = std::find(vect.begin(), vect.end(), selection.back());
                        std::vector<HSSDisplayObject::p> right(vect.begin(), it);
                        this->readNextSelectorNode();
                        std::vector< std::vector<HSSDisplayObject::p> > newSelection = this->selectSimple(right);
                        ret.insert(ret.end(), newSelection.begin(), newSelection.end());
                    }
                    return ret;
                }
                    
                default:
                    break;
            }
        }  
    }
    
    return selections;
    
//    std::vector< std::vector<HSSDisplayObject::p> > selections = this->selectSimple(scope);
//    unsigned i, size;
//    for (i=0, size=selections.size(); i<size; i++) {
//        std::vector<HSSDisplayObject::p> selection = selections[i];
//        //only continue if it actually found anything
//        if(selection.size() > 0){
//            if(!this->isAtEndOfSelector()){
//                
//                if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator))
//                {
//                    HSSCombinator::p combinator = boost::static_pointer_cast<HSSCombinator>(this->currentSelectorNode);
//                    HSSCombinatorType combinatorType = combinator->getCombinatorType();
//                    switch (combinatorType) {
//                        case HSSCombinatorTypeSiblings:
//                        {
//                            //if found, just select the right part
//                            this->readNextSelectorNode();
//                            ret = this->selectSimple(scope);
//                            return ret;
//                        }
//                            
//                        case HSSCombinatorTypeNextSiblings:
//                        {
//                            //find the selected ones on the left, and select all the following ones that match
//                            std::vector<HSSDisplayObject::p> vect(scope);
//                            std::vector<HSSDisplayObject::p>::iterator it;
//                            it = std::find(vect.begin(), vect.end(), selection.front());
//                            std::vector<HSSDisplayObject::p> right(it+1, vect.end());
//                            this->readNextSelectorNode();
//                            ret = this->selectSimple(right);
//                            return ret;
//                        }
//                            
//                        case HSSCombinatorTypePreviousSiblings:
//                        {
//                            //find the selected ones on the left, and select all the previous ones that match
//                            std::vector<HSSDisplayObject::p> vect(scope);
//                            std::vector<HSSDisplayObject::p>::iterator it;
//                            it = std::find(vect.begin(), vect.end(), selection.back());
//                            std::vector<HSSDisplayObject::p> right(vect.begin(), it);
//                            this->readNextSelectorNode();
//                            ret = this->selectSimple(right);
//                            return ret;
//                        }
//                            
//                        default:
//                            break;
//                    }
//                }
//            }
//        }
//    }
//    
//    return selections;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectSimple(const std::vector<HSSDisplayObject::p> & scope)
{
    std::vector< std::vector<HSSDisplayObject::p> >ret;
    std::vector<HSSDisplayObject::p> currentSelection;
    
    unsigned i, size;
    HSSParserNodeType selectorType = this->currentSelectorNode->getType();
    switch (selectorType) {
        case HSSParserNodeTypeSelector:
        {
            //select only elements with matching element name
            HSSSelector::p selector = boost::static_pointer_cast<HSSSelector>(this->currentSelectorNode);
            for (i=0, size=scope.size(); i<size; i++) {
                if(scope[i]->getElementName() == selector->getElementName()){
                    currentSelection.push_back(scope[i]);
                }
            }
            
            this->readNextSelectorNode();
            break;
        }
        
        case HSSParserNodeTypeUniversalSelector:
        {
            //the entire scope will be selected
            currentSelection = scope;
            this->readNextSelectorNode();
            break;
        }
            
        case HSSParserNodeTypeFilter:
        {
            //the entire scope will be selected
            currentSelection = scope;
            break;
        }
        
        default:
            throw AXRError::p(new AXRError("AXRController", "Unknown node type "+HSSParserNode::parserNodeStringRepresentation(selectorType)+" in selector"));
    }
    
    ret = this->filterSelection(currentSelection);
    
    return ret;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::filterSelection( std::vector<HSSDisplayObject::p> &selection)
{
    std::vector< std::vector<HSSDisplayObject::p> >ret;
    
    if (this->currentSelectorNode->isA(HSSParserNodeTypeFilter)){
        HSSFilter::p theFilter = boost::static_pointer_cast<HSSFilter>(this->currentSelectorNode);
        switch (theFilter->getFilterType()) {
            case HSSFilterTypeEach:
            {
                unsigned i, size;
                for (i=0, size=selection.size(); i<size; i++) {
                    std::vector<HSSDisplayObject::p> sel;
                    sel.push_back(selection[i]);
                    ret.push_back(sel);
                }
                break;
            }
                
            default:
            {
                ret.push_back(theFilter->apply(selection));
                break;
            }
        }
        
        this->readNextSelectorNode();
        
        
    } else {
        ret.push_back(selection);
    }
    
    return ret;
}

bool AXRController::reload()
{
    if(this->_hasLoadedFile){
        std::string filepath;
        std::string filename;
        
        if(this->_isHSSOnly){
            filepath = (this->parserHSS->basepath)+"/"+(this->parserHSS->filename);
            filename = this->parserHSS->filename;
            return this->loadFileHSS(filepath, filename);
            
        } else {
            filepath = this->parserXML->getFilePath();
            filename = this->parserXML->getFileName();
            return this->loadFile(filepath, filename);
        }
    }
    
    return false;
}

std::string AXRController::toString()
{
    std::string tempstr = "AXR Controller\n";
    if(this->root != NULL){
        tempstr.append("\n\n\nROOT:");
        tempstr = this->root->toString();  
    }
    unsigned i;
    if(this->objectTree.size() > 0)
    {
        tempstr.append("\n\n\nOBJECT TREE:");
        for (i=0; i<this->objectTree.size(); i++) {
            tempstr.append("\n").append(this->objectTree[i]->toString());
        }
    }
    
    if(this->statements.size() > 0)
    {
        tempstr.append("\n\n\nSTATEMENTS:");
        for (i=0; i<this->statements.size(); i++) {
            tempstr.append("\n").append(this->statements[i]->toString());
        }
    }
    
    return tempstr;
}


void AXRController::reset()
{
    unsigned i;
    
    this->objectTree.clear();
    this->loadSheets.clear();
    this->statements.clear();
    this->rules.clear();
    this->root.reset();
    for(i=0; i<this->currentContext.size(); i++)
    {
        this->currentContext.pop();
    }
    this->parserHSS->reset();
    this->parserXML = XMLParser::p(new XMLParser(this));
    
    this->currentChain.reset();
    this->currentSelectorNode.reset();
    this->currentChainCount = 0;
    this->currentChainSize = 0;
    
    this->_render->reset();
    AXRErrorsManager::reset();
}


bool AXRController::loadXMLFile(std::string filepath, std::string filename)
{
    return this->parserXML->loadFile(filepath, filename);
}

bool AXRController::loadHSSFile(std::string filepath, std::string filename)
{
    std_log1(std::string("******************************************************************\n* opening document:\n* ").append(filepath).append("\n******************************************************************"));
    return this->parserHSS->loadFile(filepath);
}

HSSContainer::p & AXRController::getRoot()
{
    return this->root;
}

void AXRController::setRoot(HSSContainer::p newRoot){
    this->root = newRoot;
    if(this->parserHSS->currentObjectContextSize() == 0){
        this->parserHSS->currentObjectContextAdd(newRoot);
    }
}

void AXRController::enterElement(std::string elementName)
{
    //std_log1("enter element " + elementName);
    HSSContainer::p newContainer(new HSSContainer(elementName+"_source_obj"));
    newContainer->setElementName(elementName);
    this->add(newContainer);
    this->currentContext.push(newContainer);
}

void AXRController::addAttribute(std::string name, std::string value)
{
    //std_log1(std::string("adding attribute " + name + " and value " + value));
    this->currentContext.top()->attributesAdd(name, value);
}

void AXRController::setContentText(std::string text)
{
    this->currentContext.top()->setContentText(text);
}

void AXRController::appendContentText(std::string text)
{
    this->currentContext.top()->appendContentText(text);
}

void AXRController::exitElement()
{
    const HSSContainer::p &cc = this->currentContext.top();
    std::string newContentText = cc->getContentText();
    boost::trim(newContentText);
    cc->setContentText(newContentText);
    this->currentContext.pop();
}


void AXRController::add(HSSDisplayObject::p newElement)
{
    newElement->setController(this);
    
    if(!this->root){
        HSSContainer::p cont = HSSContainer::asContainer(newElement);
        if (cont){
            this->setRoot(cont);
        } else {
            std_log1("############## HSSController: cannot add non-controller as root");
        }
        
    } else {
        if(this->currentContext.size() != 0){
            HSSContainer::p theCurrent = this->currentContext.top();
            theCurrent->add(newElement);
            
        } else {
            std_log1("############## HSSController: tried to add a container to nonexistent current");
        }
    }
}

//object tree
void AXRController::objectTreeAdd(HSSObjectDefinition::p & newObject)
{
    this->objectTree.push_back(newObject);
}

void AXRController::objectTreeRemove(unsigned index)
{
    this->objectTree.erase(this->objectTree.begin()+index);
}

HSSObjectDefinition::p & AXRController::objectTreeGet(unsigned index)
{
    return this->objectTree[index];
}

HSSObjectDefinition::p & AXRController::objectTreeGet(std::string name)
{
    HSSObject::p ret = HSSObject::p();
    //FIXME: do this with an unordered_map for better performance
    unsigned i, size;
    for (i=0, size=this->objectTree.size(); i<size; i++) {
        HSSObjectDefinition::p & theObj = this->objectTreeGet(i);
        if(theObj && theObj->getObject()->name == name){
            return theObj;
        }
    }
    
    throw AXRError::p(new AXRError("AXRController", "No object with name "+name+" was found"));
}

//loadSheets

void AXRController::loadSheetsAdd(std::string sheet)
{
    this->loadSheets.push_back(sheet);
}

void AXRController::loadSheetsRemove(unsigned index)
{
    this->loadSheets.erase(this->loadSheets.begin()+index);
}

std::string AXRController::loadSheetsGet(unsigned index)
{
    return this->loadSheets[index];
}

//has loaded file

bool AXRController::hasLoadedFile()
{
    return _hasLoadedFile;
}

//statements

const std::vector<HSSStatement::p>& AXRController::getStatements() const
{
    return this->statements;
}

void AXRController::statementsAdd(HSSStatement::p & statement)
{
    this->statements.push_back(statement);
}

void AXRController::statementsRemove(unsigned index)
{
    this->statements.erase(this->statements.begin()+index);
}

HSSStatement::p & AXRController::statementsGet(unsigned index)
{
    return this->statements[index];
}

//rules

const std::vector<HSSRule::p>& AXRController::getRules() const
{
    return this->rules;
}

void AXRController::rulesAdd(HSSRule::p & statement)
{
    this->rules.push_back(statement);
}

void AXRController::rulesRemove(unsigned index)
{
    this->rules.erase(this->rules.begin()+index);
}

HSSRule::p & AXRController::rulesGet(unsigned index)
{
    return this->rules[index];
}

unsigned const AXRController::rulesSize()
{
    return this->rules.size();
}






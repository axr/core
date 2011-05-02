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
 *      Last changed: 2011/05/02
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 11
 *
 ********************************************************************/

#include "AXRController.h"
#include "AXRExceptions.h"
#include <sstream>
#include "../axr/AXRDebugging.h"
#include "../hss/objects/HSSDisplayObject.h"
#include <boost/pointer_cast.hpp>
#include "../hss/parsing/HSSSelectorChain.h"

using namespace AXR;


AXRController::AXRController()
{
    this->osHelper = OSHelper::p(new OSHelper());
    this->parserXML = XMLParser::p(new XMLParser(this));
    this->parserHSS = HSSParser::p(new HSSParser(this));
    this->_hasLoadedFile = false;
}

AXRController::~AXRController()
{
    this->objectTree.clear();
    this->loadSheets.clear();
    this->rules.clear();
    this->statements.clear();
}

bool AXRController::loadFile()
{
    std::string xmlfilepath = std::string();
    std::string xmlfilename = std::string();
    bool success = this->osHelper->openFileDialog(xmlfilepath, xmlfilename);
    if(success){
        return this->loadFile(xmlfilepath, xmlfilename);
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
    if(!this->loadXMLFile(xmlfilepath, xmlfilename)){
        //FIXME: handle the error
        std_log1("could not load the xml file");
        loadingSuccess = false;
    } else {
        //needs reset on next load
        this->_hasLoadedFile = true;
        
        //load the obtained stylesheets
        if(this->loadSheets.size() == 0){
            throw AXRNoStylesheetsException(xmlfilepath);
            //FIXME: test this to better understand exceptions
            std_log1("this comes after the exception, it get's executed");
        } else {
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
                        HSSDisplayObject::p rootObj = boost::static_pointer_cast<HSSDisplayObject>(this->root);
                        for (j=0; j<this->rules.size(); j++) {
                            HSSRule::p& rule = this->rules[j];
                            const HSSDisplayObject::p rootScopeArr[1] = {root};
                            const std::vector<HSSDisplayObject::p>rootScope(rootScopeArr, rootScopeArr+1);
                            this->recursiveMatchRulesToDisplayObjects(rule, rootScope);
                        }
                        //read the rules into values
                        rootObj->recursiveReadDefinitionObjects();
                    }
                }
            }
        }
    }
    
    return loadingSuccess;
}

void AXRController::recursiveMatchRulesToDisplayObjects(HSSRule::p & rule, const std::vector<HSSDisplayObject::p> & scope)
{
    this->currentChain = rule->selectorChain;
    this->currentChainSize = this->currentChain->size();
    this->currentChainCount = 0;
    this->currentSelectorNode = this->currentChain->get(0);
    
    const std::vector<HSSDisplayObject::p> selection = this->selectHierarchical(scope);
    unsigned i, size, j, size2;
    for (i=0, size=selection.size(); i<size; i++) {
        const HSSDisplayObject::p & displayObject = selection[i];
        std_log1("match "+displayObject->getElementName());
        displayObject->rulesAdd(rule);
        displayObject->setNeedsRereadRules(true);
        displayObject->setNeedsSurface(true);
        displayObject->setDirty(true);
        
        //if it is a container it may have children
        if(displayObject->isA(HSSObjectTypeContainer)){
            HSSContainer::p container = boost::static_pointer_cast<HSSContainer>(displayObject);
            for (j=0, size2=rule->childrenSize(); j<size2; j++) {
                HSSRule::p childRule = rule->childrenGet(j);
                this->recursiveMatchRulesToDisplayObjects(childRule, container->children);
            }
        }
    }
    
    
//    //look at the selector chain
//    HSSSelectorChain * selectorChain(rule->selectorChain.get());
//    std::vector<HSSDisplayObject::p>tempSelection;
//    unsigned i, j, size;
//    
//    unsigned selectorCount = 0;
//    unsigned selectorSize = selectorChain->size();
//    while (selectorCount < selectorSize) {
//        const HSSParserNode::p & selector = selectorChain->get(selectorCount++);
//        if(selectorChain->size() == 1){
//            if(selector->isA(HSSParserNodeTypeSelector)){
//                HSSSelector::p simpleSelector = boost::static_pointer_cast<HSSSelector>(selector);
//                //select items
//                for (i=0, size=scope.size(); i<size; i++) {
//                    const HSSDisplayObject::p & displayObject = scope[i];
//                
//                    //does it match
//                    if(simpleSelector->getElementName() == displayObject->getElementName()){
//                        std_log1("match "+displayObject->getElementName());
//                        displayObject->rulesAdd(rule);
//                        displayObject->setNeedsRereadRules(true);
//                        displayObject->setNeedsSurface(true);
//                        displayObject->setDirty(true);
//                        
//                        //if it is a container it may have children
//                        if(displayObject->isA(HSSObjectTypeContainer)){
//                            HSSContainer::p container = boost::static_pointer_cast<HSSContainer>(displayObject);
//                            for (i=0; i<container->children.size(); i++) { //FIXME: container will have an accessor for chlidrenSize in the future
//                                for (j=0; j<rule->childrenSize(); j++) {
//                                    HSSRule::p childRule = rule->childrenGet(j);
//                                    this->recursiveMatchRulesToDisplayObjects(childRule, container->children);
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        } else {
//            const HSSParserNode::p & selector2 = selectorChain->get(selectorCount++);
//            HSSParserNodeType selectorType = selector2->getType();
//            switch (selectorType) {
//                case HSSParserNodeTypeCombinator:
//                {
//                    HSSCombinator::p combinator = boost::static_pointer_cast<HSSCombinator>(selector2);
//                    const HSSCombinatorType & combinatorType = combinator->getCombinatorType();
//                    switch (combinatorType) {
//                        case HSSCombinatorTypeNextSiblings:
//                            <#statements#>
//                            break;
//                            
//                        default:
//                            break;
//                    }
//                    break;
//                }
//                    
//                default:
//                    break;
//            }
//        }
//    }
//    
//    
//    
//    //select items
//    for (i=0, size=scope.size(); i<size; i++) {
//        const HSSDisplayObject::p & displayObject = scope[i];
//        
//        
//        
//        //in case we're dealing with a displayObject which cannot have children
//        //we shortcut here
//        if(selectorChain->size() > 1 && !displayObject->isA(HSSObjectTypeContainer)){
//            //we're done
//            continue;
//        }
//        
//        for (j=0, size=selectorChain->size(); j<size; j++) {
//            const HSSParserNode::p & currentNode = selectorChain->get(j);
//            if(currentNode->isA(HSSParserNodeTypeSelector)){
//                HSSSelector::p simpleSelector = boost::static_pointer_cast<HSSSelector>(currentNode);
//                
//                //match the selector
//                if (simpleSelector->getElementName() == displayObject->getElementName()) {
//                    tempSelection.push_back(displayObject);
//                }
//            }
//        }
//    }
}

bool AXRController::readNextSelectorNode()
{
    if(this->currentChainCount < this->currentChainSize-1){
        this->currentChainCount++;
        this->currentSelectorNode = this->currentChain->get(this->currentChainCount);
        return true;
    } else {
        return false;
    }
}

const std::vector<HSSDisplayObject::p> AXRController::selectHierarchical(const std::vector<HSSDisplayObject::p> & scope)
{
    std::vector<HSSDisplayObject::p> ret = this->selectOnLevel(scope);
//    unsigned i, size;
//    for (i=0, size=left.size(); i<size; i++) {
//        
//    }
    return ret;
}

const std::vector<HSSDisplayObject::p> AXRController::selectOnLevel(const std::vector<HSSDisplayObject::p> & scope)
{
    std::vector<HSSDisplayObject::p> left = this->selectSimple(scope);
    //only continue if it actually found anything
    if(left.size() > 0){
        if(this->readNextSelectorNode()){
            std::vector<HSSDisplayObject::p> ret;
            if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator))
            {
                HSSCombinator::p combinator = boost::static_pointer_cast<HSSCombinator>(this->currentSelectorNode);
                HSSCombinatorType combinatorType = combinator->getCombinatorType();
                switch (combinatorType) {
                    case HSSCombinatorTypeSiblings:
                    {
                        //if found, just select the right part
                        this->readNextSelectorNode();
                        ret = this->selectSimple(scope);
                        break;
                    }
                        
                    case HSSCombinatorTypeNextSiblings:
                    {
                        //find the selected ones on the left, and select all the following ones that match
                        std::vector<HSSDisplayObject::p> vect(scope);
                        std::vector<HSSDisplayObject::p>::iterator it;
                        it = std::find(vect.begin(), vect.end(), left.front());
                        std::vector<HSSDisplayObject::p> right(it+1, vect.end());
                        this->readNextSelectorNode();
                        ret = this->selectSimple(right);
                        break;
                    }
                        
                    case HSSCombinatorTypePreviousSiblings:
                    {
                        //find the selected ones on the left, and select all the previous ones that match
                        std::vector<HSSDisplayObject::p> vect(scope);
                        std::vector<HSSDisplayObject::p>::iterator it;
                        it = std::find(vect.begin(), vect.end(), left.back());
                        std::vector<HSSDisplayObject::p> right(vect.begin(), it);
                        this->readNextSelectorNode();
                        ret = this->selectSimple(right);
                        break;
                    }
                        
                    default:
                        break;
                }
            }
            
            return ret;
        }
        
    }
    return left;
}

const std::vector<HSSDisplayObject::p> AXRController::selectSimple(const std::vector<HSSDisplayObject::p> & scope)
{
    std::vector<HSSDisplayObject::p> ret;
    unsigned i, size;
    HSSParserNodeType selectorType = this->currentSelectorNode->getType();
    switch (selectorType) {
        case HSSParserNodeTypeSelector:
        {
            //select only elements with matching element name
            HSSSelector::p selector = boost::static_pointer_cast<HSSSelector>(this->currentSelectorNode);
            for (i=0, size=scope.size(); i<size; i++) {
                if(scope[i]->getElementName() == selector->getElementName()){
                    ret.push_back(scope[i]);
                }
            }
            break;
        }
        
        case HSSParserNodeTypeUniversalSelector:
        {
            //the entire scope will be selected
            ret = scope;
            break;
        }
        
        default:
            throw "unexpected selector node type";
            break;
    }
    
    return ret;
}


bool AXRController::reload()
{
    if(this->_hasLoadedFile){
        std::string filepath = this->parserXML->getFilePath();
        std::string filename = this->parserXML->getFileName();
        return this->loadFile(filepath, filename);
    }
    
    return false;
}

std::string AXRController::toString()
{
    std::string tempstr;
    if(this->root != NULL){
        tempstr = this->root->toString();  
    }
    unsigned i;
    if(this->objectTree.size() > 0)
    {
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
    //the operator -> is very important here
    this->parserHSS->reset();
    this->parserXML = XMLParser::p(new XMLParser(this));
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

void AXRController::setRoot(HSSContainer::p & newRoot){
    this->root = newRoot;
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

void AXRController::exitElement()
{
    //std_log1("exit element");
    this->currentContext.pop();
}


void AXRController::add(HSSContainer::p & newContainer)
{
    if(!this->root){
        this->root = newContainer;
    } else {
        if(this->currentContext.size() != 0){
            HSSContainer::p theCurrent = this->currentContext.top();
            theCurrent->add(newContainer);
            
        } else {
            throw "tried to add a container to nonexistent current";
        }
    }
}

//object tree

void AXRController::objectTreeAdd(HSSObject::p & newObject)
{
    this->objectTree.push_back(newObject);
}

void AXRController::objectTreeRemove(unsigned index)
{
    this->objectTree.erase(this->objectTree.begin()+index);
}

HSSObject::p & AXRController::objectTreeGet(unsigned index)
{
    return this->objectTree[index];
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







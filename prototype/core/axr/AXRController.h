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
 *      Revision: 16
 *
 ********************************************************************/

#ifndef AXRCONTROLLER_H
#define AXRCONTROLLER_H

#include <vector>
#include <string>
#include <stack>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../hss/objects/HSSObject.h"
#include "../hss/objects/HSSContainer.h"
#include "../hss/parsing/HSSParser.h"
#include "../xml/XMLParser.h"
#include "../os/platform.h"

namespace AXR {
    
    class AXRRender;
    
    class AXRController
    {
    public:
        typedef boost::shared_ptr<AXRController> p;
        
        AXRController();
        virtual ~AXRController();
        virtual std::string toString();
        
        void setRender(AXRRender * render);
        
        //use this to clean up and start from fresh
        void reset();
        
        //tells wether a file has been loaded or not
        bool hasLoadedFile();
        
        bool loadFile();
        bool loadFile(std::string xmlfilepath, std::string xmlfilename);
        bool loadFileHSS(std::string hssfilepath, std::string hssfilename);
        bool reload();
        bool loadXMLFile(std::string filepath, std::string filename);
        bool loadHSSFile(std::string filepath, std::string filename);
        
        HSSContainer::p & getRoot();
        void setRoot(HSSContainer::p newRoot);
        
        void enterElement(std::string elementName);
        void addAttribute(std::string name, std::string value);
        void setContentText(std::string text);
        void appendContentText(std::string text);
        void exitElement();
        
        void add(HSSDisplayObject::p newContainer);
        
        void objectTreeAdd(HSSObjectDefinition::p & newObject);
        void objectTreeRemove(unsigned index);
        HSSObjectDefinition::p & objectTreeGet(unsigned index);
        HSSObjectDefinition::p & objectTreeGet(std::string name);
        
        void loadSheetsAdd(std::string sheet);
        void loadSheetsRemove(unsigned index);
        std::string loadSheetsGet(unsigned index);
        
        const std::vector<HSSStatement::p>& getStatements() const;
        void statementsAdd(HSSStatement::p & statement);
        void statementsRemove(unsigned index);
        HSSStatement::p & statementsGet(unsigned index);
        
        const std::vector<HSSRule::p>& getRules() const;
        void rulesAdd(HSSRule::p & statement);
        void rulesRemove(unsigned index);
        HSSRule::p & rulesGet(unsigned index);
        unsigned const rulesSize();
        
        void setSelectorChain(HSSSelectorChain::p selectorChain);
        std::vector< std::vector<HSSDisplayObject::p> > selectHierarchical(const std::vector<HSSDisplayObject::p> & scope);
        std::vector< std::vector<HSSDisplayObject::p> > selectAllHierarchical(const std::vector<HSSDisplayObject::p> & scope);
        std::vector< std::vector<HSSDisplayObject::p> > selectOnLevel(const std::vector<HSSDisplayObject::p> & scope);
        std::vector< std::vector<HSSDisplayObject::p> > selectSimple(const std::vector<HSSDisplayObject::p> & scope);
        std::vector< std::vector<HSSDisplayObject::p> > filterSelection(std::vector< HSSDisplayObject::p> &selection);
        
        std::string basepath;
        
        void recursiveMatchRulesToDisplayObjects(HSSRule::p & rule, const std::vector<HSSDisplayObject::p> & scope, HSSContainer::p container);
        
    protected:
        AXRRender * _render;
        
        std::stack<HSSContainer::p>currentContext;
        HSSContainer::p root;
        
        std::vector<HSSObjectDefinition::p>objectTree;
        std::vector<std::string>loadSheets;
        std::vector<HSSStatement::p>statements;
        std::vector<HSSRule::p>rules;
        XMLParser::p parserXML;
        HSSParser::p parserHSS;
        OSHelper::p osHelper;
        
        bool _hasLoadedFile;
        bool _isHSSOnly;
        
        HSSSelectorChain::p currentChain;
        HSSParserNode::p currentSelectorNode;
        unsigned currentChainCount;
        unsigned currentChainSize;
        void readNextSelectorNode();
        bool isAtEndOfSelector();
        
    };
}



#endif //AXRCONTROLLER_H
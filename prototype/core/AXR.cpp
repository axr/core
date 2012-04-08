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
 *      Last changed: 2012/03/21
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 7
 *
 ********************************************************************/

#include "AXR.h"

using namespace AXR;



AXRCore::p & AXRCore::getInstance()
{
    static AXRCore::p theInstance;
    if(!theInstance)
        theInstance = AXRCore::p(new AXRCore());
    
    return theInstance;
}

AXRCore::AXRCore()
{
    
}

void AXRCore::initialize(AXRWrapper * wrpr)
{
    AXRController::p ctrlr = AXRController::p(new AXRController(this));
    AXRRender::p rndr = AXRRender::p(new AXRRender(ctrlr.get(), this));
    this->setWrapper(wrpr);
    this->setController(ctrlr);
    this->setRender(rndr);
    this->setParserXML(XMLParser::p(new XMLParser(ctrlr.get())));
    this->setParserHSS(HSSParser::p(new HSSParser(ctrlr.get(), wrpr)));
    this->_hasLoadedFile = false;
}

AXRCore::~AXRCore()
{
    
}

void AXRCore::setCairo(cairo_t * cr) {
    this->render->setCairo(cr);
}

cairo_t * AXRCore::getCairo() { return this->render->getCairo(); }

void AXRCore::drawInRectWithBounds(AXRRect rect, AXRRect bounds)
{
    if(this->render){
        this->render->drawInRectWithBounds(rect, bounds);
    }
}

void AXRCore::run()
{
    //check for wrapper
    if(this->getWrapper() == NULL){
        AXRError::p(new AXRError("AXRCore", "The wrapper was not defined"))->raise();
        return;
    }
    bool loadingSuccess = this->parserXML->loadFile(this->file);
    if(!loadingSuccess){
        AXRError::p(new AXRError("AXRCore", "Could not load the XML file"))->raise();
    } else {
        //needs reset on next load
        this->_hasLoadedFile = true;
        
        HSSContainer::p root = boost::static_pointer_cast<HSSContainer>(this->controller->getRoot());
        unsigned i, size, j, size2;
        std::string hssfilename, hssfilepath;
        
        std::vector<std::string> loadSheets = this->controller->loadSheetsGet();
        for (i=0, size = loadSheets.size(); i<size; i++) {
            
            hssfilename = loadSheets[i];
            if(hssfilename.substr(0,7) == "file://"){
                hssfilepath = hssfilename;
            } else if(hssfilename.substr(0,7) == "http://"){
                AXRError::p(new AXRError("AXRCore", "HTTP has not been implemented yet"))->raise();
            } else if(hssfilename.substr(0,1) == "/"){
                hssfilepath = this->file->basePath + hssfilename;
            } else {
                hssfilepath = "file://"+this->file->basePath +"/"+ hssfilename;
            }
            AXRFile::p hssfile;
            try {
                hssfile = this->wrapper->getFile(hssfilepath);
            } catch (AXRError::p e) {
                e->raise();
                continue;
            }
            
            this->parserHSS->setBasePath(this->file->basePath);
            if(! this->parserHSS->loadFile(hssfile)){
                AXRError::p(new AXRError("AXRCore", "Could not load the HSS file"))->raise();
            } else {
                //assign the rules to the objects
                
                const std::vector<HSSRule::p> rules = this->controller->getRules();
                for (j=0, size2=rules.size(); j<size2; j++) {
                    const HSSRule::p& rule = rules[j];
                    const HSSDisplayObject::p rootScopeArr[1] = {root};
                    const std::vector<HSSDisplayObject::p>rootScope(rootScopeArr, rootScopeArr+1);
                    this->controller->recursiveMatchRulesToDisplayObjects(rule, rootScope, root, true);
                }
                root->setNeedsRereadRules(true);
            }
        }
        if (root) {
            root->recursiveReadDefinitionObjects();
            root->handleEvent(HSSEventTypeLoad, NULL);
        }
    }
}

void AXRCore::reset()
{
    this->controller->reset();
    this->parserHSS->reset();
    this->render->reset();
    this->parserXML.reset();
    this->parserXML = XMLParser::p(new XMLParser(this->controller.get()));
    this->_hasLoadedFile = false;
    this->file.reset();
}

//has loaded file
bool AXRCore::hasLoadedFile()
{
    return _hasLoadedFile;
}

AXRWrapper * AXRCore::getWrapper() { return this->wrapper; }
void AXRCore::setWrapper(AXRWrapper * wrapper) { this->wrapper = wrapper; }

AXRController::p AXRCore::getController() { return this->controller; }
void AXRCore::setController(AXRController::p controller) { this->controller = controller; }

AXRRender::p AXRCore::getRender() { return this->render; }
void AXRCore::setRender(AXRRender::p render) { this->render = render; }

AXRFile::p AXRCore::getFile() { return this->file; }
void AXRCore::setFile(AXRFile::p file) { this->file = file; }

XMLParser::p AXRCore::getParserXML() { return this->parserXML; }
void AXRCore::setParserXML(XMLParser::p parser) { this->parserXML = parser; }

HSSParser::p AXRCore::getParserHSS() { return this->parserHSS; }
void AXRCore::setParserHSS(HSSParser::p parser) { this->parserHSS = parser; }




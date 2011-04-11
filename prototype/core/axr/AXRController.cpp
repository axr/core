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
 *      Last changed: 2011/04/11
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 4
 *
 ********************************************************************/

#include "AXRController.h"
#include "AXRExceptions.h"
#include "../AXR.h"
#include <sstream>

using namespace AXR;

//use this factory method to instantiate the controller
AXRController::p AXRController::create()
{
    AXRController::p inst(new AXRController);
    inst->parserXML = XMLParser::p(new XMLParser(XMLParser::controllerPointer(AXRController::p(inst))));
    return inst;
}

AXRController::AXRController()
{
    this->parserHSS = HSSParser::p(new HSSParser());
    this->osHelper = OSHelper::p(new OSHelper());
}

AXRController::~AXRController()
{
    this->objectTree.clear();
    this->loadSheets.clear();
}

void AXRController::loadFile()
{
    std::string xmlfilepath = std::string();
    std::string xmlfilename = std::string();
    
    std::string hssfilepath = std::string();
    std::string hssfolder = std::string();
    std::string hssfilename = std::string();
    
    unsigned i;
    
    bool success = this->osHelper->openFileDialog(xmlfilepath, xmlfilename);
    if(success){
        this->loadXMLFile(xmlfilepath, xmlfilename);
        //load the obtained stylesheets
        if(this->loadSheets.size() == 0){
            throw AXRNoStylesheetsException(xmlfilepath);
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
                    
                    this->loadHSSFile(hssfilepath, hssfilename.substr(hssfilename.rfind("/", hssfilename.length())+1));
                }
            }
        }
    }
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
    
    return tempstr;
}


void AXRController::reset()
{
    this->objectTree.clear();
    this->loadSheets.clear();
    this->root.reset();
    this->current.reset();
    //the operator -> is very important here
    this->parserHSS->reset();
}


void AXRController::loadXMLFile(std::string filepath, std::string filename)
{
    this->parserXML->loadFile(filepath, filename);
    std_log1(this->toString());
}

void AXRController::loadHSSFile(std::string filepath, std::string filename)
{
    std_log1(std::string("******************************************************************\n* opening document:\n* ").append(filepath).append("\n******************************************************************"));
    this->parserHSS->loadFile(filepath);
}

HSSContainer::p AXRController::getRoot()
{
    return this->root;
}

void AXRController::setRoot(HSSContainer::p newRoot){
    this->root = newRoot;
}

void AXRController::add(HSSContainer::p newContainer)
{
    if(!this->root){
        this->root = newContainer;
    } else {
        std_log1(this->root->name);
        if(this->current){
            this->current->add(newContainer);
        } else {
            throw "tried to add a container to nonexistent current";
        }
    }
}

HSSContainer::p AXRController::getCurrent()
{
    return this->current;
}

void AXRController::setCurrent(HSSContainer::p newCurrent)
{
    this->current = newCurrent;
}

void AXRController::objectTreeAdd(HSSObject::p newObject)
{
    this->objectTree.push_back(newObject);
}

void AXRController::objectTreeRemove(unsigned index)
{
    this->objectTree.erase(this->objectTree.begin()+index);
}

HSSObject::p AXRController::objectTreeGet(unsigned index)
{
    return this->objectTree[index];
}


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







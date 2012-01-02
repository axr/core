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
 *      Last changed: 2011/11/05
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 1
 *
 ********************************************************************/

#import "HSSRequest.h"
#import "../../axr/AXRController.h"
#import "../parsing/HSSFunctionCall.h"
#import "HSSSelFunction.h"

using namespace AXR;

HSSRequest::HSSRequest()
: HSSAction(HSSActionTypeRequest)
{
    this->observedSrc
    = this->observedTarget
    = NULL;
    
    this->mode = HSSRequestModeTypeAuto;
}

HSSRequest::~HSSRequest()
{
    
}

std::string HSSRequest::toString()
{
    return "HSSRequest";
}

std::string HSSRequest::defaultObjectType()
{
    return "request";
}

void HSSRequest::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertySrc:
            this->setDSrc(value);
            break;
            
        case HSSObservablePropertyTarget:
            this->setDTarget(value);
            break;
            
//        case HSSObservablePropertyMode:
//            this->setDMode(value);
//            break;
//            
//        case HSSObservablePropertyContentTarget:
//            this->setDContentTarget(value);
            
        default:
            HSSAction::setProperty(name, value);
            break;
    }
}


void HSSRequest::fire()
{
    
    std_log("----------------------------------\nFiring request: loading file "+this->axrController->basepath+this->src+"\n----------------------------------\n");
    //if there is no target
    if (this->target.size() == 0) {
        this->axrController->loadFile(this->axrController->basepath+this->src, this->src);
    } else {
        switch (this->mode) {
            default:
            {
                AXRController fileController;
                bool loadingSuccess = fileController.loadFile(this->axrController->basepath+this->src, this->src);
                if(loadingSuccess){
                    unsigned i, size;
                    for (i=0, size=this->target.size(); i<size; i++) {
                        std_log1("Adding loaded file to target");
                        
                        if(this->target[i]->isA(HSSObjectTypeContainer)){
                            const HSSContainer::p & theCont = boost::static_pointer_cast<HSSContainer>(this->target[i]);
                            const HSSContainer::p & loadedRoot = fileController.getRoot();
                            theCont->add(loadedRoot);
                            
                            unsigned j, k, size2, size3;
                            const std::vector<HSSDisplayObject::p> &scope = theCont->getChildren();
                            for (j=0, size2=fileController.rulesSize(); j<size2; j++) {
                                HSSRule::p & theRule = fileController.rulesGet(j);
                                theRule->childrenAdd(theRule);
                            }
                            for (j=0, size2=theCont->rulesSize(); j<size2; j++) {
                                HSSRule::p theRule = theCont->rulesGet(j);
                                for (k=0, size3=theRule->childrenSize(); k<size3; k++) {
                                    HSSRule::p childRule = theRule->childrenGet(k);
                                    this->axrController->recursiveMatchRulesToDisplayObjects(childRule, scope, theCont);
                                }
                            }
                            
                            theCont->recursiveReadDefinitionObjects();
                            theCont->setNeedsLayout(true);
                        }
                    }
                }
                break;
            }
        }
    }
}


HSSParserNode::p HSSRequest::getDSrc() { return this->dSrc; }
void HSSRequest::setDSrc(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
        case HSSParserNodeTypeStringConstant:
        {
            this->dSrc = value;
            if(this->observedSrc != NULL)
            {
                this->observedSrc->removeObserver(this->observedSrcProperty, HSSObservablePropertySrc, this);
            }
            
            switch (value->getType()) {
                case HSSParserNodeTypeKeywordConstant:
                    //FIXME: what here?
                    break;
                    
                case HSSParserNodeTypeStringConstant:
                {
                    HSSStringConstant::p theString = boost::static_pointer_cast<HSSStringConstant>(value);
                    this->src = theString->getValue();
                    break;
                }
                
                default:
                    break;
            }
            
            
            
            this->notifyObservers(HSSObservablePropertySrc, &this->src);
            
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSRequest", "Invalid value for src of "+this->name));
    }
}

void HSSRequest::srcChanged(AXR::HSSObservableProperty source, void *data)
{
    AXRWarning::p(new AXRWarning("HSSRequest", "unimplemented"))->raise();
}

HSSParserNode::p HSSRequest::getDTarget() { return this->dTarget; }
void HSSRequest::setDTarget(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
        {
            this->dTarget = value;
            if(this->observedTarget != NULL)
            {
                this->observedTarget->removeObserver(this->observedTargetProperty, HSSObservablePropertyTarget, this);
            }
            
            switch (value->getType()) {
                case HSSParserNodeTypeKeywordConstant:
                    //FIXME: what here?
                    break;
                    
                case HSSParserNodeTypeFunctionCall:
                {
                    HSSFunctionCall::p fcall = boost::static_pointer_cast<HSSFunctionCall>(value);
                    HSSFunction::p fnct = fcall->getFunction();
                    if(fnct){
                        fnct->setScope(this->scope);
                        std::vector< std::vector<HSSDisplayObject::p> > selection = *(std::vector< std::vector<HSSDisplayObject::p> >*) fnct->evaluate();
                        unsigned i, size;
                        for (i=0, size=selection.size(); i<size; i++) {
                            std::vector<HSSDisplayObject::p> inner = selection[i];
                            this->target.insert(this->target.end(), inner.begin(), inner.end());
                        }
                        
                        //FIXME: potentially leaking
                        fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTarget, this, new HSSValueChangedCallback<HSSRequest>(this, &HSSRequest::targetChanged));
                        this->observedTarget = fnct.get();
                        this->observedTargetProperty = HSSObservablePropertyValue;
                    }
                    
                    break;
                }
                    
                default:
                    break;
            }
            
            
            
            this->notifyObservers(HSSObservablePropertySrc, &this->src);
            
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSRequest", "Invalid value for src of "+this->name));
    }
}

void HSSRequest::targetChanged(AXR::HSSObservableProperty source, void *data)
{
    AXRWarning::p(new AXRWarning("HSSRequest", "unimplemented"))->raise();
}









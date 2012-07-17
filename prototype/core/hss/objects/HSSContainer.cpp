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
 ********************************************************************/

#include "HSSContainer.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../../axr/AXRController.h"
#include "../parsing/HSSObjectDefinition.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include "../parsing/HSSObjectNameConstant.h"
#include "../parsing/HSSFunction.h"
#include "../../AXR.h"
#include <map>
#include <string>
#include <sstream>
#include <boost/pointer_cast.hpp>
#include <cmath>
#include "HSSShapes.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <algorithm>

using namespace AXR;


HSSDisplayObject::p HSSContainer::asDisplayObject(HSSContainer::p theContainer)
{
    return boost::static_pointer_cast<HSSDisplayObject>(theContainer);
}

//always test the return of this function. E.g. "HSSContainer::p cont = HSSContainer::asContainer(myDO); if(cont) { /* ... */ }
HSSContainer::p HSSContainer::asContainer(HSSDisplayObject::p theDisplayObject)
{
    HSSContainer::p ret;
    if (theDisplayObject->isA(HSSObjectTypeContainer)) {
        ret = boost::static_pointer_cast<HSSContainer>(theDisplayObject);
    }
    return ret;
}

HSSContainer::HSSContainer()
: HSSDisplayObject(HSSObjectTypeContainer)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: creating container");
    this->initialize();
}

void HSSContainer::initialize()
{
    this->contentText = std::string();
    
    this->contentAlignX = this->contentAlignY = 0;
    this->directionPrimary = HSSDirectionLeftToRight;
    this->directionSecondary = HSSDirectionTopToBottom;
    
    this->observedContentAlignX = this->observedContentAlignY
    = this->observedDirectionPrimary = this->observedDirectionSecondary
    = this->observedShape = this->observedTextAlign
    = NULL;
    
    this->distributeX = distributeXLinear
    = this->distributeY = distributeYLinear
    = false;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("isA");
    shorthandProperties.push_back("width");
    shorthandProperties.push_back("height");
    shorthandProperties.push_back("contentAlignX");
    shorthandProperties.push_back("contentAlignY");
    shorthandProperties.push_back("font");
    shorthandProperties.push_back("background");
    shorthandProperties.push_back("shape");
    shorthandProperties.push_back("textAlign");
    
    this->setShorthandProperties(shorthandProperties);
    
    this->registerProperty(HSSObservablePropertyContentAlignX, (void *) &this->contentAlignX);
    this->registerProperty(HSSObservablePropertyContentAlignY, (void *) &this->contentAlignY);
    this->registerProperty(HSSObservablePropertyDirectionPrimary, (void *) &this->directionPrimary);
    this->registerProperty(HSSObservablePropertyDirectionSecondary, (void *) &this->directionSecondary);
    this->registerProperty(HSSObservablePropertyShape, (void *) &this->shape);
    this->registerProperty(HSSObservablePropertyTextAlign, (void *) &this->textAlign);
}

HSSContainer::HSSContainer(const HSSContainer & orig)
: HSSDisplayObject(orig)
{
    this->initialize();
}

HSSContainer::p HSSContainer::clone() const{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: cloning container");
    return boost::static_pointer_cast<HSSContainer, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSContainer::cloneImpl() const{
    return HSSClonable::p(new HSSContainer(*this));
}

HSSContainer::~HSSContainer()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: destructing container");
    this->children.clear();
    this->allChildren.clear();
}

std::string HSSContainer::toString()
{
    std::string tempstr;
    unsigned i;
    std::map<std::string, std::string>::iterator it;
    
    if (this->isNamed()) {
        tempstr = std::string("HSSContainer: ").append(this->name);
    } else {
        tempstr = "Annonymous HSSContainer";
    }
    
    if (this->attributes.size() > 0) {
        inc_output_indent();
        tempstr.append("\n").append(output_indent("with the following attributes:"));
        for(it=this->attributes.begin(); it!=this->attributes.end(); it++)
        {
            tempstr.append("\n").append(output_indent("- ").append((*it).first).append(": ").append((*it).second));
        }
        dec_output_indent();
        tempstr.append("\n");
    }
    
    if(this->children.size() > 0){
        inc_output_indent();
        tempstr.append("\n").append(output_indent("with the following children objects:"));
        for (i=0; i<this->children.size(); i++) {
            tempstr.append("\n").append(output_indent(this->children[i]->toString()));
        }
        dec_output_indent();
        tempstr.append("\n");
    }
    
    return tempstr;
}

std::string HSSContainer::defaultObjectType()
{
    return "container";
}

std::string HSSContainer::defaultObjectType(std::string property)
{
    if (property == "shape"){
        return "roundedRect";
    } else if (property == "innerMargin"){
        return "margin";
    } else {
        return HSSDisplayObject::defaultObjectType(property);
    }
}

bool HSSContainer::isKeyword(std::string value, std::string property)
{
    if (property == "contentAlignX" || property == "contentAlignY") {
        if (   value == "center"
            || value == "middle"
            || value == "top"
            || value == "bottom"
            || value == "left"
            || value == "right"
            || value == "distribute"
            || value == "linear"){
            return true;
        }
    } else if (property == "directionPrimary" || property == "directionSecondary"){
        if (   value == "leftToRight"
            || value == "rightToLeft"
            || value == "topToBottom"
            || value == "bottomToTop"){
            return true;
        }
    } else if (property == "textAlign"){
        if (   value == "left"
            || value == "right"
            || value == "center"
            || value == "justify"){
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSDisplayObject::isKeyword(value, property);
}

void HSSContainer::add(HSSDisplayObject::p child)
{
    HSSContainer::p sharedThis = this->shared_from_this();
    child->setParent(sharedThis);
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: added child "+child->getElementName()+" to "+this->getElementName());
    child->setIndex(this->allChildren.size());
    child->setController(this->getController());
    if(!child->isA(HSSObjectTypeTextBlock)){
        this->children.push_back(child);
    }
    this->allChildren.push_back(child);
    this->notifyObservers(HSSObservablePropertyTreeChange, this);
}

void HSSContainer::remove(unsigned index)
{
    HSSDisplayObject::it it;
    for (it=this->children.begin(); it!=this->children.end(); it++) {
        HSSDisplayObject::p child = *it;
        if(child->getIndex() == index){
            this->children.erase(it);
        }
    }
    for (it=this->allChildren.begin(); it!=this->allChildren.end(); it++) {
        HSSDisplayObject::p child = *it;
        if(child->getIndex() == index){
            this->allChildren.erase(it);
        }
    }
    this->resetChildrenIndexes();
    this->notifyObservers(HSSObservablePropertyTreeChange, this);
}

void HSSContainer::resetChildrenIndexes()
{
    unsigned i,size;
    for (i=0, size=this->allChildren.size(); i<size; i++) {
        this->allChildren[i]->setIndex(i);
    }
}

void HSSContainer::setContentText(std::string text)
{
    boost::algorithm::trim(text);
    if (text != "") {
        if (this->allChildren.size() == 0) {
            HSSTextBlock::p txtBlck = HSSTextBlock::p(new HSSTextBlock());
            txtBlck->setDText(HSSStringConstant::p(new HSSStringConstant(text)));
            this->add(txtBlck);
        } else {
            HSSDisplayObject::p lastChild = this->allChildren.back();
            if (lastChild->isA(HSSObjectTypeTextBlock)){
                HSSTextBlock::p textBlock = boost::static_pointer_cast<HSSTextBlock>(lastChild);
                textBlock->setDText(HSSStringConstant::p(new HSSStringConstant(text)));
            } else {
                HSSTextBlock::p txtBlck = HSSTextBlock::p(new HSSTextBlock());
                txtBlck->setDText(HSSStringConstant::p(new HSSStringConstant(text)));
                this->add(txtBlck);
            }
        }
    }
}

void HSSContainer::appendContentText(std::string text)
{
    boost::algorithm::trim(text);
    if (text != "") {
        if (this->allChildren.size() == 0) {
            HSSTextBlock::p txtBlck = HSSTextBlock::p(new HSSTextBlock());
            txtBlck->setDText(HSSStringConstant::p(new HSSStringConstant(text)));
            this->add(txtBlck);
        } else {
            HSSDisplayObject::p lastChild = this->allChildren.back();
            if (lastChild->isA(HSSObjectTypeTextBlock)){
                HSSTextBlock::p textBlock = boost::static_pointer_cast<HSSTextBlock>(lastChild);
                textBlock->setDText(HSSStringConstant::p(new HSSStringConstant(textBlock->getText() + " " + text)));
            } else {
                HSSTextBlock::p txtBlck = HSSTextBlock::p(new HSSTextBlock());
                txtBlck->setDText(HSSStringConstant::p(new HSSStringConstant(text)));
                this->add(txtBlck);
            }
        }
    }
}

std::string HSSContainer::getContentText()
{
    std_log("whateva");
    return "bla";
}

void HSSContainer::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyContentAlignX:
            this->setDContentAlignX(value);
            break;
        case HSSObservablePropertyContentAlignY:
            this->setDContentAlignY(value);
            break;
        case HSSObservablePropertyDirectionPrimary:
            this->setDDirectionPrimary(value);
            break;
        case HSSObservablePropertyDirectionSecondary:
            this->setDDirectionSecondary(value);
            break;
        case HSSObservablePropertyShape:
            this->setDShape(value);
            break;
            
        case HSSObservablePropertyTextAlign:
            this->setDTextAlign(value);
            break;
            
        default:
            HSSDisplayObject::setProperty(name, value);
            break;
    }
}

void HSSContainer::setProperty(HSSObservableProperty name, void * value)
{
    switch (name) {
        case HSSObservablePropertyContentAlignX:
            this->contentAlignX = *(long double*) value;
            break;
        case HSSObservablePropertyContentAlignY:
            this->contentAlignY = *(long double*) value;
            break;
        case HSSObservablePropertyDirectionPrimary:
            this->directionPrimary = *(HSSDirectionValue*) value;
            break;
        case HSSObservablePropertyDirectionSecondary:
            this->directionSecondary = *(HSSDirectionValue*) value;
            break;
        case HSSObservablePropertyShape:
            this->shape = *(HSSShape::p*) value;
            break;
        case HSSObservablePropertyTextAlign:
            this->textAlign = *(HSSTextAlignType *) value;
            break;
            
        default:
            return HSSDisplayObject::setProperty(name, value);
    }
    
    this->notifyObservers(name, value);
}

void HSSContainer::recursiveReadDefinitionObjects()
{
    this->readDefinitionObjects();
    unsigned i;
    for (i=0; i<this->allChildren.size(); i++) {
        this->allChildren[i]->recursiveReadDefinitionObjects();
    }
}

void HSSContainer::recursiveRegenerateSurfaces()
{
    this->regenerateSurfaces();
    
    unsigned i, size;
    for (i=0, size = this->allChildren.size(); i<size; i++) {
        this->allChildren[i]->recursiveRegenerateSurfaces();
    }
}

void HSSContainer::recursiveDraw(cairo_t * cairo)
{
    this->draw(cairo);
    unsigned i, size;
    for (i=0, size = this->allChildren.size(); i<size; i++) {
        this->allChildren[i]->recursiveDraw(cairo);
    }
}

void HSSContainer::drawBackground()
{
    cairo_t * cairo = cairo_create(this->backgroundSurface);
    this->shape->draw(cairo, 0., 0., this->width, this->height);
    HSSDisplayObject::_drawBackground(cairo);
    cairo_destroy(cairo);
}

void HSSContainer::drawBorders()
{
    HSSBorder::it it;
    cairo_t * cairo = cairo_create(this->bordersSurface);
    long double total = 0., i = 0., offset = 0., correction = 0.;
    
    for (it=this->border.begin(); it!=this->border.end(); it++) {
        total += (*it)->getSize();
    }
    
    if ((int)total % 2){
        correction = 0.5;
    }
    
    for (it=this->border.begin(); it!=this->border.end(); it++) {
        HSSBorder::p theBorder = *it;
        long double theSize = theBorder->getSize();
        
        offset = (total/2) - i - (theSize / 2) + correction;
        
        this->shape->draw(cairo, this->borderBleeding + offset, this->borderBleeding + offset, this->width - offset*2, this->height - offset*2);
        theBorder->draw(cairo);
        
        i += theSize;
    }
    cairo_destroy(cairo);
}

//void HSSContainer::drawShape(cairo_t *cairo)
//{
//    switch (this->shape->getShapeType()) {
//        case HSSShapeTypeRectangle:
//            cairo_rectangle(cairo, 0., 0., this->width, this->height);
//            break;
//        case HSSShapeTypeCircle:
//        {
//            double long halfWidth = this->width * 0.5;
//            cairo_arc(cairo, halfWidth, this->height * 0.5, halfWidth, 0., 2*M_PI);
//            break;
//        }
//            
//        default:
//            throw AXRError::p(new AXRError("HSSContainer", "Unknown shape type"));
//            break;
//    }
//}

void HSSContainer::layout()
{
    bool done = false;
    if(this->allChildren.size() == 0) done = true;
        
    //reset flag
    this->_needsLayout = false;
    
    if(!done)
    {
        std::vector<displayGroup::p>primaryGroups;
        std::vector<displayGroup::p>secondaryGroups;
        unsigned i, size, j, k;
        //long double acc2 = 0;
        security_brake_init();
        AXRWrapper * wrapper = AXRCore::getInstance()->getWrapper();
        
        //bool secondaryIsHorizontal = (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft);
        
        //create groups and lines
        for (i=0, size = this->allChildren.size(); i<size; i++) {
            if(!done){
                HSSDisplayObject::p child = this->allChildren[i];
                //place it on the alignment point
                //horizontal
                if(child->_anchorXdefault){
                    child->x = child->alignX - (child->outerWidth/2) + child->leftMargin;
                } else {
                    child->x = child->alignX - child->anchorX;
                }
                
                //vertical
                if(child->_anchorXdefault){
                    child->y = child->alignY - (child->outerHeight/2) + child->topMargin;
                } else {
                    child->y = child->alignY - child->anchorY;
                }
                
                if(!child->getOverflow()){
                    if ((child->x + child->width + child->rightMargin) > (this->width - this->rightPadding)) child->x = (this->width - this->rightPadding) - (child->width + child->rightMargin);
                    if (child->x < child->leftMargin + this->leftPadding) child->x = child->leftMargin + this->leftPadding;
                    
                    if ((child->y + child->height + child->bottomMargin) > (this->height - this->bottomPadding)) child->y = (this->height - this->bottomPadding) - (child->height + child->bottomMargin);
                    if (child->y < child->topMargin + this->topPadding) child->y = child->topMargin + this->topPadding;
                }
                
                if(wrapper->showLayoutSteps()){
                    wrapper->nextLayoutTick();
                    wrapper->nextLayoutChild();
                    wrapper->breakIfNeeded();
                    if(wrapper->layoutStepDone()){
                        done = true;
                        break;
                    }
                }
                
                if(!done){
                    bool addedToGroup = false;
                    if(child->getFlow() == true){
                        if( i!=0 ) {
                            j = 0;
                            while (j<primaryGroups.size() && done == false) {
                                if(primaryGroups[j]->lines.size() == 0){
                                    displayGroup::p & currentPGroup = primaryGroups[j];
                                    addedToGroup = this->_addChildToGroupIfNeeded(child, currentPGroup, this->directionPrimary, false);
                                    if (!addedToGroup && currentPGroup->complete){
                                        //transform the current group into a line
                                        displayGroup::p newGroup = displayGroup::p(new displayGroup());
                                        newGroup->x = currentPGroup->x;
                                        newGroup->y = currentPGroup->y;
                                        newGroup->width = currentPGroup->width;
                                        newGroup->height = currentPGroup->height;
                                        newGroup->complete = false;
                                        newGroup->lines.push_back(currentPGroup);
                                        
                                        displayGroup::p newLine = displayGroup::p(new displayGroup());
                                        newLine->x = child->x - child->leftMargin;
                                        newLine->y = child->y - child->topMargin;
                                        newLine->width = child->outerWidth;
                                        newLine->height = child->outerHeight;
                                        newLine->complete = false;
                                        newLine->objects.push_back(child);
                                        newGroup->lines.push_back(newLine);
                                        
                                        primaryGroups[j] = newGroup;
                                        
                                        addedToGroup = true;
                                    }
                                    
                                    if(addedToGroup){
                                        k=0;
                                        while (k<primaryGroups.size()){
                                            if(k != j){
                                                displayGroup::p & otherPGroup = primaryGroups[k];
                                                bool merged = this->_mergeGroupsIfNeeded(otherPGroup, currentPGroup,  this->directionPrimary);
                                                if(merged){
                                                    primaryGroups.erase(primaryGroups.begin()+j);
                                                    j = k;
                                                } else {
                                                    k++;
                                                }
                                            } else {
                                                k++;
                                            }
                                        }
                                    }
                                    
                                    if(wrapper->showLayoutSteps()){
                                        wrapper->nextLayoutTick();
                                        wrapper->breakIfNeeded();
                                        if(wrapper->layoutStepDone()){
                                            done = true;
                                            break;
                                        }
                                    }
                                    
                                } else {
                                    displayGroup::p & currentPGroup = primaryGroups[j]->lines.back();
                                    addedToGroup = this->_addChildToGroupIfNeeded(child, currentPGroup, this->directionPrimary, false);
                                    if (!addedToGroup && currentPGroup->complete){
                                        //create new line
                                        displayGroup::p newLine = displayGroup::p(new displayGroup());
                                        newLine->x = child->x - child->leftMargin;
                                        newLine->y = child->y - child->topMargin;
                                        newLine->width = child->outerWidth;
                                        newLine->height = child->outerHeight;
                                        newLine->complete = false;
                                        newLine->objects.push_back(child);
                                        primaryGroups[j]->lines.push_back(newLine);
                                        
                                        addedToGroup = true;
                                    }
                                    
                                    if(addedToGroup){
                                        k=0;
                                        while (k<primaryGroups.size()){
                                            if(k != j){
                                                displayGroup::p & otherPGroup = primaryGroups[k];
                                                bool merged = this->_mergeGroupsIfNeeded(otherPGroup, currentPGroup,  this->directionPrimary);
                                                if(merged){
                                                    primaryGroups.erase(primaryGroups.begin()+j);
                                                    j = k;
                                                } else {
                                                    k++;
                                                }
                                            } else {
                                                k++;
                                            }
                                        }
                                    }
                                    
                                    if(wrapper->showLayoutSteps()){
                                        wrapper->nextLayoutTick();
                                        wrapper->breakIfNeeded();
                                        if(wrapper->layoutStepDone()){
                                            done = true;
                                            break;
                                        }
                                    }
                                }
                                
                                j++;
                                security_brake();
                            }
                        }
                        
                        if(!addedToGroup){
                            displayGroup::p newGroup = displayGroup::p(new displayGroup());
                            newGroup->x = child->x - child->leftMargin;
                            newGroup->y = child->y - child->topMargin;
                            newGroup->width = child->outerWidth;
                            newGroup->height = child->outerHeight;
                            newGroup->complete = false;
                            newGroup->objects.push_back(child);
                            primaryGroups.push_back(newGroup);
                        }
                    }
                    
                    
                }
            } else {
                break;
            }
        }
        
        if(!done){
            //distribute if necessary in the primary direction
            bool primaryIsHorizontal = (this->directionPrimary== HSSDirectionLeftToRight || this->directionPrimary == HSSDirectionRightToLeft);
            if ( (this->distributeX && primaryIsHorizontal) || (this->distributeY && !primaryIsHorizontal) ){
                std::vector<displayGroup::p>::iterator pgIt;
                for (pgIt=primaryGroups.begin(); pgIt!=primaryGroups.end(); pgIt++) {
                    displayGroup::p & pgGrp = *pgIt;
                    if(pgGrp->lines.size() == 0){
                        this->_distribute(pgGrp, this->directionPrimary);
                    } else {
                        std::vector<displayGroup::p>::iterator pgLineIt;
                        for (pgLineIt=pgGrp->lines.begin(); pgLineIt!=pgGrp->lines.end(); pgLineIt++) {
                            this->_distribute(*pgLineIt, this->directionPrimary);
                            if(wrapper->showLayoutSteps()){
                                wrapper->nextLayoutTick();
                                wrapper->breakIfNeeded();
                                if(wrapper->layoutStepDone()){
                                    done = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                }
            }
            
            security_brake_reset();
            
            std::vector<displayGroup::p>::iterator pgIt;
            for (pgIt=primaryGroups.begin(); pgIt!=primaryGroups.end(); pgIt++) {
                displayGroup::p theDG = *pgIt;
                if(theDG->lines.size() > 0){
                    displayGroup::p lineA, lineB, targetA, targetB;
                    std::vector<HSSDisplayObject::p>::iterator lineAIt, lineBIt;
                    
                    std::vector<displayGroup::p>::iterator pglIt = theDG->lines.begin();
                    lineA = *pglIt;
                    lineAIt = lineA->objects.begin();
                    
                    pglIt++;
                    lineB = *pglIt;
                    lineBIt = lineB->objects.begin();
                    displayGroup::p newGroup;
                    
                    this->_recursiveCreateSecondaryGroups(lineAIt, lineA->objects.end(), lineBIt, lineB->objects.end(), targetA, targetB, newGroup, pglIt, theDG->lines.end(), true, secondaryGroups, true, false);
                } else {
                    displayGroup::p newGroup = displayGroup::p(new displayGroup());
                    newGroup->lines.push_back(theDG);
                    newGroup->height = 0.;
                    std::vector<HSSDisplayObject::p>::iterator it;
                    for (it=theDG->objects.begin(); it!=theDG->objects.end(); it++) {
                        HSSDisplayObject::p & theDO = *it;
                        if(theDO->outerHeight > newGroup->height) newGroup->height = theDO->outerHeight;
                    }
                    secondaryGroups.push_back(newGroup);
                }
                
            } //for each primary group
            
        } //if !done
        
//        std::vector<displayGroup::p>::iterator linesIt;
//        for (linesIt=newGroup->lines.begin(); linesIt!=newGroup->lines.end(); linesIt++) {
//            newGroup->height += (*linesIt)->height;
//        }
        
        //sort biggest group first, smallest last
        this->_qs_sort(secondaryGroups, 0, secondaryGroups.size() - 1);
        
        std::vector<displayGroup::p>::iterator sgIt;
        bool first = true;
        for (sgIt=secondaryGroups.begin(); sgIt!=secondaryGroups.end(); sgIt++) {
            if(!this->_arrangeLines(*sgIt, this->directionSecondary, first)){
                break;
            }
            first = false;
        }
        
        
        //assign the globalX and globalY and clean up flags
        for(i=0, size = this->allChildren.size(); i<size; i++){
            HSSDisplayObject::p &child = this->allChildren[i];
            child->setGlobalX(round(this->globalX + child->x));
            child->setGlobalY(round(this->globalY + child->y));
            child->_layoutFlagIsInSecondaryGroup = false;
            child->_layoutFlagIsInSecondaryLine = false;
            child->_layoutFlagLockTop = false;
            child->_layoutFlagLockBottom = false;
        }
        
        if(this->heightByContent){
            long double maxHeight = 0.;
            
            for (i=0, size = secondaryGroups.size(); i<size; i++) {
                if(secondaryGroups[i]->height > maxHeight){
                    maxHeight = secondaryGroups[i]->height;
                }
            }
            if(size > 0){
                this->height = maxHeight + this->topPadding + this->bottomPadding;
                this->_setInnerDimensions();
                this->_setOuterDimensions();
                this->setNeedsSurface(true);
                this->setDirty(true);
                this->notifyObservers(HSSObservablePropertyHeight, &this->height);
            }
        }
    }
}

void HSSContainer::_recursiveCreateSecondaryGroups(
    std::vector<HSSDisplayObject::p>::iterator  lineAIt,
    std::vector<HSSDisplayObject::p>::iterator  lineAStopIt,
    std::vector<HSSDisplayObject::p>::iterator  lineBIt,
    std::vector<HSSDisplayObject::p>::iterator  lineBStopIt,
    displayGroup::p                             &targetA,
    displayGroup::p                             &targetB,
    displayGroup::p                             &newGroup,
    std::vector<displayGroup::p>::iterator      pglIt,
    std::vector<displayGroup::p>::iterator      pglStopIt,
    bool                                        addToSecondaryGroups,
    std::vector<displayGroup::p>                &secondaryGroups,
    bool                                        needsShoveling,
    bool                                        onlyAddToBIfNotInGroupYet
){
    bool lineADone = false;
    bool lineBDone = false;
    //bool needsShoveling = true;
    
    
    HSSDisplayObject::p objA, objB;
    std::vector<HSSDisplayObject::p>::iterator firstBIt = lineBIt;
    objA = *lineAIt;
    objB = *lineBIt;
    
    while(!lineADone || !lineBDone){
        if(this->_overlaps(objA, objB)){
            //add objA to first line in new group and objB into second line
        
            if(!targetA){
                targetA = displayGroup::p(new displayGroup());
                targetA->objects.push_back(objA);
                targetA->height = objA->outerHeight;
                objA->_layoutFlagIsInSecondaryLine = true;
                
            } else {
                if(!objA->_layoutFlagIsInSecondaryLine){
                    targetA->objects.push_back(objA);
                    objA->_layoutFlagIsInSecondaryLine = true;
                    long double currentHeight = targetA->height;
                    if(objA->outerHeight > currentHeight){
                        targetA->height = objA->outerHeight;
                        newGroup->height += currentHeight - targetA->height;
                    }
                }
            }
            if(!onlyAddToBIfNotInGroupYet || (onlyAddToBIfNotInGroupYet && !objB->_layoutFlagIsInSecondaryLine)){
                if(!targetB){
                    targetB = displayGroup::p(new displayGroup());
                    targetB->objects.push_back(objB);
                    targetB->height = objB->outerHeight;
                    objB->_layoutFlagIsInSecondaryLine = true;
                } else {
                    targetB->objects.push_back(objB);
                    objB->_layoutFlagIsInSecondaryLine = true;
                    long double currentHeight = targetB->height;
                    if(objB->outerHeight > currentHeight){
                        targetB->height = objB->outerHeight;
                        newGroup->height += currentHeight - targetB->height;
                    }
                }
            }
            
            if(addToSecondaryGroups){
                if(!newGroup){
                    newGroup = displayGroup::p(new displayGroup());
                    newGroup->height = 0.;
                    newGroup->lines.push_back(targetA);
                    newGroup->lines.push_back(targetB);
                    secondaryGroups.push_back(newGroup);
                    newGroup->height = targetA->height + targetB->height;
                }
            }
            
            //try to shovel in more objects of the first line
            if(needsShoveling){
                HSSDisplayObject::p firstObj = objA;
                std::vector<HSSDisplayObject::p>::iterator firstObjIt = lineAIt;
                bool shovelDone = false;
                while(!shovelDone){
                    lineAIt++;
                    if(lineAIt != lineAStopIt){
                        objA = *lineAIt;
                        
                        if( this->_overlaps(objA, objB)
                           && (objA->x + objA->outerWidth < objB->x + objB->outerWidth)
                           ){
                            targetA->objects.push_back(objA);
                            objA->_layoutFlagIsInSecondaryLine = true;
                            firstObj = objA;
                            firstObjIt = lineAIt;
                            long double currentHeight = targetA->height;
                            if(objA->outerHeight > currentHeight){
                                targetA->height = objA->outerHeight;
                                newGroup->height += currentHeight - targetA->height;
                            }
                            
                        } else {
                            shovelDone = true;
                        }
                    } else {
                        shovelDone = true;
                    }
                }
                needsShoveling = false;
                objA = firstObj;
                lineAIt = firstObjIt;
                
            } //needs shoveling
            
            //get next objB
            lineBIt++;
            if(lineBIt != lineBStopIt){
                objB = *lineBIt;
            } else {
                lineBDone = true;
            }
        //if no overlap
        } else {
            //get next objB
            lineBIt++;
            if(lineBIt != lineBStopIt){
                objB = *lineBIt;
            } else {
                lineBDone = true;
            }
        }
        
        if(lineBDone){
            std::vector<displayGroup::p>::iterator pglItStore = pglIt;
            pglIt++;
            if(pglIt!=pglStopIt){
                displayGroup::p nextLine = *pglIt;
                if(targetB){
                    displayGroup::p dummyTargetA;
                    displayGroup::p newTargetB;
                    this->_recursiveCreateSecondaryGroups(targetB->objects.begin(), targetB->objects.end(), nextLine->objects.begin(), nextLine->objects.end(), dummyTargetA, newTargetB, newGroup, pglIt, pglStopIt, false, secondaryGroups, false, onlyAddToBIfNotInGroupYet);
                    if(newTargetB){
                        newGroup->lines.push_back(newTargetB);
                    }
                }
                
                this->_recursiveCreateSecondaryGroups(lineAIt, lineAStopIt, nextLine->objects.begin(), nextLine->objects.end(), targetA, targetB, newGroup, pglIt, pglStopIt, false, secondaryGroups, false, true);
            }
            pglIt = pglItStore;
            lineAIt++;
            if(lineAIt != lineAStopIt){
                objA = *lineAIt;
                lineBIt = firstBIt;
                objB = *lineBIt;
                lineBDone = false;
                if(addToSecondaryGroups){
                    targetA.reset();
                    targetB.reset();
                    newGroup.reset();
                    needsShoveling = true;
                }
            } else {
                lineADone = true;
            }
            
        } // lineBDone
        
     } //while !lineADone || !lineBDone
}

/* quicksort algorithm taken from http://developer-resource.blogspot.com.es/2008/09/quicksort-c.html */

void HSSContainer::_qs_swap(std::vector<displayGroup::p> &arr, int a, int b) {
    std::swap(arr[a], arr[b]);
}

int HSSContainer::_qs_partition(std::vector<displayGroup::p> &arr, int left, int right, int pivotIndex) {
    displayGroup::p pivotValue = arr[pivotIndex];
    this->_qs_swap(arr, right, pivotIndex);
    int storeIndex = left;
    for (int i=left;i<=right -1; ++i) {
        if (arr[i]->height > pivotValue->height) {
            this->_qs_swap(arr, i, storeIndex);
            storeIndex++;
        }
    }
    this->_qs_swap(arr, storeIndex, right);
    return storeIndex;
}

void HSSContainer::_qs_sort(std::vector<displayGroup::p> &arr, int left , int right) {
    if  (right > left) {
        int pivotIndex = left;
        int pivotNewIndex = this->_qs_partition(arr,left,right,pivotIndex); 
        this->_qs_sort(arr,left,pivotNewIndex-1);
        this->_qs_sort(arr,pivotNewIndex+1,right);
    }
}

bool HSSContainer::_overlaps(HSSDisplayObject::p & childA, HSSDisplayObject::p & childB)
{
    return this->_overlaps_horizontal(childA, childB) && this->_overlaps_vertical(childA, childB);
}

bool HSSContainer::_overlaps_horizontal(HSSDisplayObject::p & childA, HSSDisplayObject::p & childB)
{
    if (
        (childA->x + childA->width + childA->rightMargin) > (childB->x - childB->leftMargin)
        && (childA->x - childA->leftMargin) < (childB->x + childB->width + childB->rightMargin)
    ){
        return true;
    } else {
        return false;
    }
}

bool HSSContainer::_overlaps_vertical(HSSDisplayObject::p & childA, HSSDisplayObject::p & childB)
{
    if (
        (childA->y + childA->height + childA->bottomMargin) > (childB->y - childB->topMargin)
        && (childA->y - childA->topMargin) < (childB->y + childB->height + childB->bottomMargin)
    ){
        return true;
    } else {
        return false;
    }
}

bool HSSContainer::_addChildToGroupIfNeeded(HSSDisplayObject::p &child, AXR::HSSContainer::displayGroup::p &group, HSSDirectionValue direction, bool overflow)
{
    unsigned i, size;
    bool isHorizontal = (direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft);
    bool addedToGroup = false;
    long double lineTotalPrimary = 0;
    
    long double originalX = child->x;
    long double originalY = child->y;
    
    for (i=0, size = group->objects.size(); i<size; i++) {
        HSSDisplayObject::p & otherChild = group->objects[i];
        if( isHorizontal ){
            lineTotalPrimary += otherChild->outerWidth;
        } else {
            lineTotalPrimary += otherChild->outerHeight;
        }
        if (
            ((child->x + child->outerWidth)  > (otherChild->x - otherChild->leftMargin)) && ((child->x - child->leftMargin) < (otherChild->x + otherChild->outerWidth))
            && ((child->y + child->outerHeight) > (otherChild->y - otherChild->topMargin)) && ((child->y - child->topMargin) < (otherChild->y + otherChild->outerHeight))
            ){
            //it will go into a group
            
            //if it is the last one
            if(i>=size-1){
                //check if we have enough space to add it to the end of the line
                if(!overflow){
                    if( isHorizontal ){
                        if( lineTotalPrimary + child->outerWidth > this->innerWidth){
                            group->complete = true;
                        }
                    } else {
                        if( lineTotalPrimary + child->outerHeight > this->innerHeight){
                            group->complete = true;
                        }
                    }
                }
                
                if (!group->complete){
                    //put it into the group
                    group->objects.push_back(child);
                    addedToGroup = true;
                    
                    switch (direction) {
                        case HSSDirectionTopToBottom:
                        case HSSDirectionBottomToTop:
                            group->height += child->outerHeight;
                            break;
                            
                        case HSSDirectionRightToLeft:
                        default:
                            group->width += child->outerWidth;
                            break;
                    }
                    
                    this->_arrange(group, direction);
                } else {
                    //restore the original position
                    child->x = originalX;
                    child->y = originalY;
                }
                
            } else {
                
                //push it further in the primary direction, and check again
                switch (direction) {
                    case HSSDirectionRightToLeft:
                    {
                        child->x = otherChild->x - child->outerWidth;
                        break;
                    }
                        
                    case HSSDirectionTopToBottom:
                    {
                        child->y = otherChild->y + otherChild->outerHeight;
                        break;
                    }
                        
                    case HSSDirectionBottomToTop:
                    {
                        child->y = otherChild->y - child->outerHeight;
                        break;
                    }
                        
                    default:
                    {
                        child->x = otherChild->x + otherChild->outerWidth;
                        break;
                    }
                }
                
            }
        }//if overlap
        
    }// for each child
    
    return addedToGroup;
}

//std::vector<HSSContainer::displayGroup::p> HSSContainer::_getGroupsOverlapping(HSSDisplayObject::p &child, std::vector<HSSContainer::displayGroup::p> &groups, HSSDirectionValue direction)
//{
//    std::vector<HSSContainer::displayGroup::p>::iterator it;
//    std::vector<HSSContainer::displayGroup::p> ret;
//    unsigned i, size, j, size2;
//    bool overlaps = false;
//    
//    for (it=groups.begin(); it!=groups.end(); it++) {
//        HSSContainer::displayGroup::p group = *it;
//        for (j=0, size2=group->lines.size(); j<size2; j++) {
//            const HSSContainer::displayGroup::p & otherGroup = group->lines[j];
//            if(
//               ((child->x + child->outerWidth) > otherGroup->x) && ((child->x - child->leftMargin) < (otherGroup->x + otherGroup->width))
//               && ((child->y + child->outerHeight) > otherGroup->y) && ((child->y - child->leftMargin) < (otherGroup->y + otherGroup->height))
//               ){
//                for (i=0, size = otherGroup->objects.size(); i<size; i++) {
//                    const HSSDisplayObject::p & otherChild = otherGroup->objects[i];
//                    if(direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft){
//                        if( ((child->y + child->outerHeight) > (otherChild->y - otherChild->topMargin)) && ((child->y - child->topMargin) < (otherChild->y + otherChild->outerHeight)) ){
//                            overlaps = true;
//                        }
//                    } else {
//                        if( ((child->x + child->outerWidth) > (otherChild->x - otherChild->leftMargin)) && ((child->x - child->leftMargin) < (otherChild->x + otherChild->outerWidth)) ){
//                            overlaps = true;
//                        }
//                    }
//                    
//                    if (overlaps) {
//                        ret.push_back(group);
//                        overlaps = false;
//                        continue;
//                    }
//                }
//            }
//        }
//    }
//    
//    return ret;
//}

bool HSSContainer::_mergeGroupsIfNeeded(displayGroup::p &group, displayGroup::p &otherGroup, HSSDirectionValue direction)
{
    unsigned i, size, j, size2;
    if (
        ((group->x + group->width) > otherGroup->x) && (group->x < (otherGroup->x + otherGroup->width))
        && ((group->y + group->height) > otherGroup->y) && (group->y < (otherGroup->y + otherGroup->height))
        ){
        //if the group bounds overlap, check each individual element against each other
        for (i=0, size = group->objects.size(); i<size; i++) {
            HSSDisplayObject::p & child = group->objects[i];
            for (j=0, size2 = otherGroup->objects.size(); j<size2; j++) {
                HSSDisplayObject::p &otherChild = otherGroup->objects[j];
                if (
                    ((child->x + child->outerWidth)  > (otherChild->x - otherChild->leftMargin)) && ((child->x - child->leftMargin) < (otherChild->x + otherChild->outerWidth))
                    && ((child->y + child->outerHeight) > (otherChild->y - otherChild->topMargin)) && ((child->y - child->topMargin) < (otherChild->y + otherChild->outerHeight))
                    ){
                    //there is an overlap, merge the groups
                    //add all the elements of this group to the overlapping one
                    group->objects.insert(group->objects.end(), otherGroup->objects.begin(), otherGroup->objects.end());
                    if(direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft){
                        group->width += otherGroup->width;
                    } else {
                        group->height += otherGroup->height;
                    }
                    
                    this->_arrange(group, direction);
                       
                    return true;
                }
            }
        }
    }
    
    
    
    return false;
}

void HSSContainer::_arrange(displayGroup::p &group, HSSDirectionValue direction)
{
    unsigned i, size;
    
    switch (direction) {
        case HSSDirectionRightToLeft:
        {
            bool byAnchors = false;
            for (i=0, size=group->objects.size(); i<size; i++) {
                if(!group->objects[i]->_anchorXdefault){
                    byAnchors = true;
                    break;
                }
            }
            if(byAnchors){
                //calculate the new alignment and anchor point for the group
                HSSDisplayObject::p & groupFirst = group->objects.front();
                long double alignmentTotal = 0.;
                long double accWidth = groupFirst->anchorX;
                long double anchorsTotal = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignX;
                    if(i>0){
                        anchorsTotal += accWidth + currentChild->anchorX;
                        accWidth += currentChild->outerWidth;
                    }
                }
                double long groupAlignX = alignmentTotal / size;
                double long groupAnchorX = anchorsTotal / size;
                
                //reposition the elements in the group
                double long startX = groupAlignX +  groupAnchorX + (groupFirst->width - groupFirst->anchorX);
                if(startX - group->width < 0) startX = group->width;
                if(startX > this->width) startX = this->width;
                accWidth = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & otherChild2 = group->objects[i];
                    otherChild2->x = startX - otherChild2->width - accWidth;
                    accWidth += otherChild2->width;
                }
                group->x = group->objects.front()->x - group->objects.front()->leftMargin;
                group->y = group->objects.front()->y - group->objects.front()->topMargin;
            } else {
                
                //calculate the alignment point for the group
                long double alignmentTotal = 0.;
                long double accWidth = 0.;
                long double widthsTotal = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignX;
                    widthsTotal += currentChild->outerWidth;
                }
                double long groupAlignX = alignmentTotal / size;
                double long startX = groupAlignX + widthsTotal / 2;
                if(startX > (this->width - this->rightPadding)) startX = (this->width - this->rightPadding);
                if(startX - group->width < this->leftPadding) startX = this->leftPadding + group->width;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    accWidth += currentChild->outerWidth;
                    currentChild->x = (startX - accWidth) - currentChild->rightMargin;
                }
                group->x = startX - group->width;
            }
            break;
        }
            
        case HSSDirectionTopToBottom:
        {    
            bool byAnchors = false;
            for (i=0, size=group->objects.size(); i<size; i++) {
                if(!group->objects[i]->_anchorYdefault){
                    byAnchors = true;
                    break;
                }
            }
            if(byAnchors){
                //calculate the new alignment and anchor point for the group
                HSSDisplayObject::p & groupFirst = group->objects.front();
                long double alignmentTotal = 0.;
                long double accHeight = groupFirst->height - groupFirst->anchorY;
                long double anchorsTotal = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignY;
                    if(i>0){
                        anchorsTotal += accHeight + currentChild->anchorY;
                        accHeight += currentChild->outerHeight;
                    }
                }
                double long groupAlignY = alignmentTotal / size;
                double long groupAnchorY = anchorsTotal / size;
                
                //reposition the elements in the group
                double long startY = groupAlignY - groupAnchorY - groupFirst->anchorY;
                if(startY > this->height - group->height) startY = this->height - group->height;
                if(startY < 0) startY = 0.;
                accHeight = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & otherChild2 = group->objects[i];
                    otherChild2->y = startY + accHeight;
                    accHeight += otherChild2->height;
                }
                group->x = group->objects.front()->x - group->objects.front()->leftMargin;
                group->y = group->objects.front()->y - group->objects.front()->topMargin;
            } else {
                //calculate the alignment point for the group
                long double alignmentTotal = 0.;
                long double accHeight = 0.;
                long double heightsTotal = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignY;
                    heightsTotal += currentChild->outerHeight;
                }
                double long groupAlignY = alignmentTotal / size;
                double long startY = groupAlignY - heightsTotal / 2;
                if(startY > (this->width - this->bottomPadding) - group->height) startY = (this->height - this->bottomPadding) - group->height;
                if(startY < this->topPadding) startY = this->topPadding;
                group->y = startY;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    currentChild->y = startY + accHeight + currentChild->topMargin;
                    accHeight += currentChild->outerHeight;
                }
            }
            break;
        }
            
        case HSSDirectionBottomToTop:
        {
            bool byAnchors = false;
            for (i=0, size=group->objects.size(); i<size; i++) {
                if(!group->objects[i]->_anchorYdefault){
                    byAnchors = true;
                    break;
                }
            }
            if(byAnchors){
                //calculate the new alignment and anchor point for the group
                HSSDisplayObject::p & groupFirst = group->objects.front();
                long double alignmentTotal = 0;
                long double accHeight = groupFirst->anchorY;
                long double anchorsTotal = 0;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignY;
                    if(i>0){
                        anchorsTotal += accHeight + currentChild->anchorY;
                        accHeight += currentChild->outerHeight;
                    }
                }
                double long groupAlignY = alignmentTotal / size;
                double long groupAnchorY = anchorsTotal / size;
                
                //reposition the elements in the group
                double long startY = groupAlignY +  groupAnchorY + (groupFirst->height - groupFirst->anchorY);
                if(startY - group->height < 0) startY = group->height;
                if(startY > this->height) startY = this->height;
                accHeight = 0;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & otherChild2 = group->objects[i];
                    otherChild2->y = startY - otherChild2->height - accHeight;
                    accHeight += otherChild2->height;
                }
                group->x = group->objects.front()->x;
                group->y = group->objects.front()->y;
            } else {
                //calculate the alignment point for the group
                long double alignmentTotal = 0.;
                long double accHeight = 0.;
                long double heightsTotal = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignY;
                    heightsTotal += currentChild->outerHeight;
                }
                double long groupAlignY = alignmentTotal / size;
                double long startY = groupAlignY + heightsTotal / 2;
                if(startY > (this->height - this->bottomPadding)) startY = (this->height - this->bottomPadding);
                if(startY - group->height < this->topPadding) startY = this->topPadding + group->height;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    accHeight += currentChild->outerHeight;
                    currentChild->y = (startY - accHeight) - currentChild->bottomMargin;
                }
                group->y = startY - group->height;
            }
            
            break;
        }
            
        default:
        {
            bool byAnchors = false;
            for (i=0, size=group->objects.size(); i<size; i++) {
                if(!group->objects[i]->_anchorXdefault){
                    byAnchors = true;
                    break;
                }
            }
            if(byAnchors){
                //calculate the new alignment and anchor point for the group
                HSSDisplayObject::p & groupFirst = group->objects.front();
                long double alignmentTotal = 0.;
                long double accWidth = groupFirst->outerWidth - groupFirst->anchorX;
                long double anchorsTotal = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignX;
                    if(i>0){
                        anchorsTotal += accWidth + currentChild->anchorX;
                        accWidth += currentChild->outerWidth;
                    }
                }
                double long groupAlignX = alignmentTotal / size;
                double long groupAnchorX = anchorsTotal / size;
                
                //reposition the elements in the group
                double long startX = groupAlignX - groupAnchorX - groupFirst->anchorX;
                if(startX > (this->width - this->rightPadding) - group->width) startX = (this->width - this->rightPadding) - group->width;
                if(startX < 0.) startX = 0.;
                accWidth = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & otherChild2 = group->objects[i];
                    otherChild2->x = startX + accWidth + otherChild2->leftMargin;
                    accWidth += otherChild2->outerWidth;
                }
                group->x = group->objects.front()->x - group->objects.front()->leftMargin;
                group->y = group->objects.front()->y - group->objects.front()->topMargin;
                
            } else {
                //calculate the alignment point for the group
                long double alignmentTotal = 0.;
                long double accWidth = 0.;
                long double widthsTotal = 0.;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    alignmentTotal += currentChild->alignX;
                    widthsTotal += currentChild->outerWidth;
                }
                double long groupAlignX = alignmentTotal / size;
                double long startX = groupAlignX - widthsTotal / 2;
                if(startX > (this->width - this->rightPadding) - group->width) startX = (this->width - this->rightPadding) - group->width;
                if(startX < this->leftPadding) startX = this->leftPadding;
                group->x = startX;
                for (i=0, size = group->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = group->objects[i];
                    currentChild->x = startX + accWidth + currentChild->leftMargin;
                    accWidth += currentChild->outerWidth;
                }
            }
            break;
        }
    }
}

bool HSSContainer::_arrangeLines(displayGroup::p &group, HSSDirectionValue direction, bool isFirstGroup)
{
    if(group->lines.size() < 2){
        return true;
    }
    
    AXRWrapper * wrapper = AXRCore::getInstance()->getWrapper();
    unsigned i, j, size, size2;
    
    switch (direction) {
        case HSSDirectionRightToLeft:
        {            
            
            break;
        }
            
        case HSSDirectionTopToBottom:
        {
            bool byAnchors = false;
            for (i=0, size=group->lines.size(); i<size; i++) {
                displayGroup::p line = group->lines[i];
                for (j=0, size2=line->objects.size(); j<size2; j++) {
                    if(!line->objects[j]->_anchorYdefault){
                        byAnchors = true;
                        break;
                    }
                }
            }
            if(byAnchors){
                
            } else {
                long double alignmentTotal = 0.;
                long double accHeight = 0.;
                long double totalSize = 0.;
                long double biggest = 0.;
                
                //create "push groups" to find out which one is the strongest
                displayGroup::p biggestGroup;
                bool pushGroupsDone = false;
                std::vector<displayGroup::p>::iterator pglIt = group->lines.begin();
                displayGroup::p lineA, lineB;
                std::vector<HSSDisplayObject::p>::iterator lineAIt, lineBIt;
                HSSDisplayObject::p objA, objB;
                lineA = *pglIt;
                lineAIt = lineA->objects.begin();
                objA = *lineAIt;
                pglIt++;
                lineB = *pglIt;
                lineBIt = lineB->objects.begin();
                objB = *lineBIt;
                
                while (!pushGroupsDone){
                    displayGroup::p newGroup;
                    this->_recursiveGetPushGroup(objA, objB, pglIt, group->lines.end(), newGroup);
                    if(newGroup && newGroup->height >= biggest){
                        biggestGroup = newGroup;
                        biggest = newGroup->height;
                    }
                    lineBIt++;
                    if(lineBIt != lineB->objects.end()){
                        objB = *lineBIt;
                    } else {
                        lineBIt = lineB->objects.begin();
                        objB = *lineBIt;
                        
                        lineAIt++;
                        if(lineAIt != lineA->objects.end()){
                            objA = *lineAIt;
                        } else {
                            pushGroupsDone = true;
                        }
                    }
                }
                if(!biggestGroup){
                    biggestGroup = group->lines.back();
                }
                
                group->height = biggestGroup->height;
                
                for (i=0, size=biggestGroup->objects.size(); i<size; i++) {
                    HSSDisplayObject::p & currentChild = biggestGroup->objects[i];
                    alignmentTotal += currentChild->alignY;
                    totalSize += currentChild->outerHeight;
                }
                
                //calculate the alignment point for the biggest elements
                long double groupAlignY = alignmentTotal / size;
                
                //reposition the elements in the group
                long double startY = groupAlignY - totalSize / 2;
                if(startY > (this->height - this->bottomPadding) - group->height) startY = (this->height - this->bottomPadding) - group->height;
                if(startY < this->topPadding) startY = this->topPadding;
                HSSDisplayObject::it bobjIt, bobjIt2;
                int i = 0, size = biggestGroup->objects.size();
                for (bobjIt=biggestGroup->objects.begin(); bobjIt!=biggestGroup->objects.end(); bobjIt++) {
                    HSSDisplayObject::p otherChild2 = *bobjIt;
                    long double newValue = startY + accHeight + otherChild2->topMargin;
                    if(!isFirstGroup){
                        //find the bottom constraint of the previous line
                        long double prevConstraintBottom = this->topPadding;
                        bool hasPrevConstraintBottom = false;
                        if(i!=0){
                            displayGroup::p previousLine = group->lines[i-1];
                            hasPrevConstraintBottom = this->_recursiveFindBottomConstraint(prevConstraintBottom, group, i, otherChild2);
                        }
                        //find the top constraint of the next line
                        long double nextConstraintTop = this->height - this->bottomPadding;
                        bool hasNextConstraintTop = false;
                        if(group->lines.size() > i+1){
                            displayGroup::p nextLine = group->lines[i+1];
                            hasNextConstraintTop = this->_recursiveFindTopConstraint(nextConstraintTop, group, i, otherChild2);
                        }
                        
                        if((!otherChild2->_layoutFlagLockTop || newValue > otherChild2->y)
                           && (!otherChild2->_layoutFlagLockBottom || newValue + otherChild2->height + otherChild2->bottomMargin < otherChild2->y + otherChild2->height + otherChild2->bottomMargin)
                           ){
                            if(hasNextConstraintTop && newValue + otherChild2->outerHeight < nextConstraintTop){
                                otherChild2->y = nextConstraintTop - (otherChild2->height + otherChild2->topMargin);
                            } else if (hasPrevConstraintBottom && newValue + otherChild2->outerHeight > prevConstraintBottom) {
                                otherChild2->y = prevConstraintBottom + otherChild2->bottomMargin;
                            } else {
                                otherChild2->y = newValue;
                            }
                        }
                    } else {
                        otherChild2->y = newValue;
                    }
                    
                    accHeight += otherChild2->outerHeight;
                    
                    if(wrapper->showLayoutSteps()){
                        wrapper->nextLayoutTick();
                        wrapper->breakIfNeeded();
                        if(wrapper->layoutStepDone()){
                            return false;
                        }
                    }
                    
                    if(i==0){
                        otherChild2->_layoutFlagLockBottom = true;
                        //otherChild2->_layoutLockBottomPosition = otherChild2->y + otherChild2->height + otherChild2->bottomMargin;
                    } else if(i == size -1){
                        //otherChild2->_layoutLockTopPosition = otherChild2->y + otherChild2->topMargin;
                        otherChild2->_layoutFlagLockTop = true;
                    } else {
                        otherChild2->_layoutFlagLockTop = true;
                        //otherChild2->_layoutLockTopPosition = otherChild2->y + otherChild2->topMargin;
                        otherChild2->_layoutFlagLockBottom = true;
                        //otherChild2->_layoutLockBottomPosition = otherChild2->y + otherChild2->height + otherChild2->bottomMargin;
                    }
                    i++;
                }
                
                for (i=0, size=group->lines.size(); i<size; i++) {
                    displayGroup::p & line = group->lines[i];
                    for (j=0, size2 = line->objects.size(); j<size2; j++) {
                        HSSDisplayObject::p & currentChild = line->objects[j];
                        
                        if(!currentChild->_layoutFlagLockBottom){
                            //find the constraint towards the bottom
                            long double constraintBottom = this->height - this->bottomPadding;
                            bool needsConstraintBottom = false;
                            if(group->lines.size() > i+1){
                                displayGroup::p nextLine = group->lines[i+1];
                                std::vector<HSSDisplayObject::p>::iterator nlIt;
                                for (nlIt=nextLine->objects.begin(); nlIt!=nextLine->objects.end(); nlIt++) {
                                    HSSDisplayObject::p nlObj = *nlIt;
                                    if(nlObj->_layoutFlagLockTop && this->_overlaps_horizontal(nlObj, currentChild)){
                                        if(nlObj->y-nlObj->topMargin < constraintBottom){
                                            constraintBottom = nlObj->y-nlObj->topMargin;
                                            needsConstraintBottom = true;
                                        }
                                    }
                                }
                            }
                            if(needsConstraintBottom && currentChild->alignY + (currentChild->outerHeight/2) > constraintBottom){
                                currentChild->y = constraintBottom - currentChild->bottomMargin - currentChild->height;
                                currentChild->_layoutFlagLockBottom = true;
                                if(wrapper->showLayoutSteps()){
                                    wrapper->nextLayoutTick();
                                    wrapper->breakIfNeeded();
                                    if(wrapper->layoutStepDone()){
                                        return false;
                                    }
                                }
                            }
                        }
                        
                        if(!currentChild->_layoutFlagLockTop){
                            //find the constraint towards the top
                            long double constraintTop = this->topPadding;
                            bool needsConstraintTop = false;
                            if(i!=0){
                                displayGroup::p previousLine = group->lines[i-1];
                                std::vector<HSSDisplayObject::p>::iterator plIt;
                                for (plIt=previousLine->objects.begin(); plIt!=previousLine->objects.end(); plIt++) {
                                    HSSDisplayObject::p plObj = *plIt;
                                    if(plObj->_layoutFlagLockBottom && this->_overlaps_horizontal(plObj, currentChild)){
                                        if(plObj->y+plObj->outerHeight > constraintTop){
                                            constraintTop = plObj->y+plObj->height+plObj->bottomMargin;
                                            needsConstraintTop = true;
                                        }
                                    }
                                }
                            }
                            if(needsConstraintTop && currentChild->alignY - (currentChild->outerHeight/2) < constraintTop){
                                currentChild->y = constraintTop + currentChild->topMargin;
                                currentChild->_layoutFlagLockTop = true;
                                if(wrapper->showLayoutSteps()){
                                    wrapper->nextLayoutTick();
                                    wrapper->breakIfNeeded();
                                    if(wrapper->layoutStepDone()){
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            break;
        }
            
        case HSSDirectionBottomToTop:
        {            
            
            break;
        }
            
        default:
        {
            
            break;
        }
    }
    
    return true;
}

void HSSContainer::_recursiveGetPushGroup(HSSDisplayObject::p objA, HSSDisplayObject::p objB, std::vector<displayGroup::p>::iterator linesIt, std::vector<displayGroup::p>::iterator stopIt, displayGroup::p &ret)
{
    if(this->_overlaps_horizontal(objA, objB)){
        if(!ret){
            ret = displayGroup::p(new displayGroup());
            ret->objects.push_back(objA);
            ret->objects.push_back(objB);
            ret->height = objA->outerHeight + objB->outerHeight;
        } else {
            ret->objects.push_back(objB);
            ret->height += objB->outerHeight;
        }
    }
    linesIt++;
    if(linesIt != stopIt){
        displayGroup::p nextLine = *linesIt;
        HSSDisplayObject::p objC = *nextLine->objects.begin();
        this->_recursiveGetPushGroup(objB, objC, linesIt, stopIt, ret);
    }
}

bool HSSContainer::_recursiveFindTopConstraint(long double &constraint, displayGroup::p group, int i, HSSDisplayObject::p child){
    bool ret = false;
    long double constraintStore = constraint;
    displayGroup::p nextLine;
    if (i >= group->lines.size()-1) {
        return false;
    }
    nextLine = group->lines[i+1];
    
    std::vector<HSSDisplayObject::p>::iterator nlIt;
    for (nlIt=nextLine->objects.begin(); nlIt!=nextLine->objects.end(); nlIt++) {
        HSSDisplayObject::p nlObj = *nlIt;
        if(nlObj->_layoutFlagLockTop && this->_overlaps_horizontal(nlObj, child)){
            if(nlObj->y-nlObj->topMargin < constraint){
                constraint = nlObj->y-nlObj->topMargin;
                ret = true;
            }
        } else {
            if(i+1 < group->lines.size()){
                long double newConstraint = constraintStore;
                bool needsNewConstraint = this->_recursiveFindTopConstraint(newConstraint, group, i+1, nlObj);
                if(needsNewConstraint){
                    long double newValue = newConstraint - nlObj->outerHeight;
                    if(newValue < constraint){
                        constraint = newValue;
                        ret = true;
                    }
                }
            }
        }
    }
    return ret;
}

bool HSSContainer::_recursiveFindBottomConstraint(long double &constraint, displayGroup::p group, int i, HSSDisplayObject::p child){
    bool ret = false;
    long double constraintStore = constraint;
    displayGroup::p nextLine;
    if (i < 1) {
        return false;
    }
    nextLine = group->lines[i-1];
    
    std::vector<HSSDisplayObject::p>::iterator nlIt;
    for (nlIt=nextLine->objects.begin(); nlIt!=nextLine->objects.end(); nlIt++) {
        HSSDisplayObject::p nlObj = *nlIt;
        if(nlObj->_layoutFlagLockBottom && this->_overlaps_horizontal(nlObj, child)){
            if(nlObj->y+nlObj->outerHeight > constraint){
                constraint = nlObj->y+nlObj->height+nlObj->bottomMargin;
                ret = true;
            }
        } else {
            if(i<2){
                long double newConstraint = constraintStore;
                bool needsNewConstraint = this->_recursiveFindBottomConstraint(newConstraint, group, i-1, nlObj);
                if(needsNewConstraint){
                    long double newValue = newConstraint + nlObj->outerHeight;
                    if(newValue > constraint){
                        constraint = newValue;
                        ret = true;
                    }
                }
            }
        }
    }
    return ret;
}

void HSSContainer::_distribute(displayGroup::p &group, HSSDirectionValue direction)
{
    switch (direction) {
        case HSSDirectionRightToLeft:
        {
            break;
        }
            
        case HSSDirectionTopToBottom:
        {
            HSSDisplayObject::it it;
            long double accHeight = this->topPadding;
            long double totalHeight = 0.;
            
            //calculate the total height of the group
            for (it=group->objects.begin(); it!=group->objects.end(); it++) {
                totalHeight += (*it)->outerHeight;
            }
            //std_log(totalHeight);
            //now get the remaining space
            long double remainingSpace = this->innerHeight - totalHeight;
            //divide it by the number of elements+1
            long double spaceChunk = remainingSpace / (group->objects.size() + 1);
            unsigned i = 0;
            for (it=group->objects.begin(); it!=group->objects.end(); it++) {
                (*it)->y = accHeight + spaceChunk + (spaceChunk*i) + (*it)->topMargin;
                accHeight += (*it)->outerHeight;
                i++;
            }
            group->x = group->objects.front()->x;
            group->y = 0.;
            break;
        }
            
        case HSSDirectionBottomToTop:
        {            
            break;
        }
            
        default:
        {
            HSSDisplayObject::it it;
            long double accWidth = this->leftPadding;
            long double totalWidth = 0.;
            
            //calculate the total width of the group
            for (it=group->objects.begin(); it!=group->objects.end(); it++) {
                totalWidth += (*it)->outerWidth;
            }
            //std_log(totalWidth);
            //now get the remaining space
            long double remainingSpace = this->innerWidth - totalWidth;
            //divide it by the number of elements+1
            long double spaceChunk = remainingSpace / (group->objects.size() + 1);
            unsigned i = 0;
            for (it=group->objects.begin(); it!=group->objects.end(); it++) {
                (*it)->x = accWidth + spaceChunk + (spaceChunk*i) + (*it)->leftMargin;
                accWidth += (*it)->outerWidth;
                i++;
            }
            group->x = 0.;
            group->y = group->objects.front()->y;
            break;
        }
    }
}

void HSSContainer::recursiveLayout()
{
    unsigned i, size;
    for (i=0, size = this->allChildren.size(); i<size; i++) {
        this->allChildren[i]->recursiveLayout();
    }
    this->layout();
}

void HSSContainer::setGlobalX(long double newValue)
{
    long double delta = newValue - this->globalX;
    HSSDisplayObject::setGlobalX(newValue);
    unsigned i, size;
    for (i=0, size=this->allChildren.size(); i<size; i++){
        HSSDisplayObject::p theChild = this->allChildren[i];
        theChild->setGlobalX(theChild->globalX + delta);
    }
}

void HSSContainer::setGlobalY(long double newValue)
{
    long double delta = newValue - this->globalY;
    HSSDisplayObject::setGlobalY(newValue);
    unsigned i, size;
    for (i=0, size=this->allChildren.size(); i<size; i++){
        HSSDisplayObject::p theChild = this->allChildren[i];
        theChild->setGlobalY(theChild->globalY + delta);
    }
}

void HSSContainer::setChildren(std::vector<HSSDisplayObject::p> newChildren)
{
    this->children = newChildren;
    this->allChildren = newChildren;
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->setParent(this->shared_from_this());
    }
}

const std::vector<HSSDisplayObject::p>& HSSContainer::getChildren() const
{
    return this->getChildren(false);
}

const std::vector<HSSDisplayObject::p>& HSSContainer::getChildren(bool includeTextBlocks) const
{
    if (includeTextBlocks) {
        return this->allChildren;
    } else {
        return this->children;
    }
}

//contentAlignX
HSSParserNode::p HSSContainer::getDContentAlignX() { return this->dContentAlignX; }
void HSSContainer::setDContentAlignX(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSContainer", "Invalid value for contentAlignX of "+this->getElementName()));
    }
    
    if(this->observedContentAlignX != NULL)
    {
        this->observedContentAlignX->removeObserver(this->observedContentAlignXProperty, HSSObservablePropertyAlignX, this);
        this->observedContentAlignX = NULL;
    }
    this->dContentAlignX = value;
    
    if(value->isA(HSSParserNodeTypeKeywordConstant)){
        HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant>(value);
        if (keywordValue->getValue() == "left"){
            this->setDContentAlignX(HSSParserNode::p(new HSSNumberConstant(0)));
        } else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center"){
            this->setDContentAlignX(HSSParserNode::p(new HSSPercentageConstant(50)));
        } else if (keywordValue->getValue() == "right"){
            this->setDContentAlignX(HSSParserNode::p(new HSSPercentageConstant(100)));
        } else if (keywordValue->getValue() == "distribute"){
            this->distributeX = true;
            this->distributeXLinear = false;
        } else if (keywordValue->getValue() == "linear"){
            this->distributeX = true;
            this->distributeXLinear = true;
        } else {
            throw AXRWarning::p(new AXRWarning("HSSContainer", "Invalid value for contentAlignX of "+this->getElementName()));
        }
        
    } else {
        HSSContainer::p parentContainer = this->getParent();
        const std::vector<HSSDisplayObject::p> * scope;
        if(parentContainer){
            scope = &(parentContainer->getChildren());
        } else {
            scope = &(this->getChildren());
        }
        this->contentAlignX = this->_setLDProperty(
                                            &HSSContainer::contentAlignXChanged,
                                            value,
                                            this->width,
                                            HSSObservablePropertyWidth,
                                            this,
                                            HSSObservablePropertyAlignX,
                                            this->observedContentAlignX,
                                            this->observedContentAlignXProperty,
                                            scope
                                            );
        this->notifyObservers(HSSObservablePropertyContentAlignX, &this->contentAlignX);
        #if AXR_DEBUG_LEVEL > 0
        this->setDirty(true);
        #endif
    }
}

void HSSContainer::contentAlignXChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dContentAlignX->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dContentAlignX);
            this->contentAlignX = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dContentAlignX);
            this->contentAlignX = expressionValue->evaluate();
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyContentAlignX, &this->contentAlignX);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}

//contentAlignY
HSSParserNode::p HSSContainer::getDContentAlignY() { return this->dContentAlignX; }
void HSSContainer::setDContentAlignY(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSContainer", "Invalid value for contentAlignY of "+this->getElementName()));
    }
    
    if(this->observedContentAlignY != NULL)
    {
        this->observedContentAlignY->removeObserver(this->observedContentAlignYProperty, HSSObservablePropertyAlignY, this);
        this->observedContentAlignY = NULL;
    }
    this->dContentAlignY = value;
    
    
    if(value->isA(HSSParserNodeTypeKeywordConstant)){
        
        HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant>(value);
        if (keywordValue->getValue() == "top"){
            this->setDContentAlignY(HSSParserNode::p(new HSSNumberConstant(0)));
        } else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center"){
            this->setDContentAlignY(HSSParserNode::p(new HSSPercentageConstant(50)));
        } else if (keywordValue->getValue() == "bottom"){
            this->setDContentAlignY(HSSParserNode::p(new HSSPercentageConstant(100)));
        } else if (keywordValue->getValue() == "distribute"){
            this->distributeY = true;
            this->distributeYLinear = false;
        } else if (keywordValue->getValue() == "linear"){
            this->distributeY = true;
            this->distributeYLinear = true;
        } else {
            throw AXRWarning::p(new AXRWarning("HSSContainer", "Invalid value for contentAlignY of "+this->getElementName()));
        }
        
    } else {
        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
        
        HSSContainer::p parentContainer = this->getParent();
        const std::vector<HSSDisplayObject::p> * scope;
        if(parentContainer){
            scope = &(parentContainer->getChildren());
        } else {
            scope = NULL;
        }
        this->contentAlignY = this->_setLDProperty(
                                            &HSSContainer::contentAlignYChanged,
                                            value,
                                            this->height,
                                            observedProperty,
                                            this,
                                            HSSObservablePropertyAlignY,
                                            this->observedContentAlignY,
                                            this->observedContentAlignYProperty,
                                            scope
                                            );
        this->notifyObservers(HSSObservablePropertyContentAlignY, &this->contentAlignY);
        #if AXR_DEBUG_LEVEL > 0
        this->setDirty(true);
        #endif
    }
}

void HSSContainer::contentAlignYChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dContentAlignY->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dContentAlignY);
            this->contentAlignY = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dContentAlignY);
            this->contentAlignY = expressionValue->evaluate();
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyContentAlignY, &this->contentAlignY);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}


//directionPrimary
HSSParserNode::p HSSContainer::getDDirectionPrimary() { return this->dDirectionPrimary; }
void HSSContainer::setDDirectionPrimary(HSSParserNode::p value)
{
    this->dDirectionPrimary = value;
    if(this->observedDirectionPrimary != NULL)
    {
        this->observedDirectionPrimary->removeObserver(this->observedDirectionPrimaryProperty, HSSObservablePropertyDirectionPrimary, this);
    }
    
    if (value->isA(HSSParserNodeTypeKeywordConstant)){
        std::string stringValue = boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue();
        if(stringValue == "leftToRight" || stringValue == "rightToLeft"){
            if (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft){
                this->directionSecondary = HSSDirectionTopToBottom;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "leftToRight"){
                this->directionPrimary = HSSDirectionLeftToRight;
            } else {
                this->directionPrimary = HSSDirectionRightToLeft;
            }
            
        } else if (stringValue == "topToBottom" || stringValue == "bottomToTop") {
            if (this->directionSecondary == HSSDirectionTopToBottom || this->directionSecondary == HSSDirectionBottomToTop){
                this->directionSecondary = HSSDirectionLeftToRight;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "topToBottom"){
                this->directionPrimary = HSSDirectionTopToBottom;
            } else {
                this->directionPrimary = HSSDirectionBottomToTop;
            }
        }
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
}

void HSSContainer::directionPrimaryChanged(HSSObservableProperty source, void *data)
{
    if(this->dDirectionPrimary->isA(HSSParserNodeTypeKeywordConstant)){
        
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
}

//directionSecondary
HSSParserNode::p HSSContainer::getDDirectionSecondary() { return this->dDirectionSecondary; }
void HSSContainer::setDDirectionSecondary(HSSParserNode::p value)
{
    this->dDirectionSecondary = value;
    if(this->observedDirectionSecondary != NULL)
    {
        this->observedDirectionSecondary->removeObserver(this->observedDirectionSecondaryProperty, HSSObservablePropertyDirectionSecondary, this);
    }
    
    if (value->isA(HSSParserNodeTypeKeywordConstant)){
        std::string stringValue = boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue();
        if(stringValue == "leftToRight" || stringValue == "rightToLeft"){
            if (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft){
                this->directionSecondary = HSSDirectionTopToBottom;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "leftToRight"){
                this->directionSecondary = HSSDirectionLeftToRight;
            } else {
                this->directionSecondary = HSSDirectionRightToLeft;
            }
            
        } else if (stringValue == "topToBottom" || stringValue == "bottomToTop") {
            if (this->directionSecondary == HSSDirectionTopToBottom || this->directionSecondary == HSSDirectionBottomToTop){
                this->directionSecondary = HSSDirectionLeftToRight;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "topToBottom"){
                this->directionSecondary = HSSDirectionTopToBottom;
            } else {
                this->directionSecondary = HSSDirectionBottomToTop;
            }
        }
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
}

void HSSContainer::directionSecondaryChanged(HSSObservableProperty source, void *data)
{
    if(this->dDirectionSecondary->isA(HSSParserNodeTypeKeywordConstant)){
        
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
}


//directionSecondary
HSSParserNode::p HSSContainer::getDShape() { return this->dShape; }
void HSSContainer::setDShape(HSSParserNode::p value)
{
    bool valid = false;
    
    this->dShape = value;
    if(this->observedShape != NULL)
    {
        this->observedShape->removeObserver(this->observedShapeProperty, HSSObservablePropertyShape, this);
    }
    
    switch (value->getType()) {
        case HSSParserNodeTypeKeywordConstant:
        {
            std::string stringValue = boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue();
            if(stringValue == "default"){
                this->shape = HSSRectangle::p(new HSSRectangle());
                valid = true;
            }
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                objdef->setThisObj(this->shared_from_this());
                objdef->apply();
                HSSObject::p theObject = objdef->getObject();
                if (theObject->isA(HSSObjectTypeShape)){
                    this->shape = boost::static_pointer_cast<HSSShape>(theObject);
                    valid = true;
                }
                
            } catch (AXRError::p e) {
                e->raise();
                
            } catch (AXRWarning::p e) {
                e->raise();
            }
            
            break;
        }
            
        default:
            break;
    }
    
    switch (value->getStatementType()) {
        case HSSStatementTypeObjectDefinition:
        {
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            HSSObject::p objValue = objdef->getObject();
            if(objValue->isA(HSSObjectTypeShape)){
                this->shape = boost::static_pointer_cast<HSSShape>(objValue);
                valid = true;
            }
            break;
        }
            
        default:
            break;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for shape of "+this->getElementName()));
    
    this->notifyObservers(HSSObservablePropertyShape, &this->shape);
}

void HSSContainer::shapeChanged(HSSObservableProperty source, void *data)
{
    this->setDirty(true);
    this->shape = *(HSSShape::p *)data;
    this->notifyObservers(HSSObservablePropertyShape, &this->shape);
}

HSSTextAlignType HSSContainer::getTextAlign() { return this->textAlign; }
HSSParserNode::p HSSContainer::getDTextAlign() { return this->dTextAlign; }
void HSSContainer::setDTextAlign(HSSParserNode::p value)
{
    bool valid = false;
    
    switch (value->getType()) {
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            this->dTextAlign = value;
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                this->setDTextAlign(objdef);
                valid = true;
            } catch (AXRError::p e) {
                e->raise();
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dTextAlign = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->textAlign = *(HSSTextAlignType *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTextAlign, this, new HSSValueChangedCallback<HSSContainer>(this, &HSSContainer::textAlignChanged));
                valid = true;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            this->dTextAlign = value;
            this->textAlign = HSSTextBlock::textAlignTypeFromString(boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue());
            valid = true;
            break;
        }
            
        default:
            break;
    }
    
    switch (value->getStatementType()) {
        case HSSStatementTypeObjectDefinition:
        {
            this->dTextAlign = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->setScope(this->scope);
            objdef->setThisObj(this->shared_from_this());
            objdef->apply();
            HSSObject::p theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeValue)) {
                //this->textAlign = HSSTextBlock::textAlignTypeFromString(boost::static_pointer_cast<HSSValue>(theobj)->getStringValue());
                std_log("######## FIXME ################");
                valid = true;
            }
            
            break;
        }
            
        default:
            break;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for textAlign of "+this->name));
    
    this->notifyObservers(HSSObservablePropertyTextAlign, &this->textAlign);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSContainer::textAlignChanged(HSSObservableProperty source, void *data)
{
    switch (this->dTextAlign->getType()) {
        case HSSParserNodeTypeFunctionCall:
            this->textAlign = *(HSSTextAlignType *)data;
            break;
            
        default:
            break;
    }
    
    switch (this->dTextAlign->getStatementType()) {
        case HSSStatementTypeObjectDefinition:
            this->textAlign = *(HSSTextAlignType *)data;
            break;
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyTextAlign, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSContainer::setDefaults()
{
    HSSDisplayObject::setDefaults();
    
    //contentAlignX
    HSSPercentageConstant::p newDContentAlignX(new HSSPercentageConstant(50));
    this->setDContentAlignX(newDContentAlignX);
    //contentAlignY
    HSSNumberConstant::p newDContentAlignY(new HSSNumberConstant(0));
    this->setDContentAlignY(newDContentAlignY);
    //directionPrimary
    HSSKeywordConstant::p newDDirectionPrimary(new HSSKeywordConstant("leftToRight"));
    this->setDDirectionPrimary(newDDirectionPrimary);
    //directionSecondary
    HSSKeywordConstant::p newDDirectionSecondary(new HSSKeywordConstant("topToBottom"));
    this->setDDirectionSecondary(newDDirectionSecondary);
    //shape
    HSSKeywordConstant::p newDShape(new HSSKeywordConstant("default"));
    this->setDShape(newDShape);
    //textAlign
    this->setDTextAlign(HSSKeywordConstant::p(new HSSKeywordConstant("left")));
}



long double HSSContainer::_setLDProperty(
                                             void(HSSContainer::*callback)(HSSObservableProperty property, void* data),
                                             HSSParserNode::p       value,
                                             long double            percentageBase,
                                             HSSObservableProperty  observedProperty,
                                             HSSObservable *        observedObject,
                                             HSSObservableProperty  observedSourceProperty,
                                             HSSObservable *        &observedStore,
                                             HSSObservableProperty  &observedStoreProperty,
                                             const std::vector<HSSDisplayObject::p> * scope
                                             )
{
    long double ret;
    
    HSSParserNodeType nodeType = value->getType();
    switch (nodeType) {
        case HSSParserNodeTypeNumberConstant:
        {
            HSSNumberConstant::p numberValue = boost::static_pointer_cast<HSSNumberConstant>(value);
            ret = numberValue->getValue();
            break;
        }
            
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(value);
            ret = percentageValue->getValue(percentageBase);
            if(callback != NULL)
            {
                observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer>(this, callback));
                observedStore = observedObject;
                observedStoreProperty = observedProperty;
            }
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(value);
            expressionValue->setPercentageBase(percentageBase);
            expressionValue->setPercentageObserved(observedProperty, observedObject);
            expressionValue->setScope(scope);
            ret = expressionValue->evaluate();
            if(callback != NULL){
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer>(this, callback));
                observedStore = expressionValue.get();
                observedStoreProperty = HSSObservablePropertyValue;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            break;
        
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            fnct->setPercentageBase(percentageBase);
            fnct->setPercentageObserved(observedProperty, observedObject);
            fnct->setScope(scope);
            
            ret = *(long double*)fnct->evaluate();
            if(callback != NULL){
                fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer>(this, callback));
                observedStore = fnct.get();
                observedStoreProperty = HSSObservablePropertyValue;
            }
            break;
        }
            
        default:
            throw AXRError::p(new AXRError("HSSContainer", "Unknown parser node type while setting LDProperty"));
    }
    
    return ret;
}


bool HSSContainer::handleEvent(HSSEventType type, void* data)
{
    HSSDisplayObject::it it;
    bool handled = false;
    for (it=this->allChildren.begin(); it < this->allChildren.end(); it++) {
        HSSDisplayObject::p child = *it;
        bool childHandled = false;
        childHandled = child->handleEvent(type, data);
        if (childHandled) {
            handled = true;
        }
    }
    
    bool superHandled = HSSDisplayObject::handleEvent(type, data);
    if(superHandled){
        handled = true;
    }
    
    return handled;
}


void HSSContainer::setController(AXRController * controller)
{
    //propagate
    HSSDisplayObject::it it;
    for (it=this->allChildren.begin(); it < this->allChildren.end(); it++) {
        HSSDisplayObject::p child = *it;
        child->setController(controller);
    }
    
    HSSDisplayObject::setController(controller);
}

HSSContainer::p HSSContainer::shared_from_this()
{
    return boost::static_pointer_cast<HSSContainer>(HSSDisplayObject::shared_from_this());
}

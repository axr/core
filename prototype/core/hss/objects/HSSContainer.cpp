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
 *      Last changed: 2011/05/08
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 16
 *
 ********************************************************************/

#include "HSSContainer.h"
#include "../../axr/AXRDebugging.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include <map>
#include <string>
#include <sstream>

using namespace AXR;

HSSContainer::HSSContainer()
: HSSDisplayObject()
{
    this->contentText = std::string();
    this->type = HSSObjectTypeContainer;
    
    this->contentAlignX = this->contentAlignY = 0;
    
    observedContentAlignX = observedContentAlignY
    = NULL;
}

HSSContainer::HSSContainer(std::string name)
: HSSDisplayObject(name)
{
    this->contentText = std::string();
    this->type = HSSObjectTypeContainer;
    
    this->contentAlignX = this->contentAlignY = 0;
    
    observedContentAlignX = observedContentAlignY
    = NULL;
}

HSSContainer::~HSSContainer()
{
    unsigned i;
    for(i=0; i<this->children.size(); i++){
        this->children.pop_back();
    }
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

std::string HSSContainer::defaultObjectType(std::string property)
{
    if (property == "shape"){
        return "roundedRect";
    } else if (property == "innerMargin"){
        return "margin";
    } else if (property == "background"){
        return "image";
    } else {
        return HSSDisplayObject::defaultObjectType(property);
    }
}

bool HSSContainer::isKeyword(std::string value, std::string property)
{
    if (value == "center"){
        if (   property == "contentAlignX"
            || property == "contentAlignY" ) {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSDisplayObject::isKeyword(value, property);
}

void HSSContainer::add(HSSDisplayObject::p child)
{
    this->children.push_back(child);
    child->setParent(shared_from_this());
}

void HSSContainer::remove(unsigned index)
{
    this->children.erase(this->children.begin()+index);
}

void HSSContainer::readDefinitionObjects()
{
    //this->setDefaults();
    
    HSSDisplayObject::readDefinitionObjects();
    
    unsigned i, j;
    
    std::string propertyName;
    for (i=0; i<this->rules.size(); i++) {
        HSSRule::p& rule = this->rules[i];
        
        for (j=0; j<rule->propertiesSize(); j++) {
            HSSPropertyDefinition::p& propertyDefinition = rule->propertiesGet(j);
            propertyName = propertyDefinition->getName();
            
            if(propertyName == "contentAlignX"){
                //store a copy of the value
                this->setDContentAlignX(propertyDefinition->getValue());
                
            } else if(propertyName == "contentAlignY"){
                //store a copy of the value
                this->setDContentAlignY(propertyDefinition->getValue());
            }
        }
    }
}

void HSSContainer::recursiveReadDefinitionObjects()
{
    if (this->_needsRereadRules) {
        this->readDefinitionObjects();
    }
    
    unsigned i;
    for (i=0; i<this->children.size(); i++) {
        this->children[i]->readDefinitionObjects();
    }
}

void HSSContainer::recursiveRegenerateSurfaces()
{
    this->regenerateSurfaces();
    
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->regenerateSurfaces();
    }
}

void HSSContainer::recursiveDraw(cairo_t * cairo)
{
    this->draw(cairo);
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->recursiveDraw(cairo);
    }
}

void HSSContainer::layout()
{
    //reset flag
    this->_needsLayout = false;
    //emtpy the lines
    this->layoutLines.clear();
    unsigned i, j = 0, k = 0, l, size, size2, size3;
    long double accWidth = 0;
    
    //sort the elements in two arrays: those aligned by contentAlign and those by align
    std::vector<HSSDisplayObject::p>byContentAlign;
    std::vector<HSSDisplayObject::p>byAlign;
    
    for (i=0, size = this->children.size(); i<size; i++) {
        HSSDisplayObject::p child = this->children[i];
        //FIXME: instead of just looking for a keyword, check to see if the end value is "auto"
        if (! child->dAlignX->isA(HSSParserNodeTypeKeywordConstant) || ! child->dAlignY->isA(HSSParserNodeTypeKeywordConstant)) {
            byAlign.push_back(child);
        } else {
            byContentAlign.push_back(child);
        }
    }
    
    
    std::vector<displayGroup>groups;
    
    //first layout the elments with align set
    for (i=0, size = byAlign.size(); i<size; i++) {
        HSSDisplayObject::p child = byAlign[i];
        
        //place it on the alignment point for the primary direction
        child->x = child->alignX - child->anchorX;
        if (child->x < 0) child->x = 0;
        if ((child->x + child->width) > this->width) child->x = this->width - child->width;
        
        //check if it will go in any of the groups
        bool intoGroup = false;
        j = 0;
        while (j<groups.size()) {
            displayGroup & currentGroup = groups[j];
            
            //search for overlaps
            for (k=0, size2 = currentGroup.objects.size(); k<size2; k++) {
                HSSDisplayObject::p & otherChild = currentGroup.objects[k];
                if (((child->x + child->width) >= otherChild->x) && (child->x <= (otherChild->x + otherChild->width))){
                    intoGroup = true;
                    break;
                }
            }
            
            //place it into the group
            if (intoGroup) {
                
                currentGroup.objects.push_back(child);
                currentGroup.width += child->width;
                
                //calculate the new alignment point for the group
                //std::list<long double> alignmentPoints;
                long double alignmentTotal = 0;
                for (k=0, size3 = currentGroup.objects.size(); k<size3; k++) {
                    //alignmentPoints.push_back(currentGroup.objects[k]->alignX);
                    alignmentTotal += currentGroup.objects[k]->alignX;
                }
                double long groupAlignX = alignmentTotal / size3;
                //fixme: add each (50% - anchorx) / size3
                //find the weighted middle for all points
                //double long groupAlignX = this->weightedCenter(alignmentPoints);

                HSSDisplayObject::p & currentGroupFirst = currentGroup.objects.front();
                HSSDisplayObject::p & currentGroupLast = child;
                double long anchorSpace = currentGroup.width - (currentGroupFirst->anchorX + (currentGroupLast->width - currentGroupLast->anchorX));
                
                //reposition the elements in the group
                double long startX = groupAlignX - (anchorSpace/2) - currentGroupFirst->anchorX;
                if(startX > this->width - currentGroup.width) startX = this->width - currentGroup.width;
                if(startX < 0) startX = 0;
                for (k=0, size3 = currentGroup.objects.size(); k<size3; k++) {
                    HSSDisplayObject::p & otherChild = currentGroup.objects[k];
                    otherChild->x = startX + accWidth;
                    accWidth += otherChild->width;
                }
                
                
                currentGroup.x = currentGroup.objects.front()->x;
                currentGroup.y = currentGroup.objects.front()->y;
                
                accWidth = 0;
                
                //reduce groups if necessary
                k = 0;
                while (k<groups.size()) {
                    if(k != j)
                    {
                        displayGroup & otherGroup = groups[k];
                        //if there is overlap with another group
                        if (((otherGroup.x + otherGroup.width) >= currentGroup.x) && (otherGroup.x <= (currentGroup.x + currentGroup.width))){
                            //add all the elements of this group to the overlapping one
                            otherGroup.objects.insert(otherGroup.objects.end(), currentGroup.objects.begin(), currentGroup.objects.end());
                            otherGroup.width += currentGroup.width;
                            
                            std_log1("######### reducing groups");
                            
                            //calculate the new alignment point for the group
                            long double alignmentTotal2 = 0;
                            for (l=0, size3 = otherGroup.objects.size(); l<size3; l++) {
                                //alignmentPoints.push_back(currentGroup.objects[k]->alignX);
                                alignmentTotal2 += otherGroup.objects[l]->alignX;
                            }
                            double long groupAlignX2 = alignmentTotal2 / size3;
//                            std::list<long double> alignmentPoints;
//                            for (l=0, size3 = otherGroup.objects.size(); l<size3; l++) {
//                                alignmentPoints.push_back(otherGroup.objects[l]->alignX);
//                            }
//                            //find the weighted middle for all points
//                            double long groupAlignX2 = this->weightedCenter(alignmentPoints);
                            
                            HSSDisplayObject::p & otherGroupFirst = otherGroup.objects.front();
                            HSSDisplayObject::p & otherGroupLast = otherGroup.objects.back();
                            double long ogw = otherGroup.width;
                            double long ogfax = otherGroupFirst->anchorX;
                            double long oglw = otherGroupLast->width;
                            double long oglax = otherGroupLast->anchorX;
                            double long anchorSpace2 = ogw - (ogfax + (oglw - oglax));
                            
                            //reposition the elements in the group
                            double long startX = groupAlignX2 - (anchorSpace2/2) - otherGroupFirst->anchorX;
                            if(startX > this->width - otherGroup.width) startX = this->width - otherGroup.width;
                            if(startX < 0) startX = 0;
                            for (l=0, size3 = otherGroup.objects.size(); l<size3; l++) {
                                HSSDisplayObject::p & otherChild2 = otherGroup.objects[l];
                                otherChild2->x = startX + accWidth;
                                accWidth += otherChild2->width;
                            }
                            
                            
//                            //calculate the new alignment point for the otherGroup
//                            //it will always be the midpoint between the alignment points of the first and last elements
//                            //but staying inside the parent
//                            HSSDisplayObject::p & otherGroupFirst = otherGroup.objects.front();
//                            HSSDisplayObject::p & otherGroupLast = otherGroup.objects.back();
//                            double long anchorSpace = otherGroup.width - (otherGroupFirst->anchorX + (otherGroupLast->width - otherGroupLast->anchorX));
//                            double long anchorMid = (otherGroupFirst->x + otherGroupFirst->anchorX) + (((otherGroupLast->x + otherGroupLast->anchorX) - otherGroupFirst->alignX) / 2);
//                            
//                            //reposition the elements in the group
//                            double long startX = anchorMid - otherGroupFirst->anchorX - (anchorSpace / 2);
//                            if(startX > this->width - otherGroup.width) startX = this->width - otherGroup.width;
//                            if(startX < 0) startX = 0;
//                            for (l=0, size3 = otherGroup.objects.size(); l<size3; l++) {
//                                HSSDisplayObject::p & otherChild2 = otherGroup.objects[l];
//                                
//                                otherChild2->x = startX + accWidth;
//                                accWidth += otherChild2->width;
//                            }
                            
                            
                            otherGroup.x = otherGroup.objects.front()->x;
                            otherGroup.y = otherGroup.objects.front()->y;
                            
                            accWidth = 0;
                            
                            groups.erase(groups.begin()+j);
                            j = k;
                            
                        } else {
                            k++;
                        }
                    } else {
                        k++;
                    }
                }
            }
            
            j++;
        }
        if(! intoGroup){
            displayGroup newGroup;
            newGroup.x = child->x;
            newGroup.y = child->y;
            newGroup.width = child->width;
            newGroup.height = child->height;
            newGroup.objects.push_back(child);
            groups.push_back(newGroup);
        }
    }
    
//if (i!= j && ((tempX + child->width) >= otherChild->x) && (tempX <= (otherChild->x + otherChild->width)))

   

    
//    std::ostringstream childXStream;
//    childXStream << child->x;
//    std::ostringstream otherChildXStream;
//    otherChildXStream << otherChild->x;
//    
//    std::ostringstream otherChildWidthStream;
//    otherChildWidthStream << otherChild->width;
//    
//    std_log1("child->x: "+childXStream.str()+" otherChild->x: "+otherChildXStream.str()+" and otherChild->width: "+otherChildWidthStream.str());
    
    
    //insert elements into line
    
    long double accHeight = 0;
    long double lineHeight = 0;
    std::vector<HSSDisplayObject::p> tempLine;
    for (i=0, size = byContentAlign.size(); i<size; i++) {
        HSSDisplayObject::p child = byContentAlign[i];
        
        //force the first one into the line
        if(i==0){
            tempLine.push_back(child);
            child->x = this->contentAlignX - child->anchorX + accWidth;
            accWidth += child->width;
            lineHeight = child->height;
        } else {
            if(accWidth + child->width <= this->width){
                tempLine.push_back(child);
                child->x = this->contentAlignX - child->anchorX + accWidth;
                child->y = accHeight;
                accWidth += child->width;
                if(child->height > lineHeight){
                    lineHeight = child->height;
                }
            } else {
                //push the templine onto the stack and initialize a new one
                this->layoutLines.push_back(tempLine);
                tempLine = std::vector<HSSDisplayObject::p>();
                accWidth = 0;
                accHeight += lineHeight;
                tempLine.push_back(child);
                child->x = this->contentAlignX - child->anchorX + accWidth;
                child->y = accHeight;
                accWidth += child->width;
                lineHeight = child->height;
            }
        }
    }
    
    
//    unsigned i, size;
//    long double accWidth = 0;
//    long double accHeight = 0;
//    long double lineHeight = 0;
//    std::vector<HSSDisplayObject::p> tempLine;
//    for (i=0, size = this->children.size(); i<size; i++) {
//        HSSDisplayObject::p child = this->children[i];
//        
//        //force the first one into the line
//        if(i==0){
//            tempLine.push_back(child);
//            child->x = accWidth;
//            accWidth += child->width;
//            lineHeight = child->height;
//        } else {
//            if(accWidth + child->width <= this->width){
//                tempLine.push_back(child);
//                child->x = accWidth;
//                child->y = accHeight;
//                accWidth += child->width;
//                if(child->height > lineHeight){
//                    lineHeight = child->height;
//                }
//            } else {
//                //push the templine onto the stack and initialize a new one
//                this->layoutLines.push_back(tempLine);
//                tempLine = std::vector<HSSDisplayObject::p>();
//                accWidth = 0;
//                accHeight += lineHeight;
//                tempLine.push_back(child);
//                child->x = accWidth;
//                child->y = accHeight;
//                accWidth += child->width;
//                lineHeight = child->height;
//            }
//        }
//    }
}

void HSSContainer::recursiveLayout()
{
    this->layout();
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->recursiveLayout();
    }
}


long double HSSContainer::weightedCenter(std::list<long double> &alignmentPoints)
{
    while (1) {
        unsigned thesize = alignmentPoints.size();
        if(thesize == 1){
            break;
        }
        std::list<long double> tempPoints;
        for (std::list<long double>::iterator it = alignmentPoints.begin(); it != alignmentPoints.end() ; ){
            
            long double tempPt;
            long double pt1 = *it;
            it++;
            if(it == alignmentPoints.end()){
                break;
            }
            long double pt2 = *it;
            
            tempPt = pt1 + ((pt2 - pt1) / 2);
            tempPoints.push_back(tempPt);
        }
        alignmentPoints = tempPoints;
    }
    
    return alignmentPoints.front();
}


const std::vector<HSSDisplayObject::p>& HSSContainer::getChildren() const
{
    return this->children;
}

//contentAlignX
HSSParserNode::p HSSContainer::getDContentAlignX() { return this->dContentAlignX; }
void HSSContainer::setDContentAlignX(HSSParserNode::p value)
{
    this->dContentAlignX = value;
    if(this->observedContentAlignX != NULL)
    {
        this->observedContentAlignX->removeObserver(this->observedContentAlignXProperty, HSSObservablePropertyAlignX, this);
    }
    HSSContainer::p parentContainer = this->getParent();
    const std::vector<HSSDisplayObject::p> * scope;
    if(parentContainer){
        scope = &(parentContainer->getChildren());
    } else {
        scope = NULL;
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
    this->dContentAlignY = value;
    HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
    if(this->observedContentAlignY != NULL)
    {
        this->observedContentAlignY->removeObserver(this->observedContentAlignYProperty, HSSObservablePropertyAlignY, this);
    }
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
                                        this->width,
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


void HSSContainer::setDefaults()
{
    HSSDisplayObject::setDefaults();
    
    //contentAlignX
    HSSPercentageConstant::p newDContentAlignX(new HSSPercentageConstant(50));
    this->setDContentAlignX(newDContentAlignX);
    //contentAlignY
    HSSNumberConstant::p newDContentAlignY(new HSSNumberConstant(0));
    this->setDContentAlignY(newDContentAlignY);
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
            
        default:
            throw "unknown parser node type while setting LDProperty";
            break;
    }
    
    return ret;
}


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
 *      Last changed: 2011/09/11
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 18
 *
 ********************************************************************/

#ifndef HSSCONTAINER_H
#define HSSCONTAINER_H

#include <string>
#include <vector>
#include "HSSDisplayObject.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cairo/cairo.h>
#include <list>

namespace AXR {
    enum HSSDirectionValue
    {
        HSSDirectionLeftToRight,
        HSSDirectionRightToLeft,
        HSSDirectionTopToBottom,
        HSSDirectionBottomToTop
    };
    
    class HSSContainer : public boost::enable_shared_from_this<HSSContainer>, public HSSDisplayObject
    {
    public:
        typedef boost::shared_ptr<HSSContainer> p;
        static bool isKeyword(std::string value, std::string property);
        
        friend class HSSDisplayObject;
        
        struct displayGroup
        {
            long double x;
            long double y;
            long double width;
            long double height;
            bool complete;
            std::vector<displayGroup>lines;
            std::vector<HSSDisplayObject::p>objects;
        };
        
        HSSContainer();
        HSSContainer(std::string name);
        virtual ~HSSContainer();
        virtual std::string toString();
        virtual std::string defaultObjectType(std::string property);
        
        void add(HSSDisplayObject::p child);
        void remove(unsigned index);
        
        void readDefinitionObjects();
        void recursiveReadDefinitionObjects();
        void recursiveRegenerateSurfaces();
        void recursiveDraw(cairo_t * cairo);
        
        void layout();
        void recursiveLayout();
        
        //FIXME: make protected and provide accessors
        std::vector<HSSDisplayObject::p>children;
        const std::vector<HSSDisplayObject::p>& getChildren() const;
        
        //alignX
        HSSParserNode::p getDContentAlignX();
        void setDContentAlignX(HSSParserNode::p value);
        void contentAlignXChanged(HSSObservableProperty source, void*data);
        //alignY
        HSSParserNode::p getDContentAlignY();
        void setDContentAlignY(HSSParserNode::p value);
        void contentAlignYChanged(HSSObservableProperty source, void*data);
        
        //directionPrimary
        HSSParserNode::p getDDirectionPrimary();
        void setDDirectionPrimary(HSSParserNode::p value);
        void directionPrimaryChanged(HSSObservableProperty source, void*data);
        //directionSecondary
        HSSParserNode::p getDDirectionSecondary();
        void setDDirectionSecondary(HSSParserNode::p value);
        void directionSecondaryChanged(HSSObservableProperty source, void*data);
        
        void setDefaults();
        
    protected:
        HSSParserNode::p dContentAlignX;
        long double contentAlignX;
        HSSObservable * observedContentAlignX;
        HSSObservableProperty observedContentAlignXProperty;
        HSSParserNode::p dContentAlignY;
        long double contentAlignY;
        HSSObservable * observedContentAlignY;
        HSSObservableProperty observedContentAlignYProperty;
        
        HSSParserNode::p dDirectionPrimary;
        HSSDirectionValue directionPrimary;
        HSSObservable * observedDirectionPrimary;
        HSSObservableProperty observedDirectionPrimaryProperty;
        HSSParserNode::p dDirectionSecondary;
        HSSDirectionValue directionSecondary;
        HSSObservable * observedDirectionSecondary;
        HSSObservableProperty observedDirectionSecondaryProperty;

        
    private:
        long double _setLDProperty(
                                   void(HSSContainer::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedProperty,
                                   HSSObservable *          observedObject,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty,
                                   const std::vector<HSSDisplayObject::p> * scope
                                   );
        
        bool _addChildToGroupIfNeeded(HSSDisplayObject::p &child, displayGroup &group, HSSDirectionValue direction);
        bool _mergeGroupsIfNeeded(displayGroup &group, displayGroup &otherGroup, HSSDirectionValue direction);
        void _arrange(displayGroup &group, HSSDirectionValue direction);
    };
}

#endif
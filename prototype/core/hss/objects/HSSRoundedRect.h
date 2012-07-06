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

#ifndef HSSROUNDEDRECT_H
#define HSSROUNDEDRECT_H

#include "HSSShape.h"
#include "HSSMultipleValue.h"
#include "HSSDisplayObject.h"

namespace AXR {
    
    /**
     *  @brief A shape object type that allows to create rectangles with rounded corners.
     *
     *  Right now, it only handles all corners at once through the "corners" property,
     *  but the plan is to have individual control.
     *  
     *  @todo Implement properties to allow for individual control of the corners.
     */
    class HSSRoundedRect : public HSSShape
    {
    public:
        typedef boost::shared_ptr<HSSRoundedRect> p;
        
        /**
         *  Creates a new instance of a rounded  rect object.
         */
        HSSRoundedRect();
        
        /**
         *  Copy constructor for HSSRoundedRect objects. Do not call directly, use clone() instead.
         */
        HSSRoundedRect(const HSSRoundedRect & orig);
        
        /**
         *  Clones an instance of HSSRoundedRect and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRoundedRect
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSRoundedRect();
        
        //see HSSObject.h for documentation of these
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        //see HSSShape.h for documentation of these
        virtual void draw(cairo_t * cairo, double long x, double long y, double long width, double long height);
        
        /**
         *  Getter for the actual value of corners. Always stored inside a multiple value.
         */
        HSSMultipleValue::p getCorners();
        
        /**
         *  Setter for the definition object of corners. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of corners.
         */
        void setDCorners(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect the top left corner.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void cornerTLChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Method to be passed as callback when observing changes that will affect the top right corner.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void cornerTRChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Method to be passed as callback when observing changes that will affect the bottom right corner.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void cornerBRChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Method to be passed as callback when observing changes that will affect bottom left corner.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void cornerBLChanged(HSSObservableProperty source, void*data);
        
    
    protected:
        HSSMultipleValue::p corners;
        HSSParserNode::p dCorners;
        HSSObservable * observedTLCorner;
        HSSObservableProperty observedTLCornerProperty;
        HSSObservable * observedTRCorner;
        HSSObservableProperty observedTRCornerProperty;
        HSSObservable * observedBRCorner;
        HSSObservableProperty observedBRCornerProperty;
        HSSObservable * observedBLCorner;
        HSSObservableProperty observedBLCornerProperty;
        double long cornerTL, cornerTR, cornerBR, cornerBL;
        
    private:
        long double _setLDProperty(
                                   void(HSSRoundedRect::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p       value,
                                   long double            percentageBase,
                                   HSSObservableProperty  observedProperty,
                                   HSSObservable *        observedObject,
                                   HSSObservableProperty  observedSourceProperty,
                                   HSSObservable *        &observedStore,
                                   HSSObservableProperty  &observedStoreProperty,
                                   const std::vector<HSSDisplayObject::p> * scope
                                   );
        HSSClonable::p cloneImpl() const;
    };
}

#endif

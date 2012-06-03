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
 *      Last changed: 2012/06/03
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 2
 *
 ********************************************************************/

#ifndef HSSPOLYGON_H
#define HSSPOLYGON_H

#include "HSSShape.h"
#include "HSSDisplayObject.h"

namespace AXR {
    
    /**
     *  @brief Shape object, representing the HSS object \@polygon.
     */
    class HSSPolygon : public HSSShape
    {
    public:
        typedef boost::shared_ptr<HSSPolygon> p;
        
        /**
         *  Creates a new instance of a polygon shape.
         */
        HSSPolygon();
        
        /**
         *  Copy constructor for HSSPolygon objects. Do not call directly, use clone() instead.
         */
        HSSPolygon(const HSSPolygon & orig);
        
        /**
         *  Clones an instance of HSSPolygon and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSPolygon.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSPolygon();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual bool isKeyword(std::string value, std::string property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        virtual void draw(cairo_t * cairo, double long x, double long y, double long width, double long height);
        
        /**
         *  Getter for the actual value of the sides property.
         *  @return A long double containing the amount of sides.
         */
        long double getSides();
        
        /**
         *  Getter for the definition object of sides.
         *  @return A shared pointer to the parser node containing the definition object of sides.
         */
        HSSParserNode::p getDSides();
        
        /**
         *  Setter for the definition object of sides. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of sides.
         */
        void setDSides(HSSParserNode::p value);
        
        /**
         *  Method to be passed as callback when observing changes that will affect sides.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void sidesChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Getter for the actual value of the angle property.
         *  @return A long double containing the amount of angle.
         */
        long double getAngle();
        
        /**
         *  Getter for the definition object of angle.
         *  @return A shared pointer to the parser node containing the definition object of angle.
         */
        HSSParserNode::p getDAngle();
        
        /**
         *  Setter for the definition object of angle. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of angle.
         */
        void setDAngle(HSSParserNode::p value);
        
        /**
         *  Method to be passed as callback when observing changes that will affect angle.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void angleChanged(HSSObservableProperty source, void*data);
        
    protected:
        //sides
        HSSParserNode::p dSides;
        long double sides;
        HSSObservable * observedSides;
        HSSObservableProperty observedSidesProperty;
        
        //angle
        HSSParserNode::p dAngle;
        long double angle;
        HSSObservable * observedAngle;
        HSSObservableProperty observedAngleProperty;
        
    private:
        HSSClonable::p cloneImpl() const;
        long double _setLDProperty(
                                   void(HSSPolygon::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
    };
}

#endif

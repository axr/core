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

#ifndef HSSBORDER_H
#define HSSBORDER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <cairo/cairo.h>
#include "HSSObject.h"

namespace AXR {
    /**
     *  @brief Abstract base class for all border objects.
     */
    class HSSBorder : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSBorder> p;
        typedef std::vector<HSSBorder::p>::iterator it;
        
        /**
         *  Creates a new HSSBorder object.
         */
        HSSBorder();
        /**
         *  Copy constructor for HSSBorder objects.
         */
        HSSBorder(const HSSBorder & orig);
        /**
         *  Destructor for HSSBorder objects.
         */
        virtual ~HSSBorder();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        /**
         *  Each type of border implements its own drawing routines. Call this method
         *  when you need to draw the border on the surface that is represented by the
         *  cairo handle.
         *  @param cairo    A regular pointer to a cairo handle, holding the surface on
         *                  which to draw on.
         */
        virtual void draw(cairo_t * cairo) = 0;
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        /**
         *  Getter for the value of size.
         *  @return A long double containing the value of size.
         */
        long double getSize();
        
        /**
         *  Setter for the definition object of size. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of size.
         */
        void setDSize(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect size.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void sizeChanged(HSSObservableProperty source, void*data);
        
    protected:
        //size
        long double size;
        HSSParserNode::p dSize;
        HSSObservable * observedSize;
        HSSObservableProperty observedSizeProperty;
        
        long double _setLDProperty(
                                   void(HSSBorder::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
    };
}


#endif

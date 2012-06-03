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
 *      Last changed: 2012/06/01
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 11
 *
 ********************************************************************/

#ifndef HSSLINEBORDER_H
#define HSSLINEBORDER_H

#include <string>
#include "HSSBorder.h"
#include <boost/shared_ptr.hpp>
#include "HSSRgb.h"

namespace AXR {
    
    /**
     *  @brief The object type representing line borders in HSS.
     *
     *  This border style renders solid lines on whatever path they are applied.
     */
    class HSSLineBorder : public HSSBorder
    {
    public:
        typedef boost::shared_ptr<HSSLineBorder> p;
        
        /**
         *  Creates a new instance of a line border object.
         */
        HSSLineBorder();
        
        /**
         *  Copy constructor for HSSLineBorder objects. Don't use directly, use clone() instead.
         */
        HSSLineBorder(const HSSLineBorder & orig);
        
        /**
         *  Clones an instance of HSSLineBorder and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLineBorder
         */
        p clone() const;
        
        /**
         *  Destructs the container, clearing the children and allChildren.
         */
        virtual ~HSSLineBorder();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        /**
         *  Getter for the actual value of color.
         *  @return A shared pointer to the object that encapsulates the color information for this border.
         */
        HSSObject::p getColor();
        
        /**
         *  Setter for the definition object of color. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of color.
         */
        void setDColor(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect color.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void colorChanged(HSSObservableProperty source, void*data);
        
        virtual void draw(cairo_t * cairo);
        
    protected:
        //color
        HSSRgb::p color;
        HSSParserNode::p dColor;
        HSSObservable * observedColor;
        HSSObservableProperty observedColorProperty;
    private:
        HSSClonable::p cloneImpl() const;
    };
}


#endif

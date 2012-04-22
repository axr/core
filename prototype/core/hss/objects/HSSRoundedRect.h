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
 *      Last changed: 2012/04/22
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 5
 *
 ********************************************************************/

#ifndef HSSROUNDEDRECT_H
#define HSSROUNDEDRECT_H

#include "HSSShape.h"
#include "HSSMultipleValue.h"
#include "HSSDisplayObject.h"

namespace AXR {
    class HSSRoundedRect : public HSSShape
    {
    public:
        typedef boost::shared_ptr<HSSRoundedRect> p;
        
        /**
         *  Constructor for HSSRoundedRect objects
         */
        HSSRoundedRect();
        /**
         *  Copy constructor for HSSRoundedRect objects
         */
        HSSRoundedRect(const HSSRoundedRect & orig);
        /**
         *  Clones an instance of HSSRoundedRect and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRoundedRect
         */
        p clone() const;
        virtual ~HSSRoundedRect();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        virtual void draw(cairo_t * cairo, double long x, double long y, double long width, double long height);
        
        HSSMultipleValue::p getCorners();
        void setDCorners(HSSParserNode::p);
        void cornerTLChanged(HSSObservableProperty source, void*data);
        void cornerTRChanged(HSSObservableProperty source, void*data);
        void cornerBRChanged(HSSObservableProperty source, void*data);
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

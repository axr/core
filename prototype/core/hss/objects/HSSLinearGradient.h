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
 *      Last changed: 2012/03/25
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 10
 *
 ********************************************************************/

#ifndef HSSLINEARGRADIENT_H
#define HSSLINEARGRADIENT_H

#include <string>
#include "HSSGradient.h"
#include <boost/shared_ptr.hpp>
#include "HSSRgb.h"

namespace AXR {
    class HSSLinearGradient : public HSSGradient
    {
    public:
        typedef boost::shared_ptr<HSSLinearGradient> p;
        
        /**
         *  Constructor for HSSLinearGradient objects
         */
        HSSLinearGradient();
        /**
         *  Copy constructor for HSSLinearGradient objects
         */
        HSSLinearGradient(const HSSLinearGradient & orig);
        /**
         *  Clones an instance of HSSLinearGradient and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLinearGradient
         */
        p clone() const;
        virtual ~HSSLinearGradient();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        //startX
        long double getStartX();
        HSSParserNode::p getDStartX();
        void setDStartX(HSSParserNode::p value);
        void startXChanged(HSSObservableProperty source, void *data);
        //startY
        long double getStartY();
        HSSParserNode::p getDStartY();
        void setDStartY(HSSParserNode::p value);
        void startYChanged(HSSObservableProperty source, void *data);
        //endX
        long double getEndX();
        HSSParserNode::p getDEndX();
        void setDEndX(HSSParserNode::p value);
        void endXChanged(HSSObservableProperty source, void *data);
        //endY
        long double getEndY();
        HSSParserNode::p getDEndY();
        void setDEndY(HSSParserNode::p value);
        void endYChanged(HSSObservableProperty source, void *data);
        
        virtual void draw(cairo_t * cairo);
        
    protected:
        //startX
        long double startX;
        HSSParserNode::p dStartX;
        HSSObservable * observedStartX;
        HSSObservableProperty observedStartXProperty;
        
        //startY
        long double startY;
        HSSParserNode::p dStartY;
        HSSObservable * observedStartY;
        HSSObservableProperty observedStartYProperty;
        
        //endX
        long double endX;
        HSSParserNode::p dEndX;
        HSSObservable * observedEndX;
        HSSObservableProperty observedEndXProperty;
        
        //endY
        long double endY;
        HSSParserNode::p dEndY;
        HSSObservable * observedEndY;
        HSSObservableProperty observedEndYProperty;
        
        long double _setLDProperty(
                                   void(HSSLinearGradient::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p       value,
                                   long double            percentageBase,
                                   HSSObservableProperty  observedProperty,
                                   HSSObservable *        observedObject,
                                   HSSObservableProperty  observedSourceProperty,
                                   HSSObservable *        &observedStore,
                                   HSSObservableProperty  &observedStoreProperty
                                   );
    private:
        virtual HSSClonable::p cloneImpl() const;

    };
}


#endif

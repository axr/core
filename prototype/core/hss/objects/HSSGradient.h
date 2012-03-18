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
 *      Last changed: 2012/03/15
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 2
 *
 ********************************************************************/

#ifndef HSSGRADIENT_H
#define HSSGRADIENT_H

#include <string>
#include "HSSObject.h"
#include "HSSRgb.h"
#include <boost/shared_ptr.hpp>
#include <cairo/cairo.h>

namespace AXR {
    class HSSGradient : public HSSObject
    {
    public:
        
        HSSGradient();
        virtual ~HSSGradient();
        
        typedef boost::shared_ptr<HSSGradient> p;
        typedef std::vector<HSSGradient::p>::iterator it;
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        virtual void draw(cairo_t * cairo) = 0;
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        //startColor
        HSSRgb::p getStartColor();
        HSSParserNode::p getDStartColor();
        void setDStartColor(HSSParserNode::p);
        void startColorChanged(HSSObservableProperty source, void*data);
        
        //endColor
        HSSRgb::p getEndColor();
        HSSParserNode::p getDEndColor();
        void setDEndColor(HSSParserNode::p);
        void endColorChanged(HSSObservableProperty source, void*data);
        
        //balance
        long double getBalance();
        HSSParserNode::p getDBalance();
        void setDBalance(HSSParserNode::p);
        void balanceChanged(HSSObservableProperty source, void*data);
        
        //colorStops
        HSSParserNode::p getDColorStops();
        void setDColorStops(HSSParserNode::p value);
        void addDColorStops(HSSParserNode::p value);
        void colorStopsChanged(HSSObservableProperty source, void*data);
        
    protected:
        //startColor
        HSSRgb::p startColor;
        HSSParserNode::p dStartColor;
        HSSObservable * observedStartColor;
        HSSObservableProperty observedStartColorProperty;
        
        //endColor
        HSSRgb::p endColor;
        HSSParserNode::p dEndColor;
        HSSObservable * observedEndColor;
        HSSObservableProperty observedEndColorProperty;
        
        //balance
        long double balance;
        HSSParserNode::p dBalance;
        HSSObservable * observedBalance;
        HSSObservableProperty observedBalanceProperty;
        
        //colorStops
        HSSParserNode::p dColorStops;
        HSSObservable * observedColorStops;
        HSSObservableProperty observedColorStopsProperty;
        std::vector<HSSObject::p> colorStops;
        
        long double _setLDProperty(
                                   void(HSSGradient::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
    };
}


#endif

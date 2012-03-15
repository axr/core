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
 *      Last changed: 2012/03/08
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSCOLORSTOP_H
#define HSSCOLORSTOP_H

#include <string>
#include "HSSObject.h"
#include "HSSRgb.h"
#include <boost/shared_ptr.hpp>
#include <cairo/cairo.h>

namespace AXR {
    class HSSColorStop : public HSSObject
    {
    public:
        
        HSSColorStop();
        virtual ~HSSColorStop();
        
        typedef boost::shared_ptr<HSSColorStop> p;
        typedef std::vector<HSSColorStop::p>::iterator it;
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        //color
        HSSRgb::p getColor();
        HSSParserNode::p getDColor();
        void setDColor(HSSParserNode::p);
        void colorChanged(HSSObservableProperty source, void*data);
        
        //position
        long double getPosition();
        HSSParserNode::p getDPosition();
        void setDPosition(HSSParserNode::p);
        void positionChanged(HSSObservableProperty source, void*data);
        
        //balance
        long double getBalance();
        HSSParserNode::p getDBalance();
        void setDBalance(HSSParserNode::p);
        void balanceChanged(HSSObservableProperty source, void*data);
        
    protected:
        //color
        HSSRgb::p color;
        HSSParserNode::p dColor;
        HSSObservable * observedColor;
        HSSObservableProperty observedColorProperty;
        
        //position
        long double position;
        HSSParserNode::p dPosition;
        HSSObservable * observedPosition;
        HSSObservableProperty observedPositionProperty;
        
        //balance
        long double balance;
        HSSParserNode::p dBalance;
        HSSObservable * observedBalance;
        HSSObservableProperty observedBalanceProperty;
        
        long double _setLDProperty(
                                   void(HSSColorStop::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
    };
}

#endif

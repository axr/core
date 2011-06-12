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
 *      Last changed: 2011/06/11
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSRGBA_H
#define HSSRGBA_H

#include <string>
#include "HSSObject.h"
#include <boost/shared_ptr.hpp>

namespace AXR {
    class HSSRgba : public HSSObject
    {
    public:
        HSSRgba();
        virtual ~HSSRgba();
        
        typedef boost::shared_ptr<HSSRgba> p;
        
        virtual std::string toString();
        virtual std::string defaultObjectType(std::string property);
//        virtual bool isKeyword(std::string value, std::string property);
        
        virtual void setProperty(std::string name, HSSParserNode::p value);
        
        long double getRed();
        void redChanged(HSSObservableProperty source, void*data);
        long double getGreen();
        void greenChanged(HSSObservableProperty source, void*data);
        long double getBlue();
        void blueChanged(HSSObservableProperty source, void*data);
        long double getAlpha();
        void alphaChanged(HSSObservableProperty source, void*data);
        
    private:
        long double red;
        long double green;
        long double blue;
        long double alpha;
        
        
        HSSObservable * observedRed;
        HSSObservableProperty observedRedProperty;
        HSSObservable * observedGreen;
        HSSObservableProperty observedGreenProperty;
        HSSObservable * observedBlue;
        HSSObservableProperty observedBlueProperty;
        HSSObservable * observedAlpha;
        HSSObservableProperty observedAlphaProperty;
        
        long double _setLDProperty(
                                   void(HSSRgba::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
    };

}


#endif
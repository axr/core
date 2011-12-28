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
 *      Last changed: 2011/12/28
 *      HSS version: 1.0
 *      Core version: 0.43
 *      Revision: 3
 *
 ********************************************************************/

#ifndef HSSFONT_H
#define HSSFONT_H

#include <string>
#include "HSSObject.h"
#include "HSSRgb.h"
#include "../parsing/HSSKeywordConstant.h"

namespace AXR {
    class HSSFont : public HSSObject
    {
    public:        
        friend class HSSParser;
        
        HSSFont();
        virtual ~HSSFont();
        
        typedef boost::shared_ptr<HSSFont> p;
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        //size
        long double getSize();
        void setDSize(HSSParserNode::p);
        void sizeChanged(HSSObservableProperty source, void*data);
        
        //face
        std::string getFace();
        void setDFace(HSSParserNode::p);
        void faceChanged(HSSObservableProperty source, void*data);
        
        //color
        HSSRgb::p getColor();
        HSSParserNode::p getDColor();
        void setDColor(HSSParserNode::p);
        void colorChanged(HSSObservableProperty source, void*data);
        
        //weight
        HSSKeywordConstant::p getWeight();
        void setDWeight(HSSParserNode::p);
        void weightChanged(HSSObservableProperty source, void*data);
        
    private:
        //size
        long double size;
        HSSParserNode::p dSize;
        HSSObservable * observedSize;
        HSSObservableProperty observedSizeProperty;
        //face
        std::string face;
        HSSParserNode::p dFace;
        HSSObservable * observedFace;
        HSSObservableProperty observedFaceProperty;
        //startColor
        HSSRgb::p color;
        HSSParserNode::p dColor;
        HSSObservable * observedColor;
        HSSObservableProperty observedColorProperty;
        //weight
        HSSKeywordConstant::p weight;
        HSSParserNode::p dWeight;
        HSSObservable * observedWeight;
        HSSObservableProperty observedWeightProperty;
        
        
        long double _setLDProperty(
                                   void(HSSFont::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
    };
    
}


#endif
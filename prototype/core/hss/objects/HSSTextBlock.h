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
 *      Last changed: 2012/01/28
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSTEXTBLOCK_H
#define HSSTEXTBLOCK_H

#include "HSSDisplayObject.h"
#include <pango/pangocairo.h>

namespace AXR {
    enum HSSTextTransformType
    {
        HSSTextTransformTypeNone = 0,
        HSSTextTransformTypeLowercase,
        HSSTextTransformTypeUppercase,
        HSSTextTransformTypeCapitalize,
        HSSTextTransformTypeHumanize
    };
    
    enum HSSTextAlignType
    {
        HSSTextAlignTypeNone = 0,
        HSSTextAlignTypeLeft,
        HSSTextAlignTypeRight,
        HSSTextAlignTypeCenter,
        HSSTextAlignTypeJustify
    };
    
    class HSSTextBlock : public HSSDisplayObject
    {
    public:
        
        static HSSTextTransformType textTransformTypeFromString(std::string value);
        static HSSTextAlignType textAlignTypeFromString(std::string value);

        typedef boost::shared_ptr<HSSTextBlock> p;
        typedef std::vector<HSSTextBlock::p>::iterator it;
        
        HSSTextBlock();
        virtual ~HSSTextBlock();
        
        virtual std::string defaultObjectType();
        virtual bool isKeyword(std::string value, std::string property);
        virtual std::string toString();
        virtual void setDefaults();
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        virtual void drawForeground();
        virtual void layout();
        
        //transform
        HSSTextTransformType getTransform();
        HSSParserNode::p getDTransform();
        void setDTransform(HSSParserNode::p value);
        void transformChanged(HSSObservableProperty source, void*data);
        
        //textAlign
        HSSTextAlignType getTextAlign();
        HSSParserNode::p getDTextAlign();
        void setDTextAlign(HSSParserNode::p value);
        void textAlignChanged(HSSObservableProperty source, void*data);
        
        //text
        std::string getText();
        HSSParserNode::p getDText();
        void setDText(HSSParserNode::p value);
        void textChanged(HSSObservableProperty source, void*data);
        
        void trimContentText();
        
    protected:
        //transform
        HSSTextTransformType transform;
        HSSParserNode::p dTransform;
        HSSObservable * observedTransform;
        HSSObservableProperty observedTransformProperty;
        
        //textAlign
        HSSTextAlignType textAlign;
        HSSParserNode::p dTextAlign;
        HSSObservable * observedTextAlign;
        HSSObservableProperty observedTextAlignProperty;
        
        //text
        std::string text;
        HSSParserNode::p dText;
        HSSObservable * observedText;
        HSSObservableProperty observedTextProperty;
        
        PangoLayout * _layout;
    };
}

#endif

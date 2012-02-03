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
 *      Last changed: 2012/02/02
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 12
 *
 ********************************************************************/

#ifndef HSSOBSERVABLEPROPERTIES_H
#define HSSOBSERVABLEPROPERTIES_H

namespace AXR {
    enum HSSObservableProperty
    {
        //HSSObject
        HSSObservablePropertyNone = 0,
        HSSObservablePropertyIsA,
        
        //HSSDisplayObject
        HSSObservablePropertyValue,
        HSSObservablePropertyWidth,
        HSSObservablePropertyHeight,
        HSSObservablePropertyAnchorX,
        HSSObservablePropertyAnchorY,
        HSSObservablePropertyFlow,
        HSSObservablePropertyAlignX,
        HSSObservablePropertyAlignY,
        HSSObservablePropertyMargin,
        HSSObservablePropertyPadding,
        HSSObservablePropertyBorder,
        
        //HSSContainer
        HSSObservablePropertyContentAlignX,
        HSSObservablePropertyContentAlignY,
        HSSObservablePropertyDirectionPrimary,
        HSSObservablePropertyDirectionSecondary,
        HSSObservablePropertyBackground,
        HSSObservablePropertyFont,
        HSSObservablePropertyShape,
        HSSObservablePropertyBehavior,
        
        //HSSTextBlock
        HSSObservablePropertyTransform,
        HSSObservablePropertyTextAlign,
        HSSObservablePropertyText,
        
        //HSSRgb
        HSSObservablePropertyRed,
        HSSObservablePropertyGreen,
        HSSObservablePropertyBlue,
        HSSObservablePropertyAlpha,
        
        //HSSGradient
        HSSObservablePropertyStartColor,
        HSSObservablePropertyEndColor,
        HSSObservablePropertyColorStops,
        HSSObservablePropertyBalance,
        
        //HSSLinearGradient
        HSSObservablePropertyStartX,
        HSSObservablePropertyStartY,
        HSSObservablePropertyEndX,
        HSSObservablePropertyEndY,
        
        //HSSFont
        HSSObservablePropertySize,
        HSSObservablePropertyFace,
        HSSObservablePropertyColor,
        HSSObservablePropertyWeight,
        
        //HSSRoundedRect
        HSSObservablePropertyCorners,
        
        //HSSEvent
        HSSObservablePropertyAction,
        
        //HSSRequest
        HSSObservablePropertySrc,
        HSSObservablePropertyTarget,
        HSSObservablePropertyMode,
        HSSObservablePropertyContentTarget,
    };
}

#endif //HSSOBSERVABLEPROPERTIES_H




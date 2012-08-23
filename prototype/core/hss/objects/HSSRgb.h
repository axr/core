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

#ifndef HSSRGB_H
#define HSSRGB_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "HSSObject.h"

namespace AXR {
    
    /**
     *  @brief The object type representing a \@rgb color in HSS.
     *
     *  Many objects accept color objects for their properties, e.g. HSSContainer's background property.
     *
     *  This object encapsulates
     */
    class HSSRgb : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSRgb> p;
        
        //virtual bool isKeyword(std::string value, std::string property);
        
        friend class HSSParser;
        
        /**
         *  Constructor for HSSRgb objects
         */
        HSSRgb();
        
        /**
         *  Copy constructor for HSSRgb objects
         */
        HSSRgb(const HSSRgb & orig);
        
        /**
         *  Clones an instance of HSSRgb and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRgb
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSRgb();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        
        /**
         *  Getter for the actual value of the red channel.
         *  @return A long double containing the value for the red channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        long double getRed();
        
        /**
         *  Setter for the definition object of red. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of red.
         */
        void setDRed(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect the red channel.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void redChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Getter for the actual value of the green channel.
         *  @return A long double containing the value for the green channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        long double getGreen();
        
        /**
         *  Setter for the definition object of green. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of green.
         */
        void setDGreen(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect the green channel.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void greenChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Getter for the actual value of the blue channel.
         *  @return A long double containing the value for the blue channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        long double getBlue();
        
        /**
         *  Setter for the definition object of blue. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of blue.
         */
        void setDBlue(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect the blue channel.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void blueChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Getter for the actual value of the alpha channel.
         *  @return A long double containing the value for the alpha channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        long double getAlpha();
        
        /**
         *  Setter for the definition object of alpha. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of alpha.
         */
        void setDAlpha(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect the alpha channel.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void alphaChanged(HSSObservableProperty source, void*data);
        
    private:
        long double red;
        long double green;
        long double blue;
        long double alpha;
        
        HSSParserNode::p dRed;
        HSSObservable * observedRed;
        HSSObservableProperty observedRedProperty;
        HSSParserNode::p dGreen;
        HSSObservable * observedGreen;
        HSSObservableProperty observedGreenProperty;
        HSSParserNode::p dBlue;
        HSSObservable * observedBlue;
        HSSObservableProperty observedBlueProperty;
        HSSParserNode::p dAlpha;
        HSSObservable * observedAlpha;
        HSSObservableProperty observedAlphaProperty;
        
        long double _setLDProperty(
                                   void(HSSRgb::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
        HSSClonable::p cloneImpl() const;
    };

}


#endif

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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
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

#include <QSharedPointer>
#include "HSSObject.h"
#include "HSSUnits.h"

namespace AXR
{
    /**
     *  @brief The object type representing a \@rgb color in HSS.
     *
     *  Many objects accept color objects for their properties, e.g. HSSContainer's background property.
     *
     *  This object encapsulates
     */
    class AXR_API HSSRgb : public HSSObject
    {
    public:
        typedef QSharedPointer<HSSRgb> p;

        //virtual bool isKeyword(AXRString value, AXRString property);

        friend class HSSParser;

        /**
         *  Creates a new instance of a black color.
         *  @return A shared pointer to the new color object.
         */
        static HSSRgb::p blackColor();

        /**
         *  Creates a new instance of a white color.
         *  @return A shared pointer to the new color object.
         */
        static HSSRgb::p whiteColor();

        /**
         *  Constructor for HSSRgb objects
         */
        HSSRgb(HSSUnit red = 0, HSSUnit green = 0, HSSUnit blue = 0, HSSUnit alpha = 255);

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

        inline static p transparentColor() { return p(new HSSRgb(0, 0, 0, 0)); }
        inline static p defaultColor() { return p(new HSSRgb()); }

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);


        /**
         *  Getter for the actual value of the red channel.
         *  @return A HSSUnit containing the value for the red channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getRed();

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
         *  @return A HSSUnit containing the value for the green channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getGreen();

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
         *  @return A HSSUnit containing the value for the blue channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getBlue();

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
         *  @return A HSSUnit containing the value for the alpha channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getAlpha();

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
        HSSUnit red;
        HSSUnit green;
        HSSUnit blue;
        HSSUnit alpha;

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

        HSSUnit _evaluatePropertyValue(
                                   void(HSSRgb::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty
                                   );
        HSSClonable::p cloneImpl() const;
    };
}

Q_DECLARE_METATYPE(AXR::HSSRgb::p);
Q_DECLARE_METATYPE(AXR::HSSRgb::p*);

#endif

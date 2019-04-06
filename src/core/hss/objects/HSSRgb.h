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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
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

#include "HSSObject.h"

namespace AXR
{
    class HSSValue;

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
        //virtual bool isKeyword(AXRString value, AXRString property);

        friend class HSSParser;

        /**
         *  Creates a new instance of a black color.
         *  @return A shared pointer to the new color object.
         */
        static QSharedPointer<HSSRgb> blackColor(AXRController * controller);

        /**
         *  Creates a new instance of a white color.
         *  @return A shared pointer to the new color object.
         */
        static QSharedPointer<HSSRgb> whiteColor(AXRController * controller);

        /**
         *  Constructor for HSSRgb objects
         */
        HSSRgb(AXRController * controller);

        /**
         *  Copy constructor for HSSRgb objects
         */
        HSSRgb(const HSSRgb & orig);

        /**
         *  Clones an instance of HSSRgb and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRgb
         */
        QSharedPointer<HSSRgb> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSRgb();

        inline static QSharedPointer<HSSRgb> transparentColor(AXRController * controller)
        {
            QSharedPointer<HSSRgb> ret(new HSSRgb(controller));
            ret->setAlpha(0., 1);
            return ret;
        }
        inline static QSharedPointer<HSSRgb> defaultColor(AXRController * controller) { return QSharedPointer<HSSRgb>(new HSSRgb(controller)); }

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);

        QSharedPointer<HSSObject> computeRed(QSharedPointer<HSSParserNode> parserNode);
        void listenRed(QSharedPointer<HSSObject> theObj);
        void notifyRed(QSharedPointer<HSSObject> theObj);
        /**
         *  Getter for the actual value of the red channel.
         *  @return A HSSUnit containing the value for the red channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getRed() const;

        /**
         *  Setter for the definition object of red. It will use the value as needed.
         *  @param value    The new value.
         *  @param specificity A HSSUnit that determines how much "weight" the property has.
         */
        void setRed(HSSUnit value, HSSUnit specificity);

        QSharedPointer<HSSObject> computeGreen(QSharedPointer<HSSParserNode> parserNode);
        void listenGreen(QSharedPointer<HSSObject> theObj);
        void notifyGreen(QSharedPointer<HSSObject> theObj);

        /**
         *  Getter for the actual value of the green channel.
         *  @return A HSSUnit containing the value for the green channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getGreen() const;

        /**
         *  Setter for the definition object of green. It will use the value as needed.
         *  @param value    The new value.
         *  @param specificity A HSSUnit that determines how much "weight" the property has.
         */
        void setGreen(HSSUnit value, HSSUnit specificity);

        QSharedPointer<HSSObject> computeBlue(QSharedPointer<HSSParserNode> parserNode);
        void listenBlue(QSharedPointer<HSSObject> theObj);
        void notifyBlue(QSharedPointer<HSSObject> theObj);
        /**
         *  Getter for the actual value of the blue channel.
         *  @return A HSSUnit containing the value for the blue channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getBlue() const;

        /**
         *  Setter for the definition object of blue. It will use the value as needed.
         *  @param value    The new value.
         *  @param specificity A HSSUnit that determines how much "weight" the property has.
         */
        void setBlue(HSSUnit value, HSSUnit specificity);

        QSharedPointer<HSSObject> computeAlpha(QSharedPointer<HSSParserNode> parserNode);
        void listenAlpha(QSharedPointer<HSSObject> theObj);
        void notifyAlpha(QSharedPointer<HSSObject> theObj);
        /**
         *  Getter for the actual value of the alpha channel.
         *  @return A HSSUnit containing the value for the alpha channel. The range of this
         *  value is from 0.0 to 255.0.
         */
        HSSUnit getAlpha() const;

        /**
         *  Setter for the definition object of alpha. It will use the value as needed.
         *  @param value    The new value.
         *  @param specificity A HSSUnit that determines how much "weight" the property has.
         */
        void setAlpha(HSSUnit value, HSSUnit specificity);

    private:
        void _initialize();
        QSharedPointer<HSSValue> _computeChannelValue(QSharedPointer<HSSParserNode> parserNode) const;

        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

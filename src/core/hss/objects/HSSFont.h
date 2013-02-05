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

#ifndef HSSFONT_H
#define HSSFONT_H

#include "HSSObject.h"

namespace AXR
{
    class HSSKeywordConstant;
    class HSSRgb;

    /**
     *  @brief This class represents the HSS object type \@font.
     */
    class AXR_API HSSFont : public HSSObject
    {
    public:
        friend class HSSParser;

        static const HSSUnit DEFAULT_SIZE;

        /**
         *  Creates a new instance of a HSSFont object.
         */
        HSSFont(AXRController * controller);

        /**
         *  Copy constructor for HSSFont objects. Do not call directly, use clone() instead.
         */
        HSSFont(const HSSFont & orig);

        /**
         *  Clones an instance of HSSFont and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFont
         */
        QSharedPointer<HSSFont> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFont();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);
        bool isKeyword(AXRString value, AXRString property);

        /**
         *  Getter for the actual value of size.
         *  @return A HSSUnit containing the size in points of the font.
         */
        HSSUnit getSize();

        /**
         *  Setter for the definition object of size. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of size.
         */
        void setDSize(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect size.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void sizeChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of face.
         *  @return A string containing the name of the typeface that will be used.
         */
        AXRString getFace();

        /**
         *  Setter for the definition object of face. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of face.
         */
        void setDFace(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect face.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void faceChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of color.
         *  @return A shared pointer to a HSSRgb object representing the used color.
         *
         *  @todo what about using gradients on text?
         */
        QSharedPointer<HSSRgb> getColor();

        /**
         *  Getter for the definition object of color.
         *  @return A shared pointer to the parser node containing the definition object of color.
         */
        QSharedPointer<HSSParserNode> getDColor();

        /**
         *  Setter for the definition object of color. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of color.
         */
        void setDColor(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect color.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void colorChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of weight.
         *  @return A shared pointer to a keyword constant representing the used weight.
         */
        QSharedPointer<HSSKeywordConstant> getWeight();

        /**
         *  Setter for the definition object of weight. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of weight.
         */
        void setDWeight(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect weight.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void weightChanged(HSSObservableProperty source, void*data);

    private:
        //size
        HSSUnit size;
        QSharedPointer<HSSParserNode> dSize;
        HSSObservable * observedSize;
        HSSObservableProperty observedSizeProperty;
        //face
        AXRString face;
        QSharedPointer<HSSParserNode> dFace;
        HSSObservable * observedFace;
        HSSObservableProperty observedFaceProperty;
        //startColor
        QSharedPointer<HSSRgb> color;
        QSharedPointer<HSSParserNode> dColor;
        HSSObservable * observedColor;
        HSSObservableProperty observedColorProperty;
        //weight
        QSharedPointer<HSSKeywordConstant> weight;
        QSharedPointer<HSSParserNode> dWeight;
        HSSObservable * observedWeight;
        HSSObservableProperty observedWeightProperty;


        HSSUnit _evaluatePropertyValue(
                                   void(HSSFont::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty
                                   );
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSFont> >)
Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSFont> >*)

#endif

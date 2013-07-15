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

        static QSharedPointer<HSSFont> defaultFont(AXRController * controller);

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

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        bool isKeyword(AXRString value, AXRString property);

        /**
         *  Getter for the computed value of size.
         *  @return A HSSUnit containing the size in points of the font.
         */
        const HSSUnit getSize() const;

        /**
         *  Getter for the computed value of face.
         *  @return A string containing the name of the typeface that will be used.
         */
        const AXRString getFace() const;

        /**
         *  Getter for the computed value of color.
         *  @return A shared pointer to a HSSRgb object representing the used color.
         *
         *  @todo what about using gradients on text?
         */
        const QSharedPointer<HSSRgb> getColor() const;
        QSharedPointer<HSSObject> computeColor(QSharedPointer<HSSParserNode> parserNode);

        /**
         *  Getter for the computed value of weight.
         *  @return A string representing the used weight.
         */
        const AXRString getWeight() const;

    private:
        void _initialize();
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

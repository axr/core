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

#ifndef HSSROUNDEDRECT_H
#define HSSROUNDEDRECT_H

#include "HSSShape.h"

namespace AXR
{
    class HSSMultipleValue;

    /**
     *  @brief A shape object type that allows to create rectangles with rounded corners.
     *
     *  Right now, it only handles all corners at once through the "corners" property,
     *  but the plan is to have individual control.
     *
     *  @todo Implement properties to allow for individual control of the corners.
     */
    class AXR_API HSSRoundedRect : public HSSShape
    {
    public:
        /**
         *  Creates a new instance of a rounded  rect object.
         */
        HSSRoundedRect(AXRController * controller);

        /**
         *  Copy constructor for HSSRoundedRect objects. Do not call directly, use clone() instead.
         */
        HSSRoundedRect(const HSSRoundedRect & orig);

        /**
         *  Clones an instance of HSSRoundedRect and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRoundedRect
         */
        QSharedPointer<HSSRoundedRect> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSRoundedRect();

        //see HSSObject.h for documentation of these
        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        //see HSSShape.h for documentation of these
        virtual void createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, QList<QSharedPointer<HSSParserNode> > segments);

        void createRoundedRect(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, HSSUnit offset);

        virtual void drawBorders(QPainter &painter, QList<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY);

        void stackCorners(QSharedPointer<HSSParserNode> parserNode);
        void stackTop(QSharedPointer<HSSParserNode> parserNode);
        void stackRight(QSharedPointer<HSSParserNode> parserNode);
        void stackBottom(QSharedPointer<HSSParserNode> parserNode);
        void stackLeft(QSharedPointer<HSSParserNode> parserNode);

        const HSSUnit getLeftTop() const;
        const HSSUnit getLeftBottom() const;
        const HSSUnit getRightTop() const;
        const HSSUnit getRightBottom() const;

    private:
        void _initialize();
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

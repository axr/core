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

#ifndef HSSSHAPE_H
#define HSSSHAPE_H

#include <QSharedPointer>
#include "HSSObject.h"
#include "HSSUnits.h"
#include <QPainter>

namespace AXR
{
    class HSSBorder;

    /**
     *  @brief Abstract base class for all shape objects.
     */
    class AXR_API HSSShape : public HSSObject
    {
    public:
        typedef QSharedPointer<HSSShape> p;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSShape();

        /**
         *  Allows you to check if this shape is of the given type.
         *  @param  type    The shape type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSShapeType type);

        /**
         *  @return The shape type of this instance.
         */
        HSSShapeType getShapeType();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        /**
         *  Each shape object subclass implements its own drawing routines, creating the path as needed.
         *  The x, y, width and height parameters are used to define the size of the resulting shape. Usually
         *  theses are the x:0, y:0 (upper left corner) and as wide and tall as the container, but they can be
         *  different in case of joining paths together, for example.
         *
         *  @param path     A QPainterPath to configure the shape of the object.
         *  @param x        The horizontal coordinate for the upper left corner of the bounding box of the shape.
         *  @param y        The vertical coordinate for the upper left corner of the bounding box of the shape.
         *  @param width    The width of the bounding box of the shape.
         *  @param height   The height of the bounding box of the shape.
         *  @param segments The segments that will be drawn.
         */
        virtual void createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::vector<HSSParserNode::p> segments) = 0;

        /**
         *  Convenience function to create a path for all segments.
         *
         *  @param path     A QPainterPath to configure the shape of the object.
         *  @param x        The horizontal coordinate for the upper left corner of the bounding box of the shape.
         *  @param y        The vertical coordinate for the upper left corner of the bounding box of the shape.
         *  @param width    The width of the bounding box of the shape.
         *  @param height   The height of the bounding box of the shape.
         */
        void createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height);

        virtual void drawBorders(QPainter &painter, std::vector<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit borderBleeding) = 0;

    protected:
        /**
         *  Creates a new instance of a shape object. Do call directly, use one of the sublcasses instead.
         *  @param type     The type of this shape subclass for identification purposes.
         */
        HSSShape(HSSShapeType type, AXRController * controller);

        /**
         *  Copy constructor for HSSShape objects. Do not call directly, use clone() instead.
         */
        HSSShape(const HSSShape & orig);

    private:
        HSSShapeType shapeType;
    };
}

Q_DECLARE_METATYPE(AXR::HSSShape::p)
Q_DECLARE_METATYPE(AXR::HSSShape::p*)

#endif

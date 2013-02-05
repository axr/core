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
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef HSSBORDER_H
#define HSSBORDER_H

#include <QSharedPointer>
#include "HSSObject.h"

class QPainter;
class QPainterPath;

namespace AXR
{
    /**
     *  @brief Abstract base class for all border objects.
     */
    class AXR_API HSSBorder : public HSSObject
    {
    public:
        typedef std::vector<QSharedPointer<HSSBorder> >::iterator it;

        /**
         *  Destructor for HSSBorder objects.
         */
        virtual ~HSSBorder();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        /**
         *  Each type of border implements its own drawing routines. Call this method
         *  when you need to draw the border on a surface.
         *  @param painter A QPainter context used to draw onto a surface.
         *  @param path The path comprising the object shape.
         */
        virtual void draw(QPainter &painter, const QPainterPath &path) = 0;

        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);

        /**
         *  Getter for the value of size.
         *  @return An HSSUnit containing the value of size.
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
         *  Getter for the value of position.
         *  @return A HSSBorderPosition containing the value for position.
         */
        HSSBorderPosition getPosition();

        /**
         *  Getter for the definition object of position.
         *  @return A shared pointer to the parser node containing the definition object of position.
         */
        QSharedPointer<HSSParserNode> getDPosition();

        /**
         *  Setter for the definition object of position. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of position.
         */
        void setDPosition(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect position.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void positionChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the value of segments.
         *  @return A vector of shared pointers to parser nodes.
         */
        std::vector<QSharedPointer<HSSParserNode> > getSegments();

        /**
         *  Getter for the definition object of segments.
         *  @return A shared pointer to the parser node containing the definition object of segments.
         */
        QSharedPointer<HSSParserNode> getDSegments();

        /**
         *  Setter for the definition object of segments. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of segments.
         */
        void setDSegments(QSharedPointer<HSSParserNode> value);

        /**
         *  Since this property accepts multiple values, this allows to append a value instead of
         *  replacing the whole thing.
         *  @param value    A shared pointer to the parser node containing the definition object to be added
         *                  to segments.
         */
        void addDSegments(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect textAlign.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void segmentsChanged(HSSObservableProperty source, void*data);

    protected:
        /**
         *  Creates a new HSSBorder object.
         */
        HSSBorder(AXRController * controller);

        /**
         *  Copy constructor for HSSBorder objects.
         */
        HSSBorder(const HSSBorder & orig);

        //size
        HSSUnit size;
        QSharedPointer<HSSParserNode> dSize;
        HSSObservable * observedSize;
        HSSObservableProperty observedSizeProperty;

        //position
        HSSBorderPosition position;
        QSharedPointer<HSSParserNode> dPosition;
        HSSObservable * observedPosition;
        HSSObservableProperty observedPositionProperty;

        //segments
        QSharedPointer<HSSParserNode> dSegments;
        HSSObservable * observedSegments;
        HSSObservableProperty observedSegmentsProperty;
        std::vector<QSharedPointer<HSSParserNode> > segments;

        HSSUnit _evaluatePropertyValue(
                                   void(HSSBorder::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty
                                   );
    };
}

Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSBorder> >)
Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSBorder> >*)
Q_DECLARE_METATYPE(AXR::HSSBorderPosition*)

#endif

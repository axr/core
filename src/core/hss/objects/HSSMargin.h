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

#ifndef HSSMARGIN_H
#define HSSMARGIN_H

#include <QSharedPointer>
#include "HSSObject.h"

namespace AXR
{
    class HSSDisplayObject;
    class HSSSimpleSelection;

    /**
     *  @brief Abstract base class to provide common functionality for margin object types.
     *
     *  Margins are spaces around elements, that won't be occupied by other elements that
     *  are in flow.
     */
    class AXR_API HSSMargin : public HSSObject
    {
    public:
        /**
         *  Creates a new instance of a margin object. Don't use directly, this is only for
         *  subclasses.
         */
        HSSMargin(AXRController * controller);

        /**
         *  Copy constructor for HSSMargin objects. Don't call directly, but use clone() instead.
         */
        HSSMargin(const HSSMargin & orig);

        /**
         *  Clones an instance of HSSMargin and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSMargin
         */
        QSharedPointer<HSSMargin> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSMargin();

        //see HSSObject.h for documentation of these
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);

        /**
         *  Getter for the definition object of size.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        const QSharedPointer<HSSParserNode> getDSize() const;

        /**
         *  Setter for the definition object of size. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of value.
         */
        void setDSize(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect size.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void sizeChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of top.
         *  @return An HSSUnit containing the value of size.
         */
        HSSUnit getTop() const;

        /**
         *  Getter for the definition object of top.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        const QSharedPointer<HSSParserNode> getDTop() const;

        /**
         *  Setter for the definition object of top. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of value.
         */
        void setDTop(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect top.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void topChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of right.
         *  @return An HSSUnit containing the value of size.
         */
        HSSUnit getRight() const;

        /**
         *  Getter for the definition object of right.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        const QSharedPointer<HSSParserNode> getDRight() const;

        /**
         *  Setter for the definition object of right. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of value.
         */
        void setDRight(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect right.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void rightChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of bottom.
         *  @return An HSSUnit containing the value of size.
         */
        HSSUnit getBottom() const;

        /**
         *  Getter for the definition object of bottom.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        const QSharedPointer<HSSParserNode> getDBottom() const;

        /**
         *  Setter for the definition object of bottom. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of value.
         */
        void setDBottom(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect bottom.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void bottomChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of left.
         *  @return An HSSUnit containing the value of size.
         */
        HSSUnit getLeft() const;

        /**
         *  Getter for the definition object of left.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        const QSharedPointer<HSSParserNode> getDLeft() const;

        /**
         *  Setter for the definition object of left. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of value.
         */
        void setDLeft(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect left.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void leftChanged(HSSObservableProperty source, void*data);

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        //size
        QSharedPointer<HSSParserNode> dSize;
        //top
        HSSObservable * observedTop;
        HSSObservableProperty observedTopProperty;
        //right
        HSSObservable * observedRight;
        HSSObservableProperty observedRightProperty;
        //bottom
        HSSObservable * observedBottom;
        HSSObservableProperty observedBottomProperty;
        //left
        HSSObservable * observedLeft;
        HSSObservableProperty observedLeftProperty;
        HSSUnit top, right, bottom, left;

        HSSUnit _evaluatePropertyValue(
                                   void(HSSMargin::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedProperty,
                                   HSSObservable * observedObject,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty,
                                   QSharedPointer<HSSSimpleSelection> scope
                                   );

    };
}

#endif

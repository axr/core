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

#ifndef HSSCOLORSTOP_H
#define HSSCOLORSTOP_H

#include <QSharedPointer>
#include "HSSObject.h"
#include "HSSRgb.h"

namespace AXR
{
    /**
     *  @brief This class represents HSS objects of type \@colorStop.
     */
    class AXR_API HSSColorStop : public HSSObject
    {
    public:
        typedef QSharedPointer<HSSColorStop> p;
        typedef std::vector<HSSColorStop::p>::iterator it;

        /**
         *  Creates a new instace of a color stop object.
         */
        HSSColorStop();
        /**
         *  Copy constructor for HSSColorStop objects
         */
        HSSColorStop(const HSSColorStop & orig);
        /**
         *  Clones an instance of HSSColorStop and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSColorStop
         */
        p clone() const;
        virtual ~HSSColorStop();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);

        /**
         *  Getter for the actual value of color
         *  @return A shared pointer to a HSSRgb object that is currently used.
         */
        HSSRgb::p getColor();

        /**
         *  Getter for the definition object of color.
         *  @return A shared pointer to the parser node containing the definition object of color.
         */
        HSSParserNode::p getDColor();

        /**
         *  Setter for the definition object of color. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of color.
         */
        void setDColor(HSSParserNode::p);

        /**
         *  Method to be passed as callback when observing changes that will affect color.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void colorChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of position
         *  @return A HSSUnit containing the actual used position.
         */
        HSSUnit getPosition();

        /**
         *  Getter for the definition object of position.
         *  @return A shared pointer to the parser node containing the definition object of position.
         */
        HSSParserNode::p getDPosition();

        /**
         *  Setter for the definition object of position. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of position.
         */
        void setDPosition(HSSParserNode::p);

        /**
         *  Method to be passed as callback when observing changes that will affect position.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void positionChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of balance
         *  @return A HSSUnit containing the actual used balance.
         */
        HSSUnit getBalance();

        /**
         *  Getter for the definition object of balance.
         *  @return A shared pointer to the parser node containing the definition object of balance.
         */
        HSSParserNode::p getDBalance();

        /**
         *  Setter for the definition object of balance. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of balance.
         */
        void setDBalance(HSSParserNode::p);

        /**
         *  Method to be passed as callback when observing changes that will affect balance.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void balanceChanged(HSSObservableProperty source, void*data);

    protected:
        //color
        HSSRgb::p color;
        HSSParserNode::p dColor;
        HSSObservable * observedColor;
        HSSObservableProperty observedColorProperty;

        //position
        HSSUnit position;
        HSSParserNode::p dPosition;
        HSSObservable * observedPosition;
        HSSObservableProperty observedPositionProperty;

        //balance
        HSSUnit balance;
        HSSParserNode::p dBalance;
        HSSObservable * observedBalance;
        HSSObservableProperty observedBalanceProperty;

        HSSUnit _evaluatePropertyValue(
                                   void(HSSColorStop::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty
                                   );
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif

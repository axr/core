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

#ifndef HSSREQUEST_H
#define HSSREQUEST_H

#include "HSSAction.h"

namespace AXR
{
    /**
     *  @brief The object type representing an action that loads content into the page. Sort of like
     *  ajax.
     *
     *  When this action fires, the content at the given URL is loaded and inserted into the document
     *  according to the request mode type (currently only implements a basic load like links).
     */
    class AXR_API HSSRequest : public HSSAction
    {
    public:
        /**
         *  Creates a new instance of a request action.
         */
        HSSRequest(AXRController * controller);

        /**
         *  Copy constructor for HSSRequest objects. Do not call directly,
         *  use clone() instead.
         */
        HSSRequest(const HSSRequest & orig);

        /**
         *  Clones an instance of HSSRequest and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRequest
         */
        QSharedPointer<HSSRequest> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSRequest();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);

        /**
         *  Currently only causes the document to load another XML file.
         *
         *  @todo Fix the implementation of loading of content into the document.
         */
        virtual void fire();

        /**
         *  Getter for the definition object of src.
         *  @return A shared pointer to the parser node containing the definition object of src.
         */
        QSharedPointer<HSSParserNode> getDSrc();

        /**
         *  Setter for the definition object of src. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of src.
         */
        void setDSrc(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect src.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void srcChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the definition object of target.
         *  @return A shared pointer to the parser node containing the definition object of target.
         */
        QSharedPointer<HSSParserNode> getDTarget();

        /**
         *  Setter for the definition object of target. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of target.
         */
        void setDTarget(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect target.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void targetChanged(HSSObservableProperty source, void*data);

        /**
         *  @todo Implement mode property.
         */
        //        //mode
        //        QSharedPointer<HSSParserNode> getDMode();
        //        void setDMode(QSharedPointer<HSSParserNode>);
        //        void modeChanged(HSSObservableProperty source, void*data);

        /**
         *  @todo Implement target property.
         */
        //        //contentTarget
        //        QSharedPointer<HSSParserNode> getDContentTarget();
        //        void setDContentTarget(QSharedPointer<HSSParserNode>);
        //        void contentTargetChanged(HSSObservableProperty source, void*data);


    private:
        QUrl src;
        QSharedPointer<HSSParserNode> dSrc;
        HSSObservable * observedSrc;
        HSSObservableProperty observedSrcProperty;

        QSharedPointer<HSSParserNode> dTarget;
        HSSObservable * observedTarget;
        HSSObservableProperty observedTargetProperty;
        QSharedPointer<HSSSimpleSelection> target;

        HSSRequestModeType mode;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

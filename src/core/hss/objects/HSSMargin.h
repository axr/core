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

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

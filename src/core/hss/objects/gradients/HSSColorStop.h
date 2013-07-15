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

#ifndef HSSCOLORSTOP_H
#define HSSCOLORSTOP_H

#include <QSharedPointer>
#include "HSSObject.h"

namespace AXR
{
    class HSSRgb;

    /**
     *  @brief This class represents HSS objects of type \@colorStop.
     */
    class AXR_API HSSColorStop : public HSSObject
    {
    public:
        typedef std::vector<QSharedPointer<HSSColorStop> >::iterator it;

        /**
         *  Creates a new instace of a color stop object.
         */
        HSSColorStop(AXRController * controller);
        /**
         *  Copy constructor for HSSColorStop objects
         */
        HSSColorStop(const HSSColorStop & orig);
        /**
         *  Clones an instance of HSSColorStop and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSColorStop
         */
        QSharedPointer<HSSColorStop> clone() const;
        virtual ~HSSColorStop();

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        //color
        const QSharedPointer<HSSObject> getColor() const;
        QSharedPointer<HSSObject> computeColor(QSharedPointer<HSSParserNode> parserNode);
        //position
        const HSSUnit getPosition() const;
        //balance
        const HSSUnit getBalance() const;

    private:
        void _initialize();
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

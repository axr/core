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

#ifndef HSSFIRSTFILTER_H
#define HSSFIRSTFILTER_H

#include "HSSFilter.h"

namespace AXR
{
    class HSSSimpleSelection;

    /**
     *  @brief Selects the first element of the selection.
     */
    class AXR_API HSSFirstFilter : public HSSFilter
    {
    public:
        /**
         *  @brief Creates a new instance of a \@first filter.
         */
        HSSFirstFilter(AXRController * controller);

        /**
         *  Clones an instance of HSSFirstFilter and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFirstFilter
         */
        QSharedPointer<HSSFilter> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFirstFilter();

        //see HSSParserNode.h for the documentation of this method
        virtual AXRString toString();

        //see HSSParserNode.h for the documentation of this method
        AXRString stringRep();

        virtual QSharedPointer<HSSSelection> apply(QSharedPointer<HSSSelection> scope, bool processing);

    private:
        inline void _apply(QSharedPointer<HSSSimpleSelection> & ret, QSharedPointer<HSSSimpleSelection> selection);
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

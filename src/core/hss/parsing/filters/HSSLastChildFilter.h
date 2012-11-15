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

#ifndef HSSLASTCHILDFILTER_H
#define HSSLASTCHILDFILTER_H

#include "HSSFilter.h"

namespace AXR
{
    /**
     *  @brief Selects the first element of the selection.
     */
    class AXR_API HSSLastChildFilter : public HSSFilter
    {
    public:
        /**
         *  @brief Creates a new instance of a \@first filter.
         */
        HSSLastChildFilter(AXRController * controller);

        /**
         *  Clones an instance of HSSFirstChildFilter and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFirstChildFilter
         */
        HSSFilter::p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSLastChildFilter();
        virtual AXRString toString();

        virtual HSSSelection::p apply(HSSSelection::p scope, bool processing);

    private:
        inline void _apply(HSSSimpleSelection::p & ret, HSSSimpleSelection::p selection);
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif

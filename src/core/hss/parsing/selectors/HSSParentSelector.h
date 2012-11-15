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

#ifndef HSSPARENTSELECTOR_H
#define HSSPARENTSELECTOR_H

#include "HSSDisplayObject.h"
#include "HSSNameSelector.h"

namespace AXR
{
    /**
     *  @brief The special object \@parent in HSS.
     *
     *  Used in selector chains, they always return the unique parent elements of the elements in scope.
     */
    class AXR_API HSSParentSelector : public HSSNameSelector
    {
    public:
        typedef QSharedPointer<HSSParentSelector> p;

        /**
         *  Creates a new instance of a Parent selector.
         */
        HSSParentSelector(AXRController * controller);

        /**
         *  Clones an instance of HSSParentSelector and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSParentSelector
         */
        p clone() const;

        //see HSSNameSelector.h for the documentation of this method
        AXRString getElementName();

        //see HSSParserNode.h for the documentation of this method
        virtual AXRString toString();

        QSharedPointer<HSSSelection> filterSelection(QSharedPointer<HSSSelection> scope, HSSDisplayObject::p thisObj, bool processing);

    private:
        virtual HSSClonable::p cloneImpl() const;
        inline void _filterSimpleSelection(HSSSimpleSelection::p & ret, HSSSimpleSelection::p selection);
    };
}

#endif

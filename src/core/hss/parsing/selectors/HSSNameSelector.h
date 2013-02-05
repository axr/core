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

#ifndef HSSNAMESELECTOR_H
#define HSSNAMESELECTOR_H

#include "HSSSelector.h"

namespace AXR
{
    class HSSSimpleSelector;

    /**
     *  @brief Selects elements by tag name.
     *
     *  This just holds information about the name of the elements that should
     *  be selected by AXRController when creating selections.
     */
    class AXR_API HSSNameSelector : public HSSSelector
    {
    public:
        /**
         *  Creates a new instance of a selector.
         */
        HSSNameSelector(AXRString elementName, AXRController * controller);

        /**
         *  Copy constructor for HSSNameSelector objects. Do not call directly, use clone() instead.
         */
        HSSNameSelector(const HSSNameSelector &orig);

        /**
         *  Clones an instance of HSSNameSelector and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSNameSelector.
         */
        QSharedPointer<HSSNameSelector> clone() const;

        /**
         *  Getter for the name of the element.
         *  @return A string containing the name of the element.
         */
        virtual AXRString getElementName();

        //see HSSObject.h for documentation of this method
        virtual AXRString toString();

        virtual QSharedPointer<HSSSelection> filterSelection(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing);

    protected:
        AXRString elementName;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        inline void _filterSimpleSelection(QSharedPointer<HSSSimpleSelection> & ret, QSharedPointer<HSSSimpleSelection> selection);
    };
}

#endif

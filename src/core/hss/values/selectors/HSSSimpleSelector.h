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

#ifndef HSSSIMPLESELECTOR_H
#define HSSSIMPLESELECTOR_H

#include "HSSFilter.h"
#include "HSSSelector.h"

namespace AXR
{
    class HSSNameSelector;

    /**
     *  @brief Simple selectors are basic units of a selector chain.
     */
    class AXR_API HSSSimpleSelector : public HSSSelector
    {
    public:
        /**
         *  Creates a new instance of a simple selector.
         */
        HSSSimpleSelector(AXRController * controller);

        /**
         *  Copy constructor for HSSSimpleSelector objects. Do not call directly, use clone() instead.
         */
        HSSSimpleSelector(const HSSSimpleSelector &orig);
        
        ~HSSSimpleSelector();

        /**
         *  Clones an instance of HSSSimpleSelector and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSSimpleSelector.
         */
        QSharedPointer<HSSSimpleSelector> clone() const;

        //see HSSParserNode.h for documentation of this method
        virtual AXRString toString();

        //see HSSParserNode.h for documentation of this method
        AXRString stringRep();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        void setName(QSharedPointer<HSSNameSelector> name);
        const QSharedPointer<HSSNameSelector> getName() const;

        const std::list<QSharedPointer<HSSFilter> > getFilters() const;
        void filtersAdd(QSharedPointer<HSSFilter> filter);

        /**
         * Reduces the selection according to the selector nodes it has stored.
         */
        QSharedPointer<HSSSelection> filterSelection(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications);

        //see HSSParserNode.h for documentation of this method
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> value);

    private:
        QSharedPointer<HSSNameSelector> name;
        std::list<QSharedPointer<HSSFilter> > filters;

        virtual QSharedPointer<HSSClonable> cloneImpl() const;

        QSharedPointer<HSSSelection> _recursiveFilterSelection(QSharedPointer<HSSSelection> selection, std::list<QSharedPointer<HSSFilter> >::const_iterator it, std::list<QSharedPointer<HSSFilter> >::const_iterator end_it, bool processing);
    };
}

#endif

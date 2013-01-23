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

#ifndef HSSMULTIPLESELECTION_H
#define HSSMULTIPLESELECTION_H

#include <vector>
#include <QSharedDataPointer>
#include "HSSSelection.h"

namespace AXR
{
    /**
     *  @brief
     */
    class HSSMultipleSelection : public HSSSelection
    {
    public:
        typedef std::vector<QSharedPointer<HSSSimpleSelection> >::iterator iterator;
        typedef std::vector<QSharedPointer<HSSSimpleSelection> >::const_iterator const_iterator;

        /**
         *
         */
        HSSMultipleSelection();

        /**
         *
         */
        HSSMultipleSelection(iterator a, iterator b);

        /**
         *
         */
        HSSMultipleSelection(const_iterator a, const_iterator b);

        /**
         *
         */
        HSSMultipleSelection(const HSSMultipleSelection &other);

        /**
         *
         */
        virtual ~HSSMultipleSelection();

        /**
         *
         */
        HSSMultipleSelection& operator=(const HSSMultipleSelection &other);

        void add(QSharedPointer<HSSSimpleSelection> item);
        void addSelection(QSharedPointer<HSSSelection> item);
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        size_t size() const;
        void clear();
        bool empty() const;
        QSharedPointer<HSSSimpleSelection> front() const;
        QSharedPointer<HSSSimpleSelection> back() const;
        void insert(iterator position, iterator first, iterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        std::vector< QSharedPointer<HSSSimpleSelection> > getItems() const;
        QSharedPointer<HSSSimpleSelection> joinAll();
        QSharedPointer<HSSMultipleSelection> splitAll();

        virtual AXRString toString();
    protected:

    private:
        class Data;
        QSharedDataPointer<Data> d;
    };
}

Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSMultipleSelection>)
Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSMultipleSelection>*)

#endif

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

#ifndef HSSSIMPLESELECTION_H
#define HSSSIMPLESELECTION_H

#include <vector>
#include <QSharedDataPointer>
#include "HSSSelection.h"

namespace AXR
{
    class HSSDisplayObject;
    class HSSMultipleSelection;

    /**
     *  @brief
     */
    class AXR_API HSSSimpleSelection : public HSSSelection
    {
    public:
        /**
         *  @return An empty selection pointer, to be used when no scope is needed but the
         *  function signature requires a pointer to be passed.
         */
        static QSharedPointer<HSSSimpleSelection> null();

        typedef std::vector< QSharedPointer<HSSDisplayObject> >::iterator iterator;
        typedef std::vector< QSharedPointer<HSSDisplayObject> >::const_iterator const_iterator;
        typedef std::vector< QSharedPointer<HSSDisplayObject> >::reverse_iterator reverse_iterator;
        typedef std::vector< QSharedPointer<HSSDisplayObject> >::const_reverse_iterator const_reverse_iterator;

        /**
         *
         */
        HSSSimpleSelection(AXRController * controller);

        /**
         *
         */
        HSSSimpleSelection(iterator a, iterator b, AXRController * controller);

        /**
         *
         */
        HSSSimpleSelection(const_iterator a, const_iterator b, AXRController * controller);

        /**
         *
         */
        HSSSimpleSelection(const HSSSimpleSelection &other);

        /**
         * Creates a simple selection from a vector of weak pointers to display objects
         * @param weakVector     A std::vector holding weak pointers to HSSDisplayObject elements
         */
        HSSSimpleSelection(const std::vector<QWeakPointer<HSSDisplayObject> > &weakVector, AXRController * controller);

        /**
         *
         */
        virtual ~HSSSimpleSelection();

        /**
         *
         */
        HSSSimpleSelection& operator=(const HSSSimpleSelection &other);

        void add( QSharedPointer<HSSDisplayObject> );
        void addSelection(QSharedPointer<HSSSelection> item);
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        size_t size() const;
        void clear();
        bool empty() const;
        QSharedPointer<HSSDisplayObject> front() const;
        QSharedPointer<HSSDisplayObject> back() const;
        void insert(iterator position, iterator first, iterator last);
        iterator erase(iterator position);
        iterator erase (iterator first, iterator last);
        std::vector< QSharedPointer<HSSDisplayObject> > getItems() const;
        QSharedPointer<HSSSimpleSelection> joinAll() const;
        QSharedPointer<HSSMultipleSelection> splitAll();

        virtual AXRString toString();

        /**
         *  Each object overrides this method to compare against another node
         *  @param otherObj     The other object to compare to
         *  @return Wether the object is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSSelection> otherObj);

    protected:

    private:
        class Data;
        QSharedDataPointer<Data> d;
    };
}

Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSSimpleSelection>)
Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSSimpleSelection>*)

#endif

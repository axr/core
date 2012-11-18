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

#include "HSSSelection.h"
#include "HSSSimpleSelection.h"

namespace AXR
{
    /**
     *  @brief 
     */
    class HSSMultipleSelection : public HSSSelection
    {
    public:
        /**
         *  The shared pointer to this kind of object.
         */
        typedef QSharedPointer<HSSMultipleSelection> p;

        typedef std::vector<HSSSimpleSelection::p>::iterator iterator;
        typedef std::vector<HSSSimpleSelection::p>::const_iterator const_iterator;

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

        void add(HSSSimpleSelection::p item);
        void addSelection(HSSSelection::p item);
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        unsigned size() const;
        void clear();
        bool empty() const;
        HSSSimpleSelection::p front() const;
        HSSSimpleSelection::p back() const;
        void insert(iterator position, iterator first, iterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        std::vector< HSSSimpleSelection::p > getItems() const;
        QSharedPointer<HSSSimpleSelection> joinAll();
        QSharedPointer<HSSMultipleSelection> splitAll();
    protected:

    private:
        class Data;
        QSharedDataPointer<Data> d;
    };
}

Q_DECLARE_METATYPE(AXR::HSSMultipleSelection::p)
Q_DECLARE_METATYPE(AXR::HSSMultipleSelection::p*)

#endif

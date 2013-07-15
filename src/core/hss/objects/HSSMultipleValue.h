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

#ifndef HSSMULTIPLEVALUE_H
#define HSSMULTIPLEVALUE_H

#include <vector>
#include "HSSObject.h"

namespace AXR
{
    /**
     *  @brief This is a proxy class to hold multiple objects
     */
    class AXR_API HSSMultipleValue : public HSSObject
    {
    public:
        /**
         *  Constructor for MultipleValue objects
         */
        HSSMultipleValue(AXRController * controller);
        /**
         *  Copy constructor for HSSMultipleValue objects
         */
        HSSMultipleValue(const HSSMultipleValue & orig);
        /**
         *  Clones an instance of HSSMultipleValue and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSMultipleValue
         */
        QSharedPointer<HSSMultipleValue> clone() const;
        /**
         *  Destructor for MultipleValue objects
         */
        virtual ~HSSMultipleValue();
        virtual AXRString toString();
        /**
         *  Adds a parser node defining a value to the storage.
         *  @param newValue     A shared pointer to the parser node that defines the value
         *                      (definition object)
         */
        void add(QSharedPointer<HSSObject> newValue);
        /**
         *  Removes the last definition object from the storage
         */
        void removeLast();
        /**
         *  @return The first definition object in the storage.
         */
        QSharedPointer<HSSObject> first();
        /**
         *  @return The last definition object in the storage.
         */
        QSharedPointer<HSSObject> last();
        /**
         *  @return The amount of definition objects in the storage.
         */
        size_t size() const;
        /**
         *  @return The entire storage as a STL vector containing shared pointers to definition
         *  objects. Read only.
         */
        const QList<QSharedPointer<HSSObject> > getValues() const;

    protected:
        QList<QSharedPointer<HSSObject> >valueList;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

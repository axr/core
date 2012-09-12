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

#ifndef HSSMULTIPLEVALUE_H
#define HSSMULTIPLEVALUE_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "HSSObject.h"
#include "HSSParserNode.h"

namespace AXR
{

    /**
     *  @brief MultipleValue objects contain other values, and stores them
     *  together under the same object name.
     */
    class HSSMultipleValue : public HSSObject
    {
    public:

        typedef boost::shared_ptr<HSSMultipleValue> p;
        /**
         *  Constructor for MultipleValue objects
         */
        HSSMultipleValue();
        /**
         *  Copy constructor for HSSMultipleValue objects
         */
        HSSMultipleValue(const HSSMultipleValue & orig);
        /**
         *  Clones an instance of HSSMultipleValue and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSMultipleValue
         */
        p clone() const;
        /**
         *  Destructor for MultipleValue objects
         */
        virtual ~HSSMultipleValue();
        virtual std::string toString();
        /**
         *  Adds a parser node defining a value to the storage.
         *  @param newValue     A shared pointer to the parser node that defines the value
         *                      (definition object)
         */
        void add(HSSParserNode::p newValue);
        /**
         *  Removes the last definition object from the storage
         */
        void removeLast();
        /**
         *  @return The first definition object in the storage.
         */
        HSSParserNode::p first();
        /**
         *  @return The last definition object in the storage.
         */
        HSSParserNode::p last();
        /**
         *  @return The amount of definition objects in the storage.
         */
        int size();
        /**
         *  @return The entire storage as a STL vector containing shared pointers to definition
         *  objects. Read only.
         */
        const std::vector<HSSParserNode::p> getValueList() const;

    protected:
        std::vector<HSSParserNode::p>valueList;

    private:
        HSSClonable::p cloneImpl() const;
    };
}


#endif

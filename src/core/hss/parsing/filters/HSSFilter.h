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

#ifndef HSSFILTER_H
#define HSSFILTER_H

#include <QSharedPointer>
#include "HSSDisplayObject.h"
#include "HSSParserNode.h"

namespace AXR
{
    class HSSSelectorChain;

    /**
     *  @brief Abstract base class for parser node types representing filters.
     *
     *  Filters are used in the selector chain to reduce the size of the selection.
     */
    class AXR_API HSSFilter : public HSSParserNode
    {
    public:
        typedef QSharedPointer<HSSFilter> p;

        /**
         *  When logging, you often need a string representation of the filter type.
         *  @param filterType   The action type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString filterTypeStringRepresentation(HSSFilterType filterType);

        /**
         *  Gives a HSSFilterType representation from a string representation.
         *  @param name     The name of the filter (e.g. "first").
         *  @return The filter type that corresponds to that name.
         *
         *  @todo what if not found? there should be an error state in the enum
         */
        static HSSFilterType filterTypeFromString(AXRString name);

        /**
         *  Instantiates a specific subclass for the given textual representation
         *  of a string (e.g. "first").
         *  @param stringType   The textual representation of the filter.
         *  @return A shared pointer to a new instance of a filter of the given type.
         */
        static HSSFilter::p newFilterWithStringType(AXRString stringType, AXRController * controller);

        /**
         *  Instantiates a specific subclass for the given HSSFilterType.
         *  @param filterType   The filter type for the new instance.
         *  @return A shared pointer to a new instance of a filter of the given type.
         */
        static HSSFilter::p newFilterWithType(HSSFilterType filterType, AXRController * controller);

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFilter();

        virtual AXRString toString();

        /**
         *  Each subclass implements its own way of processing the selection.
         *  This base implementation is (I think) what gets called when you use
         *  an unknown filter name in the source code, which currently returns the entire scope.
         *
         *  @param scope        The original selection: A vector of shared pointers to display objects.
         *  @param processing   Wether we are processing the filter or not.
         *  @return A vector of shared pointers to the display objects in the resulting selection.
         *
         *  @todo should this return an empty selection?
         */
        virtual const std::vector<HSSDisplayObject::p> apply(const std::vector<HSSDisplayObject::p> &scope, bool processing) = 0;

        virtual HSSFilter::p clone() const = 0;

        /**
         *  Allows you to check if this filter is of the given type.
         *  @param  type    The filter type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSFilterType type);

        /**
         *  @return The filter type of this instance.
         */
        HSSFilterType getFilterType();

        bool getNegating() const;
        void setNegating(bool value);

    protected:
        /**
         *  Creates a new instance of a filter. Do not use directly.
         *  @param type     The type of the filter, to uniquely identify each subclass.
         */
        HSSFilter(HSSFilterType type, AXRController * controller);

    private:
        HSSFilterType filterType;
        bool _negating;
    };
}

#endif

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

#ifndef HSSSELECTION_H
#define HSSSELECTION_H

#include <QSharedPointer>
#include "HSSTypeEnums.h"

namespace AXR
{
    class HSSSimpleSelection;
    class HSSMultipleSelection;
    class HSSSelectorChain;

    /**
     *  @brief
     */
    class HSSSelection
    {
    public:
        virtual ~HSSSelection();

        /**
         *  When logging, you often need a string representation of the selection type.
         *  @param selectionType   The selection type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString selectionTypeStringRepresentation(HSSSelectionType selectionType);

        /**
         *  When logging, you often need a string representation of the result of a selection
         *  @param selection        A pointer to the selection that should be printed
         *  @param selectorChains   The selector chains that were used to create the selection
         *  @return A string representation of the given type.
         */
        static AXRString logSelection(const HSSSelection * selection, const std::vector<QSharedPointer<HSSSelectorChain> > & selectorChains);

        /**
         *  Allows you to check if this selection is of the given type.
         *  @param  type    The selection type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSSelectionType type) const;

        /**
         *  @return The selection type of this instance.
         */
        HSSSelectionType getSelectionType() const;

        virtual void addSelection(QSharedPointer<HSSSelection> item) =0;
        virtual QSharedPointer<HSSSimpleSelection> joinAll() const =0;
        virtual QSharedPointer<HSSMultipleSelection> splitAll() =0;

        /**
         *  Prints itself as a textual representation. Each subclass should override this method.
         *  @return A textual representation of the object.
         */
        virtual AXRString toString();

        /**
         *  Prints itself as a std string representation, calling this->toString().
         *  @return A textual representation of the object, as a std string.
         */
        std::string toStdString();

        /**
         *  Each object overrides this method to compare against another node
         *  @param otherObj     The other object to compare to
         *  @return Wether the object is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSSelection> otherObj);

    protected:
        HSSSelection(HSSSelectionType type);
        HSSSelection(const HSSSelection &other);

    private:
        HSSSelectionType selectionType;
    };
}

Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSSelection>)
Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSSelection>*)

#endif

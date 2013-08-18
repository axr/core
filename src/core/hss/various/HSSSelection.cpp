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

#include <QMap>
#include <QStringList>
#include "HSSDisplayObject.h"
#include "HSSMultipleSelection.h"
#include "HSSSelection.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

AXRString HSSSelection::selectionTypeStringRepresentation(HSSSelectionType selectionType)
{
    static QMap<HSSSelectionType, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSSelectionTypeSimpleSelection] = "HSSSelectionTypeSimpleSelection";
        types[HSSSelectionTypeMultipleSelection] = "HSSSelectionTypeMultipleSelection";
    }

    return types[selectionType];
}

HSSSelection::HSSSelection(HSSSelectionType type)
{
    this->selectionType = type;
}

HSSSelection::HSSSelection(const HSSSelection &other)
{
    this->selectionType = other.selectionType;
}

HSSSelection::~HSSSelection()
{

}

const bool HSSSelection::isA(HSSSelectionType type) const
{
    return type == this->selectionType;
}

const HSSSelectionType HSSSelection::getSelectionType() const
{
    return this->selectionType;
}

AXRString HSSSelection::toString()
{
    return "Generic selection - you forgot to override toString() in your subclass";
}

std::string HSSSelection::toStdString()
{
    AXRString tempstr = this->toString();
    return tempstr.toStdString();
}

bool HSSSelection::equalTo(QSharedPointer<HSSSelection> otherObj)
{
    //check wether pointers are the same
    if (this == otherObj.data()) return true;
    //check wether of same type
    if (otherObj->selectionType != this->selectionType) return false;
    return true;
}

AXRString HSSSelection::logSelection(const HSSSelection * selection, const std::vector<QSharedPointer<HSSSelectorChain> > & selectorChains)
{
    QStringList selections;
    QSharedPointer<HSSSimpleSelection> flatSelection = selection->joinAll();
    for (HSSSimpleSelection::iterator it = flatSelection->begin(); it != flatSelection->end(); ++it) {
        selections.append(AXRString("    %1").arg((*it)->getName()));
    }

    QStringList selectors;
    for (std::vector<QSharedPointer<HSSSelectorChain> >::const_iterator it2 = selectorChains.begin(); it2 != selectorChains.end(); ++it2) {
        selectors.append((*it2)->stringRep());
    }

    return AXRString("%1 { } selected %2 elements:\n%3\n")
            .arg(selectors.join(", "))
            .arg(AXRString::number(flatSelection->size()))
            .arg(selections.join("\n"));
}

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

#include "axr.h"
#include "HSSSelection.h"

using namespace AXR;

AXRString HSSSelection::selectionTypeStringRepresentation(HSSSelectionType selectionType)
{
    static std::map<HSSSelectionType, AXRString> types;
    if (types.empty())
    {
        types[HSSSelectionTypeSimpleSelection] = "HSSSelectionTypeSimpleSelection";
        types[HSSSelectionTypeMultipleSelection] = "HSSSelectionTypeMultipleSelection";
    }

    return types[selectionType];
}

HSSSelection::HSSSelection(HSSSelectionType type, AXRController * controller)
: HSSObject(HSSObjectTypeSelection, controller)
{
    this->selectionType = type;
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("values");
    this->setShorthandProperties(shorthandProperties);
}

HSSSelection::HSSSelection(const HSSSelection &orig)
: HSSObject(orig)
{
    this->selectionType = orig.selectionType;
}

HSSSelection::~HSSSelection()
{

}

bool HSSSelection::isA(HSSSelectionType type) const
{
    return type == this->selectionType;
}

HSSSelectionType HSSSelection::getSelectionType() const
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
    return tempstr.data();
}

AXRString HSSSelection::defaultObjectType()
{
    return "selection";
}

AXRString HSSSelection::defaultObjectType(AXRString property)
{
    if (property == "values")
    {
        return "value";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSSelection::equalTo(QSharedPointer<HSSObject> otherObj)
{
    //check wether pointers are the same
    if (this == otherObj.data()) return true;
    //check wether of same type
    if (otherObj->getSelectionType() != this->selectionType) return false;
    return true;
}

AXRString HSSSelection::logSelection(const HSSSelection * selection, const std::vector<QSharedPointer<HSSSelectorChain> > & selectorChains)
{
    std::vector<HSSString> selections;
    QSharedPointer<HSSSimpleSelection> flatSelection = selection->joinAll();
    for (HSSSimpleSelection::iterator it = flatSelection->begin(); it != flatSelection->end(); ++it) {
        selections.push_back(HSSString::format("    %s", (*it)->getName().chardata()));
    }

    std::vector<HSSString> selectors;
    for (std::vector<QSharedPointer<HSSSelectorChain> >::const_iterator it2 = selectorChains.begin(); it2 != selectorChains.end(); ++it2) {
        selectors.push_back((*it2)->stringRep());
    }
    
    return HSSString::format("%s { } selected %d elements:\n%s\n", HSSString::join(selectors, ", ").chardata(), flatSelection->size(), HSSString::join(selections, "\n").chardata());
}

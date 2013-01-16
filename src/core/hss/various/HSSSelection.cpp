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

#include "HSSMultipleSelection.h"
#include "HSSSelection.h"
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

bool HSSSelection::isA(HSSSelectionType type)
{
    return type == this->selectionType;
}

HSSSelectionType HSSSelection::getSelectionType()
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

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

#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFont.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSParserNode.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSStringConstant.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

const HSSUnit HSSFont::DEFAULT_SIZE = 16;

HSSFont::HSSFont(AXRController * controller)
: HSSObject(HSSObjectTypeFont, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFont: creating font object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("face");
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("weight");
    this->setShorthandProperties(shorthandProperties);
}

HSSFont::HSSFont(const HSSFont & orig)
: HSSObject(orig)
{
    this->_initialize();
}

void HSSFont::_initialize()
{
}

QSharedPointer<HSSFont> HSSFont::clone() const
{
    return qSharedPointerCast<HSSFont> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSFont::cloneImpl() const
{
    return QSharedPointer<HSSFont>(new HSSFont(*this));
}

HSSFont::~HSSFont()
{

}

AXRString HSSFont::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSFont: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSFont";
    }
}

AXRString HSSFont::defaultObjectType()
{
    return "font";
}

AXRString HSSFont::defaultObjectType(AXRString property)
{
    if (property == "color")
    {
        return "rgb";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSFont::isKeyword(AXRString value, AXRString property)
{
    if (property == "weight")
    {
        if (
                value == "normal"
                || value == "bold"
                || value == "medium"
                || value == "thin"
                || value == "light"
                || value == "book"
                || value == "heavy"
                || value == "ultralight"
                || value == "semibold"
                || value == "ultrabold"
                || value == "ultraheavy"
                )
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}


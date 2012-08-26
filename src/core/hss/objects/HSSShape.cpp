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

#include "HSSShape.h"

using namespace AXR;

HSSShape::HSSShape(HSSShapeType type)
: HSSObject(HSSObjectTypeShape)
{
    this->shapeType = type;
}

HSSShape::HSSShape(const HSSShape & orig)
: HSSObject(orig)
{
    this->shapeType = orig.shapeType;
}

HSSShape::~HSSShape()
{

}

bool HSSShape::isA(HSSShapeType type)
{
    return this->shapeType == type;
}

HSSShapeType HSSShape::getShapeType()
{
    return this->shapeType;
}

std::string HSSShape::toString()
{
    return "HSSShape";
}

std::string HSSShape::defaultObjectType()
{
    return "HSSShape";
}

std::string HSSShape::defaultObjectType(std::string property)
{
    return HSSObject::defaultObjectType(property);
}

bool HSSShape::isKeyword(std::string value, std::string property)
{
    return HSSObject::isKeyword(value, property);
}
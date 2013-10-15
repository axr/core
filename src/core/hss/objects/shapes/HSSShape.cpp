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

#include "HSSDisplayObject.h"
#include "HSSParserNode.h"
#include "HSSShape.h"

using namespace AXR;

HSSShape::HSSShape(HSSShapeType type, AXRController * controller)
: HSSObject(HSSObjectTypeShape, controller)
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

bool HSSShape::isA(HSSShapeType type) const
{
    return this->shapeType == type;
}

HSSShapeType HSSShape::getShapeType() const
{
    return this->shapeType;
}

AXRString HSSShape::toString()
{
    return "HSSShape";
}

bool HSSShape::equalTo(QSharedPointer<HSSObject> otherObj)
{
    //check wether pointers are the same
    if (this == otherObj.data()) return true;
    //other checks
    if ( ! HSSObject::equalTo(otherObj)) return false;
    QSharedPointer<HSSShape> castedObj = qSharedPointerCast<HSSShape>(otherObj);
    if ( this->shapeType != castedObj->shapeType ) return false;
    return true;
}

AXRString HSSShape::defaultObjectType()
{
    return "HSSShape";
}

AXRString HSSShape::defaultObjectType(AXRString property)
{
    return HSSObject::defaultObjectType(property);
}

bool HSSShape::isKeyword(AXRString value, AXRString property)
{
    return HSSObject::isKeyword(value, property);
}

void HSSShape::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height)
{
    QList<QSharedPointer<HSSParserNode> > segments;
    this->createPath(path, x, y, width, height, segments);
}

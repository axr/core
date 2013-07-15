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
#include "HSSMultipleValueDefinition.h"
#include "HSSPropertyDefinition.h"

using namespace AXR;

HSSPropertyDefinition::HSSPropertyDefinition(AXRController * controller)
: HSSStatement(HSSStatementTypePropertyDefinition, controller)
{

}

HSSPropertyDefinition::HSSPropertyDefinition(QVector<QVector<AXRString> > paths, AXRController * controller)
: HSSStatement(HSSStatementTypePropertyDefinition, controller)
{
    this->paths = paths;
}

HSSPropertyDefinition::HSSPropertyDefinition(QVector<QVector<AXRString> > name, QSharedPointer<HSSParserNode> value, AXRController * controller)
: HSSStatement(HSSStatementTypePropertyDefinition, controller)
{
    this->paths = paths;
    this->setValue(value);
}

HSSPropertyDefinition::HSSPropertyDefinition(const HSSPropertyDefinition &orig)
: HSSStatement(orig)
{
    this->paths = orig.paths;
}

QSharedPointer<HSSPropertyDefinition> HSSPropertyDefinition::clone() const
{
    return qSharedPointerCast<HSSPropertyDefinition> (this->cloneImpl());
}

HSSPropertyDefinition::~HSSPropertyDefinition()
{

}

AXRString HSSPropertyDefinition::toString()
{
    AXRString ret = "HSSPropertyDefinition: ";
    Q_FOREACH(QVector<AXRString> path, this->paths)
    {
        ret.append(path.front());
    }
    if (this->value)
    {
        ret.append(" with value: " + this->value->toString());
    }
    return ret;
}

void HSSPropertyDefinition::setPaths(QVector<QVector<AXRString> > paths)
{
    this->paths = paths;
}

QVector<QVector<AXRString> > HSSPropertyDefinition::getPaths()
{
    return this->paths;
}

void HSSPropertyDefinition::setValue(QSharedPointer<HSSParserNode> value)
{
    this->value = value;
    this->value->setParentNode(this->shared_from_this());
}

void HSSPropertyDefinition::addValue(QSharedPointer<HSSParserNode> value)
{
    if (this->value)
    {
        if (this->value->isA(HSSParserNodeTypeMultipleValueDefinition))
        {
            QSharedPointer<HSSMultipleValueDefinition> mvDef = qSharedPointerCast<HSSMultipleValueDefinition > (this->value);
            value->setParentNode(this->shared_from_this());
            mvDef->add(value);
        }
        else
        {
            QSharedPointer<HSSMultipleValueDefinition> mvDef = QSharedPointer<HSSMultipleValueDefinition>(new HSSMultipleValueDefinition(this->getController()));
            mvDef->setParentNode(this->shared_from_this());
            mvDef->add(this->value);
            value->setParentNode(this->shared_from_this());
            mvDef->add(value);
            this->value = mvDef;
        }
    }
    else
    {
        this->value = value;
    }
}

QSharedPointer<HSSParserNode> HSSPropertyDefinition::getValue()
{
    return this->value;
}

void HSSPropertyDefinition::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->value->setThisObj(value);
    HSSStatement::setThisObj(value);
}

QSharedPointer<HSSPropertyDefinition> HSSPropertyDefinition::shared_from_this()
{
    return qSharedPointerCast<HSSPropertyDefinition > (HSSStatement::shared_from_this());
}

QSharedPointer<HSSClonable> HSSPropertyDefinition::cloneImpl() const
{
    QSharedPointer<HSSPropertyDefinition> clone = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(*this));
    clone->setValue(this->value->clone());
    return clone;
}

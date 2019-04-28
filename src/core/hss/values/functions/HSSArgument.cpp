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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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
#include "HSSArgument.h"

using namespace AXR;

HSSArgument::HSSArgument(AXRController * controller)
: HSSParserNode(HSSParserNodeTypeArgument, controller)
{
    
}

HSSArgument::HSSArgument(const HSSArgument & orig)
: HSSParserNode(orig)
{
    this->_name = orig._name;
}

QSharedPointer<HSSArgument> HSSArgument::clone() const
{
    return qSharedPointerCast<HSSArgument> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSArgument::cloneImpl() const
{
    QSharedPointer<HSSArgument> clone = QSharedPointer<HSSArgument>(new HSSArgument(*this));
    if (this->_node)
    {
        clone->addValue(this->_node->clone());
    }
    return clone;
}

HSSArgument::~HSSArgument()
{
}

AXRString HSSArgument::toString()
{
    AXRString tempstr = AXRString("HSSArgument\n");
    return tempstr;
}

bool HSSArgument::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    return true;
}

void HSSArgument::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    this->_node = parserNode;
}

const QSharedPointer<HSSParserNode> & HSSArgument::node() const
{
    return this->_node;
}

void HSSArgument::setName(QSharedPointer<HSSObjectNameConstant> newName)
{
    this->_name = newName;
}

const QSharedPointer<HSSObjectNameConstant> HSSArgument::name() const
{
    return this->_name;
}

QSharedPointer<HSSParserNode> HSSArgument::evaluate()
{
    return this->_evaluate(this->_node);
}

QSharedPointer<HSSParserNode> HSSArgument::_evaluate(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(parserNode);
            if (!theFunc->isA(HSSFunctionTypeFunction))
            {
                QSharedPointer<HSSObject> remoteObj = theFunc->evaluate();
                if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                    return this->_evaluate(valueNode);
                }
            }
            break;
        }
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theString = qSharedPointerCast<HSSStringConstant>(parserNode);
            if (theString->hasArguments())
            {
                HSSString newStr = theString->evaluate();
                QSharedPointer<HSSStringConstant> ret = HSSStringConstant::stringToConstant(newStr, this->getController());
                return ret;
            }
            break;
        }
        case HSSParserNodeTypeExpression:
        {
            QSharedPointer<HSSExpression> theExp = qSharedPointerCast<HSSExpression>(parserNode);
            QSharedPointer<HSSNumberConstant> ret = HSSNumberConstant::number(theExp->evaluate(), this->getController());
            return ret;
        }
        default:
            break;
    }
    return parserNode->clone();
}


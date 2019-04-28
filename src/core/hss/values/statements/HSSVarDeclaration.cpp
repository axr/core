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
#include "HSSVarDeclaration.h"

using namespace AXR;

HSSVarDeclaration::HSSVarDeclaration(HSSString name, AXRController * controller)
: HSSStatement(HSSStatementTypeVarDeclaration, controller)
{
    this->_name = name;
}

HSSVarDeclaration::HSSVarDeclaration(const HSSVarDeclaration & orig)
: HSSStatement(orig)
{
    this->_name = orig._name;
}

QSharedPointer<HSSVarDeclaration> HSSVarDeclaration::clone() const
{
    return qSharedPointerCast<HSSVarDeclaration> (this->cloneImpl());
}

HSSVarDeclaration::~HSSVarDeclaration()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSVarDeclaration: destructing instance");
    this->cleanTrackedObservers();
}

HSSString HSSVarDeclaration::toString()
{
    return "HSSVarDeclaration";
}

bool HSSVarDeclaration::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSStatement::equalTo(otherNode)) return false;
    return true;
}

void HSSVarDeclaration::setName(HSSString newName)
{
    this->_name = newName;
}

const HSSString HSSVarDeclaration::name() const
{
    return this->_name;
}

QSharedPointer<HSSClonable> HSSVarDeclaration::cloneImpl() const
{
    QSharedPointer<HSSVarDeclaration> clone(new HSSVarDeclaration(*this));
    if (this->_assignment)
    {
        clone->setAssignment(this->_assignment->clone());
    }
    return clone;
}

void HSSVarDeclaration::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    if (this->_assignment) {
//        this->_assignment->setScope(newScope);
    }
}

void HSSVarDeclaration::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    if (this->_assignment) {
        this->_assignment->setThisObj(value);
    }
}

QSharedPointer<HSSDisplayObject> HSSVarDeclaration::getThisObj()
{
    return thisObj;
}

void HSSVarDeclaration::setAssignment(QSharedPointer<HSSAssignment> value)
{
    value->setParentNode(this->HSSParserNode::shared_from_this());
    this->_assignment = value;
}

QSharedPointer<HSSAssignment> HSSVarDeclaration::assignment() const
{
    return this->_assignment;
}

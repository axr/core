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

#include "AXRDebugging.h"
#include "HSSDisplayObject.h"
#include "HSSFilter.h"
#include "HSSNameSelector.h"
#include "HSSRule.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelector.h"

using namespace AXR;

HSSSelectorChain::HSSSelectorChain(AXRController * controller)
: HSSParserNode(HSSParserNodeTypeSelectorChain, controller)
{

}

HSSSelectorChain::HSSSelectorChain(const HSSSelectorChain &orig)
: HSSParserNode(orig)
{

}

QSharedPointer<HSSSelectorChain> HSSSelectorChain::clone() const
{
    return qSharedPointerCast<HSSSelectorChain> (this->cloneImpl());
}

HSSSelectorChain::~HSSSelectorChain()
{
    this->nodeList.clear();
}

AXRString HSSSelectorChain::toString()
{
    AXRString tempstr = "HSSSelectorChain with the following nodes:\n";
    size_t sccount = this->nodeList.size();
    if (sccount > 0)
    {
        for (size_t i = 0; i < sccount; ++i)
        {
            tempstr.append("      ").append(this->nodeList[i]->toString()).append("\n");
        }
    }

    return tempstr;
}

const QSharedPointer<HSSParserNode> & HSSSelectorChain::get(size_t i) const
{
    return this->nodeList[i];
}

void HSSSelectorChain::add(QSharedPointer<HSSParserNode> newNode)
{
    if (newNode)
    {
        std_log3("HSSSelectorChain: Added node of type " + newNode->toString());
        newNode->setParentNode(this->shared_from_this());
        this->nodeList.push_back(newNode);
    }
}

void HSSSelectorChain::prepend(QSharedPointer<HSSParserNode> newNode)
{
    if (newNode)
    {
        std_log3("HSSSelectorChain: Added node of type " + newNode->toString() + " to the front of the selector");
        newNode->setParentNode(this->shared_from_this());
        this->nodeList.push_front(newNode);
    }
}

void HSSSelectorChain::removeLast()
{
    this->nodeList.pop_back();
}

QSharedPointer<HSSParserNode> HSSSelectorChain::last()
{
    return this->nodeList.back();
}

size_t HSSSelectorChain::size() const
{
    return this->nodeList.size();
}

QSharedPointer<HSSSimpleSelector> HSSSelectorChain::subject()
{
    QSharedPointer<HSSSimpleSelector> ret;
    /**
     *  @todo subject selectors need to be implemented
     */
    if (this->nodeList.size() > 0)
    {
        if (this->nodeList.back()->isA(HSSParserNodeTypeSelector))
        {
            QSharedPointer<HSSSelector> selector = qSharedPointerCast<HSSSelector > (this->nodeList.back());
            if (selector->isA(HSSSelectorTypeSimpleSelector))
            {
                ret = qSharedPointerCast<HSSSimpleSelector > (selector);
            }
            else
            {
                std_log1("########### subject in selector chain could not be determined");
            }

        }
        else
        {
            std_log1("########### subject in selector chain could not be determined");
        }
    }
    return ret;
}

void HSSSelectorChain::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    for (std::deque<QSharedPointer<HSSParserNode> >::iterator it = this->nodeList.begin(); it != this->nodeList.end(); ++it)
    {
        (*it)->setThisObj(value);
    }

    HSSParserNode::setThisObj(value);
}

QSharedPointer<HSSSelectorChain> HSSSelectorChain::shared_from_this()
{
    return qSharedPointerCast<HSSSelectorChain > (HSSParserNode::shared_from_this());
}

QSharedPointer<HSSClonable> HSSSelectorChain::cloneImpl() const
{
    QSharedPointer<HSSSelectorChain> clone = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(*this));

    for (std::deque<QSharedPointer<HSSParserNode> >::const_iterator it = this->nodeList.begin(); it != this->nodeList.end(); ++it)
    {
        QSharedPointer<HSSParserNode> clonedNode = (*it)->clone();
        clone->add(clonedNode);
    }

    return clone;
}

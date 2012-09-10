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

#include <iostream>
#include <boost/pointer_cast.hpp>
#include "AXRDebugging.h"
#include "HSSRule.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelector.h"

using namespace AXR;

HSSSelectorChain::HSSSelectorChain()
: HSSParserNode(HSSParserNodeTypeSelectorChain)
{

}

HSSSelectorChain::HSSSelectorChain(const HSSSelectorChain &orig)
: HSSParserNode(orig)
{

}

HSSSelectorChain::p HSSSelectorChain::clone() const
{
    return boost::static_pointer_cast<HSSSelectorChain, HSSClonable > (this->cloneImpl());
}

HSSSelectorChain::~HSSSelectorChain()
{
    this->nodeList.clear();
}

std::string HSSSelectorChain::toString()
{
    std::string tempstr = "HSSSelectorChain with the following nodes:\n";
    unsigned int sccount = this->nodeList.size();
    if (sccount > 0)
    {
        for (unsigned i = 0; i < sccount; ++i)
        {
            tempstr.append("      ").append(this->nodeList[i]->toString()).append("\n");
        }
    }

    return tempstr;
}

const HSSParserNode::p & HSSSelectorChain::get(const int i) const
{
    return this->nodeList[i];
}

void HSSSelectorChain::add(HSSParserNode::p newNode)
{
    if (newNode)
    {
        std_log3("HSSSelectorChain: Added node of type " + newNode->toString());
        newNode->setParentNode(this->shared_from_this());
        this->nodeList.push_back(newNode);
    }
}

void HSSSelectorChain::prepend(HSSParserNode::p newNode)
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

HSSParserNode::p HSSSelectorChain::last()
{
    return this->nodeList.back();
}

const int HSSSelectorChain::size()
{
    return this->nodeList.size();
}

HSSSimpleSelector::p HSSSelectorChain::subject()
{
    HSSSimpleSelector::p ret;
    /**
     *  @todo subject selectors need to be implemented
     */
    if (this->nodeList.size() > 0)
    {
        if (this->nodeList.back()->isA(HSSParserNodeTypeSelector))
        {
            HSSSelector::p selector = boost::static_pointer_cast<HSSSelector > (this->nodeList.back());
            if (selector->isA(HSSSelectorTypeSimpleSelector))
            {
                ret = boost::static_pointer_cast<HSSSimpleSelector > (selector);
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

void HSSSelectorChain::setThisObj(boost::shared_ptr<HSSDisplayObject> value)
{
    for (std::deque<HSSParserNode::p>::iterator it = this->nodeList.begin(); it != this->nodeList.end(); ++it)
    {
        (*it)->setThisObj(value);
    }

    HSSParserNode::setThisObj(value);
}

HSSSelectorChain::p HSSSelectorChain::shared_from_this()
{
    return boost::static_pointer_cast<HSSSelectorChain > (HSSParserNode::shared_from_this());
}

HSSClonable::p HSSSelectorChain::cloneImpl() const
{
    HSSSelectorChain::p clone = HSSSelectorChain::p(new HSSSelectorChain(*this));

    for (std::deque<HSSParserNode::p>::const_iterator it = this->nodeList.begin(); it != this->nodeList.end(); ++it)
    {
        HSSParserNode::p clonedNode = (*it)->clone();
        clone->add(clonedNode);
    }

    return clone;
}

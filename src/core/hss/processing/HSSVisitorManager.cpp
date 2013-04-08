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
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSVisitorManager.h"

using namespace AXR;

class HSSVisitorManager::Private
{
public:
    Private() : controller(), visitors()
    {
    }

    AXRController *controller;
    std::vector<HSSAbstractVisitor*> visitors;
};

HSSVisitorManager::HSSVisitorManager(AXRController *theController)
: d(new Private)
{
    d->controller = theController;
}

HSSVisitorManager::~HSSVisitorManager()
{
    delete d;
}

void HSSVisitorManager::addVisitor(HSSAbstractVisitor *visitor)
{
    d->visitors.push_back(visitor);
}

void HSSVisitorManager::runVisitors()
{
    QSharedPointer<HSSDisplayObject> root = d->controller->root();
    runVisitors(root, HSSAbstractVisitor::VisitorFilterAll, true);
}

void HSSVisitorManager::runVisitors(HSSAbstractVisitor::VisitorFilterFlags filterFlags)
{
    QSharedPointer<HSSContainer> root = d->controller->root();
    runVisitors(root, filterFlags, true);
}

void HSSVisitorManager::runVisitors(QSharedPointer<HSSDisplayObject> root, HSSAbstractVisitor::VisitorFilterFlags filterFlags, bool traverse)
{
    if (root)
    {
        for (std::vector<HSSAbstractVisitor*>::iterator it = d->visitors.begin(); it != d->visitors.end(); ++it)
        {
            HSSAbstractVisitor* visitor = (*it);
            HSSAbstractVisitor::VisitorFilterFlags visitorFlags = visitor->getFilterFlags();
            if (visitorFlags & HSSAbstractVisitor::VisitorFilterSkip)
                continue;

            if (visitorFlags & filterFlags)
            {
                visitor->initializeVisit();
                root->accept(visitor, traverse);
                visitor->finalizeVisit();
            }
        }
    }
}

void HSSVisitorManager::reset()
{
    resetVisitors();
}

void HSSVisitorManager::resetVisitors()
{
    QSharedPointer<HSSContainer> root = d->controller->root();
    if (root)
    {
        for (std::vector<HSSAbstractVisitor*>::iterator it = d->visitors.begin(); it != d->visitors.end(); ++it)
        {
            (*it)->reset();
        }
    }
}

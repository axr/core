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

#include <QImage>
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRError.h"
#include "AXRString.h"
#include "IHSSVisitor.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSVisitorManager.h"

using namespace AXR;

HSSVisitorManager::HSSVisitorManager(AXRController *theController): controller(theController)
{
}

HSSVisitorManager::~HSSVisitorManager()
{
}

void HSSVisitorManager::addVisitor(IHSSVisitor *visitor)
{
    _visitors.push_back(visitor);
}

void HSSVisitorManager::runVisitors()
{
    QSharedPointer<HSSDisplayObject> root = this->controller->getRoot();
    runVisitors(root, IHSSVisitor::FLAG_All, true);
}

void HSSVisitorManager::runVisitors(IHSSVisitor::VisitorFilterFlags filterFlags)
{
    QSharedPointer<HSSContainer> root = this->controller->getRoot();
    runVisitors(root, filterFlags, true);
}

void HSSVisitorManager::runVisitors(QSharedPointer<HSSDisplayObject> root, IHSSVisitor::VisitorFilterFlags filterFlags, bool traverse)
{
    if (root)
    {
        for (IHSSVisitor::iterator it = _visitors.begin(); it < _visitors.end(); ++it)
        {
            IHSSVisitor* visitor = (*it);
            int visitorFlags = visitor->getFilterFlags();
            if (IHSSVisitor::FLAG_Skip & visitorFlags)
                continue;
            if (filterFlags & visitorFlags)
            {
                visitor->initializeVisit();
                root->accept(visitor, traverse);
                visitor->finalizeVisit();
            }
        }
    }
}

void HSSVisitorManager::reset(){
    resetVisitors();
}

void HSSVisitorManager::resetVisitors(){
    QSharedPointer<HSSContainer> root = this->controller->getRoot();
    if (root)
    {
        for (IHSSVisitor::iterator it = _visitors.begin(); it < _visitors.end(); ++it)
        {
            (*it)->reset();
        }
    }
}



//void HSSVisitorManager::mouseDown(HSSUnit x, HSSUnit y)
//{
//    //prepare values
//    HSSContainer::p root = this->controller->getRoot();
//
//    struct point
//    {
//        HSSUnit x;
//        HSSUnit y;
//    } thePoint;
//    thePoint.x = x;
//    thePoint.y = y;
//    if (root)
//        root->handleEvent(HSSEventTypeMouseDown, (void*) &thePoint);
//}
//
//void HSSVisitorManager::mouseUp(HSSUnit x, HSSUnit y)
//{
//    //prepare values
//    HSSContainer::p root = this->controller->getRoot();
//
//    struct point
//    {
//        HSSUnit x;
//        HSSUnit y;
//    } thePoint;
//    thePoint.x = x;
//    thePoint.y = y;
//    if (root)
//    {
//        try
//        {
//            root->handleEvent(HSSEventTypeMouseUp, (void*) &thePoint);
//        }
//        catch (const AXRError &e)
//        {
//            e.raise();
//        }
//    }
//}

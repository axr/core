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

#include <QStack>
#include "AXRBuffer.h"
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSInputEvent.h"
#include "HSSRequest.h"
#include "HSSSelFunction.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "XMLParser.h"

using namespace AXR;

HSSRequest::HSSRequest(AXRController * controller)
: HSSAction(HSSActionTypeRequest, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRequest: creating request object");

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("src");
    shorthandProperties.push_back("target");
    //shorthandProperties.push_back("mode");
    this->setShorthandProperties(shorthandProperties);
}

HSSRequest::HSSRequest(const HSSRequest & orig)
: HSSAction(orig)
{
}

QSharedPointer<HSSRequest> HSSRequest::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRequest: cloning request object");
    return qSharedPointerCast<HSSRequest> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRequest::cloneImpl() const
{
    return QSharedPointer<HSSRequest>(new HSSRequest(*this));
}

HSSRequest::~HSSRequest()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRequest: destructing request object");
}

AXRString HSSRequest::toString()
{
    return "HSSRequest";
}

AXRString HSSRequest::defaultObjectType()
{
    return "request";
}

void HSSRequest::fire()
{
    AXRController * ctrlr = this->getController();
    AXRDocument* document = ctrlr->document();

    //if there is no target
    if (this->target->empty())
    {
        document->loadXmlFile(this->src);
    }
    else
    {
        switch (this->mode)
        {
        default:
        {
            QSharedPointer<AXRBuffer> baseFile = document->file();
            QUrl newFileUrl = this->src;
            if(newFileUrl.scheme() == ""){
                newFileUrl = baseFile->sourceUrl().resolved(this->src);
            }
            QSharedPointer<AXRBuffer> newFile;
            try
            {
                newFile = document->createBufferFromUrl(newFileUrl);
            }
            catch (const AXRError &e)
            {
                e.raise();
            }

            if (newFile)
            {
                QSharedPointer<HSSContainer> tempNode = QSharedPointer<HSSContainer>(new HSSContainer(ctrlr));
                ctrlr->currentContext().push(tempNode);
                bool loadingSuccess = document->xmlParser()->loadFile(newFile);
                if (!loadingSuccess)
                {
                    AXRError("AXRDocument", "Could not load the XML file").raise();
                }
                else
                {
                    if(this->target)
                    {
                        for (HSSSimpleSelection::iterator it = tempNode->getChildren()->begin(); it != tempNode->getChildren()->end(); ++it)
                        {
                            QSharedPointer<HSSContainer> loadedRoot = HSSContainer::asContainer(*it);
                            if(loadedRoot)
                            {
                                for (HSSSimpleSelection::iterator it2 = this->target->begin(); it2 != this->target->end(); ++it2) {
                                    QSharedPointer<HSSContainer> theTarget = HSSContainer::asContainer(*it2);
                                    if(theTarget)
                                    {
                                        //QSharedPointer<HSSContainer> clonedNode = loadedRoot->clone();
                                        theTarget->add(loadedRoot);
                                    }
                                }

                                ctrlr->activateRules();
                                HSSInputEvent event(HSSEventTypeLoad);
                                loadedRoot->handleEvent(&event);
                                document->setNeedsDisplay(true);
                            }
                            break;
                        }
                    }
                }
                ctrlr->currentContext().pop();
            }
            break;
        }
        }
    }
}


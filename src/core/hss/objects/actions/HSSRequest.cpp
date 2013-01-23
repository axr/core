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

#include "AXRBuffer.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSRequest.h"
#include "HSSSelFunction.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "XMLParser.h"

using namespace AXR;

HSSRequest::HSSRequest(AXRController * controller)
: HSSAction(HSSActionTypeRequest, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRequest: creating request object");
    this->observedSrc
            = this->observedTarget
            = NULL;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("src");
    shorthandProperties.push_back("target");
    //shorthandProperties.push_back("mode");
    this->setShorthandProperties(shorthandProperties);

    this->mode = HSSRequestModeTypeAuto;
    this->target = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection);
}

HSSRequest::HSSRequest(const HSSRequest & orig)
: HSSAction(orig)
{
    this->observedSrc
            = this->observedTarget
            = NULL;

    this->mode = orig.mode;
    this->target = orig.target;
}

QSharedPointer<HSSRequest> HSSRequest::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRequest: cloning request object");
    return qSharedPointerCast<HSSRequest> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRequest::cloneImpl() const
{
    return QSharedPointer<HSSRequest>(new HSSRequest(*this));
}

HSSRequest::~HSSRequest()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRequest: destructing request object");
}

AXRString HSSRequest::toString()
{
    return "HSSRequest";
}

AXRString HSSRequest::defaultObjectType()
{
    return "request";
}

void HSSRequest::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertySrc:
        this->setDSrc(value);
        break;

    case HSSObservablePropertyTarget:
        this->setDTarget(value);
        break;

        //        case HSSObservablePropertyMode:
        //            this->setDMode(value);
        //            break;
        //
        //        case HSSObservablePropertyContentTarget:
        //            this->setDContentTarget(value);

    default:
        HSSAction::setProperty(name, value);
        break;
    }
}

void HSSRequest::fire()
{
    AXRController * ctrlr = this->getController();
    AXRDocument* document = ctrlr->document();

    //if there is no target
    if (this->target->empty())
    {
        document->loadXMLFile(this->src);
    }
    else
    {
        switch (this->mode)
        {
        default:
        {
            QSharedPointer<AXRBuffer> baseFile = document->getFile();
            QUrl newFileUrl = this->src;
            if(newFileUrl.scheme() == ""){
                newFileUrl = baseFile->sourceUrl().resolved(this->src);
            }
            QSharedPointer<AXRBuffer> newFile;
            try
            {
                newFile = document->getFile(newFileUrl);
            }
            catch (const AXRError &e)
            {
                e.raise();
            }

            if (newFile)
            {
                QSharedPointer<HSSContainer> tempNode = QSharedPointer<HSSContainer>(new HSSContainer(ctrlr));
                ctrlr->currentContext.push(tempNode);
                bool loadingSuccess = document->getParserXML()->loadFile(newFile);
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
                                loadedRoot->handleEvent(HSSEventTypeLoad, NULL);
                                document->setNeedsDisplay(true);
                            }
                            break;
                        }
                    }
                }
                ctrlr->currentContext.pop();
            }
            break;
        }
        }
    }
}

QSharedPointer<HSSParserNode> HSSRequest::getDSrc()
{
    return this->dSrc;
}

void HSSRequest::setDSrc(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeKeywordConstant:
    case HSSParserNodeTypeFunctionCall:
    case HSSParserNodeTypeStringConstant:
    {
        this->dSrc = value;
        if (this->observedSrc)
        {
            this->observedSrc->removeObserver(this->observedSrcProperty, HSSObservablePropertySrc, this);
        }

        switch (value->getType())
        {
        case HSSParserNodeTypeKeywordConstant:
            /**
             *  @todo what here?
             */
            break;

        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theString = qSharedPointerCast<HSSStringConstant > (value);
            this->src = QUrl(theString->getValue());
            break;
        }

        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<AXRString>())
            {
                this->src = QUrl(remoteValue.value<AXRString>());
            }

            break;
        }

        default:
            break;
        }

        this->notifyObservers(HSSObservablePropertySrc, &this->src);

        break;
    }

    default:
        throw AXRWarning("HSSRequest", "Invalid value for src of " + this->name);
    }
}

void HSSRequest::srcChanged(AXR::HSSObservableProperty source, void *data)
{
    AXRWarning("HSSRequest", "unimplemented").raise();
}

QSharedPointer<HSSParserNode> HSSRequest::getDTarget()
{
    return this->dTarget;
}

void HSSRequest::setDTarget(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeKeywordConstant:
    case HSSParserNodeTypeFunctionCall:
    {
        this->dTarget = value;
        if (this->observedTarget)
        {
            this->observedTarget->removeObserver(this->observedTargetProperty, HSSObservablePropertyTarget, this);
        }

        switch (value->getType())
        {
        case HSSParserNodeTypeKeywordConstant:
            /**
             *  @todo what here?
             */
            break;

        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
            if (fnct)
            {
                fnct->setScope(this->scope);
                fnct->setThisObj(this->getThisObj());
                QVariant remoteValue = fnct->evaluate();
                if (remoteValue.canConvert< QSharedPointer<HSSSelection> >())
                {
                    QSharedPointer<HSSSimpleSelection> selection = remoteValue.value< QSharedPointer<HSSSelection> >()->joinAll();
                    this->target->clear();
                    this->target->insert(this->target->end(), selection->begin(), selection->end());
                }

                /**
                 *  @todo potentially leaking
                 */
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTarget, this, new HSSValueChangedCallback<HSSRequest > (this, &HSSRequest::targetChanged));
                this->observedTarget = fnct.data();
                this->observedTargetProperty = HSSObservablePropertyValue;
            }

            break;
        }

        default:
            break;
        }

        this->notifyObservers(HSSObservablePropertySrc, &this->src);

        break;
    }

    default:
        throw AXRWarning("HSSRequest", "Invalid value for src of " + this->name);
    }
}

void HSSRequest::targetChanged(AXR::HSSObservableProperty source, void *data)
{
    AXRWarning("HSSRequest", "unimplemented").raise();
}

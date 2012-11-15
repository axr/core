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

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QUrl>
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRTestThread.h"
#include "AXRTestRunner.h"
#include "AXRWarning.h"
#include "HSSFunction.h"
#include "HSSSelFunction.h"

using namespace AXR;

AXRTestRunner::AXRTestRunner()
{
    document_ = new AXRDocument();
}

AXRTestRunner::~AXRTestRunner()
{
    delete document_;
}

AXRDocument* AXRTestRunner::document() const
{
    return document_;
}

AXRString AXRTestRunner::getPathToTestsFile()
{
    QDir dir = QDir(QCoreApplication::applicationDirPath());
#ifdef Q_WS_MACX
    dir.cdUp();
#endif
    dir.cd("Resources");
    dir.cd("views");
    return dir.filePath("layoutTests.hss");
}

void AXRTestRunner::executeLayoutTests(HSSObservableProperty passnull, void*data)
{
    HSSContainer::p status;
    HSSContainer::p root = document_->getController()->getRoot();
    std::deque<HSSParserNode::p> arguments = *(std::deque<HSSParserNode::p>*)data;

    for (std::deque<HSSParserNode::p>::iterator it = arguments.begin(); it != arguments.end(); ++it)
    {
        HSSParserNode::p argument = *it;
        if (argument->isA(HSSFunctionTypeSel))
        {
            HSSSelFunction::p selFunction = qSharedPointerCast<HSSSelFunction>(argument);
            QVariant remoteValue = selFunction->evaluate();
            if (remoteValue.canConvert<HSSSelection::p >())
            {
                HSSSelection::p selection = remoteValue.value<HSSSelection::p >();
                HSSSimpleSelection::p innerSelection = selection->joinAll();
                status = HSSContainer::asContainer(innerSelection->front());
            }
        }
    }

    AXRTestThread thrd(this, QUrl::fromLocalFile(this->getPathToTestsFile()), status);
    thrd.start();
}

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

#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRTestProducer.h"
#include "AXRTestRunner.h"
#include "AXRTestThread.h"
#include "HSSContainer.h"

using namespace AXR;

AXRTestThread::AXRTestThread(AXRTestRunner *runner, QUrl url, HSSContainer::p status)
{
    this->runner = runner;
    this->url = url;
    this->totalPassed = 0;
    this->totalTests = 0;
    this->status = status;
}

void AXRTestThread::run()
{
    operator()();
}

void AXRTestThread::operator () ()
{
    try
    {
        //load the XML file
        AXRDocument* document = runner->document();
        XMLParser::p parser = document->getParserXML();
        HSSContainer::p status = this->status;
        AXRBuffer::p testsFile = document->getFile(this->url);
        bool loadingSuccess = parser->loadFile(testsFile);
        if (loadingSuccess)
        {
            //find all the tests that need to be executed
            std::vector<std::vector<QUrl> > tests;
            AXRController::p controller = document->getController();
            HSSContainer::p root = controller->getRoot();
            const std::vector<HSSDisplayObject::p> & children = root->getChildren(true);

            for (std::vector<HSSDisplayObject::p>::const_iterator it = children.begin(); it != children.end(); ++it)
            {
                const HSSDisplayObject::p & child = *it;
                if (child->attributes.find("href") != child->attributes.end() && child->attributes.find("expect") != child->attributes.end())
                {
                    const AXRString test[2] = {child->attributes["href"], child->attributes["expect"]};
                    std::vector<QUrl>testVect(test, test + 2);
                    tests.push_back(testVect);
                    this->totalTests += 1;
                }
                else
                {
                    std_log("the test element needs to have expect and href attributes");
                }
            }
            //execute all the tests
            QThreadPool producers;

            for (std::vector<std::vector<QUrl> >::iterator it2 = tests.begin(); it2 != tests.end(); ++it2)
            {
                AXRTestProducer prdcr(this->runner, *it2, &this->totalTests, &this->totalPassed, status);
                producers.start(&prdcr);
                QThread::yieldCurrentThread();
            }

            producers.waitForDone();

            std_log("\n\nTEST RESULTS SUMMARY");
            std_log("===============================");
            std_log(AXRString("Passed %1 out of %2").arg(this->totalPassed).arg(this->totalTests));
            std_log("===============================");

        }
        else
        {
            std_log("Could not load the tests XML file");
        }

    }
    catch (const AXRError &e)
    {
        e.raise();
    }
}

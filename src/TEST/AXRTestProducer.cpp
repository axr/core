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

using namespace AXR;

// Constructor with name and the queue to use
AXRTestProducer::AXRTestProducer(AXRTestRunner *runner, std::vector<QUrl> test, unsigned * totalTests, unsigned * totalPassed, HSSContainer::p status)
{
    this->runner = runner;
    this->test = test;
    this->totalPassed = totalPassed;
    this->totalTests = totalTests;
    this->status = status;
}

// The thread function fills the queue with data
QMutex AXRTestProducer::statusMutex;

void AXRTestProducer::run()
{
    operator()();
}

void AXRTestProducer::operator () ()
{

    bool testLoaded = false;
    bool expectedLoaded = false;
    bool result = false;
    AXRString testRep;
    AXRString expectedRep;

    //load the XML
    AXRDocument *document = AXRDocument::getInstance();

    testLoaded = document->loadXMLFile(this->test[0]);

    if (testLoaded)
    {
        AXRController::p controller = document->getController();
        HSSContainer::p root = controller->getRoot();
        document->getRender()->windowWidth = 400.;
        document->getRender()->windowHeight = 400.;
        root->recursiveReadDefinitionObjects();
        root->recursiveLayout();
        testRep = root->toString();
        //std_log(testRep);
    }
    else
    {
        std_log("could not load the test file");
    }

    //load the "expected" file
    if (testLoaded)
    {
        AXRBuffer::p expectedFile = document->getFile(test[1]);
        if (!expectedFile->isValid())
        {
            std_log("could not load file with expected results");
        }
        else
        {
            expectedLoaded = true;
            expectedRep = AXRString(expectedFile->getBuffer());
            //std_log(expectedRep);
        }
    }

    //compare the two
    if (testLoaded && expectedLoaded)
    {
        result = testRep.compare(expectedRep) == 0;
    }

    if (result)
    {
        std_log("PASSED test " + test[0].toString());
        *this->totalPassed += 1;
        this->statusMutex.lock();
        this->status->setContentText(AXRString("Passed %1 out of %2").arg(*this->totalPassed).arg(*this->totalTests));
        this->statusMutex.unlock();
        document->setNeedsDisplay(true);
    }
    else
    {
        std_log("FAILED test " + test[0].toString());
        std_log("This is the dump of the test:");
        std_log("===============================");
        std_log(testRep);
        std_log("===============================");
    }
}

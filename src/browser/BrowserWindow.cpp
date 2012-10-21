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

#include <QErrorMessage>
#include <QFileDialog>
#include <QPaintEvent>
#include <QTextEdit>
#include <QUrl>

#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRTestRunner.h"
#include "XMLParser.h"

#include "LogWindow.h"
#include "PreferencesDialog.h"
#include "BrowserApplication.h"
#include "BrowserSettings.h"
#include "BrowserWindow.h"

using namespace AXR;

#include "ui_BrowserWindow.h"

class BrowserWindow::Private
{
public:
    Private()
    {
        document = new AXRDocument();
        testRunner = new AXRTestRunner();
        axr_debug_device = qApp->loggingDevice();
    }

    ~Private()
    {
        axr_debug_device = NULL;

        delete document;
        delete testRunner;
    }

    AXRDocument *document;
    AXRTestRunner *testRunner;
};

BrowserWindow::BrowserWindow(QWidget *parent)
: QMainWindow(parent), d(new Private), ui(new Ui::BrowserWindow)
{
    ui->setupUi(this);

    ui->enableAntialiasingAction->setChecked(d->document->getRender()->globalAntialiasingEnabled());

    // Tell the widget to render this window's document
    ui->renderingView->setDocument(d->document);

    // The subview needs to accept drops as well even though the main window handles it
    ui->renderingView->setAcceptDrops(true);

    ui->openAction->setShortcuts(QKeySequence::Open);
    ui->reloadAction->setShortcuts(QKeySequence::Refresh);
    ui->closeAction->setShortcuts(QKeySequence::Close);
    ui->exitAction->setShortcuts(QKeySequence::Quit);

    ui->enableAntialiasingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_A));

    ui->previousLayoutStepAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_K));
    ui->nextLayoutStepAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_L));

    ui->listXmlElementsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_X));
    ui->listHssStatementsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_C));
    ui->listHssTokensAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_V));

    ui->runLayoutTestsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_T));

    ui->logAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));

    this->closeFile();
}

BrowserWindow::~BrowserWindow()
{
    delete ui;
    delete d;
}

void BrowserWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        Q_FOREACH (QUrl url, urlList)
        {
            QFileInfo fi(url.toLocalFile());
            if (fi.exists() && (fi.suffix() == "xml" || fi.suffix() == "hss"))
            {
                event->setDropAction(Qt::CopyAction);
                event->setAccepted(true);
                break;
            }
        }
    }
    else
    {
        event->setAccepted(false);
    }
}

void BrowserWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        QStringList pathList;

        Q_FOREACH (QUrl url, urlList)
        {
            pathList += url.toLocalFile();
        }

        event->accept();
        this->openFiles(pathList);
    }
    else
    {
        event->ignore();
    }
}

void BrowserWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open XML/HSS File"), QString(), "AXR Files (*.xml *.hss)");
    if (!file.isEmpty())
    {
        openFile(file);
    }
}

void BrowserWindow::openFile(const QString &filePath)
{
    setWindowTitle(QString());
    setWindowFilePath(filePath);

    d->document->loadFileByPath(QUrl::fromLocalFile(filePath));
    qApp->settings()->setLastFileOpened(filePath);
    update();
}

void BrowserWindow::openFiles(const QStringList &filePaths)
{
    // TODO: This actually needs to open new windows or tabs
    Q_FOREACH (QString path, filePaths)
    {
        openFile(path);
    }
}

void BrowserWindow::reloadFile()
{
    d->document->reload();
    update();
}

void BrowserWindow::closeFile()
{
    setWindowTitle(QCoreApplication::applicationName());
    setWindowFilePath(QString());

    // TODO: Actually close the file...
}

void BrowserWindow::previousLayoutStep()
{
    d->document->setShowLayoutSteps(true);
    d->document->previousLayoutStep();
    update();
}

void BrowserWindow::nextLayoutStep()
{
    d->document->setShowLayoutSteps(true);
    d->document->nextLayoutStep();
    update();
}

void BrowserWindow::listXmlElements()
{
    AXRString file = QFileDialog::getOpenFileName(this, tr("Open XML File"), AXRString(), "XML Files (*.xml)");
    if (!file.isEmpty())
    {
        QFile qfile(file);
        QFileInfo fi(qfile);
        qfile.open(QIODevice::ReadOnly);

        AXRBuffer::p f(new AXRBuffer(fi));

        AXRController *controller = new AXRController(d->document);
        XMLParser parser(controller);

        if (!parser.loadFile(f))
        {
            QErrorMessage error(this);
            error.showMessage(AXR::fromStdString("Could not parse the given file"));
        }

        LogWindow w(this);
        w.setLogText(controller->toString());
        w.exec();
    }
}

void BrowserWindow::listHssStatements()
{
    AXRString file = QFileDialog::getOpenFileName(this, tr("Open HSS File"), AXRString(), "HSS Files (*.hss)");
    if (!file.isEmpty())
    {
        QFile qfile(file);
        QFileInfo fi(qfile);
        qfile.open(QIODevice::ReadOnly);

        AXRBuffer::p f(new AXRBuffer(fi));

        AXRController controller(d->document);
        HSSParser hssparser(&controller);

        if (hssparser.loadFile(f))
        {
            /*const std::vector<AXR::HSSStatement::p> statements = controller.getStatements();
            for (unsigned i = 0; i < statements.size(); ++i)
            {
                std_log1(statements[i]->toString());
            }*/
        }
        else
        {
            QErrorMessage error(this);
            error.showMessage(AXR::fromStdString("Could not parse the given file"));
        }
    }
}

void BrowserWindow::listHssTokens()
{
    AXRString file = QFileDialog::getOpenFileName(this, tr("Open HSS File"), AXRString(), "HSS Files (*.hss)");
    if (!file.isEmpty())
    {
        QFile qfile(file);
        QFileInfo fi(qfile);
        qfile.open(QIODevice::ReadOnly);

        AXRBuffer::p f(new AXRBuffer(fi));

        HSSTokenizer tokenizer;
        tokenizer.setFile(f);

        tokenizer.readNextChar();

        AXR::HSSToken::p token;
        AXRString ss;
        forever
        {
            if (token)
                token.clear();

            token = tokenizer.readNextToken();

            if (!token)
                break;

            ss += token->toString() + "\n";
        }

        QErrorMessage error(this);
        error.showMessage(ss);
    }
}

void BrowserWindow::runLayoutTests()
{
    d->document->registerCustomFunction("AXRLayoutTestsExecute", new HSSValueChangedCallback<AXRTestRunner>(d->testRunner, &AXRTestRunner::executeLayoutTests));
    this->openFile(d->testRunner->getPathToTestsFile());
}

void BrowserWindow::showErrorLog()
{
    qApp->showLogWindow();
}

void BrowserWindow::showPreferences()
{
    qApp->showPreferencesDialog();
}

void BrowserWindow::showAbout()
{
}

void BrowserWindow::toggleAntialiasing(bool on)
{
    d->document->getRender()->setGlobalAntialiasingEnabled(on);
    update();
}

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
#include "AXRInitializer.h"
#include "AXRWrapper.h"
#include "XMLParser.h"

#include "LogWindow.h"
#include "PreferencesDialog.h"
#include "PrototypeApplication.h"
#include "PrototypeSettings.h"
#include "PrototypeWindow.h"

using namespace AXR;

#include "ui_PrototypeWindow.h"

class PrototypeWindow::Private
{
public:
    Private() : wrapper(new AXRWrapper()), logWindow(new LogWindow())
    {
        logWindow->setWindowTitle(tr("Error Log"));
    }

    ~Private()
    {
        delete wrapper;
        delete logWindow;
    }

    AXRWrapper *wrapper;
    LogWindow *logWindow;
};

PrototypeWindow::PrototypeWindow(QWidget *parent)
: QMainWindow(parent), d(new Private), ui(new Ui::PrototypeWindow)
{
    ui->setupUi(this);

    AXRCore::tp &core = AXRCore::getInstance();
    ui->enableAntialiasingAction->setChecked(core->getRender()->globalAntialiasingEnabled());

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

    ui->errorLogAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
}

PrototypeWindow::~PrototypeWindow()
{
    delete ui;
    delete d;
}

void PrototypeWindow::dragEnterEvent(QDragEnterEvent *event)
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

void PrototypeWindow::dropEvent(QDropEvent *event)
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

void PrototypeWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open XML/HSS File"), QString(), "AXR Files (*.xml *.hss)");
    if (!file.isEmpty())
    {
        openFile(file);
    }
}

void PrototypeWindow::openFile(const QString &filePath)
{
    setWindowFilePath(filePath);
    d->wrapper->loadFileByPath(filePath);
    qApp->settings()->setLastFileOpened(filePath);
    update();
}

void PrototypeWindow::openFiles(const QStringList &filePaths)
{
    // TODO: This actually needs to open new windows or tabs
    Q_FOREACH (QString path, filePaths)
    {
        openFile(path);
    }
}

void PrototypeWindow::reloadFile()
{
    openFile(windowFilePath());
}

void PrototypeWindow::closeFile()
{
}

void PrototypeWindow::previousLayoutStep()
{
    d->wrapper->setShowLayoutSteps(true);
    d->wrapper->previousLayoutStep();
    update();
}

void PrototypeWindow::nextLayoutStep()
{
    d->wrapper->setShowLayoutSteps(true);
    d->wrapper->nextLayoutStep();
    update();
}

void PrototypeWindow::listXmlElements()
{
    AXRString file = QFileDialog::getOpenFileName(this, tr("Open XML File"), AXRString(), "XML Files (*.xml)");
    if (!file.isEmpty())
    {
        QFile qfile(file);
        QFileInfo fi(qfile);
        qfile.open(QIODevice::ReadOnly);

        AXRFile::p f(new AXRFile(fi));

        AXRController *controller = new AXRController();
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

void PrototypeWindow::listHssStatements()
{
    AXRString file = QFileDialog::getOpenFileName(this, tr("Open HSS File"), AXRString(), "HSS Files (*.hss)");
    if (!file.isEmpty())
    {
        QFile qfile(file);
        QFileInfo fi(qfile);
        qfile.open(QIODevice::ReadOnly);

        AXRFile::p f(new AXRFile(fi));

        AXRController controller;
        HSSParser hssparser(&controller, d->wrapper);

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

void PrototypeWindow::listHssTokens()
{
    AXRString file = QFileDialog::getOpenFileName(this, tr("Open HSS File"), AXRString(), "HSS Files (*.hss)");
    if (!file.isEmpty())
    {
        QFile qfile(file);
        QFileInfo fi(qfile);
        qfile.open(QIODevice::ReadOnly);

        AXRFile::p f(new AXRFile(fi));

        HSSTokenizer tokenizer;
        tokenizer.setFile(f);

        tokenizer.readNextChar();

        AXR::HSSToken::p token;
        AXRString ss;
        forever
        {
            if (token)
                token.reset();

            token = tokenizer.readNextToken();

            if (!token)
                break;

            ss += token->toString() + "\n";
        }

        QErrorMessage error(this);
        error.showMessage(ss);
    }
}

void PrototypeWindow::runLayoutTests()
{
    QDir dir = QDir(QCoreApplication::applicationDirPath());
#ifdef Q_WS_MACX
    dir.cdUp();
#endif
    dir.cd("Resources");
    dir.cd("views");

    AXRCore::tp &core = AXRCore::getInstance();
    core->registerCustomFunction("AXRLayoutTestsExecute", new HSSValueChangedCallback<AXRWrapper>(d->wrapper, &AXRWrapper::executeLayoutTests));
    this->openFile(dir.filePath("layoutTests.hss"));
}

void PrototypeWindow::showErrorLog()
{
    d->logWindow->show();
}

void PrototypeWindow::showPreferences()
{
    qApp->showPreferencesDialog();
}

void PrototypeWindow::showAbout()
{
}

void PrototypeWindow::toggleAntialiasing(bool on)
{
    AXRCore::tp &core = AXRCore::getInstance();
    core->getRender()->setGlobalAntialiasingEnabled(on);
    update();
}

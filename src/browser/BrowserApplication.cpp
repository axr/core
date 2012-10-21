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

#include <QFileOpenEvent>
#include <QMessageBox>
#include <QUrl>
#include "config.h"
#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "LogWindow.h"
#include "PreferencesDialog.h"
#include "BrowserApplication.h"
#include "BrowserSettings.h"
#include "BrowserWindow.h"

class BrowserApplication::Private
{
public:
    BrowserSettings *settings;
    PreferencesDialog *preferencesDialog;
    LogWindow *logWindow;
    BrowserWindow *mainWindow;
};

BrowserApplication::BrowserApplication(int &argc, char **argv)
: QApplication(argc, argv), d(new Private)
{
    Q_INIT_RESOURCE(Resources);

    setOrganizationName(AXR_VENDOR);
    setOrganizationDomain(AXR_DOMAIN);
    setApplicationVersion(AXR_VERSION_STRING);
    setApplicationName("AXR Browser");

    d->settings = new BrowserSettings();
    d->preferencesDialog = new PreferencesDialog();
    d->logWindow = new LogWindow();
    d->mainWindow = new BrowserWindow();
    d->mainWindow->show();

    // Check if the user wanted to load a file by command line
    QStringList args = arguments();
    args.removeFirst();
    if (!args.empty())
    {
        d->mainWindow->openFiles(args);
    }
    else
    {
        // Otherwise use the default action according to preferences
        switch (d->settings->fileLaunchAction())
        {
            case BrowserSettings::FileLaunchActionOpenLastFile:
                d->mainWindow->openFile(d->settings->lastFileOpened());
                break;
            case BrowserSettings::FileLaunchActionShowOpenFileDialog:
                d->mainWindow->openFile();
                break;
            default:
                break;
        }
    }

    axr_debug_activate_channel(d->settings->debuggingChannelsMask());
}

BrowserApplication::~BrowserApplication()
{
    delete d->mainWindow;
    delete d->logWindow;
    delete d->preferencesDialog;
    delete d->settings;
    delete d;
}

bool BrowserApplication::event(QEvent *e)
{
    switch (e->type())
    {
        case QEvent::FileOpen:
        {
            QFileOpenEvent *open = static_cast<QFileOpenEvent*>(e);

            // TODO: this gets //localhost appended to the front for some reason
            QUrl url = open->url();
            if (url.scheme() == "file" && url.host() == "localhost")
                url.setHost(QString());

            d->mainWindow->openFile(url.toLocalFile());
            return true;
        }
        default:
            return QApplication::event(e);
    }
}

// TODO: Remove this method once exceptions are eliminated from the core library
bool BrowserApplication::notify(QObject *receiver, QEvent *event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (AXR::AXRError &e)
    {
        QMessageBox::critical(d->mainWindow, "Fatal error",
                              "The application has encountered a critical error and will terminate.\n\n"
                              "This is the developers' fault.\n\n"
                              "Complain here https://github.com/AXR/Prototype/issues/168 until they fix it!\n\n"
                              + e.toString());
        QApplication::exit(-1);
    }

    return false;
}

BrowserSettings* BrowserApplication::settings() const
{
    return d->settings;
}

QIODevice* BrowserApplication::loggingDevice() const
{
    return d->logWindow->logBuffer();
}

void BrowserApplication::showPreferencesDialog()
{
    d->preferencesDialog->show();
}

void BrowserApplication::showLogWindow()
{
    d->logWindow->show();
}

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

#include "config.h"
#include "AXRDebugging.h"
#include "PreferencesDialog.h"
#include "PrototypeApplication.h"
#include "PrototypeSettings.h"
#include "PrototypeWindow.h"

class PrototypeApplication::Private
{
public:
    PrototypeSettings *settings;
    PreferencesDialog *preferencesDialog;
    PrototypeWindow *mainWindow;
};

PrototypeApplication::PrototypeApplication(int &argc, char **argv)
: QApplication(argc, argv), d(new Private)
{
    Q_INIT_RESOURCE(Resources);

    setOrganizationName(AXR_VENDOR);
    setOrganizationDomain(AXR_DOMAIN);
    setApplicationVersion(AXR_VERSION_STRING);
    setApplicationName("AXR Prototype");

    d->settings = new PrototypeSettings();
    d->preferencesDialog = new PreferencesDialog();
    d->mainWindow = new PrototypeWindow();
    d->mainWindow->show();

    switch (d->settings->fileLaunchAction())
    {
        case PrototypeSettings::FileLaunchActionOpenLastFile:
            d->mainWindow->openFile(d->settings->lastFileOpened());
            break;
        case PrototypeSettings::FileLaunchActionShowOpenFileDialog:
            d->mainWindow->openFile();
            break;
        default:
            break;
    }

    axr_debug_activate_channel(d->settings->debuggingChannelsMask());
}

PrototypeApplication::~PrototypeApplication()
{
    delete d->mainWindow;
    delete d->preferencesDialog;
    delete d->settings;
    delete d;
}

PrototypeSettings* PrototypeApplication::settings() const
{
    return d->settings;
}

void PrototypeApplication::showPreferencesDialog()
{
    d->preferencesDialog->show();
}

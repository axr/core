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

#include <QAction>
#include <QDesktopWidget>
#include <QIcon>
#include <QSettings>
#include <QToolBar>
#include "AXRDebugging.h"
#include "PreferencesDialog.h"
#include "PrototypeApplication.h"
#include "PrototypeSettings.h"
#include "ui_PreferencesDialog.h"

class PreferencesDialog::Private
{
public:
    Private() : pageActionsGroup(NULL), finishedLoading(false) { }

    QActionGroup *pageActionsGroup;
    bool finishedLoading;
};

PreferencesDialog::PreferencesDialog(QWidget *parent)
: QMainWindow(parent), d(new Private), ui(new Ui::PreferencesDialog)
{
    this->ui->setupUi(this);

    d->pageActionsGroup = new QActionGroup(this);

    // Icons from /System/Library/CoreServices/CoreTypes.bundle/Contents
    d->pageActionsGroup->addAction(ui->generalAction); // General.icns
    d->pageActionsGroup->addAction(ui->debugAction); // KEXT.icns

    loadPreferences();
    switchPage();

#ifdef Q_WS_MACX
    this->setWindowModality(Qt::NonModal);
    this->ui->buttonBox->hide();

    // OS X seems to prefer settings dialogs to be placed at (<center>, 150)
    this->adjustSize();
    this->move((qApp->desktop()->width() - this->width()) / 2, 150);
#endif
}

PreferencesDialog::~PreferencesDialog()
{
    delete d;
    delete this->ui;
}

void PreferencesDialog::loadPreferences()
{
    ui->fileLaunchActionComboBox->setCurrentIndex(qApp->settings()->fileLaunchAction());

    quint32 mask = qApp->settings()->debuggingChannelsMask();
    ui->onChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_ON);
    ui->overviewChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_OVERVIEW);
    ui->generalChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_GENERAL);
    ui->generalSpecificChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_GENERAL_SPECIFIC);
    ui->xmlChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_XML);
    ui->hssChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_HSS);
    ui->observingChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_OBSERVING);
    ui->eventsChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_EVENTS);
    ui->eventsSpecificChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_EVENTS_SPECIFIC);
    ui->tokenizingChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_TOKENIZING);
    ui->fullFilenamesChannelCheckBox->setChecked(mask & AXR_DEBUG_CH_FULL_FILENAMES);

    d->finishedLoading = true;
}

void PreferencesDialog::accept()
{
    // Don't save settings until the dialog is fully loaded, otherwise we'll
    // overwrite settings as we're reading them during the loading stage
    if (!d->finishedLoading)
        return;

    qApp->settings()->setFileLaunchAction(static_cast<PrototypeSettings::FileLaunchAction>(ui->fileLaunchActionComboBox->currentIndex()));

    quint32 mask = 0;
    mask |= ui->onChannelCheckBox->isChecked() ? AXR_DEBUG_CH_ON : 0;
    mask |= ui->overviewChannelCheckBox->isChecked() ? AXR_DEBUG_CH_OVERVIEW : 0;
    mask |= ui->generalChannelCheckBox->isChecked() ? AXR_DEBUG_CH_GENERAL : 0;
    mask |= ui->generalSpecificChannelCheckBox->isChecked() ? AXR_DEBUG_CH_GENERAL_SPECIFIC : 0;
    mask |= ui->xmlChannelCheckBox->isChecked() ? AXR_DEBUG_CH_XML : 0;
    mask |= ui->hssChannelCheckBox->isChecked() ? AXR_DEBUG_CH_HSS : 0;
    mask |= ui->observingChannelCheckBox->isChecked() ? AXR_DEBUG_CH_OBSERVING : 0;
    mask |= ui->eventsChannelCheckBox->isChecked() ? AXR_DEBUG_CH_EVENTS : 0;
    mask |= ui->eventsSpecificChannelCheckBox->isChecked() ? AXR_DEBUG_CH_EVENTS_SPECIFIC : 0;
    mask |= ui->tokenizingChannelCheckBox->isChecked() ? AXR_DEBUG_CH_TOKENIZING : 0;
    mask |= ui->fullFilenamesChannelCheckBox->isChecked() ? AXR_DEBUG_CH_FULL_FILENAMES : 0;
    qApp->settings()->setDebuggingChannelsMask(mask);
    axr_debug_activate_channel(mask);

#ifndef Q_WS_MACX
    close();
#endif
}

void PreferencesDialog::reject()
{
    close();
}

void PreferencesDialog::preferenceChanged()
{
#ifdef Q_WS_MACX
    accept();
#endif
}

void PreferencesDialog::switchPage()
{
    QAction *current = d->pageActionsGroup->checkedAction();
    if (!current)
    {
        current = ui->generalAction;
        current->setChecked(true);
    }

    if (current == ui->generalAction)
        ui->stackedWidget->setCurrentWidget(ui->generalPage);
    else if (current == ui->debugAction)
        ui->stackedWidget->setCurrentWidget(ui->debugPage);

    this->adjustSize();
}

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

#ifndef AXRWRAPPER_H
#define AXRWRAPPER_H

#include <QMap>
#include <QMutex>
#include <QSharedPointer>
#include <QThread>
#include <QThreadPool>
#include "AXRError.h"
#include "AXRBuffer.h"
#include "HSSContainer.h"
#include "HSSObservableProperties.h"

namespace AXR
{
    class AXRCore;

    /**
     *  @brief This is the abstract superclass from with all OS-specific wrappers
     *  should inherit from.
     */
    class AXR_API AXRWrapper
    {
    public:
        /**
         *  The shared pointer to the wrapper.
         */
        typedef QSharedPointer<AXRWrapper> p;

        /**
         *  Creates a new instance of the wrapper. It will obtain the singleton instance of
         *  the AXRCore and initialize it.
         */
        AXRWrapper();

        /**
         *  Destructs the wrapper
         */
        virtual ~AXRWrapper();
        /**
         *  Creates a AXRBuffer representation from the url you provide.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param url  A string containing the url to the file
         */
        virtual AXRBuffer::p getFile(QUrl url);
        virtual bool needsDisplay() const;
        /**
         *  This is to be called when something happens that needs to trigger a redraw.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param  newValue    A boolean containing the new state.
         */
        virtual void setNeedsDisplay(bool newValue);
        /**
         *  Getter for the core.
         *  @return A shared pointer to the core object.
         */
        QSharedPointer<AXRCore> getCore();
        /**
         *  Setter for the core.
         *  param xcr   A shared pointer to the core object.
         */
        void setCore(QSharedPointer<AXRCore> xcr);
        /**
         *  This creates a string containing a basic XML document with 1 element called "root".
         *  It is used when a HSS file is loaded directly.
         *  @return A shared pointer to the AXRBuffer representation of the basic XML document.
         */
        AXRBuffer::p createDummyXML(QUrl hssUrl);
        /**
         *  Loads the XML file at the path you provide.
         *  @param  xmlfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadXMLFile(QUrl url);
        bool loadXMLFile(AXRBuffer::p buffer);
        /**
         *  Loads the file you provide, and then handles it according to its file extension.
         *  @param  filepath    A string containing the path to the file on the local system, can be
         *                      either an XML or an HSS file.
         *
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadFileByPath(QUrl url);
        /**
         *  Loads the HSS file at the path you provide.
         *  @param  hssfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadHSSFile(QUrl url);
        /**
         *  Reloads the file that is currently loaded.
         *  @return Wether it has been reloaded successfully or not.
         */
        bool reload();
        /**
         *  Tells if there is a file already loaded.
         *  @return Wether a file has already been loaded or not.
         */
        bool hasLoadedFile();
        /**
         *  Each platform specific wrapper defines how it gets the path to the resources folder.
         *  @return A string containing the path to where the HSS resources files are
         *  stored.
         */
        virtual AXRString getPathToResources();

        /**
         */
        virtual AXRString getPathToTestsFile();

        bool showLayoutSteps();
        void setShowLayoutSteps(bool value);
        void previousLayoutStep();
        void nextLayoutStep();
        void nextLayoutTick();
        void resetLayoutTicks();
        bool layoutStepDone();
        void nextLayoutChild();
        void resetLayoutChild();
        bool layoutChildDone();
        void breakIfNeeded();

        void executeLayoutTests(HSSObservableProperty passnull, void*data);

        QMap<unsigned, AXRBuffer::p> files;

    private:
        bool _isHSSOnly;
        bool _hasLoadedFile;
        bool _showLayoutSteps;
        unsigned int _currentLayoutStep;
        unsigned int _currentLayoutTick;
        unsigned int _currentLayoutChild;
        bool _needsDisplay;

    public:
        AXRString _layoutTestsFilePath;
    };
}

#endif

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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef AXRDOCUMENT_H
#define AXRDOCUMENT_H

#include "AXRGlobal.h"

class QDir;
template <class T> class QSharedPointer;

namespace AXR
{
    class AXRBuffer;
    class AXRController;
    class AXRDocumentPrivate;
    class AXRRender;
    class HSSCallback;
    class HSSInputEvent;
    class HSSParser;
    class HSSRect;
    class HSSVisitorManager;
    class XMLParser;

    /**
     * @brief Represents a single AXR document that can be rendered. A document
     * may be composed of one or more files.
     */
    class AXR_API AXRDocument
    {
        Q_DISABLE_COPY(AXRDocument)
    public:
        /**
         *  Creates a new AXRDocument object.
         */
        AXRDocument();

        /**
         *  Destroys the object
         */
        virtual ~AXRDocument();

        /**
         * Gets the path to where the HSS framework resource files are stored.
         */
        virtual AXRString resourcesPath() const;

        /**
         *  After everything is set up, this puts everything in motion:
         *  loads xml, loads hss, applies rules, etc
         */
        void run();
        /**
         *  Reset everything to default values, so that we can start
         *  from fresh.
         */
        void reset();
        /**
         *  @return tells wether a file has been loaded or not
         */
        bool isFileLoaded() const;

        /**
         *  @return returns the current AXRBuffer that is opened. This can be either
         *  an XML or an HSS file.
         */
        QSharedPointer<AXRBuffer> file() const;
        /**
         *  Sets the current AXRBuffer to be used as the base document.
         *  @param file     The AXRBuffer to store for later use.
         */
        void setFile(QSharedPointer<AXRBuffer> file);
        /**
         *  @return the path to the directory in which 'file' is stored
         */
        QDir directory() const;
        /**
         *  Getter for shared pointer to the AXRController that is managing the
         *  object tree.
         *  @return Shared pointer to the AXRController
         */
        QSharedPointer<AXRController> controller() const;
        /**
         *  Setter for the AXRController that is managing the object tree.
         *  @param controller   A shared pointer to the controller.
         */
        void setController(QSharedPointer<AXRController> controller);
        /**
         *  Getter for shared pointer to the HSSVisitorManager which is the dispatcher for all HSSAbstractVisitors.
         *  @return A shared pointer to the HSSVisitorManager
         */
        QSharedPointer<HSSVisitorManager> visitorManager() const;

        /**
         *  Setter for the HSSVisitorManager that will be used.
         *  @param visitorManager   A shared pointer to the HSSVisitorManager.
         */
        void setVisitorManager(QSharedPointer<HSSVisitorManager> visitorManager);
        /**
         *  Getter for the shared pointer to the XML parser.
         *  @return A shared pointer to the XML parser.
         */
        QSharedPointer<XMLParser> xmlParser() const;
        /**
         *  Setter for the XMLParser that will be used to parse XML files.
         *  @param parser   A shared pointer to the XML parser.
         */
        void setXmlParser(QSharedPointer<XMLParser> parser);

        /**
         *  Getter for the shared pointer to the HSS parser.
         *  @return A shared pointer to the HSS parser.
         */
        QSharedPointer<HSSParser> hssParser() const;
        /**
         *  Setter for the XMLParser that will be used to parse HSS files.
         *  @param parser   A shared pointer to the HSS parser.
         */
        void setHssParser(QSharedPointer<HSSParser> parser);

        /**
         *  When a function name in HSS is checked for existence, this tells wether
         *  it corresponds to a custom function that has been registered on the core.
         *  @param name     A string containing the function name.
         *  @return Wether the string is registered as a custom function
         */
        bool isCustomFunction(const AXRString &name) const;

        /**
         *  Allows for registering a custom function name. @todo explain more
         *  @param name     A string containing the function name.
         *  @param fn       A pointer to the callback that encapsulates the
         *                  c++ function that will be called.
         */
        void registerCustomFunction(const AXRString &name, HSSCallback* fn);
        void evaluateCustomFunction(const AXRString &name, void* data);

        // From AXRWrapper
    public:
        /**
         *  Creates a AXRBuffer representation from the url you provide.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param url  A string containing the url to the file
         */
        virtual QSharedPointer<AXRBuffer> createBufferFromUrl(const QUrl &resourceUrl);
        virtual bool needsDisplay() const;
        /**
         *  This is to be called when something happens that needs to trigger a redraw.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param  newValue    A boolean containing the new state.
         */
        virtual void setNeedsDisplay(bool newValue);

        /**
         *  This creates a string containing a basic XML document with 1 element called "root".
         *  It is used when a HSS file is loaded directly.
         *  @return A shared pointer to the AXRBuffer representation of the basic XML document.
         */
        QSharedPointer<AXRBuffer> createDummyXml(const QUrl &hssUrl);
        /**
         *  Loads the XML file at the path you provide.
         *  @param  xmlfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadXmlFile(const QUrl &url);
        bool loadXmlFile(QSharedPointer<AXRBuffer> buffer);
        /**
         *  Loads the file you provide, and then handles it according to its file extension.
         *  @param  filepath    A string containing the path to the file on the local system, can be
         *                      either an XML or an HSS file.
         *
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadFileByPath(const QUrl &url);
        /**
         *  Loads the HSS file at the path you provide.
         *  @param  hssfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadHssFile(const QUrl &url);
        /**
         *  Reloads the file that is currently loaded.
         *  @return Wether it has been reloaded successfully or not.
         */
        bool reload();

        bool handleEvent(HSSInputEvent *event);

        // Layout stuff from AXRWrapper
    public:
        bool showLayoutSteps() const;
        void setShowLayoutSteps(bool value);
        void previousLayoutStep();
        void nextLayoutStep();
        void nextLayoutTick();
        void resetLayoutTicks();
        bool layoutStepDone() const;
        void nextLayoutChild();
        void resetLayoutChild();
        bool layoutChildDone() const;
        void breakIfNeeded();

        int windowWidth() const;
        int windowHeight() const;

        void setWindowSize(int width, int height);

    private:
        AXRDocumentPrivate *const d;
    };
}

#endif //AXR_H

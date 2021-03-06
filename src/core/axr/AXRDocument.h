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
#include "HSSParserReceiver.h"

template <class T> class QSharedPointer;

namespace AXR
{
    class AXRBuffer;
    class AXRController;
    class AXRPlatform;
    class HSSDisplayObject;
    class AXRDocumentDelegate;
    class AXRDocumentPrivate;
    class AXRExecutionFrame;
    class HSSAbstractValueChangedCallback;
    class HSSCodeParser;
    class HSSFunction;
    class HSSInputEvent;
    class HSSObject;
    class HSSPoint;
    class HSSRenderer;
    class HSSVisitorManager;
    class XMLParser;

    /**
     * @brief Represents a single AXR document that can be rendered. A document
     * may be composed of one or more files.
     */
    class AXR_API AXRDocument : public HSSParserReceiver
    {
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
         *  Returns the delegate object
         */
        AXRDocumentDelegate * delegate();

        /**
         *  Sets the delegate object
         */
        void setDelegate(AXRDocumentDelegate * delegate);

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
         *  Loads only one HSS file and runs it
         */
        void runHSS(const AXRString & url);
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
         *  @return returns the current XML file that is opened.
         */
        QSharedPointer<AXRBuffer> xmlFile() const;
        /**
         *  Sets the current XML file to be used as the base document.
         *  @param file     The AXRBuffer to store for later use.
         */
        void setXmlFile(QSharedPointer<AXRBuffer> file);
        /**
         *  @return the path to the directory in which 'file' is stored
         */
        AXRString directory() const;
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
        QSharedPointer<HSSCodeParser> hssParser() const;
        /**
         *  Setter for the XMLParser that will be used to parse HSS files.
         *  @param parser   A shared pointer to the HSS parser.
         */
        void setHssParser(QSharedPointer<HSSCodeParser> parser);
        
        AXRPlatform * platform() const;

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
        void registerCustomFunction(const AXRString &name, HSSAbstractValueChangedCallback* fn);
        void evaluateCustomFunction(const AXRString &name,  const QSharedPointer<HSSObject> theObj);

        bool needsBreak() const;
        void setNeedsBreak(bool value);
        bool needsBreakOnLine(long long line);
        bool needsBreakOnNextLine();
        void addBreakpointOnLine(long long line);
        void removeBreakpointOnLine(long long line);
        bool executesSingleStep() const;
        bool stepsIn() const;
        void setStepsIn(bool value);
        bool stepsOut() const;
        void setStepsOut(bool value);
        void setExecutesSingleStep(bool value);
        void addStackFrame(AXRExecutionFrame newFrame);
        void popStackFrame();
        bool stackFrameHasNext();
        void stackFrameGoToNext();
        void resumeExecution();
        void execute();
        void moveToLine(long long line);
        void moveToNextLine();
        void cleanBreakpointDecorations();

        /**
         *  Getter for the render visitor
         *  @return A pointer to the render visitor
         */
        HSSRenderer * getRenderVisitor() const;

        /**
         *  Creates a AXRBuffer representation from the url you provide.
         *  @param url  A string containing the url to the file
         */
        QSharedPointer<AXRBuffer> createBufferFromUrl(const HSSString &url);
        HSSString getDirectory(QSharedPointer<AXRBuffer> theFile) const;

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
        QSharedPointer<AXRBuffer> createDummyXml(const AXRString &hssUrl);
        /**
         *  Loads the XML file at the path you provide.
         *  @param  xmlfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadXmlFile(const AXRString &url);
        bool loadXmlFile(QSharedPointer<AXRBuffer> buffer);
        /**
         *  Loads the file you provide, and then handles it according to its file extension.
         *  @param  filepath    A string containing the path to the file on the local system, can be
         *                      either an XML or an HSS file.
         *
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadFileByPath(const HSSString &url);
        /**
         *  Loads the HSS file at the path you provide.
         *  @param  hssfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadHssFile(const AXRString &url);

        bool loadHssFile(QSharedPointer<AXRBuffer> buffer);
        void receiveParserEvent(HSSParserEvent eventType, QSharedPointer<HSSParserNode> node);

        /**
         *  Reloads the file that is currently loaded.
         *  @return Wether it has been reloaded successfully or not.
         */
        bool reload();

        bool handleEvent(HSSInputEvent *event);
        void handleSelection(HSSPoint thePoint);
        void selectionChanged(QSharedPointer<HSSDisplayObject> theDO);
        void startTimer(AXRString timerName, HSSFunction * timerFunction, unsigned int ms, bool repeats);
        void startTimerOnce(HSSFunction * timerFunction, unsigned int ms);
        void stopTimer(AXRString timerName);
        bool hasTimer(AXRString timerName);

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
        
        /**
         *  This allows to check if a string contains a function name
         *  or not.
         *  @param value        The identifier.
         *  @return Wether the identifier is a function.
         */
        virtual bool isFunction(AXRString value);

        /**
         *  This allows to check if a string contains a reserved keyword
         *  or not.
         *  @param value        The identifier.
         *  @return Wether the identifier is a function.
         */
        virtual bool isKeyword(AXRString value);

    private:
        AXRDocumentPrivate *const d;
    };
}

#endif //AXR_H

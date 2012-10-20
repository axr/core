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

/**
 *  @mainpage AXR Prototype Documentation Overview
 *
 *  Welcome to the documentation of the AXR prototype! It was automatically generated from the
 *  JavaDoc-style comment blocks that are in the header (.h) files, with a tool called Doxygen.
 *
 *  The goal of this document is to provide an overview of how the code is structured, how the
 *  different classes relate to each other and, in general terms, how text files containing
 *  source code is transformed into pixels on screen at the end.
 *
 *  Most of the code is C++, which was chosen because of wide interoperability and existence of
 *  libraries and resources, as well as the native speed and fine grained control when flipping
 *  bits a rendering engine needs.
 *
 *  We manage the code via GitHub, as well as the issues for bugs and stuff like that. If you'd
 *  like to contribute, you should create a GitHub account if you don't have one already and
 *  then click the "Fork" button on https://github.com/AXR/Prototype. You then need to download,
 *  or "clone" the repository to your computer (install git if you don't have it already).
 *  Execute the following commands in the command line:
 *
 *  cd path/to/directory
 *  git clone https://github.com/yourusername/Prototype.git
 *
 *  @section folders Diving into the folder structure
 *
 *  Let's have a look at what's inside the repository folder:
 *
 *  - src/core: this is the core library of the rendering engine
 *  - src/app: prototype application used to test AXE's functionality
 *  - tests: this is a bunch of XML and HSS files that were used when developing the library
 *
 *  In Xcode you can use Interface Builder and make a custom view to be of the AXRView class
 *  and interact with it directly.
 *
 *  Now let's focus on the core. In it we find the AXR.h file that contains the AXRDocument class,
 *  which encapsulates the library, and the following folders:
 *
 *  - axr: Here we find the guts of the library which is not related to one specific language
 *  but manage stuff across the rendering engine.
 *  - hss: In here we find the HSS parser, the parser nodes and all the object types.
 *  - xml: In here we find the XML parser
 *
 *  @section parsing Parsing the content and getting up and runnig
 *
 *  So you will probably be wondering, where do we start? This of course depends on how you
 *  are embedding it, but in a general sense, you will create an \a AXRDocument instance
 *  and then tell it to load a file by pathname. When the path is to a XML file, it gets the
 *  file and sets it on the core, then calls the run() method on it. If it is to an HSS file,
 *  it will create a dummy XML file on the fly, which contains only one element called "root".
 *
 *  So now let's jump to AXRDocument.cpp, and have a look at the run() method. It gives the XML
 *  file to the XML parser. We currently use Qt, and the file XMLParser.cpp parses the
 *  elements and adds them on axr/AXRController.cpp using enterElement(), addAttribute(),
 *  appendContentText() and exitElement().
 *
 *  When it finds an XML instruction it will parse it and if it is a link to a HSS stylesheet
 *  it calls loadSheetsAdd() on the controller to take note. Which brings us back to
 *  AXRDocument::run(), where we now pass all the stylesheets that were found to the HSS parser.
 *
 *  Similarly to how the XMl parser does, the hss/parsing/HSSParser.cpp calls rulesAdd(),
 *  parserTreeAdd(), and objectTreeAdd() on the controller, and then we loop over the found
 *  rules and recursively match them to the content tree. On each of the matched elements,
 *  setNeedsRereadRules(true) is called, so that before the next (and first) time we draw, the
 *  properties from the rules are passed into each "definition object setter" (those which look
 *  like setD<propertynamehere>()), where they will be stored according to the value type that
 *  is passed.
 *
 *  @section drawing Drawing on screen
 *
 *  Currently for drawing we are using Qt.
 *
 *  When the OS asks for drawing, it is the duty of the client app to tell the core to draw in a given
 *  rectangle (the current implementation still always redraws the whole screen, though), which will then
 *  get get to the AXRRender class. This one will check if the window size has changed, and if so change
 *  the width and height of the root object. It will then tell the root element to recursively read the
 *  "definition objects", which are the HSSParserNodes that hold the values (inside property definitions,
 *  which in turn are inside the rules that are associated with the elements) into the instance
 *  variables that hold the actual calculated values that will determine how and where the stuff is rendered.
 *
 *  Next, it tells it to recursively layout, which is a process in which display objects arrange the things
 *  that are inside, such as text in the case of HSSTextBlock elements or other display objects in the case of
 *  HSSContainer.
 *
 *  Then it will regenerate all the surfaces as needed and finally it will tell it to draw, also as a
 *  recursive operation, and finally composites all the individual surfaces of the objects into the final
 *  surface which is retrieved by the client and blitted onto the screen in an OS and toolkit-dependent manner.
 *
 *  In the process of drawing, each object type which draws (display objects) implements and handles
 *  all the properties inside to calculate the visual rendering, each generating bitmap surfaces for the
 *  background, borders, etc, and finally compositing it all together.
 *
 *  This is all done in a pretty straightforward an naïve way, and should receive quite a lot of facelifting
 *  and perfomance improvements in the near future.
 *
 *  @section diagram Diagram of the structure of the library
 *
 *  In the following diagram you can see a graphical representation of how the AXR core library is structured:
 *  @image html library-structure.jpg
 */

#ifndef AXRGLOBAL_H
#define AXRGLOBAL_H

#include <QtGlobal>
#include <QMetaType>

Q_DECLARE_METATYPE(bool*)
Q_DECLARE_METATYPE(unsigned int*)

#if defined(AXR_EXPORTS)
#define AXR_API Q_DECL_EXPORT
#elif defined(AXR_IMPORTS)
#define AXR_API Q_DECL_IMPORT
#else
#define AXR_API
#endif

#if defined(AXR_NO_DEPRECATED)
#define AXR_DEPRECATED
#define AXR_DEPRECATED_VARIABLE
#else
#define AXR_DEPRECATED Q_DECL_DEPRECATED
#define AXR_DEPRECATED_VARIABLE Q_DECL_VARIABLE_DEPRECATED
#endif

// define AXR_NO_DEPRECATED_ENUMS to restrict usage of deprecated enum members,
// and surround them with #ifndef AXR_NO_DEPRECATED_ENUMS, because the compiler
// doesn't let us deprecate enum members in the same manner as classes and
// class members

#define HSSFRAMEWORK_PROTOCOL "axr"

#endif

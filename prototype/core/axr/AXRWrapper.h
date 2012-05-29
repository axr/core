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
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2012/05/25
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 3
 *
 ********************************************************************/

#ifndef AXRWRAPPER_H
#define AXRWRAPPER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <stdio.h>
#include "AXRFile.h"
#include "errors/AXRError.h"

namespace AXR
{
    typedef unsigned AXRFileHandle; //FIXME: are we going to use this?
    
    class AXRCore;
    /**
     *  @brief This is the abstract superclass from with all OS-specific wrappers
     *  should inherit from.
     */
    class AXRWrapper
    {
    public:
        /**
         *  The shared pointer to the wrapper.
         */
        typedef boost::shared_ptr<AXRWrapper> p;
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
         *  Creates a AXRFile representation from the url you provide.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param url  A string containing the url to the file
         */
        virtual AXRFile::p getFile(std::string url) = 0;
        /**
         *  Reads the data in the file.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param theFile  A shared pointer to the file to be read.
         *  @return the size 
         */
        virtual size_t readFile(AXRFile::p theFile) = 0;
        /**
         *  Closes the file. To be called after the file has been read to the end.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param theFile  A shared pointer to the file to be closed.
         */
        virtual void closeFile(AXRFile::p theFile) = 0;
        /**
         *  The wrapper is responsible for handling the error (maybe show it in a console)
         *  Subclasses should override this method with the OS specific implementation.
         *  @param theError A shared pointer to the error that should be handled.
         */
        virtual void handleError(AXRError::p theError) = 0;
        /**
         *  Shows a dialog to choose a local file.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param filePath     A regular pointer to the string that will hold the path that was
         *                      selected by the user
         *  @return Wether a path has been selected or not.
         */
        virtual bool openFileDialog(std::string &filePath) = 0;
        /**
         *  This is to be called when something happens that needs to trigger a redraw.
         *  Subclasses should override this method with the OS specific implementation.
         *  @param  newValue    A boolean containing the new state.
         */
        virtual void setNeedsDisplay(bool newValue) = 0;
        /**
         *  Getter for the core.
         *  @return A shared pointer to the core object.
         */
        boost::shared_ptr<AXRCore> getCore();
        /**
         *  Setter for the core.
         *  param xcr   A shared pointer to the core object.
         */
        void setCore(boost::shared_ptr<AXRCore> xcr);
        /**
         *  This creates a string containing a basic XML document with 1 element called "root".
         *  It is used when a HSS file is loaded directly.
         *  @return A shared pointer to the AXRFile representation of the basic XML document.
         */
        AXRFile::p createDummyXML(std::string stylesheet);
        /**
         *  Shows a dialog asking for a file to load, and then opens it.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadFile();
        /**
         *  Loads the XML file at the path you provide.
         *  @param  xmlfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadXMLFile(std::string xmlfilepath);
        /**
         *  Loads the HSS file at the path you provide.
         *  @param  hssfilepath A string containing the path to the file on the local system.
         *  @return Wether it has been loaded successfully or not.
         */
        bool loadHSSFile(std::string hssfilepath);
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
        
        boost::unordered_map<AXRFileHandle, AXRFile::p> files;
        
    protected:
        boost::shared_ptr<AXRCore> core;
        
    private:
        bool _isHSSOnly;
        bool _hasLoadedFile;
    };
}

#endif

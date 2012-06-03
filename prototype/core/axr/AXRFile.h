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
 *      Revision: 4
 *
 ********************************************************************/

#ifndef AXRFILE_H
#define AXRFILE_H

#include <boost/shared_ptr.hpp>

namespace AXR {
    /**
     *  @brief This class encapsulates a representation of a file (.xml or .hss)
     *  to be used inside the parsers. While this class holds the buffer to the
     *  data, the actual loading of the data is up to the OS-specific wrapper.
     */
    class AXRFile
    {
    public:
        /**
         *  The shared pointer to a file.
         */
        typedef boost::shared_ptr<AXRFile> p;
        /**
         *  Creates a new instance of a file.
         */
        AXRFile();
        /**
         *  Destructor.
         */
        virtual ~AXRFile();
        /**
         *  Setter for fileName.
         *  @param value    A string containing the file name. Should include the extension.
         */
        void setFileName(std::string value);
        /**
         *  Getter for fileName
         *  @return A string containing the file name, including extension.
         */
        std::string getFileName();
        /**
         *  @return A textual representation of the file.
         */
        std::string toString();
        /**
         *  Call this to check if the file is marked as being at end of file.
         *  @return true if it is at the end of the file, false otherwise.
         */
        bool isAtEndOfFile();
        /**
         *  Sets the file to be at end of file or not.
         *  @param newValue A boolean containing the new status.
         */
        void setAtEndOfFile(bool newValue);
        
        /**
         *  @todo make these private and provide accessors
         */
        char * buffer;
        long int bufferSize;
        long int fileSize;
        std::string mimeType;
        std::string basePath;
        std::string extension;
        FILE * fileHandle;

    protected:
        std::string fileName;
        
    private:
        bool _atEndOfFile;
    };
}

#endif

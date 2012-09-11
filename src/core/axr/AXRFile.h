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

#ifndef AXRFILE_H
#define AXRFILE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <QByteArray>

namespace AXR
{

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
         *  Setter for the buffer where the contents of the file will be stored.
         *  @param buffer   A char * pointer to the buffer.
         */
        void setBuffer(const QByteArray &buffer);

        /**
         *  Getter for the buffer where the contents of the file will be stored.
         *  @return A char * pointer to the buffer.
         */
        QByteArray& getBuffer();

        /**
         *  Getter for the size of the file.
         *  @return A long int holding the size of the file.
         */
        int getFileSize();

        /**
         *  Getter for the length of the buffer.
         *  @return A long int holding the size of the buffer.
         */
        int getBufferSize();

        /**
         *  Setter for the base path. This + / + the filename form the full path to
         *  the file.
         *  @param path     A string containing the base path of the file.
         */
        void setBasePath(std::string path);

        /**
         *  Getter for the base path. This + / + the filename form the full path to
         *  the file.
         *  @return A string containing the base path of the file.
         */
        std::string getBasePath();

        /**
         *  Setter for the MIME type of the file.
         *  @param mime     A string containing the MIME type of the file.
         */
        void setMimeType(std::string mime);

        /**
         *  Getter for the MIME type of the file.
         *  @return A string containing the MIME type of the file.
         */
        std::string getMimeType();

        /**
         *  Setter for the file extension. The file name already includes the
         *  file extensions, so this is mostly used for file type checking.
         *  @param value    A string containing the extension of the file.
         */
        void setExtension(std::string value);

        /**
         *  Getter for the file extension. The file name already includes the
         *  file extensions, so this is mostly used for file type checking.
         *  @return A string containing the extension of the file.
         */
        std::string getExtension();

        /**
         *  Setter for the file handle, an opaque OS pointer type.
         *  @param handle   The file handle to be stored.
         */
        void setFileHandle(FILE * handle);

        /**
         *  Getter for the file handle, an opaque OS pointer type.
         *  @return The file handle to be stored or NULL if none.
         */
        FILE * getFileHandle();

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

    protected:
        std::string fileName;

    private:
        bool _atEndOfFile;

        QByteArray buffer;
        std::string mimeType;
        std::string basePath;
        std::string extension;
        FILE * fileHandle;
    };
}

#endif

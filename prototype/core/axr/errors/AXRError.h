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
 *      Revision: 2
 *
 ********************************************************************/

#ifndef AXRERROR_H
#define AXRERROR_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>

namespace AXR {
    /**
     *  @brief  This class, and it's subclass AXRWarning, are used for error handling in
     *  the rendering engine. They can be displayed using raise() or thrown.
     */
    class AXRError : public boost::enable_shared_from_this<AXRError>
    {
    public:
        /**
         *  The shared pointer to the error.
         */
        typedef boost::shared_ptr<AXRError>p;
        /**
         *  Creates a new instance of an error, simple mode.
         *  @param origin   The name of the class where the error happened.
         *  @param message  The error message to be shown.
         */
        AXRError(std::string origin, std::string message);
        /**
         *  Creates a new instance of an error, with information about the filename,
         *  the line and column where it happened.
         *  @param origin   The name of the class where the error happened.
         *  @param message  The error message to be shown.
         *  @param filename The path to the filename where the error happened.
         *  @param line     The line index (starting at 1) in the file where the error happened.
         *  @param column   The column index (starting at 1) in the line where the error happened.
         */
        AXRError(std::string origin, std::string message, std::string filename, int line, int column);
        /**
         *  Destructor for the error.
         */
        virtual ~AXRError();
        /**
         *  Call this if you want the message to be displayed. The actual implementation is
         *  handled by the OS-specific wrapper.
         */
        void raise();
        /**
         *  Print itself as a textual representation.
         *  @return A string containing the textual representation of the error.
         */
        virtual std::string toString();
        /**
         *  Getter for message.
         *  @return The message in this error.
         */
        std::string getMessage();
        
    protected:
        std::string origin;
        std::string message;
        
        std::string filename;
        int line;
        int column;
        
        bool in_file;
    };
}

#endif

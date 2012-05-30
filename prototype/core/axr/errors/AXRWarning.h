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

#ifndef AXRWARNING_H
#define AXRWARNING_H

#include "AXRError.h"

namespace AXR {
    /**
     *  @brief This is a subclass of AXRError, that should be used when something happened
     *  which isn't really an error, but still should be shown to the user somehow.
     */
    class AXRWarning : public AXRError
    {
    public:
        /**
         *  The shared pointer to the warning.
         */
        typedef boost::shared_ptr<AXRWarning>p;
        /**
         *  Creates a new instance of a warning, simple mode.
         *  @param origin   The name of the class where the warning originated.
         *  @param message  The warning message to be shown.
         */
        AXRWarning(std::string origin, std::string message);
        /**
         *  Creates a new instance of a warning, with information about the filename,
         *  the line and column where it originated.
         *  @param origin   The name of the class where the warning originated.
         *  @param message  The warning message to be shown.
         *  @param filename The path to the filename where the warning originated.
         *  @param line     The line index (starting at 1) in the file where the warning originated.
         *  @param column   The column index (starting at 1) in the line where the warning originated.
         */
        AXRWarning(std::string origin, std::string message, std::string filename, int line, int column);
        /**
         *  Destructor for the warning.
         */
        virtual ~AXRWarning();
        
        virtual std::string toString();
    };
}


#endif

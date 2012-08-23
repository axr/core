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
 ********************************************************************/

#ifndef AXR_XMLPARSEREXCEPTIONS_H
#define AXR_XMLPARSEREXCEPTIONS_H

#include <string>
#include "AXRGlobal.h"

namespace AXR {
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLParserException
    {
    public:
        XMLParserException(std::string filename);
        XMLParserException(std::string filename, int line, int column);
        
        std::string filename;
        int line;
        int column;
        virtual std::string toString();
    protected:
        bool _tracks_file_position;
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLUnexpectedEndOfSourceException : public XMLParserException
    {
    public:
        XMLUnexpectedEndOfSourceException(std::string filename, int line, int column);
        virtual std::string toString();
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLNoElementsException : public XMLParserException
    {
    public:
        XMLNoElementsException(std::string filename, int line, int column);
        virtual std::string toString();
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLUnknownProcessingInstructionException : public XMLParserException
    {
    public:
        XMLUnknownProcessingInstructionException(std::string filename, int line, int column);
        virtual std::string toString();
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLMalformedProcessingInstructionException : public XMLParserException
    {
    public:
        XMLMalformedProcessingInstructionException(std::string filename, int line, int column);
        virtual std::string toString();
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLUnknownSheetTypeException : public XMLParserException
    {
    public:
        XMLUnknownSheetTypeException(std::string filename, int line, int column);
        virtual std::string toString();
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLUnknownCallbackTypeException
    {
    public:
        XMLUnknownCallbackTypeException();
        virtual std::string toString();
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class AXR_DEPRECATED XMLControllerNotSetException : public XMLParserException
    {
    public:
        XMLControllerNotSetException(std::string filename);
        virtual std::string toString();
    };
}


#endif //AXR_XMLPARSEREXCEPTIONS_H

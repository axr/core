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
 *      Last changed: 2012/06/02
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 3
 *
 ********************************************************************/

#ifndef HSSOBJECTEXCEPTIONS_H
#define HSSOBJECTEXCEPTIONS_H


//#include <exception>
#include <string>

namespace AXR {
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class HSSUnknownObjectTypeException
    {
    public:
        HSSUnknownObjectTypeException(std::string type);
        virtual std::string toString();
        std::string type;
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class HSSUnknownValueTypeException
    {
    public:
        virtual std::string toString();
    };
    
    /**
     *  @brief This class is deprecated. Use AXRError instead.
     *
     *  @todo remove this, and fix any dependency
     */
    class HSSObjectNotFoundException
    {
    public:
        HSSObjectNotFoundException(std::string name);
        virtual std::string toString();
        std::string name;
    };
}

#endif

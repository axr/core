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
 *      Last changed: 2012/06/11
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 3
 *
 ********************************************************************/

#include "AXRError.h"
#include "../AXRDebugging.h"
#include <sstream>
#include "../../AXR.h"

using namespace AXR;

AXRError::AXRError(std::string origin, std::string message)
{
    this->origin = origin;
    this->message = message;
    this->in_file = false;
}

AXRError::AXRError(std::string origin, std::string message, std::string filename, int line, int column)
{
    this->origin = origin;
    this->message = message;
    this->filename = filename;
    this->line = line;
    this->column = column;
    this->in_file = true;
}

AXRError::~AXRError()
{
    
}

void AXRError::raise()
{
    AXRCore::tp & core = AXRCore::getInstance();
    AXRWrapper * wrapper = core->getWrapper();
    wrapper->handleError(this->shared_from_this());
}

std::string AXRError::toString()
{
    if (this->in_file) {
        std::ostringstream linnum;
        linnum << this->line;
        std::ostringstream colnum;
        colnum << this->column;
        return "Error: "+this->message+" in "+this->filename+" on line "+linnum.str()+" on column "+colnum.str();
    } else {
        return "Error: "+this->message;
    }
}

std::string AXRError::getMessage()
{
    return this->message;
}


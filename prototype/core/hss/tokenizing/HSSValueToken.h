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
 *      Last changed: 2011/10/09
 *      HSS version: 1.0
 *      Core version: 0.4
 *      Revision: 5
 *
 ********************************************************************/

#ifndef HSSVALUETOKEN_H
#define HSSVALUETOKEN_H

#include "HSSToken.h"
#include <boost/shared_ptr.hpp>

#define VALUE_TOKEN(thetoken) (boost::static_pointer_cast<HSSValueToken>(thetoken))

namespace AXR {
    class HSSValueToken: public HSSToken
    {
    public:
        typedef boost::shared_ptr<HSSValueToken> p;
        
        HSSValueToken(HSSTokenType type, std::string value, unsigned line, unsigned column);
        HSSValueToken(HSSTokenType type, char value, unsigned line, unsigned column);
        HSSValueToken(HSSTokenType type, double long value, unsigned line, unsigned column);
        std::string getString();
        double long getLong();
        bool equals(HSSTokenType otherType, std::string otherValue);
        bool equals(HSSTokenType otherType, double long otherValue);
        std::string toString();
        bool isNumeric();
        
    private:
        std::string stringValue;
        double long longValue;
    };
}


#endif
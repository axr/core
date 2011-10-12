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

#ifndef HSSVALUE_H
#define HSSVALUE_H

#include "HSSObject.h"
#include "HSSObjectExceptions.h"
#include <boost/shared_ptr.hpp>

namespace AXR {
    enum HSSValueType {
        HSSValueNumberInt,
        HSSValueNumberFloat,
        HSSValueString,
        HSSValueKeyword
    };
    
    class HSSValue : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSValue> p;
        
        HSSValue();
        HSSValue(long int value);
        HSSValue(long double value);
        HSSValue(std::string value);
        HSSValue(void * passNULL, std::string keyword);
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        HSSValueType getValueType();
        
        void setValue(long int value);
        void setValue(long double value);
        void setValue(std::string value);
        void setKWValue(std::string keyword);
        
        std::string getStringValue();
        long int getIntValue();
        long double getFloatValue();
        
    protected:
        HSSValueType valueType;
        std::string stringValue;
        long int intValue;
        long double floatValue;
    };
}

#endif
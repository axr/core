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

#ifndef HSSVALUE_H
#define HSSVALUE_H

#include "HSSObject.h"
#include "HSSObjectExceptions.h"
#include <boost/shared_ptr.hpp>

namespace AXR {
    
    /**
     *  @brief Value objects wrap other values, to be reused with an object name. Numbers, strings
     *  and keywords are prime candidates for this, since they are written literally, without any
     *  object related syntax.
     *  @see HSSValueType
     */
    class HSSValue : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSValue> p;
        /**
         *  Constructor for value objects. Defaults to integer type and 0 as the value.
         */
        HSSValue();
        /**
         *  Constructor for value objects, initialized to an integer of given value.
         *  @param value        A long int containing the new value.
         */
        HSSValue(long int value);
        /**
         *  Constructor for value objects, initialized to a floating point number of given value.
         *  @param value        A long double containing the new value.
         */
        HSSValue(long double value);
        /**
         *  Constructor for value objects, initialized to a string of given content.
         *  @param value        A std::string containing the new value.
         */
        HSSValue(std::string value);
        /**
         *  Constructor for value objects, initialized to the given type with given content.
         *  @param type         The type of value. The only two valid types are HSSValueString
         *                      and HSSValueKeyword.
         *  @param value        A std::string containing the new value.
         */
        HSSValue(HSSValueType type, std::string value);
        /**
         *  Copy constructor for value objects
         */
        HSSValue(const HSSValue & orig);
        /**
         *  Clones an instance of HSSValue and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSValue
         */
        p clone() const;
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        /**
         *  @return The value type of this object.
         */
        HSSValueType getValueType();
        /**
         *  Sets the value object to be of type HSSValueNumberInt and of given value.
         *  @param value        A long int containing the new value.
         */
        void setValue(long int value);
        /**
         *  Sets the value object to be of type HSSValueNumberFloat and of given value.
         *  @param value        A long double containing the new value.
         */
        void setValue(long double value);
        /**
         *  Sets the value object to be of type HSSValueString and of given value.
         *  @param value        A std::string containing the new value.
         */
        void setValue(std::string value);
        /**
         *  Sets the value object to be of type HSSValueKeyword and of given value.
         *  @param value        A std::string containing the new value.
         */
        void setKWValue(std::string keyword);
        /**
         *  Returns the value of the object as a string, converting it if needed.
         *  @return The value as a std::string.
         */
        std::string getStringValue();
        /**
         *  Returns the value of the object as an integer, converting it if needed.
         *
         *  @return The value as a long int.
         *
         *  @todo right now just works with ints, no conversion is happening yet
         */
        long int getIntValue();
        /**
         *  Returns the value of the object as a floating point number, converting it if needed.
         *
         *  @return The value as a long double.
         *
         *  @todo right now just works with floats, no conversion is happening yet
         */
        long double getFloatValue();
        
    protected:
        HSSValueType valueType;
        std::string stringValue;
        long int intValue;
        long double floatValue;
        
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif

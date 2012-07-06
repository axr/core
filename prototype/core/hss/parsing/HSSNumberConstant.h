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

#ifndef HSSNUMBERCONSTANT_H
#define HSSNUMBERCONSTANT_H

#include "HSSParserNode.h"
#include <boost/shared_ptr.hpp>

namespace AXR {
    
    /**
     *  @brief Parser node that represents a number.
     *
     *  Numbers, both integers and floating points are represented as a long double inside the 
     *  value property.
     */
    class HSSNumberConstant : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSNumberConstant> p;
        
        /**
         *  Creates a new instance of a number constant node which holds the given value.
         *  @param value    A long double containing the value for the constant.
         */
        HSSNumberConstant(long double value);
        
        /**
         *  Clones an instance of HSSNumberConstant and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSNumberConstant
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSNumberConstant();
        
        /**
         *  Setter for the value.
         *  @param newValue     A long double containing the new calculated value for this constant.
         */
        void setValue(long double newValue);
        
        /**
         *  Getter for the value.
         *  @return The calculated value of the constant.
         */
        long double getValue();
        
        std::string toString();
        
    protected:
        long double value;
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif

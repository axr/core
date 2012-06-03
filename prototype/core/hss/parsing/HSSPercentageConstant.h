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
 *      Last changed: 2012/06/03
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 6
 *
 ********************************************************************/

#ifndef HSSPERCENTAGECONSTANT_H
#define HSSPERCENTAGECONSTANT_H

#include "HSSParserNode.h"
#include <string>
#include <boost/shared_ptr.hpp>

namespace AXR {
    
    /**
     *  @brief Parser node that represents a percentage constant in the HSS source.
     */
    class HSSPercentageConstant : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSPercentageConstant> p;
        
        /**
         *  Creates a new instance of a percentage constant, with given value.
         *  @param  value   A string containing the name of the keyword.
         */
        HSSPercentageConstant(long double value);
        
        /**
         *  Clones an instance of HSSParserNode and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSParserNode.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSPercentageConstant();
        
        virtual std::string toString();
        
        /**
         *  Setter for the value of the percentage constant.
         *  @param newValue     A long double containing the numeric value of the percentage.
         */
        void setValue(long double newValue);
        
        /**
         *  Call this method when you need to calculate the actual resulting value.
         *  @param baseValue    A long double containing the number that corresponds to 100%.
         *  @return A long double containing the result of the calculation of the base value with the stored
         *  percentage value.
         */
        long double getValue(long double baseValue);
    protected:
        long double value;
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}



#endif

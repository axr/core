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
 *      Last changed: 2012/06/01
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 6
 *
 ********************************************************************/

#ifndef HSSKEYWORDCONSTANT_H
#define HSSKEYWORDCONSTANT_H

#include "HSSParserNode.h"
#include <boost/shared_ptr.hpp>

namespace AXR {
    
    /**
     *  @brief Parser node that represents a keyword constant in the HSS source.
     */
    class HSSKeywordConstant : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSKeywordConstant> p;
        
        /**
         *  Creates a new instance of a keyword constant, with given content.
         *  @param  value   A string containing the name of the keyword.
         */
        HSSKeywordConstant(std::string value);
        
        /**
         *  Clones an instance of HSSKeywordConstant and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSKeywordConstant.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSKeywordConstant();
        
        /**
         *  Setter for the value of the keyword constant.
         *  @param newValue     A string containing the new name of the keyword.
         */
        void setValue(std::string newValue);
        
        /**
         *  Getter for the value of the keyword constant.
         *  @return A string containing the name of the keyword.
         */
        std::string getValue();
        
        /**
         *  Prints itself as a textual representation, useful for loggin or introspection.
         *  @return A string containing a textual representation of the keyword constant.
         */
        std::string toString();
        
    protected:
        std::string value;
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}


#endif

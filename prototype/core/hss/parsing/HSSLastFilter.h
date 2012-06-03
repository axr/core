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
 *      Revision: 4
 *
 ********************************************************************/

#ifndef HSSLASTFILTER_H
#define HSSLASTFILTER_H

#include "HSSFilter.h"

namespace AXR {
    /**
     *  @brief Filters the selection down to the last item.
     */
    class HSSLastFilter : public HSSFilter
    {
    public:
        
        /**
         *  Creates a new instance of a last filter
         */
        HSSLastFilter();
        
        /**
         *  Clones an instance of HSSLastFilter and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLastFilter.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSLastFilter();
        
        /**
         *  Prints itself as a textual representation, useful for loggin or introspection.
         *  @return A string containing a textual representation of the keyword constant.
         */
        virtual std::string toString();
        
        virtual const std::vector<HSSDisplayObject::p> apply(const std::vector<HSSDisplayObject::p> &scope, bool negating);
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif

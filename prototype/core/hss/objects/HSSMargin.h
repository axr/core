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

#ifndef HSSMARGIN_H
#define HSSMARGIN_H

#include "HSSObject.h"
#include "HSSValue.h"
#include "HSSMultipleValue.h"
#include <boost/shared_ptr.hpp>

namespace AXR {
    
    /**
     *  @brief Abstract base class to provide common functionality for margin object types.
     *
     *  Margins are spaces around elements, that won't be occupied by other elements that
     *  are in flow.
     */
    class HSSMargin : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSMargin> p;
        
        /**
         *  Creates a new instance of a margin object. Don't use directly, this is only for 
         *  subclasses.
         */
        HSSMargin();
        
        /**
         *  Copy constructor for HSSMargin objects. Don't call directly, but use clone() instead.
         */
        HSSMargin(const HSSMargin & orig);
        
        /**
         *  Clones an instance of HSSMargin and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSMargin
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSMargin();
        
        /**
         *  @todo make private and provide accessors
         */
        HSSMultipleValue segments;
        
        virtual std::string toString();
        
        /**
         *  Setter for size.
         *  @param  newSize A HSSValue object encapsulating the new size.
         * 
         *  @todo this should take a shared pointer
         */
        void setSize(HSSValue newSize);
        
        /**
         *  Setter for size.
         *  @return A HSSValue object encapsulating the size.
         *
         *  @todo this should give a shared pointer
         */
        HSSValue getSize();
        
    protected:
        HSSValue size;
    private:
        HSSClonable::p cloneImpl() const;
    };
}

#endif

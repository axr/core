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
 *      Last changed: 2012/06/04
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 3
 *
 ********************************************************************/

#ifndef HSSSELFUNCTION_H
#define HSSSELFUNCTION_H

#import "HSSFunction.h"

namespace AXR {
    
    /**
     *  @brief Implementation of the sel(<selector chain>) function in HSS.
     *
     *  Sel functions allow the author of HSS documents to create selections that can
     *  be passed as a value to a property.
     */
    class HSSSelFunction : public HSSFunction
    {
    public:
        typedef boost::shared_ptr<HSSSelFunction> p;
        
        /**
         *  Creates a new instance of a sel function.
         */
        HSSSelFunction();
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSSelFunction();
        
        //see HSSObject.h for documentation of this method
        virtual std::string toString();
        
        /**
         *  Getter for the selector chain.
         *  @return A shared pointer to the selector chain.
         */
        const HSSSelectorChain::p & getSelectorChain() const;
        
        /**
         *  Setter for the selector chain.
         *  @param newValue     A shared pointer to the new selector chain.
         */
        void setSelectorChain(HSSSelectorChain::p newValue);
        
        /**
         *  This is the actual implementation of what the function does. It selects from the elements
         *  in the scope and then returns a selection: a pointer to a vector of vectors to shared pointers
         *  to display objects std::vector< std::vector<HSSDisplayObject::p> >.
         *  @return A pointer to a selection casted to void*.
         */
        virtual void * _evaluate();
        
        /**
         *  Calls _evaluate();
         *  @deprecated Do not use this.
         */
        virtual void * _evaluate(std::deque<HSSParserNode::p> arguments);
        
//        void valueChanged(HSSObservableProperty source, void*data);
        
    protected:
        HSSSelectorChain::p selectorChain;
        std::vector< std::vector<HSSDisplayObject::p> > selection;
    };
}

#endif

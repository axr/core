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
 *      Revision: 3
 *
 ********************************************************************/

#ifndef HSSLOG_H
#define HSSLOG_H

#include "HSSAction.h"

namespace AXR {
    
    /**
     *  @brief This action object type logs values to the console.
     *
     *  It can either log stuff directly 
     */
    class HSSLog : public HSSAction
    {
    public:
        
        /**
         *  Creates a new instance of a log action object.
         */
        HSSLog();
        
        /**
         *  Copy constructor for HSSLog objects. Don't call directly, use clone() instead. 
         */
        HSSLog(const HSSLog & orig);
        
        /**
         *  Clones an instance of HSSLog and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLog.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSLog();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        virtual void fire();
        
        /**
         *  Getter for the definition object of value.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        HSSParserNode::p getDValue();
        
        /**
         *  Setter for the definition object of value. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of value.
         */
        void setDValue(HSSParserNode::p);
        
        /**
         *  Method to be passed as callback when observing changes that will affect value.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(HSSObservableProperty source, void*data);
        
        
    private:
        HSSParserNode::p dValue;
        HSSObservable * observedValue;
        HSSObservableProperty observedValueProperty;

        HSSClonable::p cloneImpl() const;
    };
}


#endif

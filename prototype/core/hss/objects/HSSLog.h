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
 *      Last changed: 2012/03/25
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 2
 *
 ********************************************************************/

#ifndef HSSLOG_H
#define HSSLOG_H

#include "HSSAction.h"

namespace AXR {
    
    class HSSLog : public HSSAction
    {
    public:
        /**
         *  Constructor for HSSLog objects
         */
        HSSLog();
        /**
         *  Copy constructor for HSSLog objects
         */
        HSSLog(const HSSLog & orig);
        /**
         *  Clones an instance of HSSLog and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLog
         */
        p clone() const;
        virtual ~HSSLog();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        virtual void fire();
        
        //value
        HSSParserNode::p getDValue();
        void setDValue(HSSParserNode::p);
        void valueChanged(HSSObservableProperty source, void*data);
        
        
    private:
        HSSParserNode::p dValue;
        HSSObservable * observedValue;
        HSSObservableProperty observedValueProperty;

        HSSClonable::p cloneImpl() const;
    };
}


#endif

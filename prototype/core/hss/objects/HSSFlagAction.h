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
 *      Last changed: 2012/03/21
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSFLAGACTION_H
#define HSSFLAGACTION_H

#include "HSSAction.h"
#include "../parsing/HSSFlagFunction.h"

namespace AXR {
    
    class HSSFlagAction : public HSSAction
    {
    public:
        typedef boost::shared_ptr<HSSFlagAction> p;
        
        /**
         *  Constructor for HSSFlagAction objects
         */
        HSSFlagAction();
        /**
         *  Copy constructor for HSSFlagAction objects
         */
        HSSFlagAction(const HSSFlagAction & orig);
        /**
         *  Clones an instance of HSSFlagAction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFlagAction
         */
        p clone() const;
        virtual ~HSSFlagAction();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        
        virtual void fire();
        
        HSSFlagFunction::p getFlagFunction();
        void setFlagFunction(HSSFlagFunction::p newValue);
        
    private:
        HSSFlagFunction::p _flagFunction;
        virtual HSSClonable::p cloneImpl() const;
    };
}


#endif

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

#ifndef HSSRECTANGLE_H
#define HSSRECTANGLE_H

#include "HSSShape.h"

namespace AXR {
    
    /**
     *  @brief Shape object, representing the HSS object \@rect.
     */
    class HSSRectangle : public HSSShape
    {
    public:
        typedef boost::shared_ptr<HSSRectangle> p;
        
        /**
         *  Constructor for HSSRectangle objects
         */
        HSSRectangle();
        
        /**
         *  Copy constructor for HSSRectangle objects
         */
        HSSRectangle(const HSSRectangle & orig);
        
        /**
         
         *  Clones an instance of HSSRectangle and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRectangle
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSRectangle();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        virtual void draw(cairo_t * cairo, double long x, double long y, double long width, double long height);
        
    private:
        HSSClonable::p cloneImpl() const;
    };
}

#endif

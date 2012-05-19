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
 *      Last changed: 2012/03/15
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 2
 *
 ********************************************************************/

#ifndef AXRWRAPPER_H
#define AXRWRAPPER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <stdio.h>
#include "AXRFile.h"
#include "errors/AXRError.h"

namespace AXR
{
    typedef unsigned AXRFileHandle; //FIXME: are we going to use this?
    
    class AXRCore;
    
    class AXRWrapper
    {
    public:
        
        typedef boost::shared_ptr<AXRWrapper> p;
        
        AXRWrapper();
        virtual ~AXRWrapper();
        virtual AXRFile::p getFile(std::string url) = 0;
        virtual size_t readFile(AXRFile::p theFile) = 0;
        virtual void closeFile(AXRFile::p theFile) = 0;
        virtual void handleError(AXRError::p theError) = 0;
        virtual bool openFileDialog(std::string &filePath) = 0;
        virtual void setNeedsDisplay(bool newValue) = 0;
        
        boost::shared_ptr<AXRCore> getCore();
        void setCore(boost::shared_ptr<AXRCore> xcr);
        AXRFile::p createDummyXML(std::string stylesheet);
        
        bool loadFile();
        bool loadFileByPath(std::string filepath);
        bool loadXMLFile(std::string xmlfilepath);
        bool loadHSSFile(std::string hssfilepath);
        bool reload();
        bool hasLoadedFile();
        
        boost::unordered_map<AXRFileHandle, AXRFile::p> files;
        
    protected:
        boost::shared_ptr<AXRCore> core;
        
    private:
        bool _isHSSOnly;
        bool _hasLoadedFile;
    };
}

#endif

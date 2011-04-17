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
 *      Last changed: 2011/04/09
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 2
 *
 ********************************************************************/

#include "HSSInstruction.h"

using namespace AXR;

HSSInstruction::HSSInstruction(HSSInstructionType type)
{
    this->instructionType = type;
    this->type = HSSStatementTypeInstruction;
}


HSSInstruction::HSSInstruction(HSSInstructionType type, std::string value)
{
    this->instructionType = type;
    this->value = value;
    this->type = HSSStatementTypeInstruction;
}

HSSInstruction::~HSSInstruction()
{
    
}

std::string HSSInstruction::toString()
{
    std::string tempstr = "HSSInstruction of type: "+this->instructionStringRepresentation(this->instructionType);
    if (this->instructionType == HSSColorInstruction)
    {
        tempstr.append(" and value: "+this->value);
    }
    return tempstr;
}

std::string HSSInstruction::instructionStringRepresentation(HSSInstructionType type){
    std::string types[20];
	types[HSSNewInstruction] = "HSSNewInstruction";
	types[HSSEnsureInstruction] = "HSSEnsureInstruction";
	types[HSSImportInstruction] = "HSSImportInstruction";
	types[HSSColorInstruction] = "HSSColorInstruction";
    return types[type];
}
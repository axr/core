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
 ********************************************************************/

#ifndef HSSFILTERTYPE_H
#define HSSFILTERTYPE_H

/**
 *  @addtogroup typeEnums
 *  @{
 *  @enum HSSFilterType
 *  The type of the filter, specific for each subclass.
 */
enum HSSFilterType {
    //position
    HSSFilterTypeFirst, /**< Selects the first element in the selection. */
    HSSFilterTypeLast, /**< Selects the last element in the selection. */
    HSSFilterTypeEven, /**< Selects elements at even positions in the selection. */
    HSSFilterTypeOdd, /**< Selects elements at odd positions in the selection. */
    HSSFilterTypeIndex, /**< Deprecated. Use HSSFilterTypeNth instead. */
    HSSFiltertypeRange, /**< Deprecated. Use HSSFilterTypeNth instead. */
    HSSFilterTypeNth, /**< Selects elements at positions in the selection according to parameters. */
    //hierarchy
    HSSFilterTypeParent, /**< Deprecated. */
    HSSFilterTypeParentOf, /**< Deprecated. Use HSSFilterTypeHas instead. */
    HSSFilterTypeAncestorOf, /**< Deprecated. Use HSSFilterTypeHas instead. */
    HSSFilterTypeHas, /**< Selects element depending if the children or descendants match a given selector. */
    //interaction
    HSSFilterTypeHover, /**< Deprecated by flags. */
    HSSFilterTypePressed, /**< Deprecated by flags. */
    HSSFilterTypeDragged, /**< Deprecated by flags. */
    HSSFilterTypeDestination, /**< Deprecated by flags. */
    HSSFilterTypeTarget, /**< Deprecated by flags. */
    HSSFilterTypeFocused, /**< Deprecated by flags. */
    HSSFilterTypeBlurred, /**< Deprecated by flags. */
    HSSFilterTypeAnimated, /**< Deprecated by flags. */
    //text matching
    HSSFilterTypeContains, /**< Selects elements if the given text is found in the context text. */
    HSSFilterTypeEquals, /**< Selects elements if the given text is the same as the context text. */
    HSSFilterTypeStartsWith, /**< Selects elements if its content text starts with the given text. */
    HSSFilterTypeEndsWith, /**< Selects elements if its content text ends with the given text. */
    HSSFilterTypeMatch, /**< Selects elements if a given regular expression matches the content text. */
    HSSFilterTypeEmpty, /**< Selects elements if its content text is empty. */
    HSSFilterTypeLength, /**< Selects elements if its content text is as long as the given number. */
    //special
    HSSFilterTypeEach, /**< Splits the selection by element. */
    /**
     *  @todo HSSFilterTypeType (:type)
     */
    HSSFilterTypeCustom /**< Selects elements filtering according to a custom filter by the author. */
    
};
/** @} */

#endif

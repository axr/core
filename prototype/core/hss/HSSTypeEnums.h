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

#ifndef HSSTYPEENUMS_H
#define HSSTYPEENUMS_H

#include "AXRGlobal.h"

/** @defgroup typeEnums Type enumerations */

namespace AXR
{

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSParserNodeType
     *  The AST is generated using various types of parser nodes, each having a different meaning or use. This
     *  enum provides a way to identifying each node type, when they are processed.
     */
    enum HSSParserNodeType
    {
        HSSParserNodeTypeGeneric = 0, /**< Error state. */
        HSSParserNodeTypeSelector, /**< Generic type for all selector nodes. */
        HSSParserNodeTypeCombinator, /**< Defines relations between parts of the selector chain. */
        HSSParserNodeTypeFilter, /**< Reduces or alters the selection. */
        HSSParserNodeTypeStatement, /**< Basic execution block. */
        HSSParserNodeTypeExpression, /**< Mathematic operation between two values (or other expressions). */
        HSSParserNodeTypeNumberConstant, /**< A basic number in the HSS source. */
        HSSParserNodeTypePercentageConstant, /**< A percentage number in the HSS source. */
        HSSParserNodeTypeStringConstant, /**< A literal string (single or double quotes) in the HSS source. */
        HSSParserNodeTypeKeywordConstant, /**< A keyword literal in the HSS source. */
        HSSParserNodeTypeObjectNameConstant, /**< Used to get the object definition by name. */
        HSSParserNodeTypeFunctionCall, /**< Which function to call, and with what arguments. */
        HSSParserNodeTypeMultipleValueDefinition, /**< Encapsulates various values separated by commas. */
        HSSParserNodeTypeSelectorChain, /**< A group of selector nodes that define what elements will be selected. */
        HSSParserNodeTypeNegation, /**< Inverts the meaning of one or more selectors. */
        HSSParserNodeTypeFlag, /**< Conditionally applies properties to elements. */
        HSSParserNodeTypeRootSelector, /**< Returns the root element. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSExpressionType
     *  The type of the expression, specific for each subclass.
     */
    enum HSSExpressionType
    {
        HSSExpressionTypeNone = 0, /**< Error state. */
        HSSExpressionTypeSum, /**< A+B. */
        HSSExpressionTypeSubtraction, /**< A-B. */
        HSSExpressionTypeMultiplication, /**< A*B. */
        HSSExpressionTypeDivision, /**< A/B. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSStatementType
     *  The type of the statement, specific for each subclass.
     */
    enum HSSStatementType
    {
        HSSStatementTypeNone = 0, /**< Error state. */
        HSSStatementTypeRule, /**< Rules are used to apply property definitions to elements. */
        HSSStatementTypePropertyDefinition, /**< Define a value for a property name. */
        HSSStatementTypeObjectDefinition, /**< Define a reusable group of property definitions under a name. */
        HSSStatementTypeComment, /**< . */
        HSSStatementTypeInstruction /**< . */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSInstructionType
     *  The type of the instruction node. It will determine what the instruction will be used for.
     */
    enum HSSInstructionType {
        HSSInstructionTypeNone = 0, /**< Error state. */
        HSSNewInstruction, /**< This will cause a new element to be created when prepended in front of rules. */
        HSSEnsureInstruction, /**< This will cause a new element to be created if not already there,
                               when prepended in front of rules. */
        HSSImportInstruction, /**< Loads code from the given file and adds it to the current document. */
        HSSMoveInstruction, /**< Moves elements from one part of the tree to another. */
        HSSDeleteInstruction, /**< Removes elements from the content tree. */
        HSSGrayscale1Instruction, /**< Creates a color object (e.g. #C). */
        HSSGrayscale2Instruction, /**< Creates a color object (e.g. #CC). */
        HSSRGBInstruction, /**< Creates a color object (e.g. #CCC). */
        HSSRGBAInstruction, /**< Creates a color object (e.g. #CCCF). */
        HSSRRGGBBInstruction, /**< Creates a color object (e.g. #CCCCCC). */
        HSSRRGGBBAAInstruction /**< Creates a color object (e.g. #CCCCCCFF). */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSSelectorType
     *  Each type of selector node has its own type.
     */
    enum HSSSelectorType
    {
        HSSSelectorTypeNone = 0, /**< Error state. */
        HSSSelectorTypeNameSelector, /**< Selects elements based on the tag name. */
        HSSSelectorTypeUniversalSelector, /**< Selects all elements in a scope. */
        HSSSelectorTypeThisSelector, /**< Selects the nearest display object. */
        HSSSelectorTypeParentSelector, /**< Selects the parent element of the nearest display object. */
        HSSSelectorTypeSuperSelector, /**< Selects the element that owns the current one. */
        HSSSelectorTypeSubjectSelector, /**< Changes what will be selected by a selector chain ($). */
        HSSSelectorTypeCombinator, /**< Defines relations between parts of the selector chain. */
        HSSSelectorTypeSimpleSelector, /**< Represents a basic block of a selector chain. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSCombinatorType
     *  The type of the combinator which will determine what the behavior will be when used in AXRController.
     */
    enum HSSCombinatorType
    {
        HSSCombinatorTypeNone = 0, /**< Error state. */
        HSSCombinatorTypeSiblings, /**< Select elements on the same level (=) */
        HSSCombinatorTypePreviousSiblings, /**< Select elements on the same level that come before the previously selected elements (-) */
        HSSCombinatorTypeNextSiblings, /**< Select elements on the same level that come after the previously selected elements (+) */
        HSSCombinatorTypeChildren, /**< Select children of the previously selected elements (whitespace) */
        HSSCombinatorTypeDescendants, /**< Select descendants of the previously selected elements (..) */
#ifndef AXR_NO_DEPRECATED_ENUMS
        HSSCombinatorTypeAllDescendants, /**< Deprecated */
#endif
        HSSCombinatorTypeTextSelection /**< Switches the selection to text space */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSFilterType
     *  The type of the filter, specific for each subclass.
     */
    enum HSSFilterType {
        HSSFilterTypeNone = 0, /**< Error state. */

        //position
        HSSFilterTypeFirst, /**< Selects the first element in the selection. */
        HSSFilterTypeLast, /**< Selects the last element in the selection. */
        HSSFilterTypeEven, /**< Selects elements at even positions in the selection. */
        HSSFilterTypeOdd, /**< Selects elements at odd positions in the selection. */
#ifndef AXR_NO_DEPRECATED_ENUMS
        HSSFilterTypeIndex, /**< Deprecated. Use HSSFilterTypeNth instead. */
        HSSFiltertypeRange, /**< Deprecated. Use HSSFilterTypeNth instead. */
#endif
        HSSFilterTypeNth, /**< Selects elements at positions in the selection according to parameters. */
        //hierarchy
#ifndef AXR_NO_DEPRECATED_ENUMS
        HSSFilterTypeParent, /**< Deprecated. */
        HSSFilterTypeParentOf, /**< Deprecated. Use HSSFilterTypeHas instead. */
        HSSFilterTypeAncestorOf, /**< Deprecated. Use HSSFilterTypeHas instead. */
#endif
        HSSFilterTypeHas, /**< Selects element depending if the children or descendants match a given selector. */
        //interaction
#ifndef AXR_NO_DEPRECATED_ENUMS
        HSSFilterTypeHover, /**< Deprecated by flags. */
        HSSFilterTypePressed, /**< Deprecated by flags. */
        HSSFilterTypeDragged, /**< Deprecated by flags. */
        HSSFilterTypeDestination, /**< Deprecated by flags. */
        HSSFilterTypeTarget, /**< Deprecated by flags. */
        HSSFilterTypeFocused, /**< Deprecated by flags. */
        HSSFilterTypeBlurred, /**< Deprecated by flags. */
        HSSFilterTypeAnimated, /**< Deprecated by flags. */
#endif
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
        HSSFilterTypeCustom, /**< Selects elements filtering according to a custom filter by the author. */
        HSSFilterTypeFlag, /**< Flags are named bits that can be turned on and off, either by the system or by the user. */
        HSSFilterTypeFirstChild, /**< Selects the elements from the selection that are first child in each parent. */
        HSSFilterTypeLastChild, /**< Selects the elements from the selection that are last child in each parent. */
        HSSFilterTypeEvenChild, /**< Selects the elements from the selection that have even index in each parent. */
        HSSFilterTypeOddChild, /**< Selects the elements from the selection that have odd index in each parent. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSFunctionType
     *  The type of the function, specific for each subclass.
     */
    enum HSSFunctionType
    {
        HSSFunctionTypeNone = 0, /**< Error state. */
        HSSFunctionTypeRef, /**< References a value on a selected element. */
        HSSFunctionTypeSel, /**< Creates a selection of elements. */
        HSSFunctionTypeMin, /**< Returns the passed value, or the minimum if lower. */
        HSSFunctionTypeMax, /**< Returns the passed value, or the maxium if higher. */
        HSSFunctionTypeFloor, /**< Rounds a number down to the next integer. */
        HSSFunctionTypeCeil, /**< Rounds a number up to the next integer. */
        HSSFunctionTypeRound, /**< Rounds a number to the nearest integer. */
        HSSFunctionTypeFlag, /**< Activates the given flag on the selected elements. */
        HSSFunctionTypeUnflag, /**< Deactivates the given flag on the selected elements. */
        HSSFunctionTypeToggleFlag, /**< Toggles the given flag on the selected elements. */
        HSSFunctionTypeCustom, /**< A function whose name has been registered on the core with a callback. */
        HSSFunctionTypeAttr, /**< Returns a value from an attribute of an XML element. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSFlagFunctionType
     *  The type of the flagging function. It will determine what the function
     *  actually does.
     */
    enum HSSFlagFunctionType
    {
        HSSFlagFunctionTypeNone = 0, /**< Error state. */
        HSSFlagFunctionTypeFlag, /**< Always set the flag to active. */
        HSSFlagFunctionTypeUnflag, /**< Always set the flag to inactive. */
        HSSFlagFunctionTypeToggleFlag /**< When a flag is active, deactivate it, and viceversa. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSRuleState
     *  The status of the rule, to determine if the properties should be applied or not.
     */
    enum HSSRuleState
    {
        HSSRuleStateOff = 0, /**< Don't apply the contents of the rule. */
        HSSRuleStateOn = 1, /**< Do apply the contents of the rule. */
        HSSRuleStateActivate, /**< Set to ruleStateOn the next cycle. */
        HSSRuleStatePurge /**< Set to ruleStateOff on the next cycle. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSValueType
     *  The type of the value, to be able to determine the correct storage
     *  of the data.
     *
     *  @todo is this a good design? and if so, aren't object definitions, object
     *  names and functions missing?
     */
    enum HSSValueType {
        HSSValueNumberInt, /**< Long Integer */
        HSSValueNumberFloat, /**< Long double */
        HSSValueString, /**< std::string */
        HSSValueKeyword/**< std::string */
    };
    /** @}*/

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSObjectType
     *  The type of the object. Some types have specific sub-types that are
     *  specific to their kind, such as gradients or shapes. In those cases
     *  they all share a generic object type.
     */
    enum HSSObjectType
    {
        HSSObjectTypeNone = 0, /**< Error state. */
        HSSObjectTypeGeneric, /**< Abstract type for generic objects, probably error state. */
        HSSObjectTypeValue, /**< Values store values. */
        HSSObjectTypeMultipleValue, /**< Multiple values separated by commas passed to a property are wrapped in these. */
        HSSObjectTypeDisplayObject, /**< Abstract type for display objects, probably error state. */
        HSSObjectTypeContainer, /**< Containers can contain other display objects. */
        HSSObjectTypeTextBlock, /**< Each container has at least one text block. It's task is to render the content text. */
        HSSObjectTypeBorder, /**< All borders share this type */
        HSSObjectTypeMargin, /**< All margins share this type */
        HSSObjectTypeRgb, /**< Colors in RGB */
        HSSObjectTypeGradient, /**< All gradients share this type */
        HSSObjectTypeFont, /**< Not font files, but HSS @font objects */
        HSSObjectTypeFunction, /**< All functions share this type */
        HSSObjectTypeShape, /**< All shapes share this type */
        HSSObjectTypeEvent, /**< All events share this type */
        HSSObjectTypeAction, /**< All actions share this type */
        HSSObjectTypeColorStop, /**< Color stops are used inside gradients */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSShapeType
     *  The type of the shape object, specific for each subclass.
     */
    enum HSSShapeType {
        HSSShapeTypeNone = 0, /**< Error state. */
        HSSShapeTypeRectangle, /**< The default shape for all containers, a rectangle. */
        HSSShapeTypeRoundedRect, /**< A rectangle with rounded corners. */
        HSSShapeTypeCircle, /**< A circle (or ellipse if width !== height). */
        HSSShapeTypePolygon, /**< A regular polygon with all its sides of the same length. */
        HSSShapeTypeStar /**< A star, with parameterized number and angles of spikes. */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSRequestModeType
     *  What to do with the loaded content.
     */
    enum HSSRequestModeType
    {
        HSSRequestModeTypeNone = 0, /**< Error state. */
        HSSRequestModeTypeAuto, /**< Automatically determine what to do. */
        HSSRequestModeTypeReplace, /**< Replaces the target with the loaded content. */
        HSSRequestModeTypeWrap, /**<    Replaces the root of the document with the loaded one and adds
                                 the current content into the loaded one. */
        HSSRequestModeTypePrepend, /**< Inserts the loaded content into the target, before existing children. */
        HSSRequestModeTypeAppend, /**< Inserts the loaded content into the target, after existing children. */
        HSSRequestModeTypeReturn, /**< Just returns the content. */ ///@todo This is not clear, needs discussion.
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSEventType
     *  The type of the event object.
     */
    enum HSSEventType
    {
        HSSEventTypeNone = 0, /**< Error state. */
        HSSEventTypeLoad, /**< Fires when everything has loaded. */
        HSSEventTypeClick, /**< Fires when a mouse down and mouse up happen in fast succession. */
        HSSEventTypeDoubleClick, /**< Fires when clicked two times in fast succession. */
        HSSEventTypeTripleClick, /**< Fires when clicked three times in fast succession. */
        HSSEventTypeMouseDown, /**< Fires when a mouse button has been pressed. */
        HSSEventTypeMouseUp, /**< Fires when a mouse button has been released. */
        HSSEventTypeMouseOver, /**< Fires when the mouse pointer enters an area of the document. */
        HSSEventTypeMouseOut, /**< Fires when the mouse pointer leaves an area of the document. */
        HSSEventTypeMouseHold, /**< Fires when a mouse button has been pressed and held down for a while. */
        HSSEventTypeMouseMove, /**< Fires when the mouse pointer is moved over an area of the screen. */
        HSSEventTypeClickSecondary, /**< Fires when clicking with the secondary mouse button. */
        HSSEventTypeClickTertiary, /**< Fires when clicking with the third (if there) mouse button. */
        HSSEventTypeScroll /**< Fires when the sroll mechanism is activated on the mouse (e.g. scroll wheel). */
    };
    /** @} */

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSActionType
     *  The type of the action object, specific for each subclass.
     */
    enum HSSActionType
    {
        HSSActionTypeNone = 0, /**< Error state. */
        HSSActionTypeRequest, /**< Requests load another xml files into the current one. */
        HSSActionTypeSetProperty, /**< This will probably not be used, it's based on an old idea. */
        HSSActionTypeJavascript, /**< HSS object representation of a piece of JS code. */
        HSSActionTypeJsFunction,  /**< This will probably not be used, it's based on an old idea. */
        HSSActionTypeAlert,  /**< Shows an alert box (not implemented yet). */
        HSSActionTypeLog, /**< Logs a value (where it appears is specific to the wrapper. */
        HSSActionTypeFlag, /**< All flagging functions have their corresponding HSS object representation. */
        HSSActionTypeFunction /**< Custom functions registered on the core use this type.. */
    };
    /** @} */
}

#endif

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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef HSSCODEPARSER_H
#define HSSCODEPARSER_H

#include <vector>
#include "AXRGlobal.h"
#include "HSSTokenType.h"

template <class T> class QSharedPointer;

namespace AXR
{
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSParserContext
     *  The parsing context determines wether selector chains that start with a combinator
     *  get @this prepended automatically, wether rules can be read, etc
     */
    enum AXR_API HSSParserContext
    {
        HSSParserContextRoot, /**< When we are at the base level of the document (rules are allowed,selector chains starting with combinators are NOT prepended with @this). */
        HSSParserContextRule, /**< When we are inside of a rule (rules are allowed, selector chains starting with combinators are prepended with @this). */
        HSSParserContextContainer, /**< When we are inside of an object definition of type container (rules are allowed). */
        HSSParserContextObjectDefinition, /**< When we are inside an object definition of other type (no rules allowed). */
        HSSParserContextEvaluables /**< When we are inside a user defined function or other evaluable blocks (no selector chains allowed, no rules allowed). */
    };
    /** @} */

    class AXRBuffer;
    class AXRController;
    class HSSArgument;
    class HSSAssignment;
    class HSSBooleanConstant;
    class HSSCodeParserPrivate;
    class HSSComparison;
    class HSSParserReceiver;
    class HSSCombinator;
    class HSSExpression;
    class HSSFilter;
    class HSSFunctionCall;
    class HSSInstruction;
    class HSSNameSelector;
    class HSSObject;
    class HSSObjectDefinition;
    class HSSParserNode;
    class HSSPropertyDefinition;
    class HSSPropertyPath;
    class HSSRefFunction;
    class HSSRule;
    class HSSSelectorChain;
    class HSSSimpleSelector;
    class HSSToken;
    class HSSVarDeclaration;

    class AXR_API HSSCodeParser
    {
    public:
        /**
         *  Creates a new HSSCodeParser object.
         *  @param receiver     The object which will receive the notifications
         */
        HSSCodeParser(HSSParserReceiver * receiver);
        
        /**
         *  Destroys the object
         */
        virtual ~HSSCodeParser();
        
        /**
         *  Reset everything to default values, so that we can start
         *  from fresh.
         */
        void reset();
        
        /**
         *  @return tells wether a file has been loaded or not
         */
        bool isFileLoaded() const;
        
        /**
         *  @return returns the current AXRBuffer that is opened. This can be either
         *  an XML or an HSS file.
         */
        QSharedPointer<AXRBuffer> file() const;
        
        /**
         *  Sets the current AXRBuffer to be used as the base document.
         *  @param file     The AXRBuffer to store for later use.
         */
        void setFile(QSharedPointer<AXRBuffer> file);

        AXRController * controller();
        void setController(AXRController * controller);
        
        bool loadFile(QSharedPointer<AXRBuffer> file);
        
        /**
         *  Parses the file and builds up the document tree
         *  @param parser   A shared pointer to the HSS parser.
         */
        bool parseHssFile(QSharedPointer<AXRBuffer> buffer);
        
        bool parseNext();
        void parseIdentifierStatement();
        void setIgnoreTokenReadCalls(bool value);
        void readNextToken();
        void updateCurrentToken(QSharedPointer<HSSToken> theToken);
        void skipInvalidToken();
        void skipStringArgument();
        QSharedPointer<HSSParserNode> readStringArgument();
        bool atEndOfSource();
        void skip(HSSTokenType type);

        /**
         *  This will read the next token until a semicolon or a closing block are found. It will recursively
         *  call itself when reading an opening block, to balance them out.
         */
        void skipUntilEndOfStatement();
        
        QSharedPointer<HSSVarDeclaration> readVarDecl();
        bool isBoolean();
        bool isAssignment();
        bool isPropertyPath();
        QSharedPointer<HSSBooleanConstant> readBoolean();
        QSharedPointer<HSSAssignment> readAssignment(QSharedPointer<HSSToken> identifier);
        QSharedPointer<HSSAssignment> readAssignment(QSharedPointer<HSSPropertyPath> thePath);
        bool isComparison();
        const bool isComparisonSign() const;
        QSharedPointer<HSSComparison> readComparison(HSSString propertyName, QSharedPointer<HSSParserNode> leftNode);

        /**
         *  Shorthand for readPropertyDefinition(bool shorhandChecked, bool isShorthand), passing
         *  false to both parameters.
         *
         *  @return The property definition that was read.
         */
        QSharedPointer<HSSPropertyDefinition> readPropertyDefinition();

        /**
         *  Reads the property definition. If it is a shorthand, it gets the property name from the
         *  current object context. It also reads the value directly in the same function.
         *  @param shorthandChecked     For performance reasons, if we already checked the shorthand
         *                              we can avoid it to be checked again passing false to this parameter.
         *  @param isShorthand          If shorthand was checked, pass the value here.
         *
         */
        QSharedPointer<HSSPropertyDefinition> readPropertyDefinition(bool shorthandChecked, bool isShorthand);
        
        bool readVals(const QSharedPointer<HSSParserNode> &target, AXRString propertyName);
        bool readVals(const QSharedPointer<HSSParserNode> &target, AXRString propertyName, bool useComma, bool strict);
        QSharedPointer<HSSParserNode> readSingleVal(AXRString propertyName, bool &valid);
        QSharedPointer<HSSParserNode> readVal(AXRString propertyName, bool &valid);
        QSharedPointer<HSSParserNode> readIdentifierValue();
        const bool isExpressionSign() const;
        QSharedPointer<HSSExpression> readExpression(AXRString propertyName, QSharedPointer<HSSParserNode> leftNode);

        /**
         *  Reads a property path, with all its nodes.
         *
         *  @return The property path that was read.
         */
        QSharedPointer<HSSPropertyPath> readPropertyPath();
        QSharedPointer<HSSPropertyPath> readPropertyPath(bool firstNodeIsVariableName);

        /**
         *  Shorthand for readInstruction(bool preferHex), passing true to the parameter.
         *
         *  @return A shared pointer to the instruction that was read.
         */
        QSharedPointer<HSSInstruction> readInstruction();
        
        /**
         *  Reads the instruction, passing preferHex to the tokenizer. If it is a hex number, it
         *  reads a color instruction, and if it is a identifier it looks for the name and creates
         *  the corresponding one.
         *
         *  @param preferHex    If we give preference to hexadecimal values when reading numbers.
         *  @return A shared pointer to the instruction that was read.
         */
        QSharedPointer<HSSInstruction> readInstruction(bool preferHex);

        /**
         *  Converts the given instruction to an object definition, depending on the type. Right now
         *  only does color instructions.
         *
         *  @return A shared pointer to the object definition.
         */
        QSharedPointer<HSSObjectDefinition> getObjectFromInstruction(QSharedPointer<HSSInstruction> instruction);

        /**
         *  Reads the instruction, then the rule and then sets the instruction on the rule.
         *
         *  @return A shared pointer to the rule that was read.
         */
        QSharedPointer<HSSRule> readInstructionRule();

        /**
         *  Reads the selector chain and then the block, looping until the block is closed.
         *  For symbols it reads the child rule, for instructions it does the same but in the instruction rule variant.
         *  Since there is syntactic overlap, for identifiers, colons and object signs, it first
         *  checks if it is a property definition and if so, reads it, else it reads the child rule.
         */
        QSharedPointer<HSSRule> readRule();

        /**
         *  Reads a selector chain until the given token. For identifiers, it's a simple selector;
         *  for symbols it's either a combinator, the universal selector or the negator; colons can
         *  be filters or flags; and for the object sign, it checks if it is the \@super, \@parent or
         *  \@root. Also, after reading each part, it checks if the whitespace is a child selector and then
         *  adds it if needed.
         *
         *  @param stopOn   The token which marks the end of the selector chain. It will be the current token after
         *                  returning.
         *
         *  @return A vector of shared pointers to selector chains.
         */
        std::vector<QSharedPointer<HSSSelectorChain> > readSelectorChains(HSSTokenType stopOn);
        QSharedPointer<HSSSimpleSelector> readSimpleSelector();
        QSharedPointer<HSSNameSelector> readObjectSelector();
        bool isNegator();
        QSharedPointer<HSSFilter> readFilter();
        QSharedPointer<HSSCombinator> readCombinator();

        /**
         *  Shorthand for isCombinator(QSharedPointer<HSSToken> token), passing the current token.
         *  @return Wether the current token is a combinator.
         */
        bool isCombinator();

        /**
         *  If we are in a context that accepts combinators, =, -, +, . and > will return true.
         *  Whitespace is checked with isWhitespace().
         *  @param token    The token to check.
         *  @return Wether the given token is a combinator.
         */
        bool isCombinator(QSharedPointer<HSSToken> token);

        /**
         *  Wether the current whitespace token is a children combinator or just regular whitespace.
         *  This will peek forward in the token stream to be able to tell for sure.
         *  @return Wether the current token is a children combinator or regular whitespace.
         *  @warning This function assumes the current token to be whitespace.
         */
        bool isChildrenCombinator();

        /**
         *  Wether the current token marks the start of a property definition, either including the declaration
         *  of the name or as a shorthand. If the tokens match the form of a selector (e.g. with a filter), it
         *  continues peeking forward until we can tell for sure.
         */
        bool isPropertyDefinition(bool * isShorthand);

        /**
         *  If the current token is whitespace, it checks if it is a children combinator and if so returns it.
         *  @return A shared pointer to a children combinator or an empty pointer if not.
         */
        QSharedPointer<HSSCombinator> readChildrenCombinatorOrSkip();

        /**
         *  Reads the current token and returns the corresponding combinator object.
         *  @return A shared pointer to a combinator node or an empty pointer if the symbol is not a combinator.
         *  @warning This function assumes the current token to be whitespace.
         */
        QSharedPointer<HSSCombinator> readSymbolCombinator();

        /**
         *  Returns the name selector with the value of the current token.
         *  @param isNegating       Wether the name selector should be negated or not
         *  @return A shared pointer to the name selector.
         *  @warning This function assumes the current token to be an identifier.
         */
        QSharedPointer<HSSNameSelector> readNameSelector(bool isNegating);

        /**
         *  Reads an object definition, getting its type or getting its name from the current object context
         *  if it was not declared. If the object type is invalid, it will fall back to \@value.
         *  After getting its name, if there, it reads the block in a very similar fashion to how readRule() works,
         *  but allows to define child object definitions with the symbol "&".
         *
         *  @param propertyName     The name of the property we are reading, or an empty string if not in a
         *                          property definition.
         *  @return A shared pointer to the object definition that was read.
         *  @warning This function assumes the current token to be an object sign.
         */
        QSharedPointer<HSSObjectDefinition> readObjectDefinition(AXRString propertyName);

        /**
         *  Recursively adds the given object definition to the object tree on the controller as well as copying
         *  all its property definitions to the child rules.
         *
         *  @param objDef   A shared pointer to the object definition to be added.
         */
        void recursiveAddObjectDefinition(QSharedPointer<HSSObjectDefinition> objDef);
        
        /**
         *  Reads a reference using the shorthand version with dot notation.
         *  @return A shared pointer to the resulting reference function.
         */
        QSharedPointer<HSSRefFunction> readObjectPath();

        /**
         *  Checks if we are dealing with a object path.
         *  @return true if it is an object path and false otherwise.
         */
        bool isObjectPath();

        /**
         *  Reads an expression, or the value if it isn't one. If it contains parentheses it will recursively
         *  @return A shared pointer to the parser node that was read.
         */
        QSharedPointer<HSSParserNode> readExpression();
        
        /**
         *  The part that reads sums and subtractions. Do not call directly, use readExpression() instead.
         *  It will call readMultiplicativeExpression() for each member.
         *  @return A shared pointer to the parser node that was read.
         */
        QSharedPointer<HSSParserNode> readAdditiveExpression();
        
        /**
         *  The part that reads multiplications and divisions. Do not call directly, use readExpression() instead.
         *  It will call readBaseExpression() for each member.
         *  @return A shared pointer to the parser node that was read.
         */
        QSharedPointer<HSSParserNode> readMultiplicativeExpression();

        /**
         *  The part that reads a member of an expression. Do not call directly, use readExpression() instead.
         *  @return A shared pointer to the parser node that was read.
         */
        QSharedPointer<HSSParserNode> readBaseExpression();

        /**
         *  Wether we are reading a unary expression.
         *  @return true if it is a unary expression, false otherwise
         */
        bool isUnaryExpression();

        /**
         *  Read a expression with only one operand.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readUnaryExpression();

        /**
         *  Creates a flag and sets its name with the value of the current token.
         *  @return A shared pointer to the flag that was read.
         *  @warning This method assumes the current token to be an identifier
         */
        QSharedPointer<HSSParserNode> readFlag();
        
        /**
         *  If an identifier is read, and depending on the name in the current token, it creates the corresponding
         *  function, along with the parentheses and parameters.
         *  @return A shared pointer to the function node.
         *
         *  @todo why does this not return a HSSFunction pointer?
         */
        QSharedPointer<HSSParserNode> readFunction();
        
        /**
         *  Reads a function of type ref([<property> ]of <selector-chain>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readRefFunction();
        
        /**
         *  Reads a function of type sel(<selector-chain>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readSelFunction();
        
        /**
         *  Reads a function of type flag|unflag|addFlag| takeFlag(<flag name>[ of
         *  <selector-chain>]) | replaceFlag(<flag name>, <flag name2>[ of <selector-chain>]).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readFlagFunction();
        
        /**
         *  Reads a function of type attr(<attribute name>[ of <selector-chain>]).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readAttrFunction();
        
        /**
         *  Reads a function of type min(<minimum value>, <otherwise>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readMinFunction();
        
        /**
         *  Reads a function of type max(<maximum value>, <otherwise>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readMaxFunction();
        
        /**
         *  Reads a function of type ceil(<value>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readCeilFunction();
        
        /**
         *  Reads a function of type floor(<value>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readFloorFunction();
        
        /**
         *  Reads a function of type round(<value>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readRoundFunction();
        
        /**
         *  Reads a function of type log(<value>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readLogFunction();
        
        /**
         *  Reads a function of type override(<selector>) { <property defs> }.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readOverrideFunction();

        /**
         *  Reads a function of type startTimer(<id>, <function>, <ms>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readStartTimerFunction();

        /**
         *  Reads a function of type stopTimer(<id>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readStopTimerFunction();

        /**
         *  Reads a function of type toggleTimer(<id>, <function>, <ms>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readToggleTimerFunction();

        /**
         *  Read a function of type insert(<text>).
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readInsertFunction();
        
        /**
         *  Read a function of type if(<condition>) { <evaluables>; ... }.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readIfFunction();
        
        /**
         *  Read a function of type else [if(<condition>)] { <evaluables>; ... }.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readElseFunction();

        /**
         *  Read an user defined function(<arg1>, ...) { <evaluables>; ... }.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readUserDefinedFunction();
        
        bool isFunctionCall();

        /**
         *  Read a call to a user defined function.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSFunctionCall> readFunctionCall();
        QSharedPointer<HSSArgument> readArgument(bool & isValid);
        
        bool readEvaluables(QSharedPointer<HSSParserNode> target);
        
        /**
         *  Reads the return function, which tells an user defined function to
         *  use a value as the result of evaluating the function.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readReturnFunction();

        /**
         *  Reads a custom function.
         *  @return A shared pointer to the function node.
         */
        QSharedPointer<HSSParserNode> readCustomFunction();

        /**
         *  Removes the last element from the current object context.
         */
        void currentObjectContextRemoveLast();

        /**
         *  @return The size of the current object context stack.
         */
        size_t currentObjectContextSize() const;

        /**
         *  Puts the given object at the top new current object context stack.
         *  @param theObject        A shared pointer to the object that should be made the new object context.
         */
        void currentObjectContextAdd(QSharedPointer<HSSObject> theObject);
        
    private:
        HSSCodeParserPrivate *const d;
    };
}
#endif /* HSSCODEPARSER_H */

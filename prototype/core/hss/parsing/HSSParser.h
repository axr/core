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
 *      COPYRIGHT: ©2012 - All Rights Reserved
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

#ifndef HSSPARSER_H
#define HSSPARSER_H

#include <stack>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include "AXRFile.h"
#include "HSSParserNodes.h"
#include "HSSObjects.h"
#include "HSSSelectorNodes.h"
#include "HSSTokenizer.h"

#define HSSFRAMEWORK_PROTOCOL "axr://"
#define HSSFRAMEWORK_PROTOCOL_LEN 6

namespace AXR
{

    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSParserContext
     *  This is meant to identify the current parsing context, although it has not been
     *  fully used yet, only in one or to places in the parser. This will probably useful
     *  in the future for better error handling and stuff like that.
     */
    enum HSSParserContext
    {
        HSSParserContextRoot, /**< When we are at the base level of the document. */
        HSSParserContextBlock, /**< When we are inside of a rule. */
        HSSParserContextObjectDefinition, /**< When we are inside an object definition. */
        HSSParserContextExpression, /**< When we are reading an expression. */
        HSSParserContextSelectorChain /**< When we are reading a selector chain. */
    };
    /** @} */

    class AXRController;
    class AXRWrapper;

    /**
     *  @brief This class reads an HSS file and, with the help of HSSTokenizer, converts
     *  the code into a tree of parser nodes.
     *
     *  This is one of the fundamental pieces of the system. When loadFile() is called it
     *  places the tokenizer at the beginning of the file and then reads through it calling
     *  readNextStatement() until done, processing each statement as needed, depending on
     *  what type it is (e.g. adds the rule to the controller).
     */
    class HSSParser
    {
    public:

        /**
         *  @todo remove this friend and fix access control
         */
        friend class AXRController;

        /**
         *  The shared pointer to instances of this class.
         */
        typedef boost::shared_ptr<HSSParser> p;

        /**
         *  Creates a new instance of a parser object, linking it to the given controller and wrapper.
         *  It will create a new instance of a HSSTokenizer automatically, and set the current context to root.
         *
         *  @param theController    A regular pointer to the controller associated to this parser.
         *  @param wrapper          A regular pointer to the wrapper associated to this parser.
         */
        HSSParser(AXRController * theController, AXRWrapper * wrapper);

        /**
         *  Destructor for this class. Clears the currentObjectContext stack and the loaded files.
         */
        ~HSSParser();

        /**
         *  Reset the parser to initial values, for example when reloading a file.
         */
        void reset();

        /**
         *  If a file is not already loaded, this will set the file on the tokenizer, initialize it
         *  to the beginning of the file and then, until done, loop over all statements found
         *  in the document.
         *
         *  @param  file    A shared pointer to the AXRFile representing the HSS file.
         */
        bool loadFile(AXRFile::p file);

        /**
         *  Reads the next statement in the document. This is called over and over again by loadFile().
         *  At this stage, we look at the current token and determine what kind of statement to create
         *  based on that. For color instructions, it creates a new object definition on the fly; for
         *  object signs, it reads the object definition; for identifiers, * and combinator symbols,
         *  it reads a rule; and comments are encapsulated and returned.
         *
         *  For a rule, it adds it to the rules list on the controller; for an object defnition it
         *  recursively adds it  to the object tree on the controller as well as copying all the property
         *  definitions from the parent to the child rule; comments are added to the parser node tree, but
         *  skipped otherwise; and for instructions, the only allowed statement instruction is #import which
         *  will be executed immediately.
         *
         *  @return Wether we arrived at the end of file or not
         */
        bool readNextStatement();

        /**
         *  Reads the selector chain and then the block, looping until the block is closed.
         *  For symbols it reads the child rule, for instructions it does the same but in the instruction rule variant.
         *  Since there is syntactic overlap, for identifiers, colons and object signs, it first
         *  checks if it is a property definition and if so, reads it, else it reads the child rule.
         */
        HSSRule::p readRule();

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
        std::vector<HSSSelectorChain::p> readSelectorChains(HSSTokenType stopOn);
        HSSSimpleSelector::p readSimpleSelector();
        HSSNameSelector::p readObjectSelector();
        bool isNegator();
        HSSFilter::p readFilter();
        HSSCombinator::p readCombinator();

        /**
         *  Shorthand for isCombinator(HSSToken::p token), passing the current token.
         *  @return Wether the current token is a combinator.
         */
        bool isCombinator();

        /**
         *  If we are in a context that accepts combinators, =, -, +, . and > will return true.
         *  Whitespace is checked with isWhitespace().
         *  @param token    The token to check.
         *  @return Wether the given token is a combinator.
         */
        bool isCombinator(HSSToken::p token);

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
         *  continues peeking forward untill we can tell for sure.
         */
        bool isPropertyDefinition(bool * isShorthand);

        /**
         *  If the current token is whitespace, it checks if it is a children combinator and if so returns it.
         *  @return A shared pointer to a children combinator or an empty pointer if not.
         */
        HSSCombinator::p readChildrenCombinatorOrSkip();

        /**
         *  Reads the current token and returns the corresponding combinator object.
         *  @return A shared pointer to a combinator node or an empty pointer if the symbol is not a combinator.
         *  @warning This function assumes the current token to be whitespace.
         */
        HSSCombinator::p readSymbolCombinator();

        /**
         *  Returns the name selector with the value of the current token.
         *  @param isNegating       Wether the name selector should be negated or not
         *  @return A shared pointer to the name selector.
         *  @warning This function assumes the current token to be an identifier.
         */
        HSSNameSelector::p readNameSelector(bool isNegating);

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
        HSSObjectDefinition::p readObjectDefinition(std::string propertyName);

        /**
         *  Recursively adds the given object definition to the object tree on the controller as well as copying
         *  all its property definitions to the child rules.
         *
         *  @param objDef   A shared pointer to the object definition to be added.
         */
        void recursiveAddObjectDefinition(HSSObjectDefinition::p objDef);

        /**
         *  Shorthand for readPropertyDefinition(bool shorhandChecked, bool isShorthand), passing
         *  false to both parameters.
         *
         *  @return The property definition that was read.
         */
        HSSPropertyDefinition::p readPropertyDefinition();

        /**
         *  Reads the property definition. If it is a shorthand, it gets the property name from the
         *  current object context. It also reads the value directly in the same function.
         *  @param shorthandChecked     For performance reasons, if we already checked the shorthand
         *                              we can avoid it to be checked again passing false to this parameter.
         *  @param isShorhand           If shorthand was checked, pass the value here.
         *
         *  @todo should reading values go into a separate function, maybe called readValue()?
         */
        HSSPropertyDefinition::p readPropertyDefinition(bool shorthandChecked, bool isShorthand);

        /**
         *  Shorthand for readInstruction(bool preferHex), passing true to the parameter.
         *
         *  @return A shared pointer to the instruction that was read.
         */
        HSSInstruction::p readInstruction();

        /**
         *  Reads the instruction, passing preferHex to the tokenizer. If it is a hex number, it
         *  reads a color instruction, and if it is a identifier it looks for the name and creates
         *  the corresponding one.
         *
         *  @param preferHex    If we give preference to hexadecimal values when reading numbers.
         *  @return A shared pointer to the instruction that was read.
         */
        HSSInstruction::p readInstruction(bool preferHex);

        /**
         *  Converts the given instruction to an object definition, depending on the type. Right now
         *  only does color instructions.
         *
         *  @return A shared pointer to the object definition.
         */
        HSSObjectDefinition::p getObjectFromInstruction(HSSInstruction::p instruction);

        /**
         *  Reads the instruction, then the rule and then sets the instruction on the rule.
         *
         *  @return A shared pointer to the rule that was read.
         */
        HSSRule::p readInstructionRule();

        /**
         *  Reads an expression, or the value if it isn't one. If it contains parentheses it will recursively
         *  @return A shared pointer to the parser node that was read.
         */
        HSSParserNode::p readExpression();

        /**
         *  The part that reads sums and subtractions. Do not call directly, use readExpression() instead.
         *  It will call readMultiplicativeExpression() for each member.
         *  @return A shared pointer to the parser node that was read.
         */
        HSSParserNode::p readAdditiveExpression();

        /**
         *  The part that reads multiplications and divisions. Do not call directly, use readExpression() instead.
         *  It will call readBaseExpression() for each member.
         *  @return A shared pointer to the parser node that was read.
         */
        HSSParserNode::p readMultiplicativeExpression();

        /**
         *  The part that reads a member of an expression. Do not call directly, use readExpression() instead.
         *  @return A shared pointer to the parser node that was read.
         */
        HSSParserNode::p readBaseExpression();

        /**
         *  Creates a filter and sets its name with the value of the current token.
         *  @return A shared pointer to the filter that was read.
         *  @warning This method assumes the current token to be an identifier
         */
        //HSSParserNode::p readFilter();

        /**
         *  Creates a flag and sets its name with the value of the current token.
         *  @return A shared pointer to the flag that was read.
         *  @warning This method assumes the current token to be an identifier
         */
        HSSParserNode::p readFlag();

        /**
         *  If an identifier is read, and depending on the name in the current token, it creates the corresponding
         *  function, along with the parentheses and parameters.
         *  @return A shared pointer to the function node.
         *
         *  @todo why does this not return a HSSFunction pointer?
         */
        HSSParserNode::p readFunction();

        /**
         *  Shorthand for readNextToken(bool checkForUnexpectedEndOfSource) passing false to the parameter.
         */
        void readNextToken();

        /**
         *  Get the next token from the tokenizer. It will also update the line and columns we are at.
         *  @param checkForUnexpectedEndOfSource    Wether to check and throw an error on unexpected end of file.
         */
        void readNextToken(bool checkForUnexpectedEndOfSource);

        /**
         *  @return Wether we are at the end of the file or not.
         */
        bool atEndOfSource();

        /**
         *  Check if we are at the end of source and if so, throw an error.
         */
        void checkForUnexpectedEndOfSource();

        /**
         *  Shorthand for skipExpected(HSSTokenType, bool) passing false to the last parameter.
         *  @param type     The token type to expect.
         */
        void skipExpected(HSSTokenType type);

        /**
         *  Checks if the type of the current token is of the given type, and if not, throws an error,
         *  otherwise reads the next token.
         *  @param type                             The token type to check against.
         *  @param checkForUnexpectedEndOfSource    Wether to check and throw an error on unexpected end of file.
         */
        void skipExpected(HSSTokenType type, bool checkForUnexpectedEndOfSource);

        /**
         *  Shorthand for skipExpected(HSSTokenType, std::string, false) passing false to the last parameter.
         *  @param type     The token type to expect.
         *  @param type     The string value to expect.
         */
        void skipExpected(HSSTokenType type, std::string value);

        /**
         *  Check if the current token matches the given type and value.
         *  @param type                             The token type to expect (not used).
         *  @param type                             The string value to expect.
         *  @param checkForUnexpectedEndOfSource    Wether to check and throw an error on unexpected end of file.
         *
         *  @warning this method assumes the current token to be a value token.
         */
        void skipExpected(HSSTokenType type, std::string value, bool checkForUnexpectedEndOfSource);

        /**
         *  Shorthand for skip(HSSTokenType, bool), passing false as the last parameter.
         *  @param type The token type to skip.
         */
        void skip(HSSTokenType type);

        /**
         *  If the current token is of given type, read the next token.
         *  @param type                             The token type to skip.
         *  @param checkForUnexpectedEndOfSource    Wether to check and throw an error on unexpected end of file.
         */
        void skip(HSSTokenType type, bool checkForUnexpectedEndOfSource);

        /**
         *  This will read the next token until a semicolon or a closing block are found. It will recursively
         *  call itself when reading an opening block, to balance them out.
         */
        void skipUntilEndOfStatement();

        /**
         *  Throws an error if the current token is not of the given type.
         *  @param type     The token type to compare against.
         */
        void expect(HSSTokenType type);

        /**
         *  Removes the last element from the current object context.
         */
        void currentObjectContextRemoveLast();

        /**
         *  @return The size of the current object context stack.
         */
        unsigned int currentObjectContextSize();

        /**
         *  Puts the given object at the top new current object context stack.
         *  @param theObject        A shared pointer to the object that should be made the new object context.
         */
        void currentObjectContextAdd(HSSObject::p theObject);

        /**
         *  Setter for the base path. When reading a file, we store the path to the folder that contains it, to be
         *  able to resolve relative paths.
         *  @param value    A string containing the base path.
         */
        void setBasePath(std::string value);

        /**
         *  Getter for the base path. When reading a file, we store the path to the folder that contains it, to be
         *  able to resolve relative paths.
         *  @return A string containing the base path.
         */
        std::string getBasePath();

        HSSParserNode::p readValue(std::string propertyName, bool &valid);

    private:
        HSSTokenizer::p tokenizer;
        //weak pointer
        AXRController * controller;
        AXRWrapper * wrapper;

        HSSToken::p currentToken;
        std::vector<HSSParserContext> currentContext;
        std::stack<HSSObject::p> currentObjectContext;

        unsigned line;
        unsigned column;

        std::string basepath;
        AXRFile::p currentFile;

        boost::unordered_set<AXRFile::p> loadedFiles;

        HSSObject::p _genericContext;
        std::string _lastObjectType;

    };
}

#endif

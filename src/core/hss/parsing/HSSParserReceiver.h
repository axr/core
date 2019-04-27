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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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

#ifndef HSSParserReceiver_h
#define HSSParserReceiver_h

#include "AXRGlobal.h"

template <class T> class QSharedPointer;

namespace AXR
{
    enum AXR_API HSSParserEvent
    {
        HSSParserEventWhitespace,
        HSSParserEventComment,
        HSSParserEventStatement,
        HSSParserEventRule,
        HSSParserEventNameSelector,
        HSSParserEventUniversalSelector,
        HSSParserEventPropertyPath,
        HSSParserEventColor,
        HSSParserEventInstruction,
        HSSParserEventBooleanConstant,
        HSSParserEventNumberConstant,
        HSSParserEventPercentageConstant,
        HSSParserEventStringConstant,
        HSSParseEventKeywordConstant,
        HSSParserEventVar,
        HSSParserEventVariableDeclaration,
        HSSParserEventVariableName,
        HSSParserEventObjectDefinition,
        HSSParserEventObjectType,
        HSSParserEventObjectName,
        HSSParserEventFilterName,
        HSSParserEventFlagName,
        HSSParserEventFunctionName,
        HSSParserEventFunctionModifier,
        HSSParserEventAssignment,
        HSSParserEventOther,
        HSSParserEventInvalid
    };
    
    class HSSParserNode;
    
    class AXR_API HSSParserReceiver
    {
    public:
        virtual void receiveParserEvent(HSSParserEvent eventType, QSharedPointer<HSSParserNode> node) = 0;
    };
}

#endif /* HSSParserReceiver_h */

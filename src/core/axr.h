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

#ifndef AXR_H
#define AXR_H

#include "utf8.h"

#include "AXRGlobal.h"
#include "HSSTypeEnums.h"

#include "AXRBuffer.h"
#include "HSSChar.h"
#include "HSSString.h"
#include "HSSPoint.h"
#include "HSSRect.h"
#include "HSSSize.h"

#include "HSSObservableProperties.h"
#include "HSSObservableMapping.h"
#include "HSSObservable.h"

#include "HSSClonable.h"
#include "HSSCallback.h"

#include "HSSAbstractVisitor.h"
#include "HSSVisitorManager.h"

#include "AXRError.h"
#include "AXRWarning.h"
#include "AXRLoggerEnums.h"
#include "AXRAbstractLogger.h"
#include "AXRGenericLogger.h"
#include "AXRLoggerManager.h"
#include "AXRPlatform.h"
#include "AXRPlatformDriver.h"

#include "HSSInputEvent.h"
#include "HSSMouseEvent.h"
#include "HSSKeyboardEvent.h"

#include "HSSTokenType.h"
#include "HSSToken.h"
#include "HSSStringToken.h"
#include "HSSTokenizer.h"
#include "HSSSelection.h"
#include "HSSSimpleSelection.h"
#include "HSSMultipleSelection.h"

#include "HSSParserNode.h"
#include "HSSPropertyPathNode.h"
#include "HSSPropertyPath.h"
#include "HSSFunctionCall.h"

#include "HSSMultipleValueDefinition.h"

#include "HSSVarDeclaration.h"
#include "HSSAssignment.h"
#include "HSSComparison.h"
#include "HSSNegatedVal.h"

#include "HSSSelectorChain.h"
#include "HSSSelector.h"
#include "HSSCombinator.h"
#include "HSSNegation.h"
#include "HSSNameSelector.h"
#include "HSSSimpleSelector.h"
#include "HSSUniversalSelector.h"
#include "HSSThisSelector.h"
#include "HSSParentSelector.h"
#include "HSSRootSelector.h"
#include "HSSEventSelector.h"
#include "HSSCommentNode.h"
#include "HSSInvalidNode.h"
#include "HSSSymbolNode.h"
#include "HSSWhitespaceNode.h"
#include "HSSNumberConstant.h"
#include "HSSBooleanConstant.h"
#include "HSSKeywordConstant.h"
#include "HSSPropertyNameConstant.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSStringConstant.h"

#include "HSSStatement.h"
#include "HSSComment.h"
#include "HSSInstruction.h"
#include "HSSObjectDefinition.h"
#include "HSSPropertyDefinition.h"
#include "HSSRule.h"

#include "HSSExpression.h"
#include "HSSSum.h"
#include "HSSSubtraction.h"
#include "HSSMultiplication.h"
#include "HSSDivision.h"
#include "HSSUnarySum.h"
#include "HSSUnarySubtraction.h"
#include "HSSComparison.h"

#include "HSSFilter.h"
#include "HSSFlag.h"
#include "HSSEmptyFilter.h"
#include "HSSFirstFilter.h"
#include "HSSLastFilter.h"
#include "HSSEvenFilter.h"
#include "HSSOddFilter.h"
#include "HSSFirstChildFilter.h"
#include "HSSLastChildFilter.h"
#include "HSSEvenChildFilter.h"
#include "HSSOddChildFilter.h"

#include "HSSFunction.h"
#include "HSSTargetedFunction.h"
#include "HSSLogFunction.h"
#include "HSSSelFunction.h"
#include "HSSRefFunction.h"
#include "HSSFlagFunction.h"
#include "HSSAttrFunction.h"
#include "HSSOverrideFunction.h"
#include "HSSStartTimerFunction.h"
#include "HSSStopTimerFunction.h"
#include "HSSToggleTimerFunction.h"
#include "HSSInsertFunction.h"
#include "HSSIfFunction.h"
#include "HSSEvaluableFunction.h"
#include "HSSArgument.h"
#include "HSSFunctionFunction.h"
#include "HSSReturnFunction.h"
#include "HSSCase.h"
#include "HSSSwitchFunction.h"
#include "HSSCustomFunction.h"

#include "HSSTextEnums.h"
#include "HSSObject.h"
#include "HSSValue.h"
#include "HSSMultipleValue.h"
#include "HSSRgb.h"
#include "HSSColorStop.h"
#include "HSSGradient.h"
#include "HSSLinearGradient.h"
#include "HSSRadialGradient.h"
#include "HSSFont.h"
#include "HSSMargin.h"
#include "HSSEvent.h"
#include "HSSAction.h"
#include "HSSFunctionAction.h"
#include "HSSRequest.h"
#include "HSSShape.h"
#include "HSSCircle.h"
#include "HSSRectangle.h"
#include "HSSRoundedRect.h"
#include "HSSPolygon.h"
#include "HSSAbstractStroke.h"
#include "HSSStroke.h"
#include "HSSDisplayObject.h"
#include "HSSTextBlock.h"

#include "XMLParser.h"

#include "HSSPath.h"
#include "HSSPathCommand.h"
#include "HSSPathMoveTo.h"
#include "HSSPathLineTo.h"
#include "HSSPathArcTo.h"
#include "HSSPathSubtract.h"
#include "HSSPathCloseSubpath.h"
#include "HSSPathAddPolygon.h"
#include "HSSPathAddEllipse.h"
#include "HSSRenderGradient.h"

#include "HSSParserReceiver.h"
#include "HSSCodeParser.h"
#include "HSSLayoutLine.h"
#include "HSSContainer.h"
#include "HSSCascader.h"
#include "HSSLayout.h"
#include "HSSRenderer.h"
#include "AXRDocument.h"
#include "AXRDocumentDelegate.h"
#include "AXRController.h"

#endif /* AXR_H */

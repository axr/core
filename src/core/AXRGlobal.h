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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
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

#ifndef AXRGLOBAL_H
#define AXRGLOBAL_H

#include <QtGlobal>
#include <QMetaType>

Q_DECLARE_METATYPE(bool*);
Q_DECLARE_METATYPE(unsigned int*);

#if defined(AXR_EXPORTS)
#define AXR_API Q_DECL_EXPORT
#elif defined(AXR_IMPORTS)
#define AXR_API Q_DECL_IMPORT
#else
#define AXR_API
#endif

#if defined(AXR_NO_DEPRECATED)
#define AXR_DEPRECATED
#define AXR_DEPRECATED_VARIABLE
#else
#define AXR_DEPRECATED Q_DECL_DEPRECATED
#define AXR_DEPRECATED_VARIABLE Q_DECL_VARIABLE_DEPRECATED
#endif

// define AXR_NO_DEPRECATED_ENUMS to restrict usage of deprecated enum members,
// and surround them with #ifndef AXR_NO_DEPRECATED_ENUMS, because the compiler
// doesn't let us deprecate enum members in the same manner as classes and
// class members

#define HSSFRAMEWORK_PROTOCOL "axr"

#endif

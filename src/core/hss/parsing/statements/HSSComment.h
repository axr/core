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

#ifndef HSSCOMMENT_H
#define HSSCOMMENT_H

#include "HSSStatement.h"

namespace AXR
{
    /**
     *  @brief Comments are statements that have no effect on the rendering of the page
     *  but are meant to be read along with the source code.
     */
    class AXR_API HSSComment : public HSSStatement
    {
        AXRString value;

    public:
        /**
         *  Creates a new instance of a comment, containing the given text.
         *  @param value    A string containing the text of the comment.
         */
        HSSComment(AXRString value, AXRController * controller);

        /**
         *  Clones an instance of HSSComment and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSComment.
         */
        QSharedPointer<HSSComment> clone() const;

        /**
         *  Getter for the value.
         *  @return A string containing the text of the comment.
         */
        AXRString getValue();

        /**
         *  Setter for the value.
         *  @param newValue     A string containing the new text for the comment.
         */
        void setValue(AXRString newValue);
        virtual AXRString toString();
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

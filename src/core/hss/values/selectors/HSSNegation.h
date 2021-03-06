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

#ifndef HSSNEGATION_H
#define HSSNEGATION_H

#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief Parser node representing the negator "!".
     *
     *  The negator is prepended in front of simple selectors to invert their meaning.
     */
    class AXR_API HSSNegation : public HSSParserNode
    {
    public:
        /**
         *  Creates a new instance of a negation.
         */
        HSSNegation(AXRController * controller);

        /**
         *  Clones an instance of HSSNegation and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSNegation
         */
        QSharedPointer<HSSNegation> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSNegation();

        //see HSSParserNode.h for the documentation of this method
        virtual AXRString toString();

        //see HSSParserNode.h for the documentation of this method
        AXRString stringRep();

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

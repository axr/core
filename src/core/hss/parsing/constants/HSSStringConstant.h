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

#ifndef HSSSTRINGCONSTANT_H
#define HSSSTRINGCONSTANT_H

#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief Parser node that represents either a single or double quoted string.
     *
     *  Text literals that are encolsed by single quotes ' or double quotes " in HSS.
     *  This class provides storage for that text.
     */
    class AXR_API HSSStringConstant : public HSSParserNode
    {
    public:
        static QSharedPointer<HSSStringConstant> stringToConstant(AXRString value, AXRController * controller);

        /**
         *  Creates a new instance of a string constant node which holds the given value.
         *  @param value    A string containing the value for the constant.
         */
        HSSStringConstant(AXRString value, AXRController * controller);

        /**
         *  Clones an instance of HSSStringConstant and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSStringConstant
         */
        QSharedPointer<HSSStringConstant> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSStringConstant();

        /**
         *  Setter for the value.
         *  @param newValue     A string containing the new value for this constant.
         */
        void setValue(AXRString newValue);

        /**
         *  Getter for the value.
         */
        AXRString getValue();

        AXRString toString();

    protected:
        AXRString value;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

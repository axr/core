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

#ifndef HSSPERCENTAGECONSTANT_H
#define HSSPERCENTAGECONSTANT_H

#include <QSharedPointer>
#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief Parser node that represents a percentage constant in the HSS source.
     */
    class AXR_API HSSPercentageConstant : public HSSParserNode
    {
    public:
        typedef QSharedPointer<HSSPercentageConstant> p;

        /**
         *  Creates a new instance of a percentage constant, with given value.
         *  @param  value   A string containing the name of the keyword.
         */
        HSSPercentageConstant(HSSUnit value, AXRController * controller);

        /**
         *  Clones an instance of HSSParserNode and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSParserNode.
         */
        p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSPercentageConstant();

        virtual AXRString toString();

        /**
         *  Setter for the value of the percentage constant.
         *  @param newValue     A HSSUnit containing the numeric value of the percentage.
         */
        void setValue(HSSUnit newValue);

        /**
         *  Call this method when you need to calculate the actual resulting value.
         *  @param baseValue    A HSSUnit containing the number that corresponds to 100%.
         *  @return A HSSUnit containing the result of the calculation of the base value with the stored
         *  percentage value.
         */
        HSSUnit getValue(HSSUnit baseValue);

    protected:
        HSSUnit value;

    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif

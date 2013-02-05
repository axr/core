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

#ifndef HSSOBJECTNAMECONSTANT_H
#define HSSOBJECTNAMECONSTANT_H

#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief Parser node that represents an object name constant.
     *
     *  When using object names in HSS, the system will look up the object with the name
     *  encapsulated in this class from the object tree on AXRController.
     */
    class AXR_API HSSObjectNameConstant : public HSSParserNode
    {
    public:
        /**
         *  Creates a new instance of a object name constant, with given name.
         *  @param  value   A string containing the object name.
         */
        HSSObjectNameConstant(AXRString value, AXRController * controller);

        /**
         *  Clones an instance of HSSObjectNameConstant and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSObjectNameConstant.
         */
        QSharedPointer<HSSObjectNameConstant> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSObjectNameConstant();

        /**
         *  Setter for the value of the object name constant.
         *  @param newValue     A string containing the new object name.
         */
        void setValue(AXRString newValue);

        /**
         *  Getter for the value of the object name constant.
         *  @return A string containing the object name.
         */
        AXRString getValue();

        /**
         *  Prints itself as a textual representation, useful for loggin or introspection.
         *  @return A string containing a textual representation of the keyword constant.
         */
        AXRString toString();

    protected:
        AXRString value;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

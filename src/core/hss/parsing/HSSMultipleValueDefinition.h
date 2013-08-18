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

#ifndef HSSMULTIPLEVALUEDEFINITION_H
#define HSSMULTIPLEVALUEDEFINITION_H

#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief This nodes holds multiple parser nodes inside.
     *
     *  Each element in the XML code is represented as a HSSContainer, which is
     *  a type of display object that can contain other display object. It is capable of
     *  laying them out and to propagate events to them. It also implements the needed
     *  rendering routines for this type of object.
     */
    class AXR_API HSSMultipleValueDefinition : public HSSParserNode
    {
    public:
        /**
         *  Creates a new instance of a multiple value definition node.
         */
        HSSMultipleValueDefinition(AXRController * controller);

        /**
         *  Clones an instance of HSSMultipleValueDefinition and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSMultipleValueDefinition
         */
        QSharedPointer<HSSMultipleValueDefinition> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSMultipleValueDefinition();

        virtual AXRString toString();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  Getter for values.
         *  @return A vector of shared pointers to parser nodes that are stored inside this
         *  multiple value definition.
         */
        std::vector<QSharedPointer<HSSParserNode> > getValues();

        /**
         *  Setter for values.
         *  @param newValues    A vector of shared pointers to parser nodes that to store them inside this
         *  multiple value definition.
         */
        void setValues(std::vector<QSharedPointer<HSSParserNode> > newValues);

        /**
         *  Adds a node to the values list.
         *  @param newValue    A vector of shared pointers to parser nodes that to store them inside this
         *  multiple value definition.
         */
        void add(QSharedPointer<HSSParserNode> newValue);

    protected:
        std::vector<QSharedPointer<HSSParserNode> > values;
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

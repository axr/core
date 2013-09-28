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

#ifndef HSSPROPERTYPATHNODE_H
#define HSSPROPERTYPATHNODE_H

#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief Each of the nodes of a property path.
     */
    class AXR_API HSSPropertyPathNode : public HSSParserNode
    {
    public:
       /**
        *  Creates a new property path node.
        *  @param propertyName A string with the name of the property.
        */
        HSSPropertyPathNode(AXRString propertyName, AXRController * controller);

        /**
         *  Copy constructor for HSSPropertyPath objects
         */
        HSSPropertyPathNode(const HSSPropertyPathNode & orig);

        /**
         *  Clones an instance of HSSPropertyPathNode and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSPropertyPathNode
         */
        QSharedPointer<HSSPropertyPathNode> clone() const;

        /**
         *  Destructor
         */
        virtual ~HSSPropertyPathNode();

        virtual AXRString toString();

        /**
         *  Each node overrides this method to compare against another node.
         *  @param otherNode    The other object to compare to.
         *  @return Wether the node is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  Getter for the property name.
         *  @return A string containing the property name.
         */
        const AXRString getPropertyName() const;

    private:
        AXRString _propertyName;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

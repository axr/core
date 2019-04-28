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

#ifndef HSSOVERRIDEFUNCTION_H
#define HSSOVERRIDEFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    class HSSPropertyDefinition;
    class HSSSelectorChain;

    /**
     *  @brief Implementation of the override(<selector chains>]){ <property definition>,... } function in HSS.
     *
     *  Overrides are properties that weigh more than the normal cascade
     */
    class AXR_API HSSOverrideFunction : public HSSFunction
    {
    public:
        /**
         *  Creates a new instance of a override function.
         */
        HSSOverrideFunction(AXRController * controller);
        
        /**
         *  Copy constructor for HSSOverrideFunction objects. Do not call directly, use clone() instead.
         */
        HSSOverrideFunction(const HSSOverrideFunction & orig);
        
        /**
         *  Clones an instance of HSSOverrideFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSOverrideFunction
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSOverrideFunction();
        
        virtual AXRString toString();
        
        void setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains);
        std::vector<QSharedPointer<HSSSelectorChain> > getSelectorChains() const;
        virtual void setThisObj(QSharedPointer<AXR::HSSDisplayObject> newThis);
        
        /**
         *  Getter for the property definitions vector.
         *  @return A vector of shared pointers to property definitions.
         */
        std::vector<QSharedPointer<HSSPropertyDefinition> > getProperties() const;
        
        /**
         *  Add a property definition to the properties vector.
         *  @param newProperty  A shared pointer to the property definition to be added.
         */
        void propertiesAdd(QSharedPointer<HSSPropertyDefinition> & newProperty);
        
        /**
         *  Get a property definition by index.
         *  @param index    An unsigned integer with the index of the property definition.
         *  @return A shared pointer to the element at that index.
         */
        QSharedPointer<HSSPropertyDefinition> &propertiesGet(size_t index);
        
        /**
         *  Removes a property definition by index.
         *  @param index    An unsigned integer with the index of the property definition to be deleted.
         */
        void propertiesRemove(off_t index);
        
        /**
         *  Removes the last element in the properties vector.
         */
        void propertiesRemoveLast();
        
        /**
         *  @return the last element of the properties vector.
         */
        QSharedPointer<HSSPropertyDefinition> &propertiesLast();
        
        /**
         *  @return the size of the properties vector
         */
        size_t propertiesSize() const;
        
    protected:
        /**
         *  This is the actual implementation of what the function does.
         *  @return An empty pointer.
         */
        virtual QSharedPointer<HSSObject> _evaluate();

        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
        
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        std::vector<QSharedPointer<HSSPropertyDefinition> > properties;
    };
}

#endif

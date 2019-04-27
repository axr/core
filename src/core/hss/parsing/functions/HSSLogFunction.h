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

#ifndef HSSLOGFUNCTION_H
#define HSSLOGFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    /**
     *  @brief Implementation of the log(<value>[, <value> ... ]) function in HSS.
     *
     *  Log functions print the given value to the console
     */
    class AXR_API HSSLogFunction : public HSSFunction
    {
    public:
        /**
         *  Creates a new instance of a log function.
         */
        HSSLogFunction(AXRController * controller);

        /**
         *  Copy constructor for HSSLogFunction objects. Do not call directly, use clone() instead.
         */
        HSSLogFunction(const HSSLogFunction & orig);

        /**
         *  Clones an instance of HSSLogFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLogFunction
         */
        QSharedPointer<HSSFunction> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSLogFunction();

        virtual AXRString toString();

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

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  This is the actual implementation of what the function does.
         *  @return An empty pointer.
         */
        virtual QSharedPointer<HSSObject> _evaluate();

        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);

    protected:
        std::vector<QSharedPointer<HSSParserNode> > values;

    private:
        void _logParserNode(QSharedPointer<HSSParserNode> parserNode) const;
        void _logObject(QSharedPointer<HSSObject> theObj) const;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif

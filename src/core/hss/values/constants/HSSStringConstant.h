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

#include "HSSScopedParserNode.h"

namespace AXR
{
    /**
     *  @brief Parser node that represents either a single or double quoted string.
     *
     *  Text literals that are encolsed by single quotes ' or double quotes " in HSS.
     *  This class provides storage for that text.
     */
    class AXR_API HSSStringConstant : public HSSScopedParserNode
    {
    public:
        static QSharedPointer<HSSStringConstant> stringToConstant(HSSString value, AXRController * controller);

        /**
         *  Creates a new instance of a string constant node which holds the given value.
         *  @param value    A string containing the value for the constant.
         */
        HSSStringConstant(HSSString value, AXRController * controller);
        
        /**
         *  Copy constructor
         *  @param orig    The instante from which to copy.
         */
        HSSStringConstant(const HSSStringConstant & orig);

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
        void setValue(HSSString newValue);

        /**
         *  Getter for the value.
         */
        HSSString getValue();

        HSSString toString();
        HSSString stringRep();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        void setHasArguments(bool newValue);
        bool hasArguments() const;
        void setArguments(const std::vector<QSharedPointer<HSSParserNode> > & args);
        void addArgument(QSharedPointer<HSSParserNode> parserNode);
        void setIndexes(const std::vector<size_t> &indexes);
        HSSString evaluate();
        bool hasStartQuote() const;
        void setHasStartQuote(bool newValue);
        bool hasEndQuote() const;
        void setHasEndQuote(bool newValue);
        
        void setThisObj(QSharedPointer<HSSDisplayObject> value);
        void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        
        /**
         *  Method to be passed as callback when observing changes that will affect value.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj);

    protected:
        HSSString value;

    private:
        bool _hasArguments;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        std::vector<QSharedPointer<HSSParserNode> > _arguments;
        std::vector<size_t> _indexes;
        bool _hasStartQuote;
        bool _hasEndQuote;

        AXRString _evaluateNode(QSharedPointer<HSSParserNode> parserNode);
        void _bindToArguments();
    };
}

#endif

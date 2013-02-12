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

#ifndef HSSFLAG_H
#define HSSFLAG_H

#include "HSSFilter.h"

namespace AXR
{
    class HSSSelectorChain;
    class HSSDisplayObject;

    /**
     *  @brief Parser Node type representing flags.
     *  Flags (e.g. \::hover) are a way to conditionally target elements in HSS, inside
     *  a selector chain.
     */
    class AXR_API HSSFlag : public HSSFilter
    {
    public:
        /**
         *  Creates a new instance of a flag node.
         */
        HSSFlag(AXRController * controller);

        HSSFlag(const HSSFlag &orig);

        /**
         *  Clones an instance of HSSFlag and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFlag
         */
        QSharedPointer<HSSFilter> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFlag();

        virtual AXRString toString();

        /**
         *  Getter for the name.
         *  @return The name of the flag. This is what comes after the double colon
         *  in HSS (e.g. "hover").
         */
        AXRString getName();

        /**
         *  Setter for the name.
         *  @param  newValue    A string containing the new name of the flag.
         */
        void setName(AXRString newValue);

        /**
         *  When the status of a flag changes, this method should be called. It will create a
         *  new selection from the original scope of the rule, and then set the new purging
         *  status on the selected elements.
         *  @param newStatus    The new status to which it should be set
         */
        void flagChanged(HSSRuleState newStatus);

        /**
         *  @return A shared pointer to the instance on which it is called. A shared pointer to this
         *  instance must already exist before calling this.
         */
        QSharedPointer<HSSFlag> shared_from_this();

        /**
         *  When selecting (not connecting, which is different) elements based on the flag,
         *  the flag acts like a filter, reducing the size of the selection.
         *  @param scope        The original selection: A vector of shared pointers to display objects.
         *  @param processing   Wether we are processing the filter or not.
         *  @return A vector of shared pointers to the display objects in the resulting selection.
         */
        QSharedPointer<HSSSelection> apply(QSharedPointer<HSSSelection> scope, bool processing);

        /**
         *  Getter for the purging state.
         *  @return The HSSRuleState that represents the current purging status.
         */
        HSSRuleState getPurging();

        /**
         *  Setter for the purging state.
         *  @param newValue     The new purging status for this flag.
         */
        void setPurging(HSSRuleState newValue);

    protected:
        AXRString _name;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        HSSRuleState _purging;

        QSharedPointer<HSSSimpleSelection>  _selectFromTop(QSharedPointer<HSSRule> theRule);
    };
}

#endif

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

#ifndef HSSFUNCTIONACTION_H
#define HSSFUNCTIONACTION_H

#include "HSSAction.h"
#include "HSSFunction.h"

namespace AXR
{

    /**
     *  @brief The action objects that represent custom functions functions.
     */
    class HSSFunctionAction : public HSSAction
    {
    public:
        typedef boost::shared_ptr<HSSFunctionAction> p;

        /**
         *  Constructor for HSSFunctionAction objects
         */
        HSSFunctionAction();

        /**
         *  Copy constructor for HSSFunctionAction objects. Do not call directly,
         *  use clone() instead.
         */
        HSSFunctionAction(const HSSFunctionAction & orig);

        /**
         *  Clones an instance of HSSFunctionAction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFunctionAction
         */
        p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFunctionAction();

        virtual std::string toString();
        virtual std::string defaultObjectType();

        /**
         *  This method is called when the action should be executed.
         */
        virtual void fire();

        /**
         *  Getter for the function.
         *  @return A shared pointer to the function object.
         */
        HSSFunction::p getFunction();

        /**
         *  Setter for the function.
         *  @param newValue A shared pointer to the new function to be used.
         */
        void setFunction(HSSFunction::p newValue);

    private:
        HSSFunction::p _function;
        virtual HSSClonable::p cloneImpl() const;
    };
}


#endif

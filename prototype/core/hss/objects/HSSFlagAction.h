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

#ifndef HSSFLAGACTION_H
#define HSSFLAGACTION_H

#include "HSSAction.h"
#include "HSSFlagFunction.h"

namespace AXR
{

    /**
     *  @brief The HSS object counterparts to flagging functions.
     *  They actually work using flagging functions internally.
     */
    class HSSFlagAction : public HSSAction
    {
    public:
        typedef boost::shared_ptr<HSSFlagAction> p;

        /**
         *  Constructor for HSSFlagAction objects
         */
        HSSFlagAction();

        /**
         *  Copy constructor for HSSFlagAction objects. Do not call directly,
         *  use clone() instead.
         */
        HSSFlagAction(const HSSFlagAction & orig);

        /**
         *  Clones an instance of HSSFlagAction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFlagAction
         */
        p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFlagAction();

        virtual std::string toString();
        virtual std::string defaultObjectType();

        /**
         *  This method is called when the flagging action should be executed.
         */
        virtual void fire();

        /**
         *  Getter for the flagging function.
         *  @return A shared pointer to the flagging function object.
         */
        HSSFlagFunction::p getFlagFunction();

        /**
         *  Setter for the flagging function.
         *  @param newValue A shared pointer to the new flagging function to be used.
         */
        void setFlagFunction(HSSFlagFunction::p newValue);

    private:
        HSSFlagFunction::p _flagFunction;
        virtual HSSClonable::p cloneImpl() const;
    };
}


#endif

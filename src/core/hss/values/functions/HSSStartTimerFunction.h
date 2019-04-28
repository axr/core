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

#ifndef HSSSTARTTIMERFUNCTION_H
#define HSSSTARTTIMERFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    class HSSPropertyDefinition;
    class HSSSelectorChain;
    
    /**
     *  @brief Implementation of the startTimer(<timer name>){ <property definition>,... } function in HSS.
     *
     *  Set up a an action that repeats by default
     */
    class AXR_API HSSStartTimerFunction : public HSSFunction
    {
    public:
        /**
         *  Creates a new instance of a start timer function.
         */
        HSSStartTimerFunction(AXRController * controller);
        
        /**
         *  Copy constructor for HSSStartTimerFunction objects. Do not call directly, use clone() instead.
         */
        HSSStartTimerFunction(const HSSStartTimerFunction & orig);
        
        /**
         *  Clones an instance of HSSStartTimerFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSStartTimerFunction
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSStartTimerFunction();
        
        virtual AXRString toString();
        
        void setTimerName(AXRString timerName);
        AXRString getTimerName() const;
        void setFunction(QSharedPointer<HSSParserNode> fun);
        QSharedPointer<HSSParserNode> getFunction() const;
        void setMs(QSharedPointer<HSSParserNode> ms);
        QSharedPointer<HSSParserNode> getMs() const;
        HSSUnit getComputedMs();
        void setRepeats(bool repeats);
        bool getRepeats();
        virtual void fireTimer(AXRString timerName);

        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);

    protected:
        /**
         *  This is the actual implementation of what the function does.
         *  @return An empty pointer.
         */
        virtual QSharedPointer<HSSObject> _evaluate();

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        AXRString timerName;
        QSharedPointer<HSSParserNode> function;
        QSharedPointer<HSSParserNode> ms;
        bool repeats;
    };
}

#endif

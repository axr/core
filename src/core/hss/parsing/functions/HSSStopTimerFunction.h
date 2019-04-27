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

#ifndef HSSSTOPTIMERFUNCTION_H
#define HSSSTOPTIMERFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    class HSSPropertyDefinition;
    class HSSSelectorChain;
    
    /**
     *  @brief Implementation of the stopTimer(<timer name>]) function in HSS.
     *
     *  Stops an action that is repeating
     */
    class AXR_API HSSStopTimerFunction : public HSSFunction
    {
    public:
        /**
         *  Creates a new instance of a start timer function.
         */
        HSSStopTimerFunction(AXRController * controller);
        
        /**
         *  Copy constructor for HSSStopTimerFunction objects. Do not call directly, use clone() instead.
         */
        HSSStopTimerFunction(const HSSStopTimerFunction & orig);
        
        /**
         *  Clones an instance of HSSStopTimerFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSStopTimerFunction
         */
        QSharedPointer<HSSFunction> clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSStopTimerFunction();
        
        virtual AXRString toString();
        
        void setTimerName(AXRString timerName);
        AXRString getTimerName() const;

    protected:
        /**
         *  This is the actual implementation of what the function does.
         *  @return An empty pointer.
         */
        virtual QSharedPointer<HSSObject> _evaluate();
        
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        AXRString timerName;
    };
}

#endif

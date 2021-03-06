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

#include "axr.h"
#include "HSSAction.h"

using namespace AXR;

AXRString HSSAction::actionTypeStringRepresentation(HSSActionType actionType)
{
    static std::map<HSSActionType, AXRString> types;
    if (types.empty())
    {
        types[HSSActionTypeRequest] = "HSSActionTypeRequest";
        types[HSSActionTypeAlert] = "HSSActionTypeAlert";
        types[HSSActionTypeLog] = "HSSActionTypeLog";
        types[HSSActionTypeFlag] = "HSSActionTypeFlag";
        types[HSSActionTypeFunction] = "HSSActionTypeFunction";
    }

    return types[actionType];
}

HSSAction::HSSAction(HSSActionType type, AXRController * controller)
: HSSObject(HSSObjectTypeAction, controller)
{
    this->actionType = type;
}

HSSAction::HSSAction(const HSSAction & orig)
: HSSObject(orig)
{
    this->actionType = orig.actionType;
}

QSharedPointer<HSSAction> HSSAction::clone() const
{
    return qSharedPointerCast<HSSAction>(this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSAction::cloneImpl() const
{
    return QSharedPointer<HSSAction>(new HSSAction(*this));
}

HSSAction::~HSSAction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSAction: destructing action object");
    this->cleanTrackedObservers();
}

AXRString HSSAction::toString()
{
    return "Generic HSSAction of type" + HSSAction::actionTypeStringRepresentation(this->actionType);
}

AXRString HSSAction::defaultObjectType()
{
    return "action";
}

bool HSSAction::isA(HSSActionType type) const
{
    return type == this->actionType;
}

HSSActionType HSSAction::getActionType() const
{
    return this->actionType;
}

void HSSAction::fire()
{
    //do nothing
}

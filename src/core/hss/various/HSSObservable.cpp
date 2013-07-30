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

#include <QHash>
#include <QMap>
#include <QSharedPointer>
#include <QVariant>
#include "AXRLoggerManager.h"
#include "HSSCallback.h"
#include "HSSObject.h"
#include "HSSObservable.h"
#include "HSSObservableMapping.h"

using namespace AXR;

AXRString HSSObservable::observablePropertyStringRepresentation(HSSObservableProperty property)
{
    static QMap<HSSObservableProperty, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSObservablePropertyNone] = "unknown";

        //HSSObject
        types[HSSObservablePropertyIsA] = "HSSObservablePropertyIsA";

        //HSSDisplayObject
        types[HSSObservablePropertyValue] = "value";
        types[HSSObservablePropertyWidth] = "width";
        types[HSSObservablePropertyInnerWidth] = "innerWidth";
        types[HSSObservablePropertyInnerHeight] = "innerHeight";
        types[HSSObservablePropertyOuterWidth] = "outerWidth";
        types[HSSObservablePropertyOuterHeight] = "outerHeight";
        types[HSSObservablePropertyHeight] = "height";
        types[HSSObservablePropertyAnchorX] = "anchorX";
        types[HSSObservablePropertyAnchorY] = "anchorY";
        types[HSSObservablePropertyFlow] = "flow";
        types[HSSObservablePropertyContained] = "contained";
        types[HSSObservablePropertyAlignX] = "alignX";
        types[HSSObservablePropertyAlignY] = "alignY";
        types[HSSObservablePropertyBackground] = "background";
        types[HSSObservablePropertyContent] = "content";
        types[HSSObservablePropertyOn] = "on";
        types[HSSObservablePropertyMargin] = "margin";
        types[HSSObservablePropertyPadding] = "padding";
        types[HSSObservablePropertyBorder] = "border";
        types[HSSObservablePropertyFlagChanged] = "flagChanged";
        types[HSSObservablePropertyVisible] = "visible";
        types[HSSObservablePropertyLineAlign] = "lineAlign";

        //HSSContainer
        types[HSSObservablePropertyContentAlignX] = "contentAlignX";
        types[HSSObservablePropertyContentAlignY] = "contentAlignY";
        types[HSSObservablePropertyDirectionPrimary] = "direction";
        types[HSSObservablePropertyDirectionSecondary] = "wrapDirection";
        types[HSSObservablePropertyFont] = "font";
        types[HSSObservablePropertyShape] = "shape";

        //HSSTextBlock
        types[HSSObservablePropertyTransform] = "transform";
        types[HSSObservablePropertyTextAlign] = "textAlign";
        types[HSSObservablePropertyText] = "text";

        //HSSRgb
        types[HSSObservablePropertyRed] = "red";
        types[HSSObservablePropertyGreen] = "green";
        types[HSSObservablePropertyBlue] = "blue";
        types[HSSObservablePropertyAlpha] = "alpha";

        //HSSGradient
        types[HSSObservablePropertyStartColor] = "startColor";
        types[HSSObservablePropertyEndColor] = "endColor";
        types[HSSObservablePropertyBalance] = "balance";
        types[HSSObservablePropertyColorStops] = "colorStops";

        //HSSLinearGradient
        types[HSSObservablePropertyStartX] = "startX";
        types[HSSObservablePropertyStartY] = "startY";
        types[HSSObservablePropertyEndX] = "endX";
        types[HSSObservablePropertyEndY] = "endY";

        //HSSColorStop
        types[HSSObservablePropertyPosition] = "position";

        //HSSFont
        types[HSSObservablePropertySize] = "size";
        types[HSSObservablePropertyFace] = "face";
        types[HSSObservablePropertyColor] = "color";
        types[HSSObservablePropertyWeight] = "wheight";

        //HSSRoundedRect
        types[HSSObservablePropertyCorners] = "corners";
        types[HSSObservablePropertyLeft] = "left";
        types[HSSObservablePropertyLeftTop] = "leftTop";
        types[HSSObservablePropertyTop] = "top";
        types[HSSObservablePropertyRightTop] = "rightTop";
        types[HSSObservablePropertyRight] = "right";
        types[HSSObservablePropertyRightBottom] = "rightBottom";
        types[HSSObservablePropertyBottom] = "bottom";
        types[HSSObservablePropertyLeftBottom] = "leftBottom";

        //HSSEvent
        types[HSSObservablePropertyAction] = "action";

        //HSSRequest
        types[HSSObservablePropertySrc] = "src";
        types[HSSObservablePropertyTarget] = "target";
        types[HSSObservablePropertyMode] = "mode";
        types[HSSObservablePropertyContentTarget] = "target";

        //various
        types[HSSObservablePropertyTreeChange] = "treeChange";

        //HSSPolygon
        types[HSSObservablePropertySides] = "sides";
        types[HSSObservablePropertyAngle] = "angle";

        //HSSBorder
        types[HSSObservablePropertySegments] = "segments";

        //HSSRadialGradient
        types[HSSObservablePropertyCenterX] = "centerX";
        types[HSSObservablePropertyCenterY] = "centerY";
        types[HSSObservablePropertyOffsetX] = "offsetX";
        types[HSSObservablePropertyOffsetY] = "offsetY";
        types[HSSObservablePropertyRoundness] = "roundness";
    }

    return types[property];
}

HSSObservableProperty HSSObservable::observablePropertyFromString(AXRString name)
{
    static QMap<AXRString, HSSObservableProperty> properties;

    if (properties.empty())
    {
        //HSSObject
        properties["isA"] = HSSObservablePropertyIsA;

        //HSSDisplayObject
        properties["value"] = HSSObservablePropertyValue;
        properties["width"] = HSSObservablePropertyWidth;
        properties["height"] = HSSObservablePropertyHeight;
        properties["anchorX"] = HSSObservablePropertyAnchorX;
        properties["anchorY"] = HSSObservablePropertyAnchorY;
        properties["flow"] = HSSObservablePropertyFlow;
        properties["contained"] = HSSObservablePropertyContained;
        properties["alignX"] = HSSObservablePropertyAlignX;
        properties["alignY"] = HSSObservablePropertyAlignY;
        properties["background"] = HSSObservablePropertyBackground;
        properties["content"] = HSSObservablePropertyContent;
        properties["on"] = HSSObservablePropertyOn;
        properties["visible"] = HSSObservablePropertyVisible;
        properties["lineAlign"] = HSSObservablePropertyLineAlign;

        //HSSContainer
        properties["contentAlignX"] = HSSObservablePropertyContentAlignX;
        properties["contentAlignY"] = HSSObservablePropertyContentAlignY;
        properties["direction"] = HSSObservablePropertyDirectionPrimary;
        properties["wrapDirection"] = HSSObservablePropertyDirectionSecondary;
        properties["font"] = HSSObservablePropertyFont;
        properties["shape"] = HSSObservablePropertyShape;
        properties["on"] = HSSObservablePropertyOn;
        properties["margin"] = HSSObservablePropertyMargin;
        properties["padding"] = HSSObservablePropertyPadding;
        properties["border"] = HSSObservablePropertyBorder;

        //HSSTextBlock
        properties["transform"] = HSSObservablePropertyTransform;
        properties["textAlign"] = HSSObservablePropertyTextAlign;
        properties["text"] = HSSObservablePropertyText;

        //HSSRgb
        properties["red"] = HSSObservablePropertyRed;
        properties["green"] = HSSObservablePropertyGreen;
        properties["blue"] = HSSObservablePropertyBlue;
        properties["alpha"] = HSSObservablePropertyAlpha;

        //HSSGradient
        properties["startColor"] = HSSObservablePropertyStartColor;
        properties["endColor"] = HSSObservablePropertyEndColor;
        properties["balance"] = HSSObservablePropertyBalance;
        properties["colorStops"] = HSSObservablePropertyColorStops;

        //HSSLinearGradient
        properties["startX"] = HSSObservablePropertyStartX;
        properties["startY"] = HSSObservablePropertyStartY;
        properties["endX"] = HSSObservablePropertyEndX;
        properties["endY"] = HSSObservablePropertyEndY;

        //HSSColorStop
        properties["position"] = HSSObservablePropertyPosition;

        //HSSFont
        properties["size"] = HSSObservablePropertySize;
        properties["face"] = HSSObservablePropertyFace;
        properties["color"] = HSSObservablePropertyColor;
        properties["weight"] = HSSObservablePropertyWeight;

        //HSSRoundedRect
        properties["corners"] = HSSObservablePropertyCorners;
        properties["left"] = HSSObservablePropertyLeft;
        properties["leftTop"] = HSSObservablePropertyLeftTop;
        properties["top"] = HSSObservablePropertyTop;
        properties["rightTop"] = HSSObservablePropertyRightTop;
        properties["right"] = HSSObservablePropertyRight;
        properties["rightBottom"] = HSSObservablePropertyRightBottom;
        properties["bottom"] = HSSObservablePropertyBottom;
        properties["leftBottom"] = HSSObservablePropertyLeftBottom;

        //HSSPolygon
        properties["sides"] = HSSObservablePropertySides;
        properties["angle"] = HSSObservablePropertyAngle;

        //HSSEvent
        properties["action"] = HSSObservablePropertyAction;

        //HSSRequest
        properties["src"] = HSSObservablePropertySrc;
        properties["target"] = HSSObservablePropertyTarget;
        properties["mode"] = HSSObservablePropertyMode;
        properties["contentTarget"] = HSSObservablePropertyContentTarget;

        //HSSBorder
        properties["segments"] = HSSObservablePropertySegments;

        //HSSRadialGradient
        properties["centerX"] = HSSObservablePropertyCenterX;
        properties["centerY"] = HSSObservablePropertyCenterY;
        properties["offsetX"] = HSSObservablePropertyOffsetX;
        properties["offsetY"] = HSSObservablePropertyOffsetY;
    }


    return properties[name];
}

HSSObservable::HSSObservable()
{

}

HSSObservable::~HSSObservable()
{
    this->_propertyObservers.clear();
}

size_t hash_combine(size_t hash1, size_t hash2)
{
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
}

void HSSObservable::observe(const AXRString target, const AXRString source, HSSObservable * object, HSSAbstractValueChangedCallback *callback)
{
    if (object->tracksObserver(source))
    {
        HSSObservable * to = object->_trackedObservers[source];
        AXRString target = object->_trackedProperties[source];
        to->removeObserver(target, source, object);
        object->untrackObserver(source);
    }
    object->trackObserver(target, source, this);

    QVariant nulldata("");

    if (this->_propertyObservers.count(target) != 0)
    {
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        QSharedPointer<HSSObservableMapping> mapping = QSharedPointer<HSSObservableMapping>(new HSSObservableMapping(object, callback, source, nulldata));
        theObserved.append(mapping);
        axr_log(LoggerChannelObserving, AXRString("observing ").append(target).append(" from ").append(source));
    }
    else
    {
        HSSObservable::observed theObserved;
        QSharedPointer<HSSObservableMapping> mapping = QSharedPointer<HSSObservableMapping>(new HSSObservableMapping(object, callback, source, nulldata));
        theObserved.append(mapping);
        this->_propertyObservers[target] = theObserved;
        axr_log(LoggerChannelObserving, AXRString("observing ").append(target).append(" from ").append(source));
    }
}

void HSSObservable::removeObserver(const AXRString target, const AXRString source, HSSObservable * object)
{
    if (this->_propertyObservers.find(target) != this->_propertyObservers.end())
    {
        observed &theObserved = this->_propertyObservers[target];
        for (observed::iterator it = theObserved.begin(); it!= theObserved.end(); ++it) {
            const QSharedPointer<HSSObservableMapping> & mapping = *it;
            if(mapping->observer == object && mapping->source == source){
                theObserved.erase(it);
                axr_log(LoggerChannelObserving, AXRString("stop observing ").append(target).append(" from ").append(source));
                return;
            }
        }
    }
    axr_log(LoggerChannelObserving, AXRString("tried to remove non existent observer for ").append(target));
}

void HSSObservable::propertyChanged(const AXRString property, const QSharedPointer<HSSObject> theObj)
{
    axr_log(LoggerChannelObserving, "property changed");
}

void HSSObservable::notifyObservers(const AXRString property, const QSharedPointer<HSSObject> theObj)
{
    if (this->_propertyObservers.contains(property))
    {
        observed &theObserved = this->_propertyObservers[property];
        for (observed::iterator it = theObserved.begin(); it != theObserved.end(); ++it)
        {
            const QSharedPointer<HSSObservableMapping> & mapping = *it;
            HSSAbstractValueChangedCallback *callback = mapping->callback;
            callback->call(property, theObj);
        }
    }
}

bool HSSObservable::tracksObserver(const AXRString source)
{
    HSSObservable * tracked = this->_trackedObservers[source];
    if (tracked)
    {
        return true;
    }
    return false;
}

HSSObservable * HSSObservable::getTrackedObserver(const AXRString source)
{
    return this->_trackedObservers[source];
}

AXRString HSSObservable::getTrackedProperty(const AXRString source)
{
    return this->_trackedProperties[source];
}

void HSSObservable::trackObserver(const AXRString target, const AXRString source, HSSObservable* observable)
{
    this->_trackedObservers.insert(source, observable);
    this->_trackedProperties.insert(source, target);
}

void HSSObservable::untrackObserver(const AXRString source)
{
    this->_trackedObservers.remove(source);
    this->_trackedProperties.remove(source);
}

void HSSObservable::cleanTrackedObservers()
{
    QMapIterator<AXRString, AXRString> it(this->_trackedProperties);
    while (it.hasNext())
    {
        it.next();
        HSSObservable * to = this->_trackedObservers[it.key()];
        if (to)
        {
            to->removeObserver(it.value(), it.key(), this);
        }
    }
    this->_trackedProperties.clear();
}

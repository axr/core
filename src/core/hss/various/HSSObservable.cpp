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

#include <iostream>
#include <QHash>
#include <QMap>
#include "AXRDebugging.h"
#include "HSSObservable.h"

using namespace AXR;

std::string HSSObservable::observablePropertyStringRepresentation(HSSObservableProperty property)
{
    static QMap<HSSObservableProperty, QString> types;
    if (types.isEmpty())
    {
        types[HSSObservablePropertyNone] = "unknown";

        //HSSObject
        types[HSSObservablePropertyIsA] = "HSSObservablePropertyIsA";

        //HSSDisplayObject
        types[HSSObservablePropertyValue] = "HSSObservablePropertyValue";
        types[HSSObservablePropertyWidth] = "HSSObservablePropertyWidth";
        types[HSSObservablePropertyInnerWidth] = "HSSObservablePropertyInnerWidth";
        types[HSSObservablePropertyInnerHeight] = "HSSObservablePropertyInnerHeight";
        types[HSSObservablePropertyOuterWidth] = "HSSObservablePropertyOuterWidth";
        types[HSSObservablePropertyOuterHeight] = "HSSObservablePropertyOuterHeight";
        types[HSSObservablePropertyHeight] = "HSSObservablePropertyHeight";
        types[HSSObservablePropertyAnchorX] = "HSSObservablePropertyAnchorX";
        types[HSSObservablePropertyAnchorY] = "HSSObservablePropertyAnchorY";
        types[HSSObservablePropertyFlow] = "HSSObservablePropertyFlow";
        types[HSSObservablePropertyContained] = "HSSObservablePropertyContained";
        types[HSSObservablePropertyAlignX] = "HSSObservablePropertyAlignX";
        types[HSSObservablePropertyAlignY] = "HSSObservablePropertyAlignY";
        types[HSSObservablePropertyBackground] = "HSSObservablePropertyBackground";
        types[HSSObservablePropertyContent] = "HSSObservablePropertyContent";
        types[HSSObservablePropertyOn] = "HSSObservablePropertyOn";
        types[HSSObservablePropertyMargin] = "HSSObservablePropertyMargin";
        types[HSSObservablePropertyPadding] = "HSSObservablePropertyPadding";
        types[HSSObservablePropertyBorder] = "HSSObservablePropertyBorder";
        types[HSSObservablePropertyFlagChanged] = "HSSObservablePropertyFlagChanged";
        types[HSSObservablePropertyVisible] = "HSSObservablePropertyVisible";

        //HSSContainer
        types[HSSObservablePropertyContentAlignX] = "HSSObservablePropertyContentAlignX";
        types[HSSObservablePropertyContentAlignY] = "HSSObservablePropertyContentAlignY";
        types[HSSObservablePropertyDirectionPrimary] = "HSSObservablePropertyDirection";
        types[HSSObservablePropertyDirectionSecondary] = "HSSObservablePropertyWrapDirection";
        types[HSSObservablePropertyFont] = "HSSObservablePropertyFont";
        types[HSSObservablePropertyShape] = "HSSObservablePropertyShape";

        //HSSTextBlock
        types[HSSObservablePropertyTransform] = "HSSObservablePropertyTransform";
        types[HSSObservablePropertyTextAlign] = "HSSObservablePropertyTextAlign";
        types[HSSObservablePropertyText] = "HSSObservablePropertyText";

        //HSSRgb
        types[HSSObservablePropertyRed] = "HSSObservablePropertyRed";
        types[HSSObservablePropertyGreen] = "HSSObservablePropertyGreen";
        types[HSSObservablePropertyBlue] = "HSSObservablePropertyBlue";
        types[HSSObservablePropertyAlpha] = "HSSObservablePropertyAlpha";

        //HSSGradient
        types[HSSObservablePropertyStartColor] = "HSSObservablePropertyStartColor";
        types[HSSObservablePropertyEndColor] = "HSSObservablePropertyEndColor";
        types[HSSObservablePropertyBalance] = "HSSObservablePropertyBalance";
        types[HSSObservablePropertyColorStops] = "HSSObservablePropertyColorStops";

        //HSSLinearGradient
        types[HSSObservablePropertyStartX] = "HSSObservablePropertyStartX";
        types[HSSObservablePropertyStartY] = "HSSObservablePropertyStartY";
        types[HSSObservablePropertyEndX] = "HSSObservablePropertyEndX";
        types[HSSObservablePropertyEndY] = "HSSObservablePropertyEndY";

        //HSSColorStop
        types[HSSObservablePropertyPosition] = "HSSObservablePropertyPosition";

        //HSSFont
        types[HSSObservablePropertySize] = "HSSObservablePropertySize";
        types[HSSObservablePropertyFace] = "HSSObservablePropertyFace";
        types[HSSObservablePropertyColor] = "HSSObservablePropertyColor";
        types[HSSObservablePropertyWeight] = "HSSObservablePropertyWeight";

        //HSSRoundedRect
        types[HSSObservablePropertyCorners] = "HSSObservablePropertyCorners";
        types[HSSObservablePropertyLeft] = "HSSObservablePropertyLeft";
        types[HSSObservablePropertyLeftTop] = "HSSObservablePropertyLeftTop";
        types[HSSObservablePropertyTop] = "HSSObservablePropertyTop";
        types[HSSObservablePropertyRightTop] = "HSSObservablePropertyRightTop";
        types[HSSObservablePropertyRight] = "HSSObservablePropertyRight";
        types[HSSObservablePropertyRightBottom] = "HSSObservablePropertyRightBottom";
        types[HSSObservablePropertyBottom] = "HSSObservablePropertyBottom";
        types[HSSObservablePropertyLeftBottom] = "HSSObservablePropertyLeftBottom";

        //HSSEvent
        types[HSSObservablePropertyAction] = "HSSObservablePropertyAction";

        //HSSRequest
        types[HSSObservablePropertySrc] = "HSSObservablePropertySrc";
        types[HSSObservablePropertyTarget] = "HSSObservablePropertyTarget";
        types[HSSObservablePropertyMode] = "HSSObservablePropertyMode";
        types[HSSObservablePropertyContentTarget] = "HSSObservablePropertyContentTarget";

        //various
        types[HSSObservablePropertyTreeChange] = "HSSObservablePropertyTreeChange";

        //HSSPolygon
        types[HSSObservablePropertySides] = "HSSObservablePropertySides";
        types[HSSObservablePropertyAngle] = "HSSObservablePropertyAngle";
    }

    return types[property].toStdString();
}

HSSObservableProperty HSSObservable::observablePropertyFromString(std::string name)
{
    static QMap<std::string, HSSObservableProperty> properties;

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

void HSSObservable::observe(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object, HSSCallback *callback)
{
    //std_log1("added observer: "+object->name);
    std::size_t hash = hash_combine(qHash(object), qHash(source));

    if (this->_propertyObservers.count(target) != 0)
    {
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        theObserved[hash] = callback;
        axr_log(AXR_DEBUG_CH_OBSERVING, "added observer for " + HSSObservable::observablePropertyStringRepresentation(target));
    }
    else
    {
        HSSObservable::observed theObserved;
        theObserved[hash] = callback;
        this->_propertyObservers[target] = theObserved;
        axr_log(AXR_DEBUG_CH_OBSERVING, "added observer for new " + HSSObservable::observablePropertyStringRepresentation(target));
    }
}

void HSSObservable::removeObserver(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object)
{
    if (this->_propertyObservers.find(target) != this->_propertyObservers.end())
    {
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        std::size_t hash = hash_combine(qHash(object), qHash(source));
        if (theObserved.count(hash) != 0)
        {
            delete theObserved[hash];
            theObserved.remove(hash);
            axr_log(AXR_DEBUG_CH_OBSERVING, "removing observer for " + HSSObservable::observablePropertyStringRepresentation(target));
            return;
        }
    }
    std_log("####### tried to remove non existent observer for " + HSSObservable::observablePropertyStringRepresentation(target));
}

void HSSObservable::propertyChanged(HSSObservableProperty property, void *data)
{
    axr_log(AXR_DEBUG_CH_OBSERVING, "property changed");
}

void HSSObservable::notifyObservers(HSSObservableProperty property, void *data)
{
    if (this->_propertyObservers.count(property) != 0)
    {
        HSSObservable::observed &theObserved = this->_propertyObservers[property];
        for (HSSObservable::observed::iterator it = theObserved.begin(); it != theObserved.end(); ++it)
        {
            HSSCallback *callback = it.value();
            if (!data)
            {
                axr_log(AXR_DEBUG_CH_OBSERVING, "data is null");
            }

            callback->call(property, data);
        }
    }
}

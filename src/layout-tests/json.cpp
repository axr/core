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

//the code in this file is based on what user2243820 posted on 
//http://stackoverflow.com/questions/4169988/easiest-way-to-parse-json-in-qt-4-7

#include "json.h"

Json::Json()
{

}

Json::~Json()
{
}


QString Json::encode(const QMap<QString,QVariant> &map)
{
    QScriptEngine engine;
    engine.evaluate("function toString() { return JSON.stringify(this) }");

    QScriptValue toString = engine.globalObject().property("toString");
    QScriptValue obj = encodeInner(map, &engine);
    return toString.call(obj).toString();

}

QMap<QString, QVariant> Json::decode(const QString &jsonStr)
{
    QScriptValue object;
    QScriptEngine engine;
    object = engine.evaluate("(" + jsonStr + ")");
    return decodeInner(object);
}

QScriptValue Json::encodeInner(const QMap<QString,QVariant> &map, QScriptEngine* engine)
{
    QScriptValue obj = engine->newObject();
    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();
        if (i.value().type() == QVariant::String)
            obj.setProperty(i.key(), i.value().toString());
        else if (i.value().type() == QVariant::Int)
            obj.setProperty(i.key(), i.value().toInt());
        else if (i.value().type() == QVariant::Double)
            obj.setProperty(i.key(), i.value().toDouble());
        else if (i.value().type() == QVariant::List)
            obj.setProperty(i.key(), qScriptValueFromSequence(engine, i.value().toList()));
        else if (i.value().type() == QVariant::Map)
            obj.setProperty(i.key(), encodeInner(i.value().toMap(), engine));
    }
    return obj;
}


QMap<QString, QVariant> Json::decodeInner(QScriptValue object)
{
    QMap<QString, QVariant> map;
    QScriptValueIterator it(object);
    while (it.hasNext()) {
        it.next();
        if (it.value().isArray())
            map.insert(it.name(),QVariant(decodeInnerToList(it.value())));
        else if (it.value().isNumber())
            map.insert(it.name(),QVariant(it.value().toNumber()));
        else if (it.value().isString())
            map.insert(it.name(),QVariant(it.value().toString()));
        else if (it.value().isNull())
            map.insert(it.name(),QVariant());
        else if(it.value().isObject())
            map.insert(it.name(),QVariant(decodeInner(it.value())));
    }
    return map;
}

QList<QVariant> Json::decodeInnerToList(QScriptValue arrayValue)
{
    QList<QVariant> list;
    QScriptValueIterator it(arrayValue);
    while (it.hasNext()) {
        it.next();
        if (it.name() == "length")
            continue;

        if (it.value().isArray())
            list.append(QVariant(decodeInnerToList(it.value())));
        else if (it.value().isNumber())
            list.append(QVariant(it.value().toNumber()));
        else if (it.value().isString())
            list.append(QVariant(it.value().toString()));
        else if (it.value().isNull())
            list.append(QVariant());
        else if(it.value().isObject())
            list.append(QVariant(decodeInner(it.value())));
    }
    return list;
}
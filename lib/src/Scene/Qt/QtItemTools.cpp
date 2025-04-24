/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtItemTools.h"

#include <QDateTime>
#include <QQmlContext>
#include <QQuickItem>
#include <stdexcept>

namespace spix {
namespace qt {

const QString repeater_class_name = QString("QQuickRepeater");
const char* item_at_method_name = "itemAt";

QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index)
{
    QQuickItem* retVal = nullptr;
    bool success = QMetaObject::invokeMethod(
        repeater, item_at_method_name, Qt::DirectConnection, Q_RETURN_ARG(QQuickItem*, retVal), Q_ARG(int, index));
    if (success)
        return retVal;

    return nullptr;
}

QString GetObjectName(QObject* object)
{
    if (object == nullptr) {
        return "";
    }

    // Allow to override id with objectName
    if (!object->objectName().isEmpty()) {
        return object->objectName();
    }
    QQmlContext* const context = qmlContext(object);
    if (context) {
        return context->nameForObject(object);
    }

    return object->objectName();
}

void ForEachChild(QObject* object, const std::function<bool(QObject*)>& callback)
{
    if (!object) {
        return;
    }
    
    // Special handling for QQuickRepeater objects
    auto rootClassName = object->metaObject()->className();
    if (rootClassName == repeater_class_name) {
        QQuickItem* repeaterItem = static_cast<QQuickItem*>(object);
        
        // Iterate through repeater's generated items
        int index = 0;
        QQuickItem* child = nullptr;
        while ((child = RepeaterChildAtIndex(repeaterItem, index++)) != nullptr) {
            if (!callback(child)) {
                return; // Stop iteration if callback returns false
            }
        }
    }
    
    // Handle QQuickItems and their childItems()
    if (auto quickItem = qobject_cast<QQuickItem*>(object)) {
        for (auto* childItem : quickItem->childItems()) {
            if (!callback(childItem)) {
                return; // Stop iteration if callback returns false
            }
        }
    }
    // Handle regular QObjects and their children()
    else {
        for (auto* child : object->children()) {
            if (!callback(child)) {
                return; // Stop iteration if callback returns false
            }
        }
    }
}

QGenericReturnArgument GetReturnArgForQMetaType(int type, QMLReturnVariant& retVar)
{
    switch (type) {
    case QMetaType::Type::Void:
        retVar = nullptr;
        return QGenericReturnArgument();
    case QMetaType::Type::Bool:
        retVar = bool();
        return QReturnArgument<bool>("bool", std::get<bool>(retVar));
    case QMetaType::Type::Int:
        retVar = int();
        return QReturnArgument<int>("int", std::get<int>(retVar));
    case QMetaType::Type::Float:
        retVar = float();
        return QReturnArgument<float>("float", std::get<float>(retVar));
    case QMetaType::Type::Double:
        retVar = double();
        return QReturnArgument<double>("double", std::get<double>(retVar));
    case QMetaType::Type::QString:
        retVar = QString();
        return QReturnArgument<QString>("QString", std::get<QString>(retVar));
    case QMetaType::Type::QDateTime:
        retVar = QDateTime();
        return QReturnArgument<QDateTime>("QDateTime", std::get<QDateTime>(retVar));
    default:
        retVar = QVariant();
        return QReturnArgument<QVariant>("QVariant", std::get<QVariant>(retVar));
    }
}

QVariant VariantToQVariant(const Variant& var)
{
    static_assert(Variant::TypeIndexCount == 9, "VariantToQVariant does not cover all Variant types");

    switch (var.index()) {
    case Variant::Nullptr:
        return QVariant();
    case Variant::Bool:
        return QVariant(std::get<bool>(var));
    case Variant::Int:
        return QVariant(std::get<long long>(var));
    case Variant::Uint:
        return QVariant(std::get<unsigned long long>(var));
    case Variant::Double:
        return QVariant(std::get<double>(var));
    case Variant::String:
        return QVariant(QString::fromStdString(std::get<std::string>(var)));
    case Variant::Time: {
        auto time = std::get<std::chrono::time_point<std::chrono::system_clock>>(var);
        std::time_t timet = std::chrono::system_clock::to_time_t(time);
        return QVariant(QDateTime::fromSecsSinceEpoch(timet));
    }
    case Variant::List: {
        QVariantList list;
        for (const auto& elem : std::get<Variant::ListType>(var))
            list.push_back(VariantToQVariant(elem));
        return QVariant(list);
    }
    case Variant::Map: {
        QVariantMap map;
        for (const auto& [key, value] : std::get<Variant::MapType>(var))
            map.insert(QString::fromStdString(key), VariantToQVariant(value));
        return QVariant(map);
    }
    default:
        throw std::runtime_error("VariantToQVariant received Variant with unknown type");
    }
}

Variant QVariantToVariant(const QVariant& var)
{
    // in Qt5 QJSValue is a user type
    int varType = var.userType();
    switch (static_cast<QMetaType::Type>(varType)) {
    case QMetaType::Type::Bool:
        return Variant(var.toBool());
    case QMetaType::Type::Char:
    case QMetaType::Type::SChar:
    case QMetaType::Type::Short:
    case QMetaType::Type::Int:
    case QMetaType::Type::Long:
    case QMetaType::Type::LongLong:
        return Variant(var.toLongLong());
    case QMetaType::Type::UChar:
    case QMetaType::Type::UShort:
    case QMetaType::Type::UInt:
    case QMetaType::Type::ULong:
    case QMetaType::Type::ULongLong:
        return Variant(var.toULongLong());
    case QMetaType::Type::Float:
    case QMetaType::Type::Double:
        return Variant(var.toDouble());
    case QMetaType::Type::QDateTime: {
        std::time_t time = var.toDateTime().toSecsSinceEpoch();
        return Variant(std::chrono::system_clock::from_time_t(time));
    }
    case QMetaType::Type::QString:
        return Variant(var.toString().toStdString());
    case QMetaType::Type::Nullptr:
    case QMetaType::Type::Void:
    case QMetaType::Type::UnknownType:
        return Variant(nullptr);
    default:
        break;
    }

    if (varType == qMetaTypeId<QJSValue>()) {
        QJSValue jsval = var.value<QJSValue>();
        return QVariantToVariant(jsval.toVariant());
    }

    if (var.canConvert(QMetaType::Type::QVariantList)) {
        const QVariantList& list = var.toList();
        Variant::ListType ret;
        for (const QVariant& elem : list) {
            Variant convertedElem = QVariantToVariant(elem);
            ret.push_back(convertedElem);
        }
        return Variant(ret);
    }

    if (var.canConvert(QMetaType::Type::QVariantMap)) {
        const QVariantMap& map = var.toMap();
        Variant::MapType ret;
        for (auto ptr = map.constBegin(); ptr != map.constEnd(); ptr++) {
            Variant convertedElem = QVariantToVariant(ptr.value());
            ret[ptr.key().toStdString()] = convertedElem;
        }
        return Variant(ret);
    }

    if (var.canConvert(QMetaType::Type::QString)) {
        return Variant(var.toString().toStdString());
    }

    std::string error = std::string("QVariantToVariant: Received unknown type for return value: ") + var.typeName();
    throw std::runtime_error(error);
}

Variant QMLReturnVariantToVariant(const QMLReturnVariant& var)
{
    return std::visit(
        [](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
                return Variant(static_cast<long long>(arg));
            } else if constexpr (std::is_unsigned_v<T> && !std::is_same_v<T, bool>) {
                return Variant(static_cast<unsigned long long>(arg));
            } else if constexpr (std::is_floating_point_v<T>) {
                return Variant(static_cast<double>(arg));
            } else if constexpr (std::is_same_v<T, QString>) {
                return Variant(arg.toStdString());
            } else if constexpr (std::is_same_v<T, QDateTime>) {
                return Variant(std::chrono::system_clock::from_time_t(arg.toLocalTime().toSecsSinceEpoch()));
            } else if constexpr (std::is_same_v<T, QVariant>) {
                return QVariantToVariant(arg);
            } else {
                return Variant(arg);
            }
        },
        var);
}

bool CanConvertArgTypes(const QMetaMethod& metaMethod, const std::vector<QVariant>& varargs)
{
    if (metaMethod.parameterCount() != varargs.size())
        return false;
    for (size_t i = 0; i < metaMethod.parameterCount(); i++) {
        int targetType = metaMethod.parameterType(i);
        if (targetType != QMetaType::Type::QVariant && !varargs[i].canConvert(targetType))
            return false;
    }
    return true;
}

bool GetMethodMetaForArgs(
    const QObject& obj, const std::string& method, const std::vector<QVariant>& varargs, QMetaMethod& ret)
{
    const QMetaObject* itemMeta = obj.metaObject();
    for (size_t i = 0; i < itemMeta->methodCount(); i++) {
        const QMetaMethod methodMeta = itemMeta->method(i);
        if (methodMeta.name().compare(method.data()) == 0 && CanConvertArgTypes(methodMeta, varargs)) {
            ret = methodMeta;
            return true;
        }
    }
    return false;
}

std::vector<QGenericArgument> ConvertAndCreateQArgumentsForMethod(
    const QMetaMethod& metaMethod, std::vector<QVariant>& varargs)
{
    std::vector<QGenericArgument> qtArgs;
    for (size_t i = 0; i < 10; i++) {
        if (i < varargs.size()) {
            int targetType = metaMethod.parameterType(i);
            if (targetType != QMetaType::Type::QVariant) {
                varargs[i].convert(targetType);
                qtArgs.push_back(QGenericArgument(varargs[i].typeName(), varargs[i].data()));
            } else {
                qtArgs.push_back(QArgument<QVariant>("QVariant", varargs[i]));
            }
        } else
            qtArgs.push_back(QGenericArgument());
    }
    return qtArgs;
}

} // namespace qt
} // namespace spix

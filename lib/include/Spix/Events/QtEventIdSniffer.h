/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <QObject>

#include <QEvent>
#include <QMouseEvent>
#include <QQmlContext>
#include <QQmlEngine>

#include <iostream>

namespace spix {

/**
 * Object used for development to inspect object ids. It reacts on mouse events.
 * The id path of the clicked item is written to stdout.
 *
 * Example Usage: app.installEventFilter(new spix::QtEventIdSniffer());
 */
class QtEventIdSniffer : public QObject {
public:
    bool eventFilter(QObject* object, QEvent* event)
    {
        if (event->type() == QEvent::MouseButtonPress) {
            auto tempObj = object;
            std::string path = "";

            while (tempObj != nullptr) {
                // take object name if given
                auto token = tempObj->objectName().toStdString();
                if (token.empty()) {
                    QQmlContext* const context = qmlContext(tempObj);
                    if (context) {
                        // use object id
                        token = context->nameForObject(tempObj).toStdString();
                    }
                }

                // add id to front
                if (!token.empty()) {
                    path = token + "/" + path;
                }
                tempObj = tempObj->parent();
            }

            std::cout << "path to clicked object: " << path << std::endl;
        }

        // always ignore event
        return false;
    }
};

} // namespace spix

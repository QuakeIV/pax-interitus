/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt for Python.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef SBK_QTDBUS_PYTHON_H
#define SBK_QTDBUS_PYTHON_H

//workaround to access protected functions
#define protected public

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtcore_python.h>

// Bound library includes
#include <QtDBus/qdbusargument.h>
#include <QtDBus/qdbusextratypes.h>
#include <QtDBus/qdbusconnectioninterface.h>
#include <QtDBus/qdbusunixfiledescriptor.h>
#include <QtDBus/qdbusinterface.h>
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbusserver.h>
#include <qtdbushelper.h>
#include <QtDBus/qdbuserror.h>
#include <QtDBus/qdbuspendingcall.h>
#include <QtDBus/qdbusabstractadaptor.h>
#include <QtDBus/qdbuscontext.h>
#include <QtDBus/qdbusmessage.h>
#include <QtDBus/qdbusvirtualobject.h>
#include <QtDBus/qdbusservicewatcher.h>
#include <QtDBus/qdbusabstractinterface.h>
// Conversion Includes - Primitive Types
#include <QAnyStringView>
#include <qabstractitemmodel.h>
#include <QString>
#include <QStringList>
#include <QStringView>

// Conversion Includes - Container Types
#include <pysideqflags.h>
#include <QList>
#include <QMap>
#include <pysideqflags.h>
#include <QMultiMap>
#include <QPair>
#include <QQueue>
#include <QSet>
#include <QStack>

// Type indices
enum : int {
    SBK_QDBUS_CALLMODE_IDX                                   = 1,
    SBK_QtDBusQDBUS_IDX                                      = 0,
    SBK_QDBUSABSTRACTADAPTOR_IDX                             = 2,
    SBK_QDBUSABSTRACTINTERFACE_IDX                           = 3,
    SBK_QDBUSABSTRACTINTERFACEBASE_IDX                       = 4,
    SBK_QDBUSARGUMENT_ELEMENTTYPE_IDX                        = 6,
    SBK_QDBUSARGUMENT_IDX                                    = 5,
    SBK_QDBUSCONNECTION_BUSTYPE_IDX                          = 8,
    SBK_QDBUSCONNECTION_REGISTEROPTION_IDX                   = 10,
    SBK_QFLAGS_QDBUSCONNECTION_REGISTEROPTION_IDX            = 34,
    SBK_QDBUSCONNECTION_UNREGISTERMODE_IDX                   = 11,
    SBK_QDBUSCONNECTION_VIRTUALOBJECTREGISTEROPTION_IDX      = 12,
    SBK_QFLAGS_QDBUSCONNECTION_VIRTUALOBJECTREGISTEROPTION_IDX = 35,
    SBK_QDBUSCONNECTION_CONNECTIONCAPABILITY_IDX             = 9,
    SBK_QFLAGS_QDBUSCONNECTION_CONNECTIONCAPABILITY_IDX      = 33,
    SBK_QDBUSCONNECTION_IDX                                  = 7,
    SBK_QDBUSCONNECTIONINTERFACE_SERVICEQUEUEOPTIONS_IDX     = 15,
    SBK_QDBUSCONNECTIONINTERFACE_SERVICEREPLACEMENTOPTIONS_IDX = 16,
    SBK_QDBUSCONNECTIONINTERFACE_REGISTERSERVICEREPLY_IDX    = 14,
    SBK_QDBUSCONNECTIONINTERFACE_IDX                         = 13,
    SBK_QDBUSCONTEXT_IDX                                     = 17,
    SBK_QDBUSERROR_ERRORTYPE_IDX                             = 19,
    SBK_QDBUSERROR_IDX                                       = 18,
    SBK_QDBUSINTERFACE_IDX                                   = 20,
    SBK_QDBUSMESSAGE_MESSAGETYPE_IDX                         = 22,
    SBK_QDBUSMESSAGE_IDX                                     = 21,
    SBK_QDBUSOBJECTPATH_IDX                                  = 23,
    SBK_QDBUSPENDINGCALL_IDX                                 = 24,
    SBK_QDBUSPENDINGCALLWATCHER_IDX                          = 25,
    SBK_QDBUSSERVER_IDX                                      = 26,
    SBK_QDBUSSERVICEWATCHER_WATCHMODEFLAG_IDX                = 28,
    SBK_QFLAGS_QDBUSSERVICEWATCHER_WATCHMODEFLAG_IDX         = 36,
    SBK_QDBUSSERVICEWATCHER_IDX                              = 27,
    SBK_QDBUSSIGNATURE_IDX                                   = 29,
    SBK_QDBUSUNIXFILEDESCRIPTOR_IDX                          = 30,
    SBK_QDBUSVARIANT_IDX                                     = 31,
    SBK_QDBUSVIRTUALOBJECT_IDX                               = 32,
    SBK_QTDBUSHELPER_QDBUSREPLY_IDX                          = 38,
    SBK_QtDBus_IDX_COUNT                                     = 39
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtDBusTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtDBusModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtDBusTypeConverters;

// Converter indices
enum : int {
    SBK_QTDBUS_QLIST_QVARIANT_IDX                            = 0, // QList<QVariant >
    SBK_QTDBUS_QHASH_QSTRING_QVARIANT_IDX                    = 1, // QHash<QString,QVariant >
    SBK_QTDBUS_QMAP_QSTRING_QVARIANT_IDX                     = 2, // QMap<QString,QVariant >
    SBK_QTDBUS_QLIST_QOBJECTPTR_IDX                          = 3, // QList<QObject* >
    SBK_QTDBUS_QLIST_QBYTEARRAY_IDX                          = 4, // QList<QByteArray >
    SBK_QTDBUS_QLIST_QSTRING_IDX                             = 5, // QList<QString >
    SBK_QtDBus_CONVERTERS_IDX_COUNT                          = 6
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QDBus::CallMode >() { return SbkPySide6_QtDBusTypes[SBK_QDBUS_CALLMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusAbstractAdaptor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSABSTRACTADAPTOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusAbstractInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSABSTRACTINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusAbstractInterfaceBase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSABSTRACTINTERFACEBASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusArgument::ElementType >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSARGUMENT_ELEMENTTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusArgument >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSARGUMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusConnection::BusType >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTION_BUSTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnection::RegisterOption >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTION_REGISTEROPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QDBusConnection::RegisterOption> >() { return SbkPySide6_QtDBusTypes[SBK_QFLAGS_QDBUSCONNECTION_REGISTEROPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnection::UnregisterMode >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTION_UNREGISTERMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnection::VirtualObjectRegisterOption >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTION_VIRTUALOBJECTREGISTEROPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QDBusConnection::VirtualObjectRegisterOption> >() { return SbkPySide6_QtDBusTypes[SBK_QFLAGS_QDBUSCONNECTION_VIRTUALOBJECTREGISTEROPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnection::ConnectionCapability >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTION_CONNECTIONCAPABILITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QDBusConnection::ConnectionCapability> >() { return SbkPySide6_QtDBusTypes[SBK_QFLAGS_QDBUSCONNECTION_CONNECTIONCAPABILITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnection >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusConnectionInterface::ServiceQueueOptions >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTIONINTERFACE_SERVICEQUEUEOPTIONS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnectionInterface::ServiceReplacementOptions >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTIONINTERFACE_SERVICEREPLACEMENTOPTIONS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnectionInterface::RegisterServiceReply >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTIONINTERFACE_REGISTERSERVICEREPLY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusConnectionInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSCONNECTIONINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusContext >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSCONTEXT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusError::ErrorType >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSERROR_ERRORTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusError >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSERROR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusMessage::MessageType >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSMESSAGE_MESSAGETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusMessage >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSMESSAGE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusObjectPath >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSOBJECTPATH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusPendingCall >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSPENDINGCALL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusPendingCallWatcher >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSPENDINGCALLWATCHER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusServer >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSSERVER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusServiceWatcher::WatchModeFlag >() { return SbkPySide6_QtDBusTypes[SBK_QDBUSSERVICEWATCHER_WATCHMODEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QDBusServiceWatcher::WatchModeFlag> >() { return SbkPySide6_QtDBusTypes[SBK_QFLAGS_QDBUSSERVICEWATCHER_WATCHMODEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDBusServiceWatcher >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSSERVICEWATCHER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusSignature >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSSIGNATURE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusUnixFileDescriptor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSUNIXFILEDESCRIPTOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusVariant >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSVARIANT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDBusVirtualObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QDBUSVIRTUALOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QtDBusHelper::QDBusReply >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDBusTypes[SBK_QTDBUSHELPER_QDBUSREPLY_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTDBUS_PYTHON_H


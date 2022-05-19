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


#ifndef SBK_QTNETWORKAUTH_PYTHON_H
#define SBK_QTNETWORKAUTH_PYTHON_H

//workaround to access protected functions
#define protected public

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtnetwork_python.h>
#include <pyside6_qtcore_python.h>

// Bound library includes
#include <QtNetworkAuth/qoauth1signature.h>
#include <QtNetworkAuth/qoauthoobreplyhandler.h>
#include <QtNetworkAuth/qabstractoauthreplyhandler.h>
#include <QtNetworkAuth/qoauth1.h>
#include <QtNetworkAuth/qoauth2authorizationcodeflow.h>
#include <QtNetworkAuth/qabstractoauth2.h>
#include <QtNetworkAuth/qoauthhttpserverreplyhandler.h>
#include <QtNetworkAuth/qabstractoauth.h>
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
    SBK_QABSTRACTOAUTH_STATUS_IDX                            = 5,
    SBK_QABSTRACTOAUTH_STAGE_IDX                             = 4,
    SBK_QABSTRACTOAUTH_ERROR_IDX                             = 3,
    SBK_QABSTRACTOAUTH_CONTENTTYPE_IDX                       = 2,
    SBK_QABSTRACTOAUTH_IDX                                   = 0,
    SBK_QABSTRACTOAUTH2_IDX                                  = 1,
    SBK_QABSTRACTOAUTHREPLYHANDLER_IDX                       = 6,
    SBK_QOAUTH1_SIGNATUREMETHOD_IDX                          = 8,
    SBK_QOAUTH1_IDX                                          = 7,
    SBK_QOAUTH1SIGNATURE_HTTPREQUESTMETHOD_IDX               = 10,
    SBK_QOAUTH1SIGNATURE_IDX                                 = 9,
    SBK_QOAUTH2AUTHORIZATIONCODEFLOW_IDX                     = 11,
    SBK_QOAUTHHTTPSERVERREPLYHANDLER_IDX                     = 12,
    SBK_QOAUTHOOBREPLYHANDLER_IDX                            = 13,
    SBK_QtNetworkAuth_IDX_COUNT                              = 14
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtNetworkAuthTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtNetworkAuthModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtNetworkAuthTypeConverters;

// Converter indices
enum : int {
    SBK_QTNETWORKAUTH_QMAP_QSTRING_QVARIANT_IDX              = 0, // QMap<QString,QVariant >
    SBK_QTNETWORKAUTH_QLIST_QOBJECTPTR_IDX                   = 1, // QList<QObject* >
    SBK_QTNETWORKAUTH_QLIST_QBYTEARRAY_IDX                   = 2, // QList<QByteArray >
    SBK_QTNETWORKAUTH_QMULTIMAP_QSTRING_QVARIANT_IDX         = 3, // QMultiMap<QString,QVariant >
    SBK_QTNETWORKAUTH_QPAIR_QSTRING_QSTRING_IDX              = 4, // QPair<QString,QString >
    SBK_QTNETWORKAUTH_QLIST_QSTRING_IDX                      = 5, // QList<QString >
    SBK_QTNETWORKAUTH_QLIST_QVARIANT_IDX                     = 6, // QList<QVariant >
    SBK_QtNetworkAuth_CONVERTERS_IDX_COUNT                   = 7
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QAbstractOAuth::Status >() { return SbkPySide6_QtNetworkAuthTypes[SBK_QABSTRACTOAUTH_STATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractOAuth::Stage >() { return SbkPySide6_QtNetworkAuthTypes[SBK_QABSTRACTOAUTH_STAGE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractOAuth::Error >() { return SbkPySide6_QtNetworkAuthTypes[SBK_QABSTRACTOAUTH_ERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractOAuth::ContentType >() { return SbkPySide6_QtNetworkAuthTypes[SBK_QABSTRACTOAUTH_CONTENTTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractOAuth >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QABSTRACTOAUTH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractOAuth2 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QABSTRACTOAUTH2_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractOAuthReplyHandler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QABSTRACTOAUTHREPLYHANDLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOAuth1::SignatureMethod >() { return SbkPySide6_QtNetworkAuthTypes[SBK_QOAUTH1_SIGNATUREMETHOD_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOAuth1 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QOAUTH1_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOAuth1Signature::HttpRequestMethod >() { return SbkPySide6_QtNetworkAuthTypes[SBK_QOAUTH1SIGNATURE_HTTPREQUESTMETHOD_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOAuth1Signature >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QOAUTH1SIGNATURE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOAuth2AuthorizationCodeFlow >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QOAUTH2AUTHORIZATIONCODEFLOW_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOAuthHttpServerReplyHandler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QOAUTHHTTPSERVERREPLYHANDLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOAuthOobReplyHandler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtNetworkAuthTypes[SBK_QOAUTHOOBREPLYHANDLER_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTNETWORKAUTH_PYTHON_H


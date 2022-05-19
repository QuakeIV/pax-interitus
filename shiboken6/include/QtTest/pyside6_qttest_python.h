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


#ifndef SBK_QTTEST_PYTHON_H
#define SBK_QTTEST_PYTHON_H

//workaround to access protected functions
#define protected public

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtcore_python.h>
#include <pyside6_qtgui_python.h>
#include <pyside6_qtwidgets_python.h>

// Bound library includes
#include <QtTest/qttestglobal.h>
#include <QtTest/qtestkeyboard.h>
#include <QtTest/qtestmouse.h>
#include <pysideqtesttouch.h>
#include <QtTest/qabstractitemmodeltester.h>
#include <QtTest/qsignalspy.h>
#include <QtTest/qbenchmarkmetric.h>
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
    SBK_QABSTRACTITEMMODELTESTER_FAILUREREPORTINGMODE_IDX    = 1,
    SBK_QABSTRACTITEMMODELTESTER_IDX                         = 0,
    SBK_QSIGNALSPY_IDX                                       = 2,
    SBK_QTEST_TESTFAILMODE_IDX                               = 8,
    SBK_QTEST_QBENCHMARKMETRIC_IDX                           = 7,
    SBK_QTEST_KEYACTION_IDX                                  = 4,
    SBK_QTEST_MOUSEACTION_IDX                                = 5,
    SBK_QtTestQTEST_IDX                                      = 3,
    SBK_QTEST_PYSIDEQTOUCHEVENTSEQUENCE_IDX                  = 6,
    SBK_QtTest_IDX_COUNT                                     = 9
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtTestTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtTestModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtTestTypeConverters;

// Converter indices
enum : int {
    SBK_QTTEST_QLIST_QVARIANT_IDX                            = 0, // QList<QVariant >
    SBK_QTTEST_QLIST_QOBJECTPTR_IDX                          = 1, // QList<QObject* >
    SBK_QTTEST_QLIST_QBYTEARRAY_IDX                          = 2, // QList<QByteArray >
    SBK_QTTEST_QLIST_QSTRING_IDX                             = 3, // QList<QString >
    SBK_QTTEST_QMAP_QSTRING_QVARIANT_IDX                     = 4, // QMap<QString,QVariant >
    SBK_QtTest_CONVERTERS_IDX_COUNT                          = 5
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QAbstractItemModelTester::FailureReportingMode >() { return SbkPySide6_QtTestTypes[SBK_QABSTRACTITEMMODELTESTER_FAILUREREPORTINGMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractItemModelTester >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtTestTypes[SBK_QABSTRACTITEMMODELTESTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSignalSpy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtTestTypes[SBK_QSIGNALSPY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTest::TestFailMode >() { return SbkPySide6_QtTestTypes[SBK_QTEST_TESTFAILMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTest::QBenchmarkMetric >() { return SbkPySide6_QtTestTypes[SBK_QTEST_QBENCHMARKMETRIC_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTest::KeyAction >() { return SbkPySide6_QtTestTypes[SBK_QTEST_KEYACTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTest::MouseAction >() { return SbkPySide6_QtTestTypes[SBK_QTEST_MOUSEACTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTest::PySideQTouchEventSequence >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtTestTypes[SBK_QTEST_PYSIDEQTOUCHEVENTSEQUENCE_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTTEST_PYTHON_H


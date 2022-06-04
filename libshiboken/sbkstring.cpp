/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "sbkstring.h"
#include "sbkstaticstrings_p.h"
#include "autodecref.h"

#include <vector>
#include <unordered_set>

namespace Shiboken::String
{

// PYSIDE-795: Redirecting PySequence to Iterable
bool checkIterable(PyObject *obj)
{
    return PyObject_HasAttr(obj, Shiboken::PyMagicName::iter());
}

static PyObject *initPathLike()
{
    PyObject *PathLike{};
    auto osmodule = PyImport_ImportModule("os");
    if (osmodule == nullptr
        || (PathLike = PyObject_GetAttrString(osmodule, "PathLike")) == nullptr) {
        PyErr_Print();
        Py_FatalError("cannot import os.PathLike");
    }
    return PathLike;
}

// PYSIDE-1499: Migrate to pathlib.Path and support __fspath__ in PySide
bool checkPath(PyObject *path)
{
    // Let normal strings through, unchanged.
    if (PyUnicode_Check(path) || PyBytes_Check(path))
        return true;
    // Without the Limited API, we could look up an `__fspath__` class attribute.
    // But we use `isinstance(os.PathLike)`, instead.
    static PyObject *PathLike = initPathLike();
    return PyObject_IsInstance(path, PathLike);
}

bool checkType(PyTypeObject *type)
{
    return type == &PyUnicode_Type;
}

bool check(PyObject *obj)
{
    return obj == Py_None || PyUnicode_Check(obj);
}

bool checkChar(PyObject *pyobj)
{
    return check(pyobj) && (len(pyobj) == 1);
}

bool isConvertible(PyObject *obj)
{
    return check(obj);
}

PyObject *fromCString(const char *value)
{
    return PyUnicode_FromString(value);
}

PyObject *fromCString(const char *value, int len)
{
    return PyUnicode_FromStringAndSize(value, len);
}

const char *toCString(PyObject *str, Py_ssize_t *len)
{
    if (str == Py_None)
        return nullptr;
    if (PyUnicode_Check(str)) {
        if (len) {
            // We need to encode the unicode string into utf8 to know the size of returned char *.
            Shiboken::AutoDecRef uniStr(PyUnicode_AsUTF8String(str));
            *len = PyBytes_GET_SIZE(uniStr.object());
        }
        // Return unicode from str instead of uniStr, because the lifetime of the returned pointer
        // depends on the lifetime of str.
        return _PepUnicode_AsString(str);
    }
    if (PyBytes_Check(str)) {
        if (len)
            *len = PyBytes_GET_SIZE(str);
        return PyBytes_AS_STRING(str);
    }
    return nullptr;
}

bool concat(PyObject **val1, PyObject *val2)
{
    if (PyUnicode_Check(*val1) && PyUnicode_Check(val2)) {
        PyObject *result = PyUnicode_Concat(*val1, val2);
        Py_DECREF(*val1);
        *val1 = result;
        return true;
    }

    if (PyBytes_Check(*val1) && PyBytes_Check(val2)) {
        PyBytes_Concat(val1, val2);
        return true;
    }

    return false;
}

PyObject *fromFormat(const char *format, ...)
{
    va_list argp;
    va_start(argp, format);
    PyObject *result = nullptr;
    result = PyUnicode_FromFormatV(format, argp);
    va_end(argp);
    return result;
}

PyObject *fromStringAndSize(const char *str, Py_ssize_t size)
{
    return PyUnicode_FromStringAndSize(str, size);
}

int compare(PyObject *val1, const char *val2)
{
    if (PyUnicode_Check(val1))
       return PyUnicode_CompareWithASCIIString(val1, val2);
    return 0;

}

Py_ssize_t len(PyObject *str)
{
    if (str == Py_None)
        return 0;

    if (PyUnicode_Check(str))
        return PepUnicode_GetLength(str);

    if (PyBytes_Check(str))
        return PyBytes_GET_SIZE(str);
    return 0;
}

///////////////////////////////////////////////////////////////////////
//
// Implementation of efficient Python strings
// ------------------------------------------
//
// Instead of repetitively executing
//
//     PyObject *attr = PyObject_GetAttrString(obj, "__name__");
//
// a helper of the form
//
// PyObject *name()
// {
//    static PyObject *const s = Shiboken::String::createStaticString("__name__");
//    return result;
// }
//
// can now be implemented, which registers the string into a static set avoiding
// repetitive string creation. The resulting code looks like:
//
//     PyObject *attr = PyObject_GetAttr(obj, name());
//

using StaticStrings = std::unordered_set<PyObject *>;

static void finalizeStaticStrings();    // forward

static StaticStrings &staticStrings()
{
    static StaticStrings result;
    return result;
}

static void finalizeStaticStrings()
{
    auto &set = staticStrings();
    for (PyObject *ob : set) {
        Py_SET_REFCNT(ob, 1);
        Py_DECREF(ob);
    }
    set.clear();
}

PyObject *createStaticString(const char *str)
{
    static bool initialized = false;
    if (!initialized) {
        Py_AtExit(finalizeStaticStrings);
        initialized = true;
    }
    PyObject *result = PyUnicode_InternFromString(str);
    if (result == nullptr) {
        // This error is never checked, but also very unlikely. Report and exit.
        PyErr_Print();
        Py_FatalError("unexpected error in createStaticString()");
    }
    auto it = staticStrings().find(result);
    if (it == staticStrings().end())
        staticStrings().insert(result);
    /*
     * Note: We always add one reference even if we have a new string.
     *       This makes the strings immortal, and we are safe if someone
     *       uses AutoDecRef, although the set cannot cope with deletions.
     *       The exit handler cleans that up, anyway.
     */
    Py_INCREF(result);
    return result;
}

///////////////////////////////////////////////////////////////////////
//
// PYSIDE-1019: Helper function for snake_case vs. camelCase names
// ---------------------------------------------------------------
//
// When renaming dict entries, `BindingManager::getOverride` must
// use adapted names.
//
// This might become more complex when we need to register
// exceptions from this rule.
//

PyObject *getSnakeCaseName(const char *name, bool lower)
{
    /*
     * Convert `camelCase` to `snake_case`.
     * Gives up when there are two consecutive upper chars.
     *
     * Also functions beginning with `gl` followed by upper case stay
     * unchanged since that are the special OpenGL functions.
     */
    if (!lower
        || strlen(name) < 3
        || (name[0] == 'g' && name[1] == 'l' && isupper(name[2])))
        return createStaticString(name);

    char new_name[200 + 1] = {};
    const char *p = name;
    char *q = new_name;
    for (; *p && q - new_name < 200; ++p, ++q) {
        if (isupper(*p)) {
            if (p != name && isupper(*(p - 1)))
                return createStaticString(name);
            *q = '_';
            ++q;
            *q = tolower(*p);
        }
        else {
            *q = *p;
        }
    }
    return createStaticString(new_name);
}

PyObject *getSnakeCaseName(PyObject *name, bool lower)
{
    // This is all static strings, not refcounted.
    if (lower)
        return getSnakeCaseName(toCString(name), lower);
    return name;
}

} // namespace Shiboken::String

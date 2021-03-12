/**********************************************************************************
 *   30/01/2015                                                                   *
 *                                                                                *
 *   www.henesis.eu                                                               *
 *                                                                                *
 *   Alessandro Bacchini - alessandro.bacchini@henesis.eu                         *
 *                                                                                *
 * Copyright (c) 2015, Henesis s.r.l. part of Camlin Group                        *
 *                                                                                *
 * The MIT License (MIT)                                                          *
 *                                                                                *
 * Permission is here by granted, free of charge, to any person obtaining a copy  *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 *********************************************************************************/

#ifndef PYERG_H
#define PYERG_H

// Workaround for some compiler that need cmath included before python
#include <cmath>

#include <Python.h>
#include <structmember.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include "erg.h"
#include "pyerg_docstrings.h"

#define PyFUNC extern "C" PyObject*


typedef struct {
    PyObject_HEAD
    erg::Reader* parser;    //!< The parser C++ implementation
} Reader;

extern "C" void Parser_dealloc(Reader* self);
PyFUNC Parser_new(PyTypeObject* type, PyObject *args, PyObject *kwds);
extern "C" int Parser_init(Reader* self, PyObject *args, PyObject *kwds);
PyFUNC Parser_open(Reader* self, PyObject *args);
PyFUNC Parser_records(Reader* self);
PyFUNC Parser_recordSize(Reader* self);
PyFUNC Parser_numQuanities(Reader* self);
PyFUNC Parser_readAll(Reader* self);
PyFUNC Parser_read(Reader* self, PyObject *args, PyObject *keywds);
PyFUNC Parser_quantitySize(Reader* self, PyObject* arg);
PyFUNC Parser_quantityName(Reader* self, PyObject* arg);
PyFUNC Parser_quantityType(Reader* self, PyObject* arg);
PyFUNC Parser_quantityUnit(Reader* self, PyObject* arg);
PyFUNC Parser_index(Reader* self, PyObject* arg);
PyFUNC Parser_isErg(Reader* self);
PyFUNC Parser_isFortran(Reader* self);
PyFUNC Parser_has(Reader* self, PyObject* arg);
PyFUNC Parser_close(Reader* self);

static PyMethodDef parser_methods[] = {
    {
        "open", (PyCFunction)Parser_open, METH_O,
        PYERG_PARSER_OPEN_DOC
    },
    {
        "records", (PyCFunction)Parser_records, METH_NOARGS,
        PYERG_PARSER_RECORDS_DOC
    },
    {
        "recordSize", (PyCFunction)Parser_recordSize, METH_NOARGS,
        PYERG_PARSER_RECORDSIZE_DOC
    },
    {
        "numQuanities", (PyCFunction)Parser_numQuanities, METH_NOARGS,
        PYERG_PARSER_NUMQUANITIES_DOC
    },
    {
        "readAll", (PyCFunction)Parser_readAll, METH_NOARGS,
        PYERG_PARSER_READALL_DOC
    },
    {
        "read", (PyCFunction)Parser_read, METH_VARARGS|METH_KEYWORDS,
        PYERG_PARSER_READ_DOC
    },
    {
        "quantitySize", (PyCFunction)Parser_quantitySize, METH_O,
        PYERG_PARSER_QUANTITYSIZE_DOC
    },
    {
        "quantityName", (PyCFunction)Parser_quantityName, METH_O,
        PYERG_PARSER_QUANTITYNAME_DOC
    },
    {
        "quantityType", (PyCFunction)Parser_quantityType, METH_O,
        PYERG_PARSER_QUANTITYTYPE_DOC
    },
    {
        "quantityUnit", (PyCFunction)Parser_quantityUnit, METH_O,
        PYERG_PARSER_QUANTITYUNIT_DOC
    },
    {
        "index", (PyCFunction)Parser_index, METH_O,
        PYERG_PARSER_INDEX_DOC
    },
    {
        "isErg", (PyCFunction)Parser_isErg, METH_NOARGS,
        PYERG_PARSER_ISERG_DOC
    },
    {
        "isFortran", (PyCFunction)Parser_isFortran, METH_NOARGS,
        PYERG_PARSER_ISFORTRAN_DOC
    },
    {
        "has", (PyCFunction)Parser_has, METH_O,
        PYERG_PARSER_HAS_DOC
    },
    {
        "close", (PyCFunction)Parser_close, METH_NOARGS,
        PYERG_PARSER_CLOSE_DOC
    },
    {nullptr}  /* Sentinel */
};

static PyTypeObject pyerg_ReaderType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyerg.Reader",            /*tp_name*/
    sizeof(Reader),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Parser_dealloc,            /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    PYERG_MODULE_DOC,      /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    parser_methods,            /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Parser_init,     /* tp_init */
    0,                         /* tp_alloc */
    Parser_new,                 /* tp_new */
};

PyFUNC py_read(PyObject* self, PyObject* filename);
PyFUNC py_can_read(PyObject* self, PyObject* filename);

static PyMethodDef pyerg_methods[] = {
    {
        "read",
        py_read,
        METH_O,
        PYERG_READ_DOC
    },
    {
        "can_read",
        py_can_read,
        METH_O,
        PYERG_CAN_READ_DOC
    },
    {nullptr}
};

#endif  // PYERG_H

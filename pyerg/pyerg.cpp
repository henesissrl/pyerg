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

#include "pyerg.h"


/*!
 * \brief Convert ERG type to numpy type.
 * \param t ERG type.
 * \return Numpy type.
 * \throws Exception if ERG type is Void.
 */
static int ergType2npyType(const erg::Type t)
{
    switch (t)
    {
    case erg::Type::Int8:
        return NPY_BYTE;
    case erg::Type::Uint8:
        return NPY_UBYTE;
    case erg::Type::Int16:
        return NPY_SHORT;
    case erg::Type::Uint16:
        return NPY_USHORT;
    case erg::Type::Int32:
        return NPY_INT;
    case erg::Type::Uint32:
        return NPY_UINT;
    case erg::Type::Int64:
        return NPY_LONGLONG;
    case erg::Type::Uint64:
        return NPY_ULONGLONG;
    case erg::Type::Float:
        return NPY_FLOAT;
    case erg::Type::Double:
        return NPY_DOUBLE;
    case erg::Type::Void:
    default:
        throw std::runtime_error("Unknown data type.");
    }
}

/*!
 * \brief Quantity index from a PyObject.
 *
 * If errors happens during the parsing, a PyErr_SetString attribute is set: use the
 * PyErr_Occurred() to check if the returned value is valid or not.
 * \param arg PyObject representing an int (quantity index) or a string (quantity name).
 * \return The quantity index.
 */
static size_t indexFromPyObject(erg::Reader* parser, PyObject* arg)
{
    size_t qindex = 0;
    try {
        // Check if the quantity is a string or an integer
        if(PyUnicode_Check(arg)) {
            // Find the quantity id
            const char* qname = PyUnicode_AsUTF8(arg);
            qindex = parser->index(std::string(qname));
        } else if(PyLong_Check(arg)){
            qindex = PyLong_AsLong(arg);
        } else {
            PyErr_SetString(PyExc_NameError, "The argument must be a quantity name or index.");
            return 0;
        }
    } catch (std::runtime_error& IndexError) {
        PyErr_SetString(PyExc_NameError, IndexError.what());
        return 0;
    }
    return qindex;
}


PyFUNC py_read(PyObject* self, PyObject* filename)
{
    PyObject* args = PyTuple_New(0);
    Reader* pyReader = (Reader*)PyObject_CallObject((PyObject*)&pyerg_ReaderType, args);
    Py_DecRef(args);

    PyObject* ret = Parser_open(pyReader, filename);
    if(ret==nullptr)
        return nullptr;

    Py_DecRef(ret);

    PyObject* data = Parser_readAll(pyReader);
    Py_DecRef((PyObject*)pyReader);
    return data;

    /*
    // self is unused.
    if(PyUnicode_Check(filename)==false) {
        PyErr_SetString(PyExc_NameError, "The input arguments must be a string");
        return nullptr;
    }

    const char* filenameStr = PyUnicode_AsUTF8(filename);
    erg::Reader parser;

    // Error variable: used to check if C++ exceptions are thrown.
    std::string error;

    // Release the GIL because the open function is an I/O
    // operation that read a file.
    Py_BEGIN_ALLOW_THREADS;
        try {
            parser.open(filenameStr);
        } catch (std::runtime_error e) {
            error = e.what();
        }
    // Reacquire the GIL
    Py_END_ALLOW_THREADS;

    // Check for exceptions
    if(error.length()>0) {
        PyErr_SetString(PyExc_NameError, error.c_str());
        return nullptr;
    }

    // Allocate a Dict of numpy arrays as the returned value.
    // Load all the numpy array raw data pointer for passing to
    // the erg::Parser::readAll() function.
    const size_t numQuantities = parser.numQuanities();
    npy_intp rows = parser.records();
    std::vector<uint8_t*> dataWrapper;
    std::vector<size_t> sizeWrapper;
    PyObject* map = PyDict_New();
    for(size_t i=0; i<numQuantities; ++i)
    {
        int type = ergType2npyType(parser.quantityType(i));
        PyObject* array = PyArray_SimpleNew(1, &rows, type);

        void* d = PyArray_DATA((PyArrayObject*)array);
        dataWrapper.push_back(reinterpret_cast<uint8_t*>(d));
        sizeWrapper.push_back(PyArray_NBYTES((PyArrayObject*)array));

        std::string name = parser.quantityName(i);
        PyDict_SetItemString(map, name.c_str(), array);

        // No need to decref the array because i never incrref it.
    }

    // Reset error variable, it should be already empty...
    error.clear();

    // Release the GIL because the readAll function is an I/O
    // operation that read a file.
    Py_BEGIN_ALLOW_THREADS;
        try {
            parser.readAll(dataWrapper, sizeWrapper);
        } catch(std::runtime_error e) {
            error = e.what();
        }
    // Reacquire the GIL
    Py_END_ALLOW_THREADS;

    // Check for exceptions
    if(error.length()>0) {
        Py_DecRef(map);
        PyErr_SetString(PyExc_NameError, error.c_str());
        return nullptr;
    }

    return map;
    */
}

PyFUNC py_can_read(PyObject* self, PyObject* filename)
{
    // self is unused.
    if(PyUnicode_Check(filename)==false) {
        PyErr_SetString(PyExc_NameError, "The input arguments must be a string");
        return nullptr;
    }

    const char* filenameStr = PyUnicode_AsUTF8(filename);
    // Flag for check for exceptions in file opening
    bool error = false;

    // Release the GIL because the open function is an I/O
    // operation that read a file.
    Py_BEGIN_ALLOW_THREADS;
        try {
            // Try to read the file: if the file is valid,
            // no exceptions are thrown
            erg::Reader parser;
            parser.open(filenameStr);
        } catch(...) {
            error = true;
        }
    // Reacquire the GIL
    Py_END_ALLOW_THREADS;

    if(error) {
        Py_RETURN_FALSE;
    }

    Py_RETURN_TRUE;
}


extern "C" void Parser_dealloc(Reader* self)
{
    PyTypeObject *tp = Py_TYPE(self);
    // free references and buffers here
    tp->tp_free(self);
    Py_DECREF(tp);
}

PyFUNC Parser_new(PyTypeObject* type, PyObject *args, PyObject *kwds)
{
    Reader* self = (Reader*)type->tp_alloc(type, 0);
    if (self != nullptr) {
        self->parser = new erg::Reader();
    }

    return (PyObject*)self;
}

extern "C" int Parser_init(Reader* self, PyObject *args, PyObject *kwds)
{
    if(args!=nullptr && PyTuple_Size(args)==1) {
        // Initialize with a filename: open the file
        PyObject* filename = PyTuple_GetItem(args, 0);

        PyObject* result = Parser_open(self, filename);
        if(result==nullptr)
            return -1;

        // DecRef the None object
        Py_DECREF(result);
    }

    return 0;
}

PyFUNC Parser_open(Reader* self, PyObject* filename)
{
    if(PyUnicode_Check(filename)==false) {
        PyErr_SetString(PyExc_NameError, "The function arguments must be a string");
        return nullptr;
    }

    const char* filenameStr = PyUnicode_AsUTF8(filename);
    std::string error;

    // Release the GIL because the open function is an I/O
    // operation that read a file.
    Py_BEGIN_ALLOW_THREADS
        try {
            self->parser->open(filenameStr);
        } catch (std::runtime_error& FileOpenError) {
            error = FileOpenError.what();
        }
    // Reacquire the GIL
    Py_END_ALLOW_THREADS;

    if (error.length()>0) {
        PyErr_SetString(PyExc_NameError, error.c_str());
        return nullptr;
    }


    Py_RETURN_NONE;
}

PyFUNC Parser_records(Reader* self)
{
    size_t records = self->parser->records();
    return PyLong_FromSize_t(records);
}

PyFUNC Parser_recordSize(Reader* self)
{
    size_t recordSize = self->parser->recordSize();
    return PyLong_FromSize_t(recordSize);
}

PyFUNC Parser_numQuanities(Reader* self)
{
    size_t numQ = self->parser->numQuanities();
    return PyLong_FromSize_t(numQ);
}

PyFUNC Parser_readAll(Reader* self)
{
    // Allocate a Dict of numpy arrays as the returned value.
    // Load all the numpy array raw data pointer for passing to
    // the erg::Parser::readAll() function.
    PyObject* map = PyDict_New();
    const size_t numQuantities = self->parser->numQuanities();
    npy_intp rows = self->parser->records();
    std::vector<uint8_t*> dataWrapper;
    std::vector<size_t> sizeWrapper;
    for(size_t i=0; i<numQuantities; ++i)
    {
        // Numpy array allocation
        int type = ergType2npyType(self->parser->quantityType(i));
        PyObject* array = PyArray_SimpleNew(1, &rows, type);

        // Append the raw data pointer in the vectors for the
        // erg::Parser::readAll() function
        void* d = PyArray_DATA((PyArrayObject*)array);
        dataWrapper.push_back(reinterpret_cast<uint8_t*>(d));
        sizeWrapper.push_back(PyArray_NBYTES((PyArrayObject*)array));

        // Put the Numpy array in the Dict
        std::string name = self->parser->quantityName(i);
        PyDict_SetItemString(map, name.c_str(), array);

        // No need to decref the array because i never incrref it.
    }

    std::string error;
    Py_BEGIN_ALLOW_THREADS;
        try {
            self->parser->readAll(dataWrapper, sizeWrapper);
        } catch(std::runtime_error& e) {
            error = e.what();
        }
    Py_END_ALLOW_THREADS;

    // Check for exceptions
    if(error.length()>0) {
        Py_DecRef(map);
        PyErr_SetString(PyExc_NameError, error.c_str());
        return nullptr;
    }

    return map;
}

PyFUNC Parser_read(Reader* self, PyObject* args, PyObject* keywds)
{
    PyObject* objIndex = nullptr;
    size_t from = 0;
    size_t count = self->parser->records();
    static char* kwlist[] = {"name", "start", "count", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, keywds, "O|ii", kwlist, &objIndex, &from, &count))
        return nullptr;

    const size_t qindex = indexFromPyObject(self->parser, objIndex);
    if(PyErr_Occurred()!=nullptr)
        return nullptr;

    // Numpy array creation
    npy_intp rows = count;
    int type = ergType2npyType(self->parser->quantityType(qindex));
    PyArrayObject* array = (PyArrayObject*)PyArray_SimpleNew(1, &rows, type);
    uint8_t* outData = (uint8_t*)PyArray_DATA(array);
    const npy_intp size = PyArray_NBYTES(array);

    std::string error;
    Py_BEGIN_ALLOW_THREADS;
        try {
            self->parser->read(qindex, from, count, outData, size);
        } catch(std::runtime_error& e) {
            error = e.what();
        }
    Py_END_ALLOW_THREADS;

    if(error.length()>0) {
        Py_DecRef((PyObject*)array);
        PyErr_SetString(PyExc_NameError, error.c_str());
        return nullptr;
    }

    return (PyObject*)array;
}

PyFUNC Parser_quantitySize(Reader* self, PyObject* arg)
{
    size_t qindex = indexFromPyObject(self->parser, arg);
    if(PyErr_Occurred()!=nullptr)
        return nullptr;
    try {
        size_t qsize = self->parser->quantitySize(qindex);
        return PyLong_FromSize_t(qsize);
    } catch (std::runtime_error& e) {
        PyErr_SetString(PyExc_NameError, e.what());
        return nullptr;
    }
}

PyFUNC Parser_quantityName(Reader* self, PyObject* arg)
{
    size_t qindex = indexFromPyObject(self->parser, arg);
    if(PyErr_Occurred()!=nullptr)
        return nullptr;
    try {
        std::string qname = self->parser->quantityName(qindex);
        return PyUnicode_FromString(qname.c_str());
    } catch (std::runtime_error& e) {
        PyErr_SetString(PyExc_NameError, e.what());
        return nullptr;
    }
}

PyFUNC Parser_quantityType(Reader* self, PyObject* arg)
{
    size_t qindex = indexFromPyObject(self->parser, arg);
    if(PyErr_Occurred()!=nullptr)
        return nullptr;
    try {
        erg::Type type = self->parser->quantityType(qindex);
        int npType = ergType2npyType(type);
        return (PyObject*)PyArray_DescrFromType(npType);
    } catch (std::runtime_error& e) {
        PyErr_SetString(PyExc_NameError, e.what());
        return nullptr;
    }
}

PyFUNC Parser_quantityUnit(Reader* self, PyObject* arg)
{
    size_t qindex = indexFromPyObject(self->parser, arg);
    if(PyErr_Occurred()!=nullptr)
        return nullptr;
    try {
        std::string qname = self->parser->quantityUnit(qindex);
        return PyUnicode_FromString(qname.c_str());
    } catch (std::runtime_error& e) {
        PyErr_SetString(PyExc_NameError, e.what());
        return nullptr;
    }
}

PyFUNC Parser_index(Reader* self, PyObject* arg)
{
    if(!PyUnicode_Check(arg)) {
        PyErr_SetString(PyExc_NameError, "Argument must be a string.");
        return nullptr;
    }

    try {
        const char* qname = PyUnicode_AsUTF8(arg);
        size_t qindex = self->parser->index(std::string(qname));
        return PyLong_FromSize_t(qindex);
    } catch (std::runtime_error& e) {
        PyErr_SetString(PyExc_NameError, e.what());
        return nullptr;
    }
}

PyFUNC Parser_isErg(Reader* self)
{
    if(self->parser->isErg()) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}

PyFUNC Parser_isFortran(Reader* self)
{
    if(self->parser->isFortran()) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}

PyFUNC Parser_has(Reader* self, PyObject* arg)
{
    if(!PyUnicode_Check(arg)) {
        PyErr_SetString(PyExc_NameError, "Argument must be a string.");
        return nullptr;
    }

    const char* qname = PyUnicode_AsUTF8(arg);
    if(self->parser->has(qname)) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}

PyFUNC Parser_close(Reader* self)
{
    self->parser->close();
    Py_RETURN_NONE;
}

static struct PyModuleDef pyergModuleDef = {
    PyModuleDef_HEAD_INIT,  // Use of undeclared indentifier PyModuleDef_HEAD_INIT
    "pyerg",            /* m_name */
    "Module for loading CarMaker .erg files.", /* m_doc */
    -1,                 /* m_size */
    pyerg_methods,      /* m_methods */
};

PyMODINIT_FUNC PyInit_pyerg(void)
{
    PyObject *pyergModule;

    pyergModule = PyModule_Create(&pyergModuleDef);
    if (pyergModule == NULL)
        return NULL;

    if (PyType_Ready(&pyerg_ReaderType) < 0)
        return NULL;

    Py_INCREF(&pyerg_ReaderType);
    if (PyModule_AddObject(pyergModule, "Reader", (PyObject*)&pyerg_ReaderType) < 0) {
        Py_DECREF(pyergModule);
        Py_DECREF(&pyerg_ReaderType);
        return NULL;
    }
    PyModule_AddStringConstant(pyergModule, "__version__", "0.6.1");

    import_array();

    return pyergModule;
}

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

#ifndef PYERG_DOCSTRINGS_H
#define PYERG_DOCSTRINGS_H

#define PYERG_MODULE_DOC \
    "Parser for version 1 and 2 `*.erg` files.\n\n" \
    "The CarMaker `ERG` specifications defines 2 files:\n" \
    "- `.erg` file with all the data\n" \
    "- `.erg.info` or `.info` file with the data type and quantity definitions\n\n" \
    "The `.erg` file is composed by an header section (header_t) and a data section. The" \
    "data section contains the data arranged in records (rows). Each record contains" \
    "a sample of each quantity defined in the `.erg.info` companion file.\n" \
    "Refer to the CarMaker manual for more details.\n" \
    "The record can contains padding bytes. the current parser implementation supports only" \
    "padding bytes at the end of the record, not between quantities. This will be" \
    "implemented later if needed.\n\n" \
    "Each columns in the file is called dataset or quantity.\n\n" \
    "Examples:\n\n" \
    "import pyerg\n" \
    "erg = pyerg.Parser('file.erg')\n" \
    "data = erg.readAll()\n\n" \
    "import pyerg\n" \
    "erg = pyerg.Parser()\n" \
    "erg.open('file.erg')\n" \
    "data = erg.readAll()"


#define PYERG_READ_DOC  \
    "data = read(filename)\n" \
    "Read a CarMaker *.erg file (with its *.erg.info file) and returns a Dict object " \
    "with all the datasets. The names of the datasets are the keys of the Dict.\n\n" \
    "Args:\n" \
    "    filename: Pathname of the erg file.\n" \
    "Returns:\n" \
    "    Dict of numpy ndarray with the datasets in the file. The names of the datasets are " \
    "the keys of the Dict.\n" \
    "Raises:\n" \
    "    Exception if the file can't be read or is not an ERG file."

#define PYERG_CAN_READ_DOC  \
    "ok = can_read(filename)\n" \
    "Check if the CarMaker *.erg file (with its *.erg.info file) is valid and readable.\n\n" \
    "Args:\n" \
    "    filename: Pathname of the erg file.\n" \
    "Returns:\n" \
    "    True if the file is readable and a valid ERG, False otherwise." \


#define PYERG_PARSER_OPEN_DOC   \
    "Open an `.erg` file, parse the its header and the companion file.\n" \
    "This function look for the `<filename>.info` in the same place of the `.erg` file.\n\n" \
    "Args:\n" \
    "    filename: Pathname of the erg file.\n" \
    "Raises:\n" \
    "    Exception if the file can't be read or is not an ERG file."

#define PYERG_PARSER_RECORDS_DOC   \
    "Number of records/rows in th `.erg` file.\n\n" \
    "Returns:\n" \
    "    The number of rows."

#define PYERG_PARSER_RECORDSIZE_DOC   \
    "Size in bytes of each record/row in the `.erg` file.\n\n" \
    "Returns:\n" \
    "    The size in bytes of each row."

#define PYERG_PARSER_NUMQUANITIES_DOC   \
    "Number of quantities in the file\n" \
    "The number of quantities is the number of columns that contains data in each record:\n" \
    "the padding columns are ignored.\n\n" \
    "Returns:\n" \
    "    The number of quantities.\n"

#define PYERG_PARSER_READALL_DOC   \
    "Read all the datasets from the file.\n\n" \
    "Returns:\n" \
    "    Dict with all the datasets as numpy ndarray with the quantity names as keys." \
    "See:\n" \
    "    quantitySize() to know the size of each dataset to known in advice the quantity of memory " \
    "that will be used."

#define PYERG_PARSER_READ_DOC   \
    "Read a single dataset from the file.\n\n" \
    "Args:\n" \
    "    name: Index or name of the dataset to read.\n" \
    "    start: Index of the row from which to start reading.\n" \
    "    count: Number of rows to read.\n" \
    "Returns:\n" \
    "    Numpy ndarray with the data.\n"  \
    "Raises:\n" \
    "    If the quantity index is out of range or the quantity name does not exists."

#define PYERG_PARSER_QUANTITYSIZE_DOC   \
    "Size in bytes of the dataset at the current index.\n" \
    "Args:\n" \
    "    qindex: Index or name of the dataset.\n" \
    "Returns:\n" \
    "    The size of the dataset.\n" \
    "Raises:\n" \
    "    If the quantity index is out of range or the quantity name does not exists."

#define PYERG_PARSER_QUANTITYNAME_DOC   \
    "Name of the dataset at the current index.\n\n" \
    "Args:\n" \
    "    qindex: Index of the dataset.\n" \
    "Returns:\n" \
    "    The name of the dataset.\n" \
    "Raises:\n" \
    "    If the quantity index is out of range."

#define PYERG_PARSER_QUANTITYTYPE_DOC   \
    "Type of the dataset at the current index.\n\n" \
    "Args:\n" \
    "    qindex: Index or name of the dataset.\n" \
    "Returns:\n" \
    "    The type of the dataset in Numpy format.\n" \
    "Raises:\n" \
    "    If the quantity index is out of range or the quantity name does not exists."

#define PYERG_PARSER_QUANTITYUNIT_DOC   \
    "Unit of the dataset at the current index.\n\n" \
    "Args:\n" \
    "    qindex: Index or name of the dataset.\n" \
    "Returns:\n" \
    "    The unit of the dataset or None if it is not present.\n" \
    "Raises:\n" \
    "    If the quantity index is out of range or the quantity name does not exists."

#define PYERG_PARSER_INDEX_DOC   \
    "Index of the quantity inside the data file.\n\n" \
    "Args:\n" \
    "    name: Name of the dataset (quantity).\n" \
    "Returns:\n" \
    "    The unit of the dataset or None if it is not present.\n" \
    "Raises:\n" \
    "    If the quantity index is out of range or the quantity name does not exists."

#define PYERG_PARSER_ISERG_DOC   \
    "Version of th ERG file.\n\n" \
    "Returns:\n" \
    "    True if the format of the file is Erg (`erg`)."

#define PYERG_PARSER_ISFORTRAN_DOC   \
    "Version of th ERG file.\n\n" \
    "Returns:\n" \
    "    True if the format of the file is Fortran binary (`FORTRAN_Binary_Data`)."

#define PYERG_PARSER_HAS_DOC   \
    "Test if the quantity is present in the file.\n" \
    "Args:\n" \
    "    name: Name of the quantity.\n" \
    "Returns:\n" \
    "    True if the quantity is present inside the file, False otherwise."

#define PYERG_PARSER_CLOSE_DOC   \
    "Close the current file and clear the data." \


#endif  // PYERG_DOCSTRINGS_H

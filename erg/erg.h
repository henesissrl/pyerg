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

#ifndef ERGPARSER_H
#define ERGPARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <algorithm>


// Workaround for Mingw 4.7 std::tostring() method bug.
#if  defined(__MINGW32__) || defined(__MINGW64__)
    #if __GNUC__==4 && __GNUC_MINOR__==7

    #include <sstream>
    namespace std
    {

    template<typename T> std::string to_string(const T& v)
    {
        std::ostringstream stm ;
        stm << v;
        return stm.str();
    }

    }

    #endif
#endif

namespace erg
{

/*!
 * \brief Code for the types of the quantities
 */
enum class Type
{
    Void,   //!< Void datatype with undefined size.
    Int8,   //!< Signed int (1 bytes)
    Int16,  //!< Signed int (2 bytes)
    Int32,  //!< Signed int (4 bytes)
    Int64,  //!< Signed int (8 bytes)
    Uint8,  //!< Unsigned int (1 bytes)
    Uint16, //!< Unsigned int (2 bytes)
    Uint32, //!< Unsigned int (4 bytes)
    Uint64, //!< Unsigned int (8 bytes)
    Float,  //!< Floating point signle precision (4 bytes)
    Double  //!< Floating point double precision (8 bytes)
};

/*!
 * \brief Byte order code
 */
enum class ByteOrder
{
    LittelEndian = 0,
    BigEndian = 1
};

enum class Format
{
    Fortran = 1,
    Erg = 2
};

/*!
 * \brief `ERG` version 2 header structure
 */
struct header_t
{
    /*!
     * \brief A string identifier of the file type.
     *
     * The `ERG` specification define this string as "CM-ERG\0x00\0x00".
     */
    std::int8_t identifier[8];

    /*!
     * \brief Version of the `ERG` file.
     *
     * The current supported version is 1.
     */
    std::uint8_t version;

    /*!
     * \brief Byte order of the data
     *
     * `0` for little endian data, `1` for big endian data.
     */
    std::uint8_t byte_order;

    /*!
     * \brief Size of each record.
     *
     * The bytes of the field are saved according to the byte_order filed.
     */
    std::uint16_t record_size;

    /*!
     * \brief Empty space.
     *
     * Reserved for future usage.
     */
    std::int8_t reserved[4];
};

/*!
 * \brief Description of a quantity
 */
class Quantity
{
public:
    std::string name;   //!< The name.
    std::string unit;   //!< The unit. It can be an empty string.
    std::string typeStr; //!< The type represented as string.
    Type type;          //!< The type.
    size_t size;        //!< The size in bytes of each data element.
    size_t offset;      //!< The offset in bytes from the start of the record.
};

/*!
 * \brief Parser for version 1 and 2 `*.erg` files.
 *
 * The CarMaker `ERG` specifications defines 2 files:
 * - `.erg` file with all the data
 * - `.erg.info` or `.info` file with the data type and quantity definitions
 *
 * The `.erg` file is composed by an header section (header_t) and a data section. The
 * data section contains the data arranged in records (rows). Each record contains
 * a sample of each quantity defined in the `.erg.info` companion file.
 *
 * Refer to the CarMaker manual for more details.
 *
 * The record can contains padding bytes. the current parser implementation supports only
 * padding bytes at the end of the record, not between quantities. This will be
 * implemented later if needed.
 *
 * \see header_t for the `ERG` version 2 header description.
 */
class Reader
{
public:

    /*!
     * \brief Construct a new parser
     */
    Reader() noexcept(true);

    /*!
     * \brief Construct a new parser object and open the file
     * \param filename Name of the file to open.
     * \throws If the file can't be read or the companion file is not found.
     * \see open().
     */
    Reader(const std::string& filename) noexcept(false);

    ~Reader();

    /*!
     * \brief Open an `.erg` file, parse the its header and the companion file.
     *
     * This function look for the `<filename>.info` in the same place of the
     * `.erg` file.
     *
     * \param filename Name of the `.erg` file to open.
     * \throws If the file can't be read or the companion file is not found.
     */
    void open(const std::string& filename)  noexcept(false);

    /*!
     * \brief Number of records/rows in th `.erg` file.
     *
     * \return Number of rows
     */
    size_t records() const noexcept(true) { return mRecordsCount; }

    /*!
     * \brief Size in bytes of each record/row in the `.erg`file.
     *
     * \return Size of each row
     */
    size_t recordSize() const noexcept(true) { return mRecordSize; }

    /*!
     * \brief Number of quantities in the file
     *
     * The number of quantities is the number of columns that contains data in each record.
     *
     * \return Number of quantities.
     */
    size_t numQuanities() const noexcept(true) { return mQuantities.size(); }

    /*!
     * \brief Read all the datasets from the file.
     *
     * The memory for each dataset must be preallocated before calling the function
     *
     * \param values Vector of pointer to the destination data of each quantity.
     * \param sizes Size of the memory allocated for each quantity.
     * \return The number of rows that has been read.
     * \see quantitySize() to know the size of each quantity to preallocate memory.
     */
    size_t readAll(std::vector<uint8_t*>& values, const std::vector<size_t>& sizes);

    /*!
     * \brief Read a single dataset from the file
     *
     * \param qindex Index of the dataset to read
     * \param dst The pre-allocated destination memory
     * \param size The size of the allocated memory
     * \return The number of records that has been read.
     */
    size_t read(const size_t qindex, uint8_t* dst, const size_t size);

    /*!
     * \brief Read a slice of single dataset from the file
     *
     * \param qindex Index of the dataset to read
     * \param from Index of the first record to read
     * \param count Maximum number of records to read
     * \param dst The pre-allocated destination memory
     * \param size The size of the allocated memory
     * \return The number of records that has been read.
     */
    size_t read(const size_t qindex, const size_t from, const size_t count, uint8_t* dst, const size_t size);

    /*!
     * \brief Size in bytes of the dataset at the current index.
     *
     * \param index Index of the dataset (column number in the record).
     * \return Size of the selected dataset
     * \throws If the quantity index is out of range.
     * \see numQuanities()
     */
    size_t quantitySize(const size_t qIndex) const noexcept(false)
    {
        if (qIndex>=mQuantities.size())
            throw std::runtime_error("The quantity with index "+std::to_string(qIndex)+" does not exists.");
        return mQuantities[qIndex].size * mRecordsCount;
    }

    /*!
     * \brief Name of the dataset at the current index.
     * \param index Index of the dataset (column number in the record).
     * \return Name of the quantity.
     * \throws If the quantity index is out of range.
     * \see numQuanities()
     */
    std::string quantityName(const size_t qIndex) const noexcept(false)
    {
        if (qIndex>=mQuantities.size())
            throw std::runtime_error("The quantity with index "+std::to_string(qIndex)+" does not exists.");
        return mQuantities[qIndex].name;
    }

    /*!
     * \brief Type of the dataset at the current index.
     * \param index Index of the dataset (column number in the record).
     * \return Type of the dataset.
     * \throws If the quantity index is out of range.
     * \see numQuanities()
     */
    Type quantityType(const size_t qIndex) const noexcept(false)
    {
        if (qIndex>=mQuantities.size())
            throw std::runtime_error("The quantity with index "+std::to_string(qIndex)+" does not exists.");
        return mQuantities[qIndex].type;
    }

    /*!
     * \brief Unit of the dataset at the current index.
     * \param index Index of the dataset (column number in the record).
     * \return Unit of the dataset or empty string if none is defined.
     * \throws If the quantity index is out of range.
     * \see numQuanities()
     */
    std::string quantityUnit(const size_t qIndex) const noexcept(false)
    {
        if (qIndex>=mQuantities.size())
            throw std::runtime_error("The quantity with index "+std::to_string(qIndex)+" does not exists.");
        return mQuantities[qIndex].unit;
    }

    /*!
     * \brief Index of the quantity inside the data file.
     * \param qname Name of the quantity
     * \return The index of the quantity
     * \throws If the quantity name is not found in the file.
     */
    size_t index(const std::string& qname) const noexcept(false);

    /*!
     * \brief Test if the quantity is present in the file.
     * \param qname Name of the quantity
     * \return `true` if the quantity is present inside the file.
     */
    bool has(const std::string& qname) const noexcept(true);

    /*!
     * \brief Close the current file and clear the data.
     */
    void close() noexcept(true);

    /*!
     * \brief True if the format of the file is Erg (`erg`).
     * \return `true` if the file is an Erg (Erg v2).
     */
    bool isErg() const noexcept(true) { return mFormat==Format::Erg; }

    /*!
     * \brief True if the format of the file is Fortran binary (`FORTRAN_Binary_Data`).
     * \return `true` if the file is an Fortran binary (Erg v1).
     */
    bool isFortran() const noexcept(true) { return mFormat==Format::Fortran; }

public:

    /*!
     * \brief Size of a datatype
     * \param type The datatype
     * \return The size of the data
     * \throws If the datatype is unknown or Type::Void.
     */
    static size_t dataSize(const Type type) noexcept(false);

    /*!
     * \brief Type code from string
     * \param typestr String representation of the datatype
     * \return The datatype code
     */
    static Type dataType(const std::string& typestr) noexcept(true);

    /*!
     * \brief Type code from string
     * \param typestr String representation of the datatype
     * \return The datatype code
     */
    static Type dataType(const char* typestr) noexcept(true);

protected:

    /*!
     * \brief Parse the `.erg.info` companion file
     *
     * Open the companion file and parse it to load the definition
     * of the quantities inside the data file.
     *
     * \throws If the file can't be load and parsed.
     */
    void parseInfoFile() noexcept(false);

    /*!
     * \brief Parse a Fortran binary (Erg v1) file.
     */
    void parseFortranFormat() noexcept(false);

    /*!
     * \brief Parse a Erg v2 file.
     */
    void parseErgFormat() noexcept(false);

    size_t initialSkipBytes() const noexcept(true)
    {
        if(isErg())
            return sizeof(header_t);
        else
            return 0;
    }

    /*!
     * \brief Convert an array from little endianess to host endianess.
     * \param data Data to convert
     * \param elementSize Size of each element in the data
     * \param count number of elements to convert
     */
    static void arrayLe2Host(uint8_t* data, const size_t elementSize, const size_t count) noexcept(true);

    /*!
     * \brief Convert an array from big endianess to host endianess.
     * \param data Data to convert
     * \param elementSize Size of each element in the data
     * \param count number of elements to convert
     */
    static void arrayBe2Host(uint8_t* data, const size_t elementSize, const size_t count) noexcept(true);

protected:
    std::string mFilename;  //!< Name of the open `.erg` file
    std::ifstream mFile;    //!< Open `.erg` file
    size_t mFileSize;       //!< Size of the file
    size_t mRecordsCount;   //!< Number of records (rows)
    Format mFormat;

    size_t mRecordSize;     //!< Size in bytes of each record
    ByteOrder mByteOrder;   //!< Data byte order in the file
    std::vector<Quantity> mQuantities;  //!< List of quantities stored in the file
};


}

#endif // ERGPARSER_H

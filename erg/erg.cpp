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

#include "erg.h"

#include <map>
#include <iostream>
#include <string>
#include <cctype>


#define HEADER_SIZE     16


namespace erg
{

/*!
 * \brief Check if the host has big endian order.
 * \return `true` if the host has big endian order.
 */
/*
static constexpr bool isBigEndian()
{
    return ((union{ uint32_t i; char c[4];}){0x01020304}).c[0] == 1;
}*/

union {
  uint16_t s;
  unsigned char c[2];
} constexpr static  d {1};

constexpr bool isBigEndian() {
  return d.c[0] != 1;
}

static constexpr uint16_t bswap16(const uint16_t x)
{
    return ((x << 8) & 0xff00) | ((x >> 8) & 0x00ff);
}

static constexpr uint32_t bswap32(const uint32_t x)
{
    return  ((x << 24) & 0xff000000 ) |
        ((x <<  8) & 0x00ff0000 ) |
        ((x >>  8) & 0x0000ff00 ) |
        ((x >> 24) & 0x000000ff );
}

static constexpr uint64_t bswap64(const uint64_t x)
{
    return  ( (x << 56) & 0xff00000000000000UL ) |
        ( (x << 40) & 0x00ff000000000000UL ) |
        ( (x << 24) & 0x0000ff0000000000UL ) |
        ( (x <<  8) & 0x000000ff00000000UL ) |
        ( (x >>  8) & 0x00000000ff000000UL ) |
        ( (x >> 24) & 0x0000000000ff0000UL ) |
        ( (x >> 40) & 0x000000000000ff00UL ) |
        ( (x >> 56) & 0x00000000000000ffUL );
}

/*!
 * \brief big endian to host conversion for 2 bytes datatype
 * \param val Input value
 * \return Input value with swapped bytes
 */
static constexpr uint16_t be2h16(const uint16_t val)
{
    return isBigEndian() ? val : bswap16(val);
}

/*!
 * \brief little endian to host conversion for 2 bytes datatype
 * \param val Input value
 * \return Input value with swapped bytes
 */
static constexpr uint16_t le2h16(const uint16_t val)
{
    return isBigEndian() ? bswap16(val) : val;
}

/*!
 * \brief big endian to host conversion for 4 bytes datatype
 * \param val Input value
 * \return Input value with swapped bytes
 */
static constexpr uint32_t be2h32(const uint32_t val)
{
    return isBigEndian() ? val : bswap32(val);
}

/*!
 * \brief little endian to host conversion for 4 bytes datatype
 * \param val Input value
 * \return Input value with swapped bytes
 */
static constexpr uint32_t le2h32(const uint32_t val)
{
    return isBigEndian() ? bswap32(val) : val;
}

/*!
 * \brief big endian to host conversion for 8 bytes datatype
 * \param val Input value
 * \return Input value with swapped bytes
 */
static constexpr uint64_t be2h64(const uint64_t val)
{
    return isBigEndian() ? val : bswap64(val);
}

/*!
 * \brief little endian to host conversion for 8 bytes datatype
 * \param val Input value
 * \return Input value with swapped bytes
 */
static constexpr uint64_t le2h64(const uint64_t val)
{
    return isBigEndian() ? bswap64(val) : val;
}




/*!
 * \brief Remove start and trailing spaces from a string.
 * \param str Input string
 * \return A new string from the input
 */
std::string trim(const std::string& str)
{
    auto front = std::find_if_not(str.begin(), str.end(), [](int c){ return ::isspace(c); });
    auto back = std::find_if_not(str.rbegin(), str.rend(), [](int c){ return ::isspace(c); }).base();

    if(back <= front)
        return std::string();

    return std::string(front, back);
}

std::string trimComments(const std::string& str)
{
    return str.substr(0, str.find('#'));
}


void Reader::arrayLe2Host(uint8_t* data, const size_t elementSize, const size_t count) noexcept(true)
{
    if (isBigEndian()==false)
        return;

    switch (elementSize) {
    case sizeof(uint16_t):
        {
            uint16_t* d = reinterpret_cast<uint16_t*>(data);
            for(size_t i=0; i<count; ++i)
                d[i] = le2h16(d[i]);
        }
        break;
    case sizeof(uint32_t):
        {
            uint32_t* d = reinterpret_cast<uint32_t*>(data);
            for(size_t i=0; i<count; ++i)
                d[i] = le2h32(d[i]);
        }
        break;
    case sizeof(uint64_t):
        {
            uint64_t* d = reinterpret_cast<uint64_t*>(data);
            for(size_t i=0; i<count; ++i)
                d[i] = le2h64(d[i]);
        }
        break;
    default:
        break;
    }
}

void Reader::arrayBe2Host(uint8_t* data, const size_t elementSize, const size_t count) noexcept(true)
{
    if (isBigEndian())
        return;

    switch (elementSize) {
    case sizeof(uint16_t):
        {
            uint16_t* d = reinterpret_cast<uint16_t*>(data);
            for(size_t i=0; i<count; ++i)
                d[i] = be2h16(d[i]);
        }
        break;
    case sizeof(uint32_t):
        {
            uint32_t* d = reinterpret_cast<uint32_t*>(data);
            for(size_t i=0; i<count; ++i)
                d[i] = be2h32(d[i]);
        }
        break;
    case sizeof(uint64_t):
        {
            uint64_t* d = reinterpret_cast<uint64_t*>(data);
            for(size_t i=0; i<count; ++i)
                d[i] = be2h64(d[i]);
        }
        break;
    default:
        break;
    }
}





Reader::Reader() noexcept(true)
{
}

Reader::Reader(const std::string& filename)  noexcept(false)
{
    open(filename);
}


Reader::~Reader()
{
    close();
}


void Reader::open(const std::string& filename)
{
    static_assert(sizeof(header_t) == HEADER_SIZE, "Header size must be 16 bytes");

    // Clear data
    close();

    mFilename = filename;

    // Parse companion file.
    parseInfoFile();

    mFile.open(filename, std::ios_base::in | std::ios_base::binary);
    if(mFile.is_open()==false) {
        close();
        throw std::runtime_error("Can't open "+filename+" file.");
    }

    if(mFormat==Format::Erg)
        parseErgFormat();
    else
        parseFortranFormat();
}


size_t Reader::readAll(std::vector<uint8_t*>& values, const std::vector<size_t>& sizes)
{
    if(values.size()!=sizes.size())
        throw std::runtime_error("Wrong input size");

    const size_t nds = numQuanities();
    if(nds<values.size())
        throw std::runtime_error("No data for all the datasets");

    for(size_t i=0; i<nds; ++i) {
        if(sizes[i] < quantitySize(i))
            throw std::runtime_error("Not enought space for dataset "+mQuantities[i].name);
    }


    mFile.seekg(initialSkipBytes(), std::ios_base::beg);

    std::vector<uint8_t> row(mRecordSize, 0);
    uint8_t* data = row.data();
    size_t readRows = 0;
    for(size_t rid=0; rid<mRecordsCount; ++rid)
    {
        mFile.read(reinterpret_cast<char*>(data), mRecordSize);
        if(mFile.eof())
            break;

        readRows += 1;

        for(size_t ds=0; ds<nds; ++ds)
        {
            const size_t outOffset = rid * mQuantities[ds].size;
            const size_t inOffset = mQuantities[ds].offset;
            uint8_t* outData = values[ds];
            std::memcpy(outData + outOffset,  data + inOffset, mQuantities[ds].size);
        }
    }

    // Correct endianess
    for(size_t ds=0; ds<nds; ++ds)
    {
        uint8_t* outData = values[ds];
        if(mByteOrder==ByteOrder::LittelEndian)
            arrayLe2Host(outData, mQuantities[ds].size, readRows);
        else
            arrayBe2Host(outData, mQuantities[ds].size, readRows);
    }

    return readRows;
}

size_t Reader::read(const size_t qindex, uint8_t* dst, const size_t size)
{
    return read(qindex, 0, mRecordsCount, dst, size);
    /*
    if(qindex>=mQuantities.size())
        throw std::runtime_error("Index "+std::to_string(qindex)+" is out of bounds.");

    const Quantity& qt = mQuantities[qindex];

    if((qt.size*mRecordsCount)>size)
        throw std::runtime_error("Not enough data allocated");

    memset(outData, 0, size);
    const size_t inOffset = qt.offset;

    mFile.seekg(initialSkipBytes(), std::ios_base::beg);

    std::vector<uint8_t> row(mRecordSize, 0);
    uint8_t* rowData = row.data();
    size_t readRows = 0;
    for(size_t rid=0; rid<mRecordsCount; ++rid)
    {
        mFile.read(reinterpret_cast<char*>(rowData), mRecordSize);
        if(mFile.eof())
            break;

        readRows += 1;

        const size_t outOffset = rid * qt.size;
        std::memcpy(outData + outOffset,  rowData + inOffset, qt.size);
    }

    if(mByteOrder==ByteOrder::LittelEndian)
        arrayLe2Host(outData, qt.size, readRows);
    else
        arrayBe2Host(outData, qt.size, readRows);

    return readRows;
    */
}

size_t Reader::read(const size_t qindex, const size_t from, const size_t count, uint8_t* dst, const size_t size)
{
    if(qindex>=mQuantities.size())
        throw std::runtime_error("Index "+std::to_string(qindex)+" is out of bounds.");

    const Quantity& qt = mQuantities[qindex];

    size_t expectedSize = qt.size * count;
    if(expectedSize>size)
        throw std::runtime_error("Not enough data allocated: "+std::to_string(size)+\
                                 " instead of "+std::to_string(expectedSize)+" bytes.");

    memset(dst, 0, size);
    const size_t inOffset = qt.offset;

    // Skip initial bytes
    size_t bytesToSkip = initialSkipBytes() + mRecordSize * from;
    mFile.seekg(bytesToSkip, std::ios_base::beg);

    std::vector<uint8_t> row(mRecordSize, 0);
    uint8_t* rowData = row.data();
    size_t readRows = 0;
    for(size_t index=0; index<count; ++index)
    {
        mFile.read(reinterpret_cast<char*>(rowData), mRecordSize);
        if(mFile.eof())
            break;

        readRows += 1;

        const size_t outOffset = index * qt.size;
        std::memcpy(dst + outOffset,  rowData + inOffset, qt.size);
    }

    if(mByteOrder==ByteOrder::LittelEndian)
        arrayLe2Host(dst, qt.size, readRows);
    else
        arrayBe2Host(dst, qt.size, readRows);

    return readRows;
}

size_t Reader::index(const std::string &qname) const noexcept(false)
{
    auto nameIt = std::find_if(mQuantities.cbegin(),
                               mQuantities.cend(),
                               [&qname](const Quantity& q)->bool{ return q.name==qname; });
    if(nameIt==mQuantities.cend())
        throw std::runtime_error("The quantity "+qname+" does not exists.");
    return std::distance(mQuantities.cbegin(), nameIt);
}

bool Reader::has(const std::string &qname) const noexcept(true)
{
    try {
        index(qname);
        return true;
    } catch(...) {
        return false;
    }
}

void Reader::close() noexcept(true)
{
    if(mFile.is_open())
        mFile.close();

    mFilename.clear();
    mFileSize = 0;
    mRecordsCount = 0;
    mRecordSize = 0;
    mFormat = Format::Erg;
    mByteOrder = ByteOrder::LittelEndian;
    mQuantities.clear();
}


void Reader::parseInfoFile()
{
    std::ifstream info;
    // Test for "filename.erg.info" companion file
    info.open(mFilename+".info");
    if (info.is_open()==false)
    {
        // Test for "filename.info" companion file
        int i = mFilename.find_last_of('.');
        std::string filename = mFilename.substr(0, i) + ".info";
        info.open(filename);
        if (info.is_open()==false)
        {
            close();
            throw std::runtime_error("Can't open "+mFilename+" companion file.");
        }
    }

    std::string line;
    std::map<std::string, std::string> infoMap;

    // Read all file as key, value pairs
    while(info.eof()==false)
    {
        // Start to read a parameter
        std::getline(info, line);
        size_t i = line.find('=');
        if(i!=std::string::npos)
        {
            line = trimComments(line);
            std::string key = trim(line.substr(0, i));
            std::string val = trim(line.substr(i+1));
            infoMap[key] = val;
        }
    }

    // Decode file format
    {
        auto formatStrIt = infoMap.find("File.Format");
        if(formatStrIt==infoMap.end())
            throw std::runtime_error("Format not specified.");
        std::string format = trim(formatStrIt->second);
        std::transform(format.begin(), format.end(), format.begin(), ::tolower);
        if(format=="erg") {
            mFormat = Format::Erg;
        } else if(format=="fortran_binary_data") {
            mFormat = Format::Fortran;
        } else {
            close();
            throw std::runtime_error("Unknown format "+format+".");
        }
    }

    // Decode byte order
    {
        auto byteOrderStrIt = infoMap.find("File.ByteOrder");
        if(byteOrderStrIt==infoMap.end())
            throw std::runtime_error("Format not specified.");
        std::string byteOrder = trim(byteOrderStrIt->second);
        std::transform(byteOrder.begin(), byteOrder.end(), byteOrder.begin(), ::tolower);
        if(byteOrder=="littleendian") {
            mByteOrder = ByteOrder::LittelEndian;
        } else if(byteOrder=="bigendian") {
            mByteOrder = ByteOrder::BigEndian;
        } else {
            close();
            throw std::runtime_error("Unknown byte order "+byteOrder+".");
        }
    }

    size_t colIndex = 0;
    std::string num;
    std::string targetName;
    std::string targetType;
    std::string targetUnit;

    // Search for all the quantities with brute force approach
    // by searching all the names an types in format
    // "File.At.<index>.Name" and "File.At.<index>.Type".
    // Both name and type must be available; if not, the search end
    // and the next quantities are ignored.
    bool done = false;
    while(!done)
    {
        colIndex += 1;
        targetName.clear();
        targetType.clear();
        targetUnit.clear();
        num = std::to_string(colIndex);

        targetName.append("File.At.").append(num).append(".Name");
        targetType.append("File.At.").append(num).append(".Type");

        Quantity q;

        auto typeIt = infoMap.find(targetType);
        if(typeIt==infoMap.end())
        {
            done = true;
            continue;
        }

        q.typeStr = typeIt->second;
        q.type = dataType(q.typeStr);
        if(q.type==Type::Void)
        {
            // Padding bytes: decode size and skipping...
            q.size = strtol(q.typeStr.c_str(), nullptr, 10);
            q.name = "";
        }
        else
        {
            auto nameIt = infoMap.find(targetName);
            if(nameIt==infoMap.end())
            {
                done = true;
                continue;
            }

            q.name = nameIt->second;

            // Decode unit if present
            targetUnit.append("Quantity.").append(q.name).append(".Unit");
            auto quantityIt = infoMap.find(targetUnit);
            if(quantityIt!=infoMap.end())
                q.unit = quantityIt->second;

            q.type = dataType(q.typeStr);
            q.size = dataSize(q.type);
        }

        mQuantities.push_back(q);
    }

    // Check record size
    const size_t numAllQuanties = mQuantities.size();
    size_t recordBytes = 0;
    for(size_t i=0; i<numAllQuanties; ++i)
    {
        mQuantities[i].offset = recordBytes;
        recordBytes += mQuantities[i].size;
    }

    mRecordSize = recordBytes;

    // Remove the padding quantities if present
    auto toRemoveIt = std::remove_if(mQuantities.begin(), mQuantities.end(),
                                     [](Quantity& x){return x.type==Type::Void;});
    mQuantities.erase(toRemoveIt, mQuantities.end());
}

void Reader::parseFortranFormat()
{
    // Read the record size
    uint32_t rowSize = 0;
    mFile.read(reinterpret_cast<char*>(&rowSize), sizeof(header_t));
    if(mByteOrder==ByteOrder::LittelEndian)
        rowSize = le2h32(rowSize);
    else
        rowSize = be2h32(rowSize);

    mRecordSize = rowSize + 2 * sizeof(uint32_t);

    // Evaluate number of records
    mFile.seekg(0, std::ios_base::end);
    mFileSize = mFile.tellg();
    mRecordsCount = mFileSize/mRecordSize;

    // Update quantities offset to ignore the record size attribute
    // in each row.
    for(Quantity& q: mQuantities)
        q.offset += sizeof(uint32_t);
}

void Reader::parseErgFormat()
{
    // Read the header of the file and check for bad format
    header_t header;
    std::memset(&header, 0, sizeof(header_t));
    mFile.seekg(0, std::ios_base::beg);
    mFile.read(reinterpret_cast<char*>(&header), sizeof(header_t));

    if(std::strncmp(reinterpret_cast<char*>(header.identifier), "CM-ERG", sizeof("CM-ERG"))!=0) {
        close();
        throw std::runtime_error("Not an erg file: "+mFilename);
    }

    if(header.version!=1) {
        close();
        throw std::runtime_error("Unsupported version: " + std::to_string(header.version));
    }

    // Check endianess and record size according to the
    // companion file.
    size_t expectedRecordSize = 0;
    if(header.byte_order==0)
    {
        if(mByteOrder!=ByteOrder::LittelEndian) {
            close();
            throw std::runtime_error("Unexpected endianess specified in .erg file.");
        }
        expectedRecordSize = le2h16(header.record_size);
    }
    else
    {
        if(mByteOrder!=ByteOrder::BigEndian) {
            close();
            throw std::runtime_error("Unexpected endianess specified in .erg file.");
        }
        expectedRecordSize = be2h16(header.record_size);
    }
    if(expectedRecordSize!=mRecordSize) {
        close();
        throw std::runtime_error("Unexpected record size specified in .erg file.");
    }

    // Evaluate number of records
    mFile.seekg(0, std::ios_base::end);
    mFileSize = mFile.tellg();
    mRecordsCount = (mFileSize-sizeof(header_t))/mRecordSize;
}


size_t Reader::dataSize(const Type type) noexcept(false)
{
    switch (type)
    {
    case Type::Int8:
    case Type::Uint8:
        return sizeof(int8_t);
    case Type::Int16:
    case Type::Uint16:
        return sizeof(int16_t);
    case Type::Int32:
    case Type::Uint32:
        return sizeof(int32_t);
    case Type::Int64:
    case Type::Uint64:
        return sizeof(int64_t);
    case Type::Float:
        return sizeof(float);
    case Type::Double:
        return sizeof(double);
    case Type::Void:
    default:
        throw std::runtime_error("Unknown data type size.");
    }

}

Type Reader::dataType(const std::string& typestr) noexcept(true)
{
    return dataType(typestr.c_str());
}

Type Reader::dataType(const char* typestr) noexcept(true)
{
    std::string t = typestr;
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);

    if(t=="double")
        return Type::Double;
    if(t=="float")
        return Type::Float;
    if(t=="longlong")
        return Type::Int64;
    if(t=="ulonglong")
        return Type::Uint64;
    if(t=="int" || t=="long")
        return Type::Int32;
    if(t=="uint" || t=="ulong")
        return Type::Uint32;
    if(t=="short")
        return Type::Int16;
    if(t=="ushort")
        return Type::Uint16;
    if(t=="char")
        return Type::Int8;
    if(t=="uchar")
        return Type::Uint8;

    return Type::Void;
}


}


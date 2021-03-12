/**********************************************************************************
 *   08/03/2015                                                                   *
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

#include <gtest/gtest.h>
#include <math.h>

#include "erg.h"

const std::string ERG_1_FILENAME = "../../data/test_data1.erg";
const std::string ERG_2_FILENAME = "../../data/test_data2.erg";
const std::string ERG_3_FILENAME = "../../data/test_data3.erg";
const std::string ERG_4_FILENAME = "../../data/fortran_data.erg";


TEST(Reader, Open)
{
    // If the file is ok, the open function don't throws
    // and the file is parsed correctly.

    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));
    ASSERT_EQ(parser.recordSize(), 56);
    ASSERT_EQ(parser.numQuanities(), 12);
    ASSERT_EQ(parser.records(), 602372);
    ASSERT_TRUE(parser.isErg());

    ASSERT_NO_THROW(parser.open(ERG_2_FILENAME));
    ASSERT_EQ(parser.recordSize(), 56);
    ASSERT_EQ(parser.numQuanities(), 12);
    ASSERT_EQ(parser.records(), 602372);
    ASSERT_TRUE(parser.isErg());

    ASSERT_ANY_THROW(parser.open(ERG_3_FILENAME));

    ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
    ASSERT_EQ(parser.recordSize(), 48);
    ASSERT_EQ(parser.numQuanities(), 10);
    ASSERT_EQ(parser.records(), 13410);
    ASSERT_FALSE(parser.isErg());
    ASSERT_TRUE(parser.isFortran());
}


TEST(Reader, Has)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_TRUE(parser.has("Data_8"));
    ASSERT_TRUE(parser.has("Data_1"));
    ASSERT_TRUE(parser.has("Data_2"));
    ASSERT_TRUE(parser.has("Data_3"));
    ASSERT_TRUE(parser.has("Data_4"));
    ASSERT_TRUE(parser.has("UserOut_00"));
    ASSERT_TRUE(parser.has("UserOut_01"));
    ASSERT_TRUE(parser.has("UserOut_02"));
    ASSERT_TRUE(parser.has("Data_5"));
    ASSERT_TRUE(parser.has("Data_6"));
    ASSERT_TRUE(parser.has("Data_7"));
    ASSERT_TRUE(parser.has("Data_9"));
    ASSERT_FALSE(parser.has("Data_10"));
    ASSERT_FALSE(parser.has("none"));
    ASSERT_FALSE(parser.has("$none$"));

    ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
    ASSERT_TRUE(parser.has("Data_8"));
    ASSERT_TRUE(parser.has("data_0"));
    ASSERT_TRUE(parser.has("data_1"));
    ASSERT_TRUE(parser.has("data_2"));
    ASSERT_TRUE(parser.has("data_3"));
    ASSERT_TRUE(parser.has("data_4"));
    ASSERT_TRUE(parser.has("data_5"));
    ASSERT_TRUE(parser.has("data_6"));
    ASSERT_TRUE(parser.has("data_7"));
    ASSERT_TRUE(parser.has("data_8"));
    ASSERT_FALSE(parser.has("$none$"));
}

TEST(Reader, Index)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_EQ(parser.index("Data_8"), 0);
    ASSERT_EQ(parser.index("Data_1"), 1);
    ASSERT_EQ(parser.index("Data_2"), 2);
    ASSERT_EQ(parser.index("Data_3"), 3);
    ASSERT_EQ(parser.index("Data_4"), 4);
    ASSERT_EQ(parser.index("UserOut_00"), 5);
    ASSERT_EQ(parser.index("UserOut_01"), 6);
    ASSERT_EQ(parser.index("UserOut_02"), 7);
    ASSERT_EQ(parser.index("Data_5"), 8);
    ASSERT_EQ(parser.index("Data_6"), 9);
    ASSERT_EQ(parser.index("Data_7"), 10);
    ASSERT_EQ(parser.index("Data_9"), 11);
    ASSERT_ANY_THROW(parser.index("Data_10"));
    ASSERT_ANY_THROW(parser.index("none"));
    ASSERT_ANY_THROW(parser.index("$none$"));

    ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
    ASSERT_EQ(parser.index("Data_8"), 9);
    ASSERT_EQ(parser.index("data_0"), 0);
    ASSERT_EQ(parser.index("data_1"), 1);
    ASSERT_EQ(parser.index("data_2"), 2);
    ASSERT_EQ(parser.index("data_3"), 3);
    ASSERT_EQ(parser.index("data_4"), 4);
    ASSERT_EQ(parser.index("data_5"), 5);
    ASSERT_EQ(parser.index("data_6"), 6);
    ASSERT_EQ(parser.index("data_7"), 7);
    ASSERT_EQ(parser.index("data_8"), 8);
    ASSERT_ANY_THROW(parser.index("$none$"));
}

TEST(Reader, DatasetSize)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));
    size_t rows = parser.records();

    ASSERT_EQ(parser.quantitySize(0), sizeof(double)*rows);
    ASSERT_EQ(parser.quantitySize(1), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(2), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(3), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(4), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(5), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(6), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(7), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(8), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(9), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(10), sizeof(float)*rows);
    ASSERT_EQ(parser.quantitySize(11), sizeof(int)*rows);
    ASSERT_ANY_THROW(parser.quantitySize(12));

    ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
    rows = parser.records();
    for(int i=0; i<parser.numQuanities(); ++i)
        ASSERT_EQ(parser.quantitySize(i), sizeof(float)*rows);
}

TEST(Reader, DatasetName)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_EQ(parser.quantityName(0), "Data_8");
    ASSERT_EQ(parser.quantityName(1), "Data_1");
    ASSERT_EQ(parser.quantityName(2), "Data_2");
    ASSERT_EQ(parser.quantityName(3), "Data_3");
    ASSERT_EQ(parser.quantityName(4), "Data_4");
    ASSERT_EQ(parser.quantityName(5), "UserOut_00");
    ASSERT_EQ(parser.quantityName(6), "UserOut_01");
    ASSERT_EQ(parser.quantityName(7), "UserOut_02");
    ASSERT_EQ(parser.quantityName(8), "Data_5");
    ASSERT_EQ(parser.quantityName(9), "Data_6");
    ASSERT_EQ(parser.quantityName(10), "Data_7");
    ASSERT_EQ(parser.quantityName(11), "Data_9");
    ASSERT_ANY_THROW(parser.quantityName(12));
}

TEST(Reader, DatasetUnit)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_EQ(parser.quantityUnit(0), "s");
    ASSERT_EQ(parser.quantityUnit(1), "m");
    ASSERT_EQ(parser.quantityUnit(2), "rad/s");
    ASSERT_EQ(parser.quantityUnit(3), "Nm");
    ASSERT_EQ(parser.quantityUnit(4), "");
    ASSERT_EQ(parser.quantityUnit(5), "");
    ASSERT_EQ(parser.quantityUnit(6), "");
    ASSERT_EQ(parser.quantityUnit(7), "");
    ASSERT_EQ(parser.quantityUnit(8), "m/s");
    ASSERT_EQ(parser.quantityUnit(9), "m/s");
    ASSERT_EQ(parser.quantityUnit(10), "m/s^2");
    ASSERT_EQ(parser.quantityUnit(11), "");
    ASSERT_ANY_THROW(parser.quantityUnit(12));
}


TEST(Reader, ReadAll)
{
    erg::Reader parser;

    // Read all erg format
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    std::vector< std::vector<uint8_t> > data(parser.numQuanities());
    for(int i=0; i<data.size(); ++i)
        data[i].assign(parser.quantitySize(i), 0);

    std::vector<uint8_t*> dataWrapper;
    std::vector<size_t> dataWrapperSize;
    for(std::vector<uint8_t>& v: data)
    {
        dataWrapper.push_back(v.data());
        dataWrapperSize.push_back(v.size());
    }

    size_t numRows = parser.readAll(dataWrapper, dataWrapperSize);

    ASSERT_EQ(numRows, parser.records());

    double* timedataset = reinterpret_cast<double*>(data[0].data());
    for(int i=0; i<numRows; ++i)
    {
        int value = std::lround(timedataset[i]*100.0);
        ASSERT_EQ(value, i);
    }

    // Read all Fortran binary format
    ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));

    data.clear();
    data.assign(parser.numQuanities(), std::vector<uint8_t>());
    for(int i=0; i<data.size(); ++i)
        data[i].assign(parser.quantitySize(i), 0);

    dataWrapper.clear();
    dataWrapperSize.clear();
    for(std::vector<uint8_t>& v: data)
    {
        dataWrapper.push_back(v.data());
        dataWrapperSize.push_back(v.size());
    }

    numRows = parser.readAll(dataWrapper, dataWrapperSize);

    ASSERT_EQ(numRows, parser.records());

    float* timedataset2 = reinterpret_cast<float*>(data[9].data());
    for(int i=0; i<numRows; ++i)
    {
        int value = std::lround(timedataset2[i]*100.0);
        ASSERT_EQ(value, i);
    }
}

TEST(Reader, Read)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    std::vector<double> Time(parser.records(), 0.0);
    size_t timeIndex = parser.index("Data_8");
    size_t numRows = parser.read(timeIndex, reinterpret_cast<uint8_t*>(Time.data()), Time.size()*sizeof(double));
    ASSERT_EQ(numRows, parser.records());

    for(int i=0; i<numRows; ++i)
    {
        int value = std::lround(Time[i]*100.0);
        ASSERT_EQ(value, i);
    }

    std::vector<double> Time2(100, 0.0);
    numRows = parser.read(timeIndex, 1000, 100, reinterpret_cast<uint8_t*>(Time2.data()), Time2.size()*sizeof(double));
    ASSERT_EQ(numRows, 100);

    for(int i=0; i<numRows; ++i)
    {
        int value = std::lround(Time2[i]*100.0);
        ASSERT_EQ(value, i+1000);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


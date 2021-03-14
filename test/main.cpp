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

const std::string ERG_1_FILENAME = "../../test-data/Test-Dataset-1_175937.erg";
//const std::string ERG_2_FILENAME = "../../test-data/test_data2.erg";
//const std::string ERG_3_FILENAME = "../../test-data/test_data3.erg";
//const std::string ERG_4_FILENAME = "../../test-data/fortran_data.erg";


TEST(Reader, Open)
{
    // If the file is ok, the open function don't throws
    // and the file is parsed correctly.

    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));
    ASSERT_EQ(parser.recordSize(), 120);
    ASSERT_EQ(parser.numQuanities(), 29);
    ASSERT_EQ(parser.records(), 411847);
    ASSERT_TRUE(parser.isErg());

    /*ASSERT_NO_THROW(parser.open(ERG_2_FILENAME));
    ASSERT_EQ(parser.recordSize(), 56);
    ASSERT_EQ(parser.numQuanities(), 12);
    ASSERT_EQ(parser.records(), 602372);
    ASSERT_TRUE(parser.isErg());*/

    //ASSERT_ANY_THROW(parser.open(ERG_3_FILENAME));

    /*ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
    ASSERT_EQ(parser.recordSize(), 48);
    ASSERT_EQ(parser.numQuanities(), 10);
    ASSERT_EQ(parser.records(), 13410);
    ASSERT_FALSE(parser.isErg());
    ASSERT_TRUE(parser.isFortran());*/
}


TEST(Reader, Has)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_TRUE(parser.has("Time"));
    ASSERT_TRUE(parser.has("Brake.Park"));
    ASSERT_TRUE(parser.has("Brake.Pedal"));
    ASSERT_TRUE(parser.has("Car.PitchVel"));
    ASSERT_TRUE(parser.has("Car.RollVel"));
    ASSERT_TRUE(parser.has("Car.WheelSpd_FL"));
    ASSERT_TRUE(parser.has("Car.WheelSpd_FR"));
    ASSERT_TRUE(parser.has("Car.WheelSpd_RL"));
    ASSERT_TRUE(parser.has("Car.WheelSpd_RR"));
    ASSERT_TRUE(parser.has("Car.ax"));
    ASSERT_TRUE(parser.has("Car.ay"));
    ASSERT_TRUE(parser.has("Car.az"));
    ASSERT_TRUE(parser.has("Driver.Brake"));
    ASSERT_TRUE(parser.has("Driver.Gas"));
    ASSERT_TRUE(parser.has("Driver.Steer.AngAcc"));
    ASSERT_TRUE(parser.has("Driver.Steer.AngVel"));
    ASSERT_TRUE(parser.has("Env.WindVel_ext.x"));
    ASSERT_TRUE(parser.has("Env.WindVel_ext.y"));
    ASSERT_TRUE(parser.has("Env.WindVel_ext.z"));
    ASSERT_TRUE(parser.has("Env.WindVel_tot.x"));
    ASSERT_TRUE(parser.has("Env.WindVel_tot.y"));
    ASSERT_TRUE(parser.has("Env.WindVel_tot.z"));
    ASSERT_TRUE(parser.has("Steer.WhlAng"));
    ASSERT_TRUE(parser.has("Vhcl.Engine.rotv"));
    ASSERT_TRUE(parser.has("Vhcl.PitchVel"));
    ASSERT_TRUE(parser.has("Vhcl.RollVel"));
    ASSERT_TRUE(parser.has("Vhcl.YawRate"));
    ASSERT_TRUE(parser.has("Vhcl.v"));
    ASSERT_TRUE(parser.has("Driver.GearNo"));

    /*ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
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
    ASSERT_FALSE(parser.has("$none$"));*/
}

TEST(Reader, Index)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_EQ(parser.index("Time"), 0);
    ASSERT_EQ(parser.index("Brake.Park"), 1);
    ASSERT_EQ(parser.index("Brake.Pedal"), 2);
    ASSERT_EQ(parser.index("Car.PitchVel"), 3);
    ASSERT_EQ(parser.index("Car.RollVel"), 4);
    ASSERT_EQ(parser.index("Car.WheelSpd_FL"), 5);
    ASSERT_EQ(parser.index("Car.WheelSpd_FR"), 6);
    ASSERT_EQ(parser.index("Car.WheelSpd_RL"), 7);
    ASSERT_EQ(parser.index("Car.WheelSpd_RR"), 8);
    ASSERT_EQ(parser.index("Car.ax"), 9);
    ASSERT_EQ(parser.index("Car.ay"), 10);
    ASSERT_EQ(parser.index("Car.az"), 11);
    ASSERT_EQ(parser.index("Driver.Brake"), 12);
    ASSERT_EQ(parser.index("Driver.Gas"), 13);
    ASSERT_EQ(parser.index("Driver.Steer.AngAcc"), 14);
    ASSERT_EQ(parser.index("Driver.Steer.AngVel"), 15);
    ASSERT_EQ(parser.index("Env.WindVel_ext.x"), 16);
    ASSERT_EQ(parser.index("Env.WindVel_ext.y"), 17);
    ASSERT_EQ(parser.index("Env.WindVel_ext.z"), 18);
    ASSERT_EQ(parser.index("Env.WindVel_tot.x"), 19);
    ASSERT_EQ(parser.index("Env.WindVel_tot.y"), 20);
    ASSERT_EQ(parser.index("Env.WindVel_tot.z"), 21);
    ASSERT_EQ(parser.index("Steer.WhlAng"), 22);
    ASSERT_EQ(parser.index("Vhcl.Engine.rotv"), 23);
    ASSERT_EQ(parser.index("Vhcl.PitchVel"), 24);
    ASSERT_EQ(parser.index("Vhcl.RollVel"), 25);
    ASSERT_EQ(parser.index("Vhcl.YawRate"), 26);
    ASSERT_EQ(parser.index("Vhcl.v"), 27);
    ASSERT_EQ(parser.index("Driver.GearNo"), 28);
    //ASSERT_ANY_THROW(parser.index("$none$"));

    /*ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
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
    ASSERT_ANY_THROW(parser.index("$none$"));*/
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

    /*ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));
    rows = parser.records();
    for(int i=0; i<parser.numQuanities(); ++i)
        ASSERT_EQ(parser.quantitySize(i), sizeof(float)*rows);*/
}

TEST(Reader, DatasetName)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_EQ(parser.quantityName(0), "Time");
    ASSERT_EQ(parser.quantityName(1), "Brake.Park");
    ASSERT_EQ(parser.quantityName(3), "Car.PitchVel");
    ASSERT_EQ(parser.quantityName(5), "Car.WheelSpd_FL");
    ASSERT_EQ(parser.quantityName(9), "Car.ax");
    ASSERT_EQ(parser.quantityName(12), "Driver.Brake");
    ASSERT_EQ(parser.quantityName(14), "Driver.Steer.AngAcc");
    ASSERT_EQ(parser.quantityName(16), "Env.WindVel_ext.x");
    ASSERT_EQ(parser.quantityName(22), "Steer.WhlAng");
    ASSERT_EQ(parser.quantityName(23), "Vhcl.Engine.rotv");
    ASSERT_EQ(parser.quantityName(27), "Vhcl.v");
    ASSERT_EQ(parser.quantityName(28), "Driver.GearNo");
}

TEST(Reader, DatasetUnit)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    ASSERT_EQ(parser.quantityUnit(0), "s");
    ASSERT_EQ(parser.quantityUnit(1), "");
    ASSERT_EQ(parser.quantityUnit(8), "rad/s");
    ASSERT_EQ(parser.quantityUnit(9), "m/s^2");
    ASSERT_EQ(parser.quantityUnit(11), "m/s^2");
    ASSERT_EQ(parser.quantityUnit(14), "rad/s^2");
    ASSERT_EQ(parser.quantityUnit(16), "m/s");
    ASSERT_EQ(parser.quantityUnit(22), "rad");
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
        int value = std::lround(timedataset[i]*1000.0);
        ASSERT_EQ(value, i);
    }

    // Read all Fortran binary format
    /*ASSERT_NO_THROW(parser.open(ERG_4_FILENAME));

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
    }*/
}

TEST(Reader, Read)
{
    erg::Reader parser;
    ASSERT_NO_THROW(parser.open(ERG_1_FILENAME));

    std::vector<double> Time(parser.records(), 0.0);
    size_t timeIndex = parser.index("Time");
    size_t numRows = parser.read(timeIndex, reinterpret_cast<uint8_t*>(Time.data()), Time.size()*sizeof(double));
    ASSERT_EQ(numRows, parser.records());

    for(int i=0; i<numRows; ++i)
    {
        int value = std::lround(Time[i]*1000.0);
        ASSERT_EQ(value, i);
    }

    std::vector<double> Time2(100, 0.0);
    numRows = parser.read(timeIndex, 1000, 100, reinterpret_cast<uint8_t*>(Time2.data()), Time2.size()*sizeof(double));
    ASSERT_EQ(numRows, 100);

    for(int i=0; i<numRows; ++i)
    {
        int value = std::lround(Time2[i]*1000.0);
        ASSERT_EQ(value, i+1000);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


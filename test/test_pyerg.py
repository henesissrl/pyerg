## ---------------------------------------------------------------------------- ##
#   08/03/2015                                                                   #
#                                                                                #
#   www.henesis.eu                                                               #
#                                                                                #
#   Alessandro Bacchini - alessandro.bacchini@henesis.eu                         #
#                                                                                #
# Copyright (c) 2015, Henesis s.r.l. part of Camlin Group                        #
#                                                                                #
# The MIT License (MIT)                                                          #
#                                                                                #
# Permission is here by granted, free of charge, to any person obtaining a copy  #
# of this software and associated documentation files (the "Software"), to deal  #
# in the Software without restriction, including without limitation the rights   #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      #
# copies of the Software, and to permit persons to whom the Software is          #
# furnished to do so, subject to the following conditions:                       #
#                                                                                #
# The above copyright notice and this permission notice shall be included in all #
# copies or substantial portions of the Software.                                #
#                                                                                #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  #
# SOFTWARE.                                                                      #
## ---------------------------------------------------------------------------- ##

import unittest
import pyerg
import numpy as np

ERG_1_FILENAME = "../test-data/Test-Dataset-1_175937.erg"
#ERG_2_FILENAME = "../data/test_data2.erg"
#ERG_3_FILENAME = "../data/test_data3.erg"
#ERG_4_FILENAME = "../data/fortran_data.erg"


class TestPyergReader(unittest.TestCase):

    def setUp(self):
        self.parser = pyerg.Reader()

    def tearDown(self):
        self.pareser = None

    def test_Open(self):
        parser = self.parser

        parser.open(ERG_1_FILENAME)
        self.assertEquals(parser.recordSize(), 56)
        self.assertEquals(parser.numQuanities(), 12)
        self.assertEquals(parser.records(), 602372)
        self.assertTrue(parser.isErg())

        #parser.open(ERG_2_FILENAME)
        #self.assertEquals(parser.recordSize(), 56)
        #self.assertEquals(parser.numQuanities(), 12)
        #self.assertEquals(parser.records(), 602372)
        #self.assertTrue(parser.isErg())

        #self.assertRaises(NameError, parser.open, ERG_3_FILENAME)

        #parser.open(ERG_4_FILENAME)
        #self.assertEquals(parser.recordSize(), 48)
        #self.assertEquals(parser.numQuanities(), 10)
        #self.assertEquals(parser.records(), 13410)
        #self.assertFalse(parser.isErg())
        #self.assertTrue(parser.isFortran())

    def test_Has(self):
        parser = self.parser

        parser.open(ERG_1_FILENAME)
        self.assertTrue(parser.has("Data_8"))
        self.assertTrue(parser.has("Data_1"))
        self.assertTrue(parser.has("Data_2"))
        self.assertTrue(parser.has("Data_3"))
        self.assertTrue(parser.has("Data_4"))
        self.assertTrue(parser.has("UserOut_00"))
        self.assertTrue(parser.has("UserOut_01"))
        self.assertTrue(parser.has("UserOut_02"))
        self.assertTrue(parser.has("Data_5"))
        self.assertTrue(parser.has("Data_6"))
        self.assertTrue(parser.has("Data_7"))
        self.assertTrue(parser.has("Data_9"))
        self.assertFalse(parser.has("Data_10"))
        self.assertFalse(parser.has("none"))
        self.assertFalse(parser.has("$none$"))

        #parser.open(ERG_4_FILENAME)
        #self.assertTrue(parser.has("Data_8"))
        #self.assertTrue(parser.has("data_0"))
        #self.assertTrue(parser.has("data_1"))
        #self.assertTrue(parser.has("data_2"))
        #self.assertTrue(parser.has("data_3"))
        #self.assertTrue(parser.has("data_4"))
        #self.assertTrue(parser.has("data_5"))
        #self.assertTrue(parser.has("data_6"))
        #self.assertTrue(parser.has("data_7"))
        #self.assertTrue(parser.has("data_8"))
        #self.assertFalse(parser.has("$none$"))

    def test_Index(self):
        parser = self.parser

        parser.open(ERG_1_FILENAME)
        self.assertEquals(parser.index("Data_8"), 0)
        self.assertEquals(parser.index("Data_1"), 1)
        self.assertEquals(parser.index("Data_2"), 2)
        self.assertEquals(parser.index("Data_3"), 3)
        self.assertEquals(parser.index("Data_4"), 4)
        self.assertEquals(parser.index("UserOut_00"), 5)
        self.assertEquals(parser.index("UserOut_01"), 6)
        self.assertEquals(parser.index("UserOut_02"), 7)
        self.assertEquals(parser.index("Data_5"), 8)
        self.assertEquals(parser.index("Data_6"), 9)
        self.assertEquals(parser.index("Data_7"), 10)
        self.assertEquals(parser.index("Data_9"), 11)
        self.assertRaises(NameError, parser.index, "Data_10")
        self.assertRaises(NameError, parser.index, "none")
        self.assertRaises(NameError, parser.index, "$none$")

        #parser.open(ERG_4_FILENAME)
        #self.assertEquals(parser.index("Data_8"), 9)
        #self.assertEquals(parser.index("data_0"), 0)
        #self.assertEquals(parser.index("data_1"), 1)
        #self.assertEquals(parser.index("data_2"), 2)
        #self.assertEquals(parser.index("data_3"), 3)
        #self.assertEquals(parser.index("data_4"), 4)
        #self.assertEquals(parser.index("data_5"), 5)
        #self.assertEquals(parser.index("data_6"), 6)
        #self.assertEquals(parser.index("data_7"), 7)
        #self.assertEquals(parser.index("data_8"), 8)
        #self.assertRaises(NameError, parser.index, "$none$")

    def test_DatasetSize(self):
        parser = self.parser

        parser.open(ERG_1_FILENAME)
        rows = parser.records()
        self.assertEquals(parser.quantitySize(0), 8*rows)
        self.assertEquals(parser.quantitySize(1), 4*rows)
        self.assertEquals(parser.quantitySize(2), 4*rows)
        self.assertEquals(parser.quantitySize(3), 4*rows)
        self.assertEquals(parser.quantitySize(4), 4*rows)
        self.assertEquals(parser.quantitySize(5), 4*rows)
        self.assertEquals(parser.quantitySize(6), 4*rows)
        self.assertEquals(parser.quantitySize(7), 4*rows)
        self.assertEquals(parser.quantitySize(8), 4*rows)
        self.assertEquals(parser.quantitySize(9), 4*rows)
        self.assertEquals(parser.quantitySize(10), 4*rows)
        self.assertEquals(parser.quantitySize(11), 4*rows)
        self.assertRaises(NameError, parser.quantitySize, 12)
        self.assertEquals(parser.quantitySize("Data_8"), 8*rows)

        #parser.open(ERG_4_FILENAME)
        #rows = parser.records()
        #for i in xrange(parser.numQuanities()):
        #    self.assertEquals(parser.quantitySize(i), 4*rows)
        #self.assertEquals(parser.quantitySize("data_0"), 4*rows)
        #self.assertEquals(parser.quantitySize("data_4"), 4*rows)
        #self.assertEquals(parser.quantitySize("Data_8"), 4*rows)

    def test_DatasetName(self):
        parser = self.parser
        parser.open(ERG_1_FILENAME)

        self.assertEquals(parser.quantityName(0), "Data_8")
        self.assertEquals(parser.quantityName(1), "Data_1")
        self.assertEquals(parser.quantityName(2), "Data_2")
        self.assertEquals(parser.quantityName(3), "Data_3")
        self.assertEquals(parser.quantityName(4), "Data_4")
        self.assertEquals(parser.quantityName(5), "UserOut_00")
        self.assertEquals(parser.quantityName(6), "UserOut_01")
        self.assertEquals(parser.quantityName(7), "UserOut_02")
        self.assertEquals(parser.quantityName(8), "Data_5")
        self.assertEquals(parser.quantityName(9), "Data_6")
        self.assertEquals(parser.quantityName(10), "Data_7")
        self.assertEquals(parser.quantityName(11), "Data_9")
        self.assertRaises(NameError, parser.quantityName, 12)

    def test_DatasetUnit(self):
        parser = self.parser

        parser.open(ERG_1_FILENAME)
        self.assertEquals(parser.quantityUnit(0), "s")
        self.assertEquals(parser.quantityUnit(1), "m")
        self.assertEquals(parser.quantityUnit(2), "rad/s")
        self.assertEquals(parser.quantityUnit(3), "Nm")
        self.assertEquals(parser.quantityUnit(4), "")
        self.assertEquals(parser.quantityUnit(5), "")
        self.assertEquals(parser.quantityUnit(6), "")
        self.assertEquals(parser.quantityUnit(7), "")
        self.assertEquals(parser.quantityUnit(8), "m/s")
        self.assertEquals(parser.quantityUnit(9), "m/s")
        self.assertEquals(parser.quantityUnit(10), "m/s^2")
        self.assertEquals(parser.quantityUnit(11), "")
        self.assertRaises(NameError, parser.quantityUnit, 12)

    def test_ReadAll(self):
        parser = self.parser

        parser.open(ERG_1_FILENAME)
        data = parser.readAll()
        self.assertIn('Data_8', data.keys())
        t = data['Data_8']
        self.assertEquals(len(t), parser.records())

    def test_Read(self):
        parser = self.parser

        parser.open(ERG_1_FILENAME)
        t = parser.read('Data_8')
        self.assertEquals(len(t), parser.records())

        t2 = parser.read('Data_8', start=10, count=90)
        self.assertEquals(len(t2), 90)
        self.assertTrue(np.all(t2 == t[10:100]))


class TestPyerg(unittest.TestCase):

    def test_read(self):
        parser = pyerg.Reader()
        parser.open(ERG_1_FILENAME)
        t0 = parser.read('Data_8')

        data = pyerg.read(ERG_1_FILENAME)
        self.assertIn('Data_8', data.keys())
        t1 = data['Data_8']
        self.assertTrue(np.all(t0 == t1))

    def test_CanRead(self):
        self.assertTrue(pyerg.can_read(ERG_1_FILENAME))
        #self.assertTrue(pyerg.can_read(ERG_2_FILENAME))
        #self.assertFalse(pyerg.can_read(ERG_3_FILENAME))
        #self.assertTrue(pyerg.can_read(ERG_4_FILENAME))

    def test_version(self):
        self.assertTrue(isinstance(pyerg.__version__, str))


if __name__ == '__main__':
    unittest.main()

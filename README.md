# Pyerg

Pyerg is a C++ library with a Python wrapper for reading the ERG files produced by IPG CarMaker program.
The library implements the specifications of the ERG file format described in the IPG CarMaker reference manual.

Henesis s.r.l. is not affiliated with IPG, the developer of CarMaker program.

## Requisites

### Python library

- Linux, Windows (tested within Anaconda)
- Python >=3.7
- Numpy >= 1.14
- GCC >= 4.7

### C++ library

- CMake
- Google test library (optional, for building the C++ library)

## Install Python extension

Run `python setup.py install` to install the `pyerg` module.

## Build C++ library

Create a `build` directory in the current folder and from that directory run:

```
cmake ..
```

And then

```
make
```

The default behavior is to build a static (`erg_s`) and a shared (`erg`) version of the Erg parser library.

### CMake options:

- `BUILD_PYTHON_MODULE` enable the building of the `pyerg` Python module.
  When building the Python module using CMake, please check that the `NUMPY_INCLUDE_DIR`
  in the `pyerg/CMakeLists.txt` contains the right path to the Numpy headers.
- `BUILD_TESTS` enable the building test program for the C++ library. Data for testing is not included.

Use:

```
cmake .. -DBUILD_PYTHON_MODULE=ON -DBUILD_TESTS=ON
```

To build the C++ library, the Python `pyerg` module and the C++ tests.


## Examples of use

### C++

```
#include <erg.h>

...

erg::Reader parser;

// Read all erg format
parser.open("my_file.erg");

// Allocate the destination memory using vector<byte> as container
std::vector<std::vector<uint8_t>> data(parser.numQuanities());
for(int i=0; i<data.size(); ++i)
    data[i].assign(parser.quantitySize(i), 0);

// Create a vector of pointer to the allocated memory to
// store the data
std::vector<uint8_t*> dataPtrList;
std::vector<size_t> dataPtrListSize;
for(std::vector<uint8_t>& v: data)
{
    dataPtrList.push_back(v.data());
    dataPtrListSize.push_back(v.size());
}

size_t numRows = parser.readAll(dataPtrList, dataWrapperSize);

// data is a vector of vector<byte> with all the quantities from
// the erg file.
```

### Python

```
import pyerg

parser = pyerg.Reader()
parser.open("my_file.erg")
data = parser.readAll()

# data is a dict of numpy.ndarray with all the quantities from
# the erg file.

```

See the test applications for more usage examples.
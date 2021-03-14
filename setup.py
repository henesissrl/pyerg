#! /usr/bin/python

# ------------------------------------------------------------------------------
# Copyright (c) 2015, Henesis s.r.l. part of Camlin Group
#
# The MIT License (MIT)
#
# Permission is here by granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# ------------------------------------------------------------------------------

from setuptools import setup, Extension
import site
import io

python_libs = site.getsitepackages()
numpyInclude0 = python_libs[0] + '/numpy/core/include'
numpyInclude1 = python_libs[1] + '/numpy/core/include'

pyergCmodule = Extension('pyerg',
                         ['erg/erg.cpp', 'pyerg/pyerg.cpp'],
                         include_dirs=[numpyInclude0, numpyInclude1, 'erg'],
                         extra_compile_args=['-std=c++11'],
                         language='c++')

classifiers = [
    # How mature is this project? Common values are
    #   3 - Alpha
    #   4 - Beta
    #   5 - Production/Stable
    'Development Status :: 4 - Beta',

    # Indicate who your project is intended for
    'Intended Audience :: Science/Research',
    'Topic :: Scientific/Engineering',

    # Pick your license as you wish (should match "license" above)
    'License :: OSI Approved :: MIT License',

    # Specify the Python versions you support here.
    'Programming Language :: Python :: 3.7',
    'Programming Language :: Python :: 3.8',
    'Programming Language :: Python :: 3.9'
]

# Choose your license
license = 'MIT'

def read(filename):
    with io.open(filename, mode="r", encoding='utf-8') as fd:
        return fd.read()

setup(name='pyerg',
      version='0.6.1',
      description='Python C extension to read IPG CarMaker ERG files',
      long_description=read("README.md"),
      long_description_content_type='text/markdown',
      author='Alessandro Bacchini',
      author_email='alessandro.bacchini@henesis.eu',
      url='http://www.henesis.eu',
      ext_modules=[pyergCmodule],
      classifiers=classifiers,
      license=license, 
      install_requires=['numpy>=1.14'])

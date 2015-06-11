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

python_libs = site.getsitepackages()
numpyInclude0 = python_libs[0] + '/numpy/core/include'
numpyInclude1 = python_libs[1] + '/numpy/core/include'

pyergCmodule = Extension('pyerg',
                         ['erg/erg.cpp', 'pyerg/pyerg.cpp'],
                         include_dirs=[numpyInclude0, numpyInclude1, 'erg'],
                         extra_compile_args=['-std=c++11'],
                         language='c++')

setup(name='pyerg',
      version='0.5.0',
      description='Python C extension for reading IPG CarMaker ERG files.',
      author='Alessandro Bacchini',
      author_email='alessandro.bacchini@henesis.eu',
      url='http://www.henesis.eu',
      ext_modules=[pyergCmodule],
      install_requires=['numpy>=1.7'])

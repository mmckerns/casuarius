from distutils.core import setup
from distutils.extension import Extension

from Cython.Distutils import build_ext
cmdclass = dict(build_ext=build_ext)

long_description = """\
A Cython wrapper for the Cassowary incremental constraint solver.

The solver source code is derived from the 0.6 release. It has been modified by
Svilen Dobrev to remove memory leaks. His work can be found separately as SWIG
bindings:

    http://pypi.python.org/pypi/cassowarypy

His modifications are available under the BSD license, while Casuarius as
a whole follows the license of Cassowary itself, LGPL v2.1 or (at your option)
a later version of the LGPL. See the files LICENSE and COPYING.LGPL for details.

Please be sure to have Cython 0.15.1 or above in order to compile this extension
module. It has been tested on OS X (using llvm-gcc 4.2) and Windows (using
mingw). Other Windows C++ compilers may or may not work.
"""

cysw_module = Extension(
    'casuarius',
    sources=[
        'casuarius.pyx',
        'cassowary/AbstractVariable.cc',
        'cassowary/Constraint.cc',
        'cassowary/DummyVariable.cc',
        'cassowary/Errors.cc',
        'cassowary/FDVariable.cc',
        'cassowary/FloatVariable.cc',
        'cassowary/LinearExpression.cc',
        'cassowary/SimplexSolver.cc',
        'cassowary/SlackVariable.cc',
        'cassowary/Solver.cc',
        'cassowary/Strength.cc',
        'cassowary/SymbolicWeight.cc',
        'cassowary/Tableau.cc',
        'cassowary/Variable.cc',
        'cysw_support.cc',
    ],
    language="c++",
    include_dirs=['.', 'cassowary']
)

setup(
    name = 'casuarius',
    version = '1.0b1',
    author = 'Robert Kern',
    author_email = 'rkern@enthought.com',
    description = 'Cython bindings for the Cassowary constraint solver.',
    long_description = long_description,
    url = 'https://github.com/enthought/casuarius',
    ext_modules = [cysw_module],
    classifiers = [
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: GNU Library or Lesser General Public License (LGPL)",
        "Operating System :: OS Independent",
        "Programming Language :: C++",
        "Programming Language :: Python",
        "Topic :: Software Development :: User Interfaces",
    ],
    cmdclass = cmdclass,
)


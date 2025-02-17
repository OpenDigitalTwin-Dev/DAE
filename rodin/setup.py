import os
import sys

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

from setuptools import find_packages

setup(
    name="rodin",
    version="0.0.1",
    description="Rodin Python bindings",
    author="Carlos Brito-Pacheco",
    license="Boost",
    packages=find_packages(where="py"),
    package_dir={"": "py"},
    # cmake_install_dir="rodin/py",
    include_package_data=True,
    python_requires=">=3.6",
)

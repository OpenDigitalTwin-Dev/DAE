^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package ament_cmake_include_directories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2.5.3 (2024-12-18)
------------------

2.5.2 (2024-07-02)
------------------

2.5.1 (2024-06-27)
------------------

2.5.0 (2024-04-16)
------------------

2.4.0 (2024-03-28)
------------------
* Update maintainer list in package.xml files (`#503 <https://github.com/ament/ament_cmake/issues/503>`_)
* Contributors: Michael Jeronimo

2.3.2 (2023-12-26)
------------------

2.3.1 (2023-11-06)
------------------

2.3.0 (2023-09-07)
------------------

2.2.2 (2023-08-21)
------------------

2.2.1 (2023-06-21)
------------------

2.2.0 (2023-06-07)
------------------

2.1.0 (2023-04-26)
------------------

2.0.2 (2023-04-12)
------------------

2.0.1 (2023-04-11)
------------------

2.0.0 (2023-04-11)
------------------

1.5.3 (2023-02-13)
------------------
* [rolling] Update maintainers - 2022-11-07 (`#411 <https://github.com/ament/ament_cmake/issues/411>`_)
  * Update maintainers to Michael Jeronimo
* Contributors: Audrow Nash

1.5.2 (2022-11-02)
------------------

1.5.1 (2022-09-13)
------------------

1.5.0 (2022-07-11)
------------------

1.4.0 (2022-04-29)
------------------

1.3.1 (2022-03-28)
------------------

1.3.0 (2022-02-17)
------------------
* Update forthcoming version in changelog
* Contributors: Audrow Nash

1.2.1 (2022-01-14)
------------------
* Make ament_include_directories_order a function to allow paths with backslashes on windows. (`#371 <https://github.com/ament/ament_cmake/issues/371>`_)
  * Repalce backslashes with forward slashes on Windows
  * Typo
  * Replace slashes in ARGN
  * Don't quote
  * Check ARGN has values before trying to string(REPLACE them
  * Make ament_include_directories_order a function
* Update maintainers to Michael Jeronimo and Michel Hidalgo (`#362 <https://github.com/ament/ament_cmake/issues/362>`_)
* Contributors: Audrow Nash, Shane Loretz

1.2.0 (2021-10-29)
------------------
* Use FindPython3 instead of FindPythonInterp (`#355 <https://github.com/ament/ament_cmake/issues/355>`_)
* Update maintainers (`#336 <https://github.com/ament/ament_cmake/issues/336>`_)
* Contributors: Chris Lalancette, Shane Loretz

1.1.4 (2021-05-06)
------------------

1.1.3 (2021-03-09)
------------------

1.1.2 (2021-02-26 22:59)
------------------------

1.1.1 (2021-02-26 19:12)
------------------------

1.1.0 (2021-02-24)
------------------

1.0.4 (2021-01-25)
------------------

1.0.3 (2020-12-10)
------------------

1.0.2 (2020-12-07)
------------------
* Update package maintainers. (`#286 <https://github.com/ament/ament_cmake/issues/286>`_)
* Contributors: Michel Hidalgo

1.0.1 (2020-09-10)
------------------

1.0.0 (2020-07-22)
------------------

0.9.6 (2020-06-23)
------------------

0.9.5 (2020-06-02)
------------------

0.9.4 (2020-05-26)
------------------

0.9.3 (2020-05-19)
------------------

0.9.2 (2020-05-07)
------------------

0.9.1 (2020-04-24 15:45)
------------------------

0.9.0 (2020-04-24 12:25)
------------------------

0.8.1 (2019-10-23)
------------------

0.8.0 (2019-10-04)
------------------

0.7.3 (2019-05-29)
------------------

0.7.2 (2019-05-20)
------------------

0.7.1 (2019-05-07)
------------------

0.7.0 (2019-04-08)
------------------
* fix behavior of ament_include_directories_order on non-Windows (`#157 <https://github.com/ament/ament_cmake/issues/157>`_)
* Contributors: Dirk Thomas

0.6.0 (2018-11-13)
------------------

0.5.1 (2018-07-17)
------------------

0.5.0 (2018-06-13)
------------------

0.4.0 (2017-12-08)
------------------
* 0.0.3
* 0.0.2
* update schema url
* add schema to manifest files
* Merge pull request `#72 <https://github.com/ament/ament_cmake/issues/72>`_ from ament/cmake35
  require CMake 3.5
* remove trailing spaces from comparisons, obsolete quotes and explicit variable expansion
* require CMake 3.5
* add explicit build type
* add missing copyright / license information, update format of existing license information
* use project(.. NONE)
* refactor several low-level packages into ament_cmake_core (environment, environment_hooks, index, package_templates, symlink_install)
* invert dependency between ament_cmake_environment and ament_cmake_environment_hooks, add dependency on ament_cmake_environment
* deal with CMake double expansion
* add ament_cmake_include_directories
* Contributors: Dirk Thomas

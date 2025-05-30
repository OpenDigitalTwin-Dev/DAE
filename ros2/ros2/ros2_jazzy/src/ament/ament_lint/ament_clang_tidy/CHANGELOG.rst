^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package ament_clang_tidy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.17.1 (2024-06-27)
-------------------

0.17.0 (2024-04-16)
-------------------

0.16.4 (2024-03-28)
-------------------
* Update maintainer list in package.xml files (`#474 <https://github.com/ament/ament_lint/issues/474>`_)
* Contributors: Michael Jeronimo

0.16.3 (2024-02-07)
-------------------
* Fix a warning from newer versions of flake8. (`#469 <https://github.com/ament/ament_lint/issues/469>`_)
* Contributors: Chris Lalancette

0.16.2 (2023-12-26)
-------------------

0.16.1 (2023-11-06)
-------------------
* remove AMENT_IGNORE check in clang-tidy when looking for compilation db (`#441 <https://github.com/ament/ament_lint/issues/441>`_)
* Contributors: Alberto Soragna

0.16.0 (2023-10-04)
-------------------

0.15.2 (2023-07-11)
-------------------

0.15.1 (2023-06-07)
-------------------

0.15.0 (2023-04-27)
-------------------

0.14.1 (2023-04-11)
-------------------

0.14.0 (2023-02-14)
-------------------
* [rolling] Update maintainers - 2022-11-07 (`#421 <https://github.com/ament/ament_lint/issues/421>`_)
* Contributors: Audrow Nash

0.13.2 (2022-09-13)
-------------------
* Update maintainers (`#379 <https://github.com/ament/ament_lint/issues/379>`_)
* recommend use of --mixin compile-commands (`#371 <https://github.com/ament/ament_lint/issues/371>`_)
* Contributors: William Woodall, methylDragon

0.13.1 (2022-05-09)
-------------------
* Improve message and avoid missing new lines between reports from files (`#373 <https://github.com/ament/ament_lint/issues/373>`_)
* Contributors: William Woodall

0.13.0 (2022-04-29)
-------------------

0.12.3 (2022-04-08)
-------------------

0.12.2 (2022-03-28)
-------------------

0.12.1 (2022-03-01)
-------------------

0.12.0 (2022-02-18)
-------------------

0.11.4 (2022-01-14)
-------------------
* Update forthcoming version in changelogs
* Contributors: Audrow Nash

0.11.3 (2022-01-14)
-------------------
* Update maintainers to Michael Jeronimo and Michel Hidalgo (`#340 <https://github.com/ament/ament_lint/issues/340>`_)
* remove google style from clang-tidy default settings, removing need for default config file (`#337 <https://github.com/ament/ament_lint/issues/337>`_)
* Improvements to ament_lint_clang_tidy. (`#316 <https://github.com/ament/ament_lint/issues/316>`_)
* Contributors: Audrow Nash, Steven! Ragnarök, William Woodall

0.11.2 (2021-06-18)
-------------------

0.11.1 (2021-06-18)
-------------------

0.11.0 (2021-06-18)
-------------------

0.10.6 (2021-05-06)
-------------------

0.10.5 (2021-04-14)
-------------------
* Remove Claire as a maintainer. (`#312 <https://github.com/ament/ament_lint/issues/312>`_)
  * Remove Claire as a maintainer.
  * Remove dead email addresses.
  * Remove more dead email addresses.
  * Switch setup.py maintainer to Audrow.
* Contributors: Chris Lalancette

0.10.4 (2021-03-18)
-------------------

0.10.3 (2021-03-03)
-------------------
* 0.10.3
* Contributors: Audrow Nash

0.10.2 (2021-02-12)
-------------------
* Add Audrow as a maintainer (`#294 <https://github.com/ament/ament_lint/issues/294>`_)
* Add multiprocessing support to ament_clang_tidy (`#288 <https://github.com/ament/ament_lint/issues/288>`_)
  * add multiprocessing support
  * fix stylistic lint issues
* Contributors: Audrow Nash, M. Mei

0.10.1 (2021-01-25)
-------------------
* Add --packages-select argument to ament_clang_tidy (`#287 <https://github.com/ament/ament_lint/issues/287>`_)
  Add comment explaining handling quoted list of space separated package names
  Update documentation for ament_clang_tidy
* Update maintainer (`#274 <https://github.com/ament/ament_lint/issues/274>`_)
  * update maintainer
  * add authors
* Contributors: Claire Wang, Tyler Weaver

0.10.0 (2020-09-18)
-------------------
* Add pytest.ini so local tests don't display warning. (`#259 <https://github.com/ament/ament_lint/issues/259>`_)
* Contributors: Chris Lalancette

0.9.4 (2020-05-26)
------------------

0.9.3 (2020-05-18)
------------------

0.9.2 (2020-05-08)
------------------
* Allow AMENT_IGNORE markers to be directories (`#232 <https://github.com/ament/ament_lint/issues/232>`_)
* Contributors: Dan Rose

0.9.1 (2020-04-10)
------------------
* 0.9.0
* Merge pull request `#213 <https://github.com/ament/ament_lint/issues/213>`_ from mm318/master
  Fix ament_clang_tidy
* fix KeyError crash in ament_clang_tidy script
* put back "checked files" section of xunit output file
* fix bug in constructing arguments for clang-tidy command
* more verbose test_flake8 error messages (same as `ros2/launch_ros#135 <https://github.com/ros2/launch_ros/issues/135>`_)
* fix flake8 linter issue
* reduce amount of changes in pull request again
* reduce amount of changes in pull request
* put back --fix-errors and --header-filter options (--fix-errors is still a no-op)
* fix python lint issues
* remove some unsupported features
* fix ament_clang_tidy
* Generate xunit files valid for the junit10.xsd (`#220 <https://github.com/ament/ament_lint/issues/220>`_)
* remove status attribute from result XML (`#212 <https://github.com/ament/ament_lint/issues/212>`_)
* Contributors: Chris Lalancette, Claire Wang, Dirk Thomas, Jose Luis Rivero, Miaofei

0.8.1 (2019-10-23)
------------------
* install resource marker file for packages (`#193 <https://github.com/ament/ament_lint/issues/193>`_)
* Contributors: Dirk Thomas

0.8.0 (2019-09-17)
------------------
* install manifest files in Python packages (`#185 <https://github.com/ament/ament_lint/issues/185>`_)
* Convert list comprehension to generator (`#179 <https://github.com/ament/ament_lint/issues/179>`_)
  Addresses flake8 C412 errors introduced by flake8-comprehension 2.2.0
* Contributors: Dirk Thomas, Scott K Logan

0.7.4 (2019-07-31)
------------------
* Fix version num for release (`#169 <https://github.com/ament/ament_lint/issues/169>`_)
* Add python3-yaml depends to ament_clang_tidy/format (`#166 <https://github.com/ament/ament_lint/issues/166>`_)
  depend -> exec_depend
  Remove whitespace
* Add error output, header out option, fix null error (`#163 <https://github.com/ament/ament_lint/issues/163>`_)
  * Add error output, header out option, fix null error
  * Add quiet option to `ament_clang_tidy`
  * Add header filter and system header option
* Add clang tidy to ament linters (`#155 <https://github.com/ament/ament_lint/issues/155>`_)
  * Basic clang-lint functionality
  * Add clang tidy cmake dir
  * Add Google/default config file
  * Add xunit output
  * Add lint tests
  * Update copyright, maintainer, verbiage
* Contributors: John

0.7.3 (2019-05-09 14:08)
------------------------

0.7.2 (2019-05-09 09:30)
------------------------

0.7.1 (2019-05-07)
------------------

0.7.0 (2019-04-11)
------------------

0.6.3 (2019-01-14)
------------------

0.6.2 (2018-12-06)
------------------

0.6.1 (2018-11-15)
------------------

0.6.0 (2018-11-14)
------------------

0.5.2 (2018-06-27)
------------------

0.5.1 (2018-06-18 13:47)
------------------------

0.5.0 (2018-06-18 10:09)
------------------------

0.4.0 (2017-12-08)
------------------

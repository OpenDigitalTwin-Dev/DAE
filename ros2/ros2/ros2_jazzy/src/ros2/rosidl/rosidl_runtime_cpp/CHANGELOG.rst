^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package rosidl_runtime_cpp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

4.6.5 (2024-12-18)
------------------

4.6.4 (2024-09-06)
------------------

4.6.3 (2024-06-27)
------------------
* Suppress warnings in the benchmarks for upstream GCC false positives. (`#810 <https://github.com/ros2/rosidl/issues/810>`_) (`#816 <https://github.com/ros2/rosidl/issues/816>`_)
  (cherry picked from commit 57940a28bd9d0aface66130f21e5a9c5945694aa)
  Co-authored-by: Chris Lalancette <clalancette@gmail.com>
* Contributors: mergify[bot]

4.6.2 (2024-05-13)
------------------
* Suppress a warning around BoundedVector. (`#803 <https://github.com/ros2/rosidl/issues/803>`_) (`#804 <https://github.com/ros2/rosidl/issues/804>`_)
  The comment has more explanation, but in short GCC 13
  has false positives around some warnings, so we suppress
  it for BoundedVector.
  (cherry picked from commit 858e76adb03edba00469b91d50dd5fe0dcb34236)
  Co-authored-by: Chris Lalancette <clalancette@gmail.com>
* Contributors: mergify[bot]

4.6.1 (2024-04-24)
------------------

4.6.0 (2024-04-16)
------------------

4.5.2 (2024-03-27)
------------------

4.5.1 (2024-02-07)
------------------

4.5.0 (2023-12-26)
------------------

4.4.2 (2023-11-06)
------------------

4.4.1 (2023-10-04)
------------------

4.4.0 (2023-09-07)
------------------

4.3.1 (2023-08-21)
------------------

4.3.0 (2023-07-11)
------------------

4.2.0 (2023-06-07)
------------------

4.1.1 (2023-05-11)
------------------

4.1.0 (2023-04-28)
------------------

4.0.0 (2023-04-11)
------------------
* Type Description Codegen and Typesupport  (rep2011) (`#727 <https://github.com/ros2/rosidl/issues/727>`_)
* Copied type_description_interfaces structs (rep2011) (`#732 <https://github.com/ros2/rosidl/issues/732>`_)
* Fix a few more clang analysis problems. (`#731 <https://github.com/ros2/rosidl/issues/731>`_)
* Return reference from BoundedVector::emplace_back (`#730 <https://github.com/ros2/rosidl/issues/730>`_)
* Contributors: Alexander Hans, Chris Lalancette, Emerson Knapp

3.4.0 (2023-02-13)
------------------
* [service introspection] generate service_event messages (`#700 <https://github.com/ros2/rosidl/issues/700>`_)
* [rolling] Update maintainers - 2022-11-07 (`#717 <https://github.com/ros2/rosidl/issues/717>`_)
* fix conversion to ‘std::streamsize’ {aka ‘long int’} from ‘size_t’ {aka ‘long unsigned int’} may change the sign of the result (`#715 <https://github.com/ros2/rosidl/issues/715>`_)
* Contributors: Audrow Nash, Brian, ralwing

3.3.1 (2022-11-02)
------------------

3.3.0 (2022-09-08)
------------------

3.2.1 (2022-06-21)
------------------

3.2.0 (2022-05-04)
------------------

3.1.3 (2022-04-08)
------------------

3.1.2 (2022-04-05)
------------------

3.1.1 (2022-03-28)
------------------

3.1.0 (2022-03-01)
------------------
* Add missing dependency on rosidl_runtime_c (`#666 <https://github.com/ros2/rosidl/issues/666>`_)
* De-duplicate Quality Level from README and QUALITY_DECLARATION (`#661 <https://github.com/ros2/rosidl/issues/661>`_)
* Install headers to include/${PROJECT_NAME} (`#658 <https://github.com/ros2/rosidl/issues/658>`_)
* Contributors: Jose Luis Rivero, Shane Loretz

3.0.1 (2022-01-13)
------------------
* Set CXX standard to 17 (`#635 <https://github.com/ros2/rosidl/issues/635>`_)
* Contributors: Øystein Sture

3.0.0 (2021-11-05)
------------------
* Update package maintainers (`#624 <https://github.com/ros2/rosidl/issues/624>`_)
* Contributors: Michel Hidalgo

2.5.0 (2021-08-10)
------------------

2.4.0 (2021-07-12)
------------------

2.3.0 (2021-06-11)
------------------

2.2.1 (2021-04-06)
------------------
* updating quality declaration links (re: `ros2/docs.ros2.org#52 <https://github.com/ros2/docs.ros2.org/issues/52>`_) (`#581 <https://github.com/ros2/rosidl/issues/581>`_)
* Contributors: shonigmann

2.2.0 (2021-03-18)
------------------

2.1.0 (2021-03-09)
------------------

2.0.3 (2021-01-25)
------------------
* Fix typo of package name in README heading (`#561 <https://github.com/ros2/rosidl/issues/561>`_)
* Contributors: Tully Foote

2.0.2 (2020-12-08)
------------------
* Update and add package.xml descriptions to README (`#553 <https://github.com/ros2/rosidl/issues/553>`_)
* Fix item number in QD (`#546 <https://github.com/ros2/rosidl/issues/546>`_)
* Update the maintainers of this repository. (`#536 <https://github.com/ros2/rosidl/issues/536>`_)
* Contributors: Chris Lalancette, Louise Poubel, Shane Loretz

2.0.1 (2020-09-28)
------------------
* QD: Add links to hosted API docs (`#533 <https://github.com/ros2/rosidl/issues/533>`_)
* Updated Quality Level to 1 (`#532 <https://github.com/ros2/rosidl/issues/532>`_)
* Add benchmarks for rosidl_runtime\_* packages (`#521 <https://github.com/ros2/rosidl/issues/521>`_)
* Contributors: Alejandro Hernández Cordero, Louise Poubel, Scott K Logan

2.0.0 (2020-09-24)
------------------
* Add to_yaml() function for C++ messages (`#527 <https://github.com/ros2/rosidl/issues/527>`_)
* Contributors: Devin Bonnie, Dirk Thomas

1.1.0 (2020-08-17)
------------------
* Add function for getting a types fully qualified name (`#514 <https://github.com/ros2/rosidl/issues/514>`_)
* Fix misuses of input iterators in BoundedVector (`#493 <https://github.com/ros2/rosidl/issues/493>`_)
* Update QD to reflect QL 2 statuses (`#499 <https://github.com/ros2/rosidl/issues/499>`_)
* Contributors: Jacob Perron, Jonathan Wakely, Stephen Brawner

1.0.1 (2020-06-03)
------------------
* Fix move assignment of bounded vector (`#496 <https://github.com/ros2/rosidl/issues/496>`_)
* Fix bug inserting bounded_vector in reverse order (`#444 <https://github.com/ros2/rosidl/issues/444>`_)
* Add Security Vulnerability Policy pointing to REP-2006 (`#494 <https://github.com/ros2/rosidl/issues/494>`_)
* QD Update Version Stability to stable version (`#495 <https://github.com/ros2/rosidl/issues/495>`_)
* Contributors: Alejandro Hernández Cordero, Chris Lalancette, Jacob Perron, Carlos San Vicente

1.0.0 (2020-05-22)
------------------
* Clean up BoundedVector (`#487 <https://github.com/ros2/rosidl/issues/487>`_)
* Contributors: Jonathan Wakely

0.9.2 (2020-05-19)
------------------
* Update API documentation (`#488 <https://github.com/ros2/rosidl/issues/488>`_)
* Add tests (`#484 <https://github.com/ros2/rosidl/issues/484>`_)
* Add feature documentation (`#482 <https://github.com/ros2/rosidl/issues/482>`_)
* Contributors: brawner

0.9.1 (2020-05-08)
------------------
* move test which only uses rosidl_runtime_cpp into that package (`#481 <https://github.com/ros2/rosidl/issues/481>`_)
* Package READMEs and QUALITY_DECLARATIONS for runtime packages (`#480 <https://github.com/ros2/rosidl/issues/480>`_)
* Added doxyfile in rosidl_runtime_c and rosidl_runtime_cpp (`#474 <https://github.com/ros2/rosidl/issues/474>`_)
* Contributors: Alejandro Hernández Cordero, Dirk Thomas, brawner

0.9.0 (2020-04-24)
------------------
* Export targets in a addition to include directories / libraries (`#471 <https://github.com/ros2/rosidl/issues/471>`_)
* Rename rosidl_runtime_c_message_initialization to rosidl_runtime_c__message_initialization (`#464 <https://github.com/ros2/rosidl/issues/464>`_)
* Rename rosidl_generator_c 'namespace' to rosidl_runtime_c (`#458 <https://github.com/ros2/rosidl/issues/458>`_)
* Move rosidl_generator_cpp headers (`#456 <https://github.com/ros2/rosidl/issues/456>`_)
* Split rosidl_generator_c and rosidl_generator_cpp in two: rosidl_generator_x and rosidl_runtime_x (`#442 <https://github.com/ros2/rosidl/issues/442>`_)
* Contributors: Alejandro Hernández Cordero, Dirk Thomas

0.8.2 (2020-01-17)
------------------

0.8.1 (2019-10-23)
------------------

0.8.0 (2019-09-24)
------------------

0.7.3 (2019-05-29)
------------------

0.7.2 (2019-05-20)
------------------

0.7.1 (2019-05-08)
------------------

0.7.0 (2019-04-12)
------------------

0.6.3 (2019-02-07)
------------------

0.6.2 (2018-12-07)
------------------

0.6.1 (2018-12-06)
------------------

0.6.0 (2018-11-15)
------------------

0.5.1 (2018-06-28)
------------------

0.5.0 (2018-06-23)
------------------

0.4.0 (2017-12-08)
------------------

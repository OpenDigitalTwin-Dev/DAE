^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package rviz2
^^^^^^^^^^^^^^^^^^^^^^^^^^^

14.1.7 (2025-01-14)
-------------------

14.1.6 (2024-12-18)
-------------------

14.1.5 (2024-09-06)
-------------------

14.1.4 (2024-08-19)
-------------------
* Detect wayland and make sure X rendering is used. (`#1253 <https://github.com/ros2/rviz/issues/1253>`_) (`#1254 <https://github.com/ros2/rviz/issues/1254>`_)
  rviz2 does not work under wayland unless using X compatibility.
  The current workaround on wayland is to set the QT_QPA_PLATFORM
  environment variable to xcb.  This patch now detects
  if rviz2 is started in a wayland session and if so sets that
  variable automatically.
  (cherry picked from commit 72c0826545b4cc173413699a381cc36da78437bf)
  Co-authored-by: Matthew Elwin <10161574+m-elwin@users.noreply.github.com>
* Contributors: mergify[bot]

14.1.3 (2024-07-19)
-------------------

14.1.2 (2024-06-27)
-------------------

14.1.1 (2024-05-13)
-------------------

14.1.0 (2024-04-16)
-------------------

14.0.0 (2024-04-07)
-------------------

13.4.2 (2024-03-27)
-------------------

13.4.1 (2024-03-26)
-------------------

13.4.0 (2024-03-09)
-------------------

13.3.1 (2024-01-24)
-------------------

13.3.0 (2023-12-26)
-------------------
* Add "R" key as shortcut for resetTime (`#1088 <https://github.com/ros2/rviz/issues/1088>`_)
* Switch to target_link_libraries. (`#1098 <https://github.com/ros2/rviz/issues/1098>`_)
* Contributors: Chris Lalancette, Paul Erik Frivold

13.2.0 (2023-11-06)
-------------------

13.1.2 (2023-10-09)
-------------------

13.1.1 (2023-10-04)
-------------------

13.1.0 (2023-09-07)
-------------------

13.0.0 (2023-08-21)
-------------------

12.8.0 (2023-08-27)
-------------------

12.7.0 (2023-07-11)
-------------------

12.6.1 (2023-06-12)
-------------------

12.6.0 (2023-06-07)
-------------------

12.5.1 (2023-05-11)
-------------------

12.5.0 (2023-04-28)
-------------------

12.4.0 (2023-04-18)
-------------------

12.3.2 (2023-04-11)
-------------------

12.3.1 (2023-03-01)
-------------------

12.3.0 (2023-02-14)
-------------------
* Make rviz1_to_rviz2.py accept configs with missing values (`#945 <https://github.com/ros2/rviz/issues/945>`_)
* Update rviz to C++17. (`#939 <https://github.com/ros2/rviz/issues/939>`_)
* [rolling] Update maintainers - 2022-11-07 (`#923 <https://github.com/ros2/rviz/issues/923>`_)
* Contributors: Audrow Nash, Chris Lalancette, Shane Loretz

12.2.0 (2022-11-07)
-------------------

12.1.0 (2022-11-02)
-------------------

12.0.0 (2022-09-13)
-------------------
* Add rviz1_to_rviz2.py conversion script (`#882 <https://github.com/ros2/rviz/issues/882>`_)
* Contributors: Shane Loretz

11.3.0 (2022-04-26)
-------------------

11.2.0 (2022-04-08)
-------------------

11.1.1 (2022-03-30)
-------------------

11.1.0 (2022-03-24)
-------------------

11.0.0 (2022-03-01)
-------------------

10.0.0 (2022-02-16)
-------------------

9.1.1 (2022-01-25)
------------------

9.1.0 (2022-01-13)
------------------

9.0.1 (2021-12-17)
------------------

9.0.0 (2021-11-18)
------------------

8.7.0 (2021-08-11)
------------------
* Change links index.ros.org -> docs.ros.org. (`#698 <https://github.com/ros2/rviz/issues/698>`_)
* Contributors: Chris Lalancette

8.6.0 (2021-05-13)
------------------

8.5.0 (2021-04-06)
------------------

8.4.0 (2021-03-18)
------------------

8.3.1 (2021-01-25)
------------------
* Use "%s" as format string literal in logging macros (`#633 <https://github.com/ros2/rviz/issues/633>`_)
* Contributors: Audrow Nash

8.3.0 (2020-12-08)
------------------
* Add linters and use ament_lint_auto (`#608 <https://github.com/ros2/rviz/issues/608>`_)
* Update maintainers (`#607 <https://github.com/ros2/rviz/issues/607>`_)
* Move and update documentation for ROS 2 (`#600 <https://github.com/ros2/rviz/issues/600>`_)
* Contributors: Chris Lalancette, Jacob Perron

8.2.0 (2020-06-23)
------------------

8.1.1 (2020-06-03)
------------------

8.1.0 (2020-06-03)
------------------
* Added missing virtual destructors (`#553 <https://github.com/ros2/rviz/issues/553>`_)
* Contributors: Ivan Santiago Paunovic

8.0.3 (2020-06-02)
------------------

8.0.2 (2020-05-21)
------------------
* Removed automoc completely. (`#545 <https://github.com/ros2/rviz/issues/545>`_)
* Contributors: Chris Lalancette

8.0.1 (2020-05-07)
------------------

8.0.0 (2020-05-01)
------------------
* Note from wjwwood: I've chosen bump the major version this time, even though the API was not broken strictly speaking, partly because of some potentially disruptive build system changes and partially in preparation for ROS Foxy, to allow for new minor/patch versions in the previous ROS release Eloquent.
* Made some code style changes. (`#504 <https://github.com/ros2/rviz/issues/504>`_)
* Contributors: Dirk Thomas

7.0.3 (2019-11-13)
------------------

7.0.2 (2019-10-23)
------------------
* Remove ROS arguments before passing to QApplication (`#474 <https://github.com/ros2/rviz/issues/474>`_)
* Contributors: Jacob Perron

7.0.1 (2019-10-04)
------------------

7.0.0 (2019-09-27)
------------------
* Remove -Werror from defualt compiler options (`#420 <https://github.com/ros2/rviz/issues/420>`_)
* Contributors: Hunter L. Allen

6.1.1 (2019-05-29)
------------------

6.1.0 (2019-05-20)
------------------

6.0.0 (2019-05-08)
------------------

5.1.0 (2019-01-14)
------------------
* Update package maintainer for rviz2 (`#365 <https://github.com/ros2/rviz/issues/365>`_)
  Thank you to Deanna for her contributions.
* Contributors: Scott K Logan

5.0.0 (2018-12-04)
------------------
* Add semicolons to all RCLCPP and RCUTILS macros. (`#357 <https://github.com/ros2/rviz/issues/357>`_)
* Made the transformation framework used by rviz pluggable. (`#346 <https://github.com/ros2/rviz/issues/346>`_)
* Contributors: Andreas Greimel, Chris Lalancette

4.0.1 (2018-06-28)
------------------

4.0.0 (2018-06-27)
------------------
* Fixed compilation errors and runtime issues on Windows. (`#175 <https://github.com/ros2/rviz/issues/175>`_)
* Introduced a ROS interface abstraction to improve testability. (`#156 <https://github.com/ros2/rviz/issues/156>`_)
* Added a dependency between rviz2 and rviz_default_plugins (`#149 <https://github.com/ros2/rviz/issues/149>`_)
* Contributors: Alessandro Bottero, Andreas Greimel, Andreas Holzner, Martin Idel, Mikael Arguedas, William Woodall

3.0.0 (2018-02-07)
------------------
* Numerous changes to support Windows.
* Make rviz runnable with ros2 run.
* Contributors: Andreas Holzner, Hunter Allen, Steven! Ragnarok, William Woodall

1.12.11 (2017-08-02)
--------------------

1.12.10 (2017-06-05 17:37)
--------------------------

1.12.9 (2017-06-05 14:23)
-------------------------

1.12.8 (2017-05-07)
-------------------

1.12.7 (2017-05-05)
-------------------

1.12.6 (2017-05-02)
-------------------

1.12.5 (2017-05-01)
-------------------

1.12.4 (2016-10-27)
-------------------

1.12.3 (2016-10-19)
-------------------

1.12.2 (2016-10-18)
-------------------

1.12.1 (2016-04-20)
-------------------

1.12.0 (2016-04-11)
-------------------

1.11.14 (2016-04-03)
--------------------

1.11.13 (2016-03-23)
--------------------

1.11.12 (2016-03-22 19:58)
--------------------------

1.11.11 (2016-03-22 18:16)
--------------------------

1.11.10 (2015-10-13)
--------------------

1.11.9 (2015-09-21)
-------------------

1.11.8 (2015-08-05)
-------------------

1.11.7 (2015-03-02)
-------------------

1.11.6 (2015-02-13)
-------------------

1.11.5 (2015-02-11)
-------------------

1.11.4 (2014-10-30)
-------------------

1.11.3 (2014-06-26)
-------------------

1.11.2 (2014-05-13)
-------------------

1.11.1 (2014-05-01)
-------------------

1.11.0 (2014-03-04 21:40)
-------------------------

1.10.14 (2014-03-04 21:35)
--------------------------

1.10.13 (2014-02-26)
--------------------

1.10.12 (2014-02-25)
--------------------

1.10.11 (2014-01-26)
--------------------

1.10.10 (2013-12-22)
--------------------

1.10.9 (2013-10-15)
-------------------

1.10.7 (2013-09-16)
-------------------

1.10.6 (2013-09-03)
-------------------

1.10.5 (2013-08-28 03:50)
-------------------------

1.10.4 (2013-08-28 03:13)
-------------------------

1.10.3 (2013-08-14)
-------------------

1.10.2 (2013-07-26)
-------------------

1.10.1 (2013-07-16)
-------------------

1.10.0 (2013-06-27)
-------------------

1.9.30 (2013-05-30)
-------------------

1.9.29 (2013-04-15)
-------------------

1.9.27 (2013-03-15 13:23)
-------------------------

1.9.26 (2013-03-15 10:38)
-------------------------

1.9.25 (2013-03-07)
-------------------

1.9.24 (2013-02-16)
-------------------

1.9.23 (2013-02-13)
-------------------

1.9.22 (2013-02-12 16:30)
-------------------------

1.9.21 (2013-02-12 14:00)
-------------------------

1.9.20 (2013-01-21)
-------------------

1.9.19 (2013-01-13)
-------------------

1.9.18 (2012-12-18)
-------------------

1.9.17 (2012-12-14)
-------------------

1.9.16 (2012-11-14 15:49)
-------------------------

1.9.15 (2012-11-13)
-------------------

1.9.14 (2012-11-14 02:20)
-------------------------

1.9.13 (2012-11-14 00:58)
-------------------------

1.9.12 (2012-11-06)
-------------------

1.9.11 (2012-11-02)
-------------------

1.9.10 (2012-11-01 11:10)
-------------------------

1.9.9 (2012-11-01 11:01)
------------------------

1.9.8 (2012-11-01 10:52)
------------------------

1.9.7 (2012-11-01 10:40)
------------------------

1.9.6 (2012-10-31)
------------------

1.9.5 (2012-10-19)
------------------

1.9.4 (2012-10-15 15:00)
------------------------

1.9.3 (2012-10-15 10:41)
------------------------

1.9.2 (2012-10-12 13:38)
------------------------

1.9.1 (2012-10-12 11:57)
------------------------

1.9.0 (2012-10-10)
------------------

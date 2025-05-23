^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package rosbag2_storage
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.26.6 (2024-12-18)
-------------------
* Add more logging info to storage and reader/writer open operations (`#1881 <https://github.com/ros2/rosbag2/issues/1881>`_) (`#1882 <https://github.com/ros2/rosbag2/issues/1882>`_)
  (cherry picked from commit 0823be2723e04715baacf99625b844cb88f58c21)
  Co-authored-by: Michael Orlov <michael.orlov@apex.ai>
* Contributors: Marco A. Gutierrez, mergify[bot]

0.26.5 (2024-09-06)
-------------------

0.26.4 (2024-06-27)
-------------------

0.26.3 (2024-05-15)
-------------------

0.26.2 (2024-04-24)
-------------------

0.26.1 (2024-04-17)
-------------------

0.26.0 (2024-04-16)
-------------------
* Support service 2/2 --- rosbag2 service play (`#1481 <https://github.com/ros2/rosbag2/issues/1481>`_)
* Use middleware send and receive timestamps from message_info during recording (`#1531 <https://github.com/ros2/rosbag2/issues/1531>`_)
* Update to use yaml-cpp version 0.8.0. (`#1605 <https://github.com/ros2/rosbag2/issues/1605>`_)
* Contributors: Barry Xu, Chris Lalancette, jmachowinski

0.25.0 (2024-03-27)
-------------------
* Use std::filesystem instead of rcpputils::fs (`#1576 <https://github.com/ros2/rosbag2/issues/1576>`_)
* Make some changes for newer versions of uncrustify. (`#1578 <https://github.com/ros2/rosbag2/issues/1578>`_)
* Add topic_id returned by storage to the TopicMetadata (`#1538 <https://github.com/ros2/rosbag2/issues/1538>`_)
* Remove rcpputils::fs dependencies from rosbag2_storages (`#1558 <https://github.com/ros2/rosbag2/issues/1558>`_)
* Improve performance in SqliteStorage::get_bagfile_size() (`#1516 <https://github.com/ros2/rosbag2/issues/1516>`_)
* Make Player and Recorder Composable (`#902 <https://github.com/ros2/rosbag2/issues/902>`_) (`#1419 <https://github.com/ros2/rosbag2/issues/1419>`_)
* Use enum values for offered_qos_profiles in code and string names in serialized metadata (`#1476 <https://github.com/ros2/rosbag2/issues/1476>`_)
* ros2 bag convert now excludes messages not in [start_time;end_time] (`#1455 <https://github.com/ros2/rosbag2/issues/1455>`_)
* Contributors: Chris Lalancette, Michael Orlov, Patrick Roncagliolo, Peter Favrholdt, Roman Sokolkov

0.24.0 (2023-07-11)
-------------------
* Fix missing cstdint include (`#1383 <https://github.com/ros2/rosbag2/issues/1383>`_)
* Implement storing and loading ROS_DISTRO from metadata.yaml and mcap files (`#1241 <https://github.com/ros2/rosbag2/issues/1241>`_)
* Contributors: Emerson Knapp, Zac Stanton

0.23.0 (2023-04-28)
-------------------

0.22.0 (2023-04-18)
-------------------
* Add type_hash in MessageDefinition struct (`#1296 <https://github.com/ros2/rosbag2/issues/1296>`_)
* Add message definition read API (`#1292 <https://github.com/ros2/rosbag2/issues/1292>`_)
* rosbag2_storage: add type description hash to topic metadata (`#1272 <https://github.com/ros2/rosbag2/issues/1272>`_)
* Contributors: Michael Orlov, james-rms

0.21.0 (2023-04-12)
-------------------
* rosbag2_cpp: move local message definition source out of MCAP plugin (`#1265 <https://github.com/ros2/rosbag2/issues/1265>`_)
* Update rosbag2 to C++17. (`#1238 <https://github.com/ros2/rosbag2/issues/1238>`_)
* Use target_link_libraries instead of ament_target_dependencies (`#1202 <https://github.com/ros2/rosbag2/issues/1202>`_)
* Contributors: Chris Lalancette, Daisuke Nishimatsu, Michael Orlov, james-rms

0.20.0 (2023-02-14)
-------------------

0.19.0 (2023-01-13)
-------------------
* rosbag2_storage: set MCAP as default plugin (`#1160 <https://github.com/ros2/rosbag2/issues/1160>`_)
* Add Michael Orlov as maintainer in rosbag2 packages (`#1215 <https://github.com/ros2/rosbag2/issues/1215>`_)
* set_read_order: return success (`#1177 <https://github.com/ros2/rosbag2/issues/1177>`_)
* Remove explicit sqlite3 from code (`#1166 <https://github.com/ros2/rosbag2/issues/1166>`_)
* Add `update_metadata(BagMetadata)` API for storage plugin interface (`#1149 <https://github.com/ros2/rosbag2/issues/1149>`_)
* Contributors: Emerson Knapp, Michael Orlov, james-rms

0.18.0 (2022-11-15)
-------------------
* rosbag2_storage: expose default storage ID as method (`#1146 <https://github.com/ros2/rosbag2/issues/1146>`_)
* Don't reopen file for every seek if we don't have to. Search directionally for the correct file (`#1117 <https://github.com/ros2/rosbag2/issues/1117>`_)
* Reverse read order API and sqlite storage implementation (`#1083 <https://github.com/ros2/rosbag2/issues/1083>`_)
* Remove YAML_CPP_DLL define (`#964 <https://github.com/ros2/rosbag2/issues/964>`_)
* Added support for excluding topics via regular expressions (`#1046 <https://github.com/ros2/rosbag2/issues/1046>`_)
* Readers/info can accept a single bag storage file, and detect its storage id automatically (`#1072 <https://github.com/ros2/rosbag2/issues/1072>`_)
* Contributors: Akash, Emerson Knapp, Esteve Fernandez, james-rms

0.17.0 (2022-07-30)
-------------------
* Use a single variable for evaluating the filter regex (`#1053 <https://github.com/ros2/rosbag2/issues/1053>`_)
* Speed optimization: Preparing copyless publish/subscribing by using const message for writing (`#1010 <https://github.com/ros2/rosbag2/issues/1010>`_)
* Renamed --topics-regex to --regex and -e in Player class to be consistent with Recorder (`#1045 <https://github.com/ros2/rosbag2/issues/1045>`_)
* Add the ability to record any key/value pair in 'custom' field in metadata.yaml (`#1038 <https://github.com/ros2/rosbag2/issues/1038>`_)
* Added support for filtering topics via regular expressions on Playback (`#1034 <https://github.com/ros2/rosbag2/issues/1034>`_)
* Contributors: DensoADAS, Joshua Hampp, Esteve Fernandez, Hunter L. Allen, Michael Orlov, Tony Peng

0.16.0 (2022-05-11)
-------------------

0.15.1 (2022-04-06)
-------------------
* Revert "Add the ability to record any key/value pair in the 'custom' field in metadata.yaml (`#976 <https://github.com/ros2/rosbag2/issues/976>`_)" (`#984 <https://github.com/ros2/rosbag2/issues/984>`_)
* Add the ability to record any key/value pair in the 'custom' field in metadata.yaml (`#976 <https://github.com/ros2/rosbag2/issues/976>`_)
* Contributors: Audrow Nash, Jorge Perez, Tony Peng

0.15.0 (2022-04-05)
-------------------
* Revert "Add the ability to record any key/value pair in the 'custom' field in metadata.yaml (`#976 <https://github.com/ros2/rosbag2/issues/976>`_)" (`#984 <https://github.com/ros2/rosbag2/issues/984>`_)
* Add the ability to record any key/value pair in the 'custom' field in metadata.yaml (`#976 <https://github.com/ros2/rosbag2/issues/976>`_)
* Contributors: Jorge Perez, Tony Peng

0.14.1 (2022-03-29)
-------------------
* Bump version number to avoid conflict
* Contributors: Chris Lalancette

0.14.0 (2022-03-29)
-------------------
* Install headers to include/${PROJECT_NAME} (`#958 <https://github.com/ros2/rosbag2/issues/958>`_)
* Remove unnecessary public definition. (`#950 <https://github.com/ros2/rosbag2/issues/950>`_)
* Contributors: Chris Lalancette, Shane Loretz

0.13.0 (2022-01-13)
-------------------

0.12.0 (2021-12-17)
-------------------
* Enable YAML encoding/decoding for RecordOptions and StorageOptions (`#916 <https://github.com/ros2/rosbag2/issues/916>`_)
* Contributors: Emerson Knapp

0.11.0 (2021-11-08)
-------------------
* Update package maintainers (`#899 <https://github.com/ros2/rosbag2/issues/899>`_)
* Provide MetadataIO interface to convert metadata to a string in memory, alongside file IO versions (`#894 <https://github.com/ros2/rosbag2/issues/894>`_)
* Contributors: Emerson Knapp, Michel Hidalgo

0.10.1 (2021-10-22)
-------------------

0.10.0 (2021-10-19)
-------------------
* Metadata per file info (`#870 <https://github.com/ros2/rosbag2/issues/870>`_)
* Implement snapshot mechanism and corresponding ROS Service (`#850 <https://github.com/ros2/rosbag2/issues/850>`_)
* added seek interface (`#836 <https://github.com/ros2/rosbag2/issues/836>`_)
* Refactor plugin query mechanism and standardize trait management (`#833 <https://github.com/ros2/rosbag2/issues/833>`_)
* Contributors: Cameron Miller, Wojciech Jaworski, sonia

0.9.0 (2021-05-17)
------------------

0.8.0 (2021-04-19)
------------------
* Remove -Werror from builds, enable it in Action CI (`#722 <https://github.com/ros2/rosbag2/issues/722>`_)
* PlayerClock initial implementation - Player functionally unchanged (`#689 <https://github.com/ros2/rosbag2/issues/689>`_)
* Explicitly add emersonknapp as maintainer (`#692 <https://github.com/ros2/rosbag2/issues/692>`_)
* Reindexer core (`#641 <https://github.com/ros2/rosbag2/issues/641>`_)
  Add a new C++ Reindexer class for reconstructing metadata from bags that are missing it.
* Contributors: Emerson Knapp, jhdcs

0.7.0 (2021-03-18)
------------------
* Remove outdated pluginlib cmake script from rosbag2_storage (`#661 <https://github.com/ros2/rosbag2/issues/661>`_)
* CLI query rosbag2_py for available storage implementations (`#659 <https://github.com/ros2/rosbag2/issues/659>`_)
* Shorten some excessively long lines of CMake (`#648 <https://github.com/ros2/rosbag2/issues/648>`_)
* Contributors: Emerson Knapp, Scott K Logan

0.6.0 (2021-02-01)
------------------
* SQLite storage optimized by default (`#568 <https://github.com/ros2/rosbag2/issues/568>`_)
  * Use optimized pragmas by default in sqlite storage. Added option to use former behavior
* Use std::filesystem compliant non-member `exists` function for path object (`#593 <https://github.com/ros2/rosbag2/issues/593>`_)
* Contributors: Adam Dąbrowski, Josh Langsfeld

0.5.0 (2020-12-02)
------------------
* Update codes since rcutils_calculate_directory_size() is changed (`#567 <https://github.com/ros2/rosbag2/issues/567>`_)
* Contributors: Barry Xu

0.4.0 (2020-11-19)
------------------
* add storage_config_uri (`#493 <https://github.com/ros2/rosbag2/issues/493>`_)
* Update the package.xml files with the latest Open Robotics maintainers (`#535 <https://github.com/ros2/rosbag2/issues/535>`_)
* Add split by time to recording (`#409 <https://github.com/ros2/rosbag2/issues/409>`_)
* Contributors: Karsten Knese, Michael Jeronimo, jhdcs

0.3.2 (2020-06-03)
------------------

0.3.1 (2020-06-01)
------------------

0.3.0 (2020-05-26)
------------------
* Export targets (`#403 <https://github.com/ros2/rosbag2/issues/403>`_)
* Contributors: Karsten Knese

0.2.8 (2020-05-18)
------------------

0.2.7 (2020-05-12)
------------------

0.2.6 (2020-05-07)
------------------
* Correct usage of rclcpp::SharedLibrary loading. (`#400 <https://github.com/ros2/rosbag2/issues/400>`_)
* Contributors: Karsten Knese

0.2.5 (2020-04-30)
------------------
* Read serialized qos profiles out of the metadata (`#359 <https://github.com/ros2/rosbag2/issues/359>`_)
* Add filter for reading selective topics (`#302 <https://github.com/ros2/rosbag2/issues/302>`_)
* Transaction based sqlite3 inserts (`#225 <https://github.com/ros2/rosbag2/issues/225>`_)
* Add QoS profiles field to metadata struct and provide serialization utilities (`#330 <https://github.com/ros2/rosbag2/issues/330>`_)
* code style only: wrap after open parenthesis if not in one line (`#280 <https://github.com/ros2/rosbag2/issues/280>`_)
* remove rosbag2 filesystem helper (`#249 <https://github.com/ros2/rosbag2/issues/249>`_)
* [Compression - 7] Add compression metadata (`#221 <https://github.com/ros2/rosbag2/issues/221>`_)
* Sanitize bagfile splitting CLI input (`#226 <https://github.com/ros2/rosbag2/issues/226>`_)
* Move get_storage_identifier and get_bagfile_size (`#209 <https://github.com/ros2/rosbag2/issues/209>`_)
* make ros tooling working group maintainer (`#211 <https://github.com/ros2/rosbag2/issues/211>`_)
* Contributors: Anas Abou Allaban, Dirk Thomas, Emerson Knapp, Karsten Knese, Mabel Zhang, Prajakta Gokhale, Sriram Raghunathan, Zachary Michaels

0.2.4 (2019-11-18)
------------------

0.2.3 (2019-11-18)
------------------
* Enhance rosbag writer capabilities to split bag files. (`#185 <https://github.com/ros2/rosbag2/issues/185>`_)
* Contributors: Zachary Michaels

0.2.2 (2019-11-13)
------------------
* (API) Generate bagfile metadata in Writer (`#184 <https://github.com/ros2/rosbag2/issues/184>`_)
* Contributors: Zachary Michaels

0.2.1 (2019-10-23)
------------------
* Add get_identifier to base io-interfaces for support in bagfile splitting (`#183 <https://github.com/ros2/rosbag2/issues/183>`_)
* Add bagfile splitting support to storage_options (`#182 <https://github.com/ros2/rosbag2/issues/182>`_)
* Change storage interfaces for bagfile splitting feature (`#170 <https://github.com/ros2/rosbag2/issues/170>`_)
* Contributors: Zachary Michaels

0.2.0 (2019-09-26)
------------------
* Fix test failures on armhf (`#135 <https://github.com/ros2/rosbag2/issues/135>`_)
* Export pluginlib to downstream packages (`#113 <https://github.com/ros2/rosbag2/issues/113>`_)
* Contributors: Esteve Fernandez, Prajakta Gokhale

0.1.2 (2019-05-20)
------------------
* Fixes an init race condition (`#93 <https://github.com/ros2/rosbag2/issues/93>`_)
  * This could probably be a race condition, for ex: When we've create a subscriber in the API, and the subscriber has the data already available in the callback (Cause of existing publishers) the db entry for the particular topic would not be availalble, which in turn returns an SqliteException. This is cause write\_->create_topic() call is where we add the db entry for a particular topic. And, this leads to crashing before any recording.
  Locally I solved it by adding the db entry first, and if
  create_subscription fails, remove the topic entry from the db and also
  erase the subscription.
  Signed-off-by: Sriram Raghunathan <rsriram7@visteon.com>
  * Fix comments for pull request https://github.com/ros2/rosbag2/pull/93
  Signed-off-by: Sriram Raghunathan <rsriram7@visteon.com>
  * Added unit test case for remove_topics from db
  Signed-off-by: Sriram Raghunathan <rsriram7@visteon.com>
  * Fix unit tests failing by adding dependent test macros
  Signed-off-by: Sriram Raghunathan <rsriram7@visteon.com>
  * Fixes the linter errors
* Contributors: Sriram Raghunathan

0.1.1 (2019-05-09)
------------------

0.1.0 (2019-05-08)
------------------
* fix logging signature (`#107 <https://github.com/ros2/rosbag2/issues/107>`_)
* Contributors: Dirk Thomas

0.0.5 (2018-12-27)
------------------

0.0.4 (2018-12-19)
------------------
* 0.0.3
* Play old bagfiles (`#69 <https://github.com/bsinno/rosbag2/issues/69>`_)
* Contributors: Karsten Knese, Martin Idel

0.0.2 (2018-12-12)
------------------
* update maintainer email
* Contributors: Karsten Knese

0.0.1 (2018-12-11)
------------------
* rename topic_with_types to topic_metadata
* GH-142 replace map with unordered map where possible (`#65 <https://github.com/ros2/rosbag2/issues/65>`_)
* Use converters when recording a bag file (`#57 <https://github.com/ros2/rosbag2/issues/57>`_)
* use uint8 for serialized message (`#61 <https://github.com/ros2/rosbag2/issues/61>`_)
* Renaming struct members for consistency (`#64 <https://github.com/ros2/rosbag2/issues/64>`_)
* Use converters when playing back files (`#56 <https://github.com/ros2/rosbag2/issues/56>`_)
* Implement converter plugin for CDR format and add converter plugins package (`#48 <https://github.com/ros2/rosbag2/issues/48>`_)
* Display bag summary using `ros2 bag info` (`#45 <https://github.com/ros2/rosbag2/issues/45>`_)
* Use directory as bagfile and add additonal record options (`#43 <https://github.com/ros2/rosbag2/issues/43>`_)
* Introduce rosbag2_transport layer and CLI (`#38 <https://github.com/ros2/rosbag2/issues/38>`_)
* Add correct timing behaviour for rosbag play (`#32 <https://github.com/ros2/rosbag2/issues/32>`_)
* Improve sqlite usage and test stability (`#31 <https://github.com/ros2/rosbag2/issues/31>`_)
* Record and play multiple topics (`#27 <https://github.com/ros2/rosbag2/issues/27>`_)
* Allow an arbitrary topic to be recorded (`#26 <https://github.com/ros2/rosbag2/issues/26>`_)
* Use serialized message directly (`#24 <https://github.com/ros2/rosbag2/issues/24>`_)
* initial version of plugin based storage api (`#7 <https://github.com/ros2/rosbag2/issues/7>`_)
* (demo, sqlite3) First working rosbag2 implementation (`#6 <https://github.com/ros2/rosbag2/issues/6>`_)
* initial setup
* Contributors: Alessandro Bottero, Andreas Greimel, Andreas Holzner, Karsten Knese, Martin Idel

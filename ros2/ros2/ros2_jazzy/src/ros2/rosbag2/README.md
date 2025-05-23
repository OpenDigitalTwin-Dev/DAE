# rosbag2
![License](https://img.shields.io/github/license/ros2/rosbag2)
[![GitHub Action Status](https://github.com/ros2/rosbag2/workflows/Test%20rosbag2/badge.svg)](https://github.com/ros2/rosbag2/actions)

Repository for implementing rosbag2 as described in its corresponding [design article](https://github.com/ros2/design/blob/ros2bags/articles/rosbags.md).

## Installation

### Debian packages

rosbag2 packages are available via debian packages, and will already be included with any `-ros-base` installation (which is included within `-desktop`)

```
$ export ROS_DISTRO=humble
$ sudo apt-get install ros-$ROS_DISTRO-rosbag2
```

### Other binaries

You can follow the instructions at http://docs.ros.org/en/humble/Installation/Alternatives/Ubuntu-Install-Binary.html (or for your chosen distro), and Rosbag2 will be included in that installation.

### Build from source

To build from source, follow the instructions in [DEVELOPING.md](DEVELOPING.md)

## Using rosbag2

rosbag2 is part of the ROS 2 command line interface as `ros2 bag`.
These verbs are available for `ros2 bag`:

* `ros2 bag burst`
* `ros2 bag convert`
* `ros2 bag info`
* `ros2 bag list`
* `ros2 bag play`
* `ros2 bag record`
* `ros2 bag reindex`

For up-to-date information on the available options for each, use `ros2 bag <verb> --help`.

Moreover, `rosbag2_transport::Player` and `rosbag2_transport::Recorder` components can be instantiated in `rclcpp` component containers, which makes possible to use intra-process communication for greater efficiency.
See [composition](#using-with-composition) section for details.

### Recording data

In order to record all topics currently available in the system:

```
$ ros2 bag record -a
```

The command above will record all available topics and discovers new topics as they appear while recording.
This auto-discovery of new topics can be disabled by given the command line argument `--no-discovery`.

To record a set of predefined topics, one can specify them on the command line explicitly.

```
$ ros2 bag record <topic1> <topic2> … <topicN>
```

The specified topics don't necessarily have to be present at start time.
The discovery function will automatically recognize if one of the specified topics appeared.
In the same fashion, this auto discovery can be disabled with `--no-discovery`.

If not further specified, `ros2 bag record` will create a new folder named to the current time stamp and stores all data within this folder.
A user defined name can be given with `-o, --output`.

#### Simulation time

In ROS 2, "simulation time" refers to publishing a clock value on the `/clock` topic, instead of using the system clock to tell time.
By passing `--use-sim-time` argument to `ros2 bag record`, we turn on this option for the recording node.
Messages written to the bag will use the latest received value of `/clock` for the timestamp of the recorded message.

Note: Until the first `/clock` message is received, the recorder will not write any messages.
Before that message is received, the time is 0, which leads to a significant time jump once simulation time begins, making the bag essentially unplayable if messages are written first with time 0 and then time N from `/clock`.

#### Splitting files during recording

rosbag2 offers the capability to split bag files when they reach a maximum size or after a specified duration. By default rosbag2 will record all data into a single bag file, but this can be changed using the CLI options.

_Splitting by size_: `ros2 bag record -a -b 100000` will split the bag files when they become greater than 100 kilobytes. Note: the batch size's units are in bytes and must be greater than `86016`. This option defaults to `0`, which means data is written to a single file.

_Splitting by time_: `ros2 bag record -a -d 9000` will split the bag files after a duration of `9000` seconds. This option defaults to `0`, which means data is written to a single file.

If both splitting by size and duration are enabled, the bag will split at whichever threshold is reached first.

#### Recording with compression

By default rosbag2 does not record with compression enabled. However, compression can be specified using the following CLI options.

For example, `ros2 bag record -a --compression-mode file --compression-format zstd` will record all topics and compress each file using the [zstd](https://github.com/facebook/zstd) compressor.

Currently, the only `compression-format` available is `zstd`. Both the mode and format options default to `none`. To use a compression format, a compression mode must be specified, where the currently supported modes are compress by `file` or compress by `message`.

It is recommended to use this feature with the splitting options.

**Note**: Some storage plugins may have their own compression methods, which are separate from the rosbag2 compression specified by the CLI options `--compression-mode` and `--compression-format`. Notably, the MCAP file format offered by the `rosbag2_storage_mcap` storage plugin supports compression in a way that produces files that are still indexable (whereas using the rosbag2 compression will not). To utilize storage plugin specific compression or other options, see [Recording with a storage configuration](#Recording-with-a-storage-configuration).

#### Recording with a storage configuration

Storage configuration can be specified in a YAML file passed through the `--storage-config-file` option.
This can be used to optimize performance for specific use cases.

See storage plugin documentation for more detail:
* [mcap](rosbag2_storage_mcap/README.md#writer-configuration)
* [sqlite3](rosbag2_storage_sqlite3/README.md#storage-configuration-file)

#### Controlling recordings via services

The rosbag2 recorder provides the following services for remote control, which can be called via `ros2 service` commandline, or from your nodes:

* `~/is_paused [rosbag2_interfaces/srv/IsPaused]`
  * Returns whether recording is currently paused.
* `~/pause [rosbag2_interfaces/srv/Pause]`
  * Pauses recording. All messages that have already arrived will be written, but all messages that arrive after pausing will be discarded. Has no effect if already paused. Takes no arguments.
* `~/resume [rosbag2_interfaces/srv/Resume]`
  * Resume recording if paused. Has no effect if not paused. Takes no arguments.
* `~/split_bagfile [rosbag2_interfaces/srv/SplitBagfile]`
  * Triggers a split to a new file, even if none of the configured split criteria (such as `--max-bag-size` or `--max-bag-duration`) have been met yet
* `~/snapshot [rosbag2_interfaces/srv/Snapshot]`
  * enabled if `--snapshot-mode` is specified. Takes no arguments, triggers a snapshot.

#### Snapshot mode

The Recorder provides a "snapshot mode", enabled via `--snapshot-mode` or `StorageOptions.snapshot_mode`, which does not write messages to disk as they come in, but instead keeps an in-memory circular buffer of size `--max-cache-size`.
This entire buffer can be dumped to disk on request, saving data only in specified circumstances such as a detected error condition or point of interest, capturing the "last N bytes" of incoming data, therefore making sure that you can trigger snapshot after the fact of the event.

The snapshot is taken by calling the `~/snapshot` service on the recorder, described previously.

### Replaying data

When you have a recorded bag, you can use Rosbag2 to play it back:

```
$ ros2 bag play <bag>
```

The bag argument can be a directory containing `metadata.yaml` and one or more storage files, or to a single storage file such as `.mcap` or `.db3`.
The Player will automatically detect which storage implementation to use for playing.

To play back multiple bags:

```
$ ros2 bag play <bag1> -i <bag2> -i <bag3>
```

Messages from all provided bags will be played in order, based on their original recording reception timestamps.

#### Controlling playback via services

The Rosbag2 player provides the following services for remote control, which can be called via `ros2 service` commandline or from your nodes,

* `~/burst [rosbag2_interfaces/srv/Burst]`
  * Can only be used while player is paused, publishes `num_messages` in order as fast as possible, moving forward the play head.
* `~/get_rate [rosbag2_interfaces/srv/GetRate]`
  * Return the current playback rate.
* `~/is_paused [rosbag2_interfaces/srv/IsPaused]`
  * Return whether playback is paused.
* `~/pause [rosbag2_interfaces/srv/Pause]`
  * Pause playback. Has no effect if already paused.
* `~/play [rosbag2_interfaces/srv/Play]`
  * Play from a starting offset timestamp, either until the end, an ending timestamp or for a set duration. Only works when stopped (not paused).
* `~/play_next [rosbag2_interfaces/srv/PlayNext]`
  * Play a single next message from the bag. Only works while paused.
* `~/resume [rosbag2_interfaces/srv/Resume]`
  * Resume playback if paused.
* `~/seek [rosbag2_interfaces/srv/Seek]`
  * Change the play head to the specified timestamp. Can be forward or backward in time, the next played message is the next immediately after the seeked timestamp.
* `~/set_rate [rosbag2_interfaces/srv/SetRate]`
  * Sets the rate of playback, for example 2.0 will play messages twice as fast.
* `~/stop [rosbag2_interfaces/srv/Stop]`
  * Stop the player, putting the play head in "undefined position" outside the bag. Must call `play` before other operations can be done.
* `~/toggle_paused [rosbag2_interfaces/srv/TogglePaused]`
  * Pause if playing, resume if paused.


### Analyzing data

The recorded data can be analyzed by displaying some meta information about it:

```
$ ros2 bag info <bag_file>
```

You should see something along these lines:

```
Files:             demo_strings.db3
Bag size:          44.5 KiB
Storage id:        sqlite3
Duration:          8.501s
Start:             Nov 28 2018 18:02:18.600 (1543456938.600)
End                Nov 28 2018 18:02:27.102 (1543456947.102)
Messages:          27
Topic information: Topic: /chatter | Type: std_msgs/String | Count: 9 | Serialization Format: cdr
                   Topic: /my_chatter | Type: std_msgs/String | Count: 18 | Serialization Format: cdr
```

### Converting bags

Rosbag2 provides a tool `ros2 bag convert` (or, `rosbag2_transport::bag_rewrite` in the C++ API).
This allows the user to take one or more input bags, and write them out to one or more output bags with new settings.
This flexible feature enables the following features:
* Merge (multiple input bags, one output bag)
* Split top-level bags (one input bag, multiple output bags)
* Split internal files (by time or size - one input bag with fewer internal files, one output bag with more, smaller, internal files)
* Compress/Decompress (output bag(s) with different compression settings than the input(s))
* Serialization format conversion
* ... and more!

Here is an example command:

```
ros2 bag convert --input /path/to/bag1 --input /path/to/bag2 storage_id --output-options output_options.yaml
```

The `--input` argument may be specified any number of times, and takes 1 or 2 values.
The first value is the URI of the input bag.
If a second value is supplied, it specifies the storage implementation of the bag.
If no storage implementation is specified, rosbag2 will try to determine it automatically from the bag.

The `--output-options` argument must point to the URI of a YAML file specifying the full recording configuration for each bag to output (`StorageOptions` + `RecordOptions`).
This file must contain a top-level key `output_bags`, which contains a list of these objects.

The only required value in the output bags is `uri` and `storage_id`. All other values are options (however, if no topic selection is specified, this output bag will be empty!).

This example notes all fields that can have an effect, with a comment on the required ones.

```
output_bags:
- uri: /output/bag1  # required
  storage_id: ""  # will use the default storage plugin, if unspecified
  max_bagfile_size: 0
  max_bagfile_duration: 0
  storage_preset_profile: ""
  storage_config_uri: ""
  all_topics: false
  topics: []
  topic_types: []
  all_services: false
  services: []
  rmw_serialization_format: ""  # defaults to using the format of the input topic
  regex: ""
  exclude_regex: ""
  exclude_topics: []
  exclude_topic_types: []
  exclude_services: []
  compression_mode: ""
  compression_format: ""
  compression_queue_size: 1
  compression_threads: 0
  include_hidden_topics: false
  include_unpublished_topics: false
```

Example merge:

```
$ ros2 bag convert -i bag1 -i bag2 -o out.yaml

# out.yaml
output_bags:
- uri: merged_bag
  all_topics: true
  all_services: true
```

Example split:

```
$ ros2 bag convert -i bag1 -o out.yaml

# out.yaml
output_bags:
- uri: split1
  topics: [/topic1, /topic2]
- uri: split2
  topics: [/topic3]
```

Example compress:

```
$ ros2 bag convert -i bag1 -o out.yaml

# out.yaml
output_bags:
- uri: compressed
  all_topics: true
  all_services: true
  compression_mode: file
  compression_format: zstd
```

### Overriding QoS Profiles

When starting a recording or playback, you can pass a YAML file that contains QoS profile settings for a specific topic.
The YAML schema for the profile overrides is a dictionary of topic names with key/value pairs for each QoS policy.
Below is an example profile set to the default ROS2 QoS settings.

```yaml
/topic_name:
  history: keep_last
  depth: 10
  reliability: reliable
  durability: volatile
  deadline:
    # unspecified/infinity
    sec: 0
    nsec: 0
  lifespan:
    # unspecified/infinity
    sec: 0
    nsec: 0
  liveliness: system_default
  liveliness_lease_duration:
    # unspecified/infinity
    sec: 0
    nsec: 0
  avoid_ros_namespace_conventions: false
```

You can then use the override by specifying the `--qos-profile-overrides-path` argument in the CLI:

```sh
# Record
ros2 bag record --qos-profile-overrides-path override.yaml -a -o my_bag
# Playback
ros2 bag play --qos-profile-overrides-path override.yaml my_bag
```

See [the official QoS override tutorial][qos-override-tutorial] and ["About QoS Settings"][about-qos-settings] for more detail.

### Using in launch

We can invoke the command line tool from a ROS launch script as an *executable* (not a *node* action).
For example, to launch the command to record all topics you can use the following launch script:

```xml
<launch>
  <executable cmd="ros2 bag record -a" output="screen" />
</launch>
```

Here's the equivalent Python launch script:

```python
import launch


def generate_launch_description():
    return launch.LaunchDescription([
        launch.actions.ExecuteProcess(
            cmd=['ros2', 'bag', 'record', '-a'],
            output='screen'
        )
    ])
```

Use the `ros2 launch` command line tool to launch either of the above launch scripts.
For example, if we named the above XML launch script, `record_all.launch.xml`:

```sh
$ ros2 launch record_all.launch.xml
```

You can also invoke the `play` and `record` functionalities provided by `rosbag2_transport` package as nodes.
The advantage to use this invocation strategy is that the Python layer handling the `ros2 bag` CLI is completely skipped.

```python
import launch

def generate_launch_description():
    return launch.LaunchDescription([
        launch.actions.Node(
            package='rosbag2_transport',
            executable='player',
            name='player',
            output="screen",
            parameters=["/path/to/params.yaml"],
        )
    ])
```

## Using with composition

Play and record are fundamental tasks of `rosbag2`. However, playing or recording data at high rates may have limitations (e.g. spurious packet drops) due to one of the following:
- low network bandwidth
- high CPU load
- slow mass memory
- ROS 2 middleware serialization/deserialization delays & overhead

ROS 2 C++ nodes can benefit from intra-process communication to partially or completely bypass network transport of messages between two nodes.

Multiple _components_ can be _composed_, either [statically](https://docs.ros.org/en/rolling/Tutorials/Intermediate/Composition.html#compile-time-composition-with-hardcoded-nodes) or [dynamically](https://docs.ros.org/en/rolling/Tutorials/Intermediate/Composition.html#run-time-composition-using-ros-services-with-a-publisher-and-subscriber): all the composed component will share the same address space because they will be loaded in a single process.

A prerequirement is for each C++ node to be [_composable_](https://docs.ros.org/en/rolling/Concepts/Intermediate/About-Composition.html?highlight=composition) and to follow the [guidelines](https://docs.ros.org/en/rolling/Tutorials/Demos/Intra-Process-Communication.html?highlight=intra) for efficient publishing & subscription.

With the above requirements met, the user can:
- compose multiple nodes together
- explicitly enable intra-process communication

Whenever a publisher and a subscriber on the same topic belong to the same _composed_ process, and intra-process is enabled for both, `rclcpp` completely bypasses RMW layer and below transport layer (i.e. DDS). Instead, messages are shared via process memory and *potentially* never copied. Some exception hold, so please have a look to the [IPC guidelines](https://docs.ros.org/en/rolling/Tutorials/Demos/Intra-Process-Communication.html?highlight=intra).

Here is an example of Python launchfile composition. Notice that composable container components do not expect YAML files to be directly passed to them: parameters have to be "dumped" out from the YAML file (if you have one). A suggestion of possible implementation is offered as a starting point.

```python
import launch
import launch_ros
import yaml

'''
Used to load parameters for composable nodes from a standard param file
'''
def dump_params(param_file_path, node_name):
    with open(param_file_path, 'r') as file:
        return [yaml.safe_load(file)[node_name]['ros__parameters']]

def generate_launch_description():
    return launch.LaunchDescription([
        launch.actions.ComposableNodeContainer(
            name='composable_container',
            package='rclcpp_components',
            executable='component_container',
            composable_node_descriptions=[
                launch_ros.descriptions.ComposableNode(
                    package='rosbag2_transport',
                    plugin='rosbag2_transport::Player',
                    name='player',
                    parameters=dump_params("/path/to/params.yaml", "player"),
                    extra_arguments=[{'use_intra_process_comms': True}]
                ),
                # your other components here
            ]
        )
    ])
}
```

Here's an example YAML configuration for both composable player and recorder:
```yaml
recorder:
  ros__parameters:
    use_sim_time: false
    record:
      all: true
      is_discovery_disabled: false
      topic_polling_interval:
        sec: 0
        nsec: 10000000
      include_hidden_topics: true
      ignore_leaf_topics: false
      start_paused: false

    storage:
      uri: "/path/to/destination/folder"
      storage_id: "sqlite3"
      max_cache_size: 20000000
```
and
```yaml
player:
  ros__parameters:
    play:
      read_ahead_queue_size: 1000
      node_prefix: ""
      rate: 1.0
      loop: false
      # Negative timestamps will make the playback to not stop.  
      playback_duration:
        sec: -1
        nsec: 00000000
      start_paused: false

    storage:
      uri: "path/to/rosbag/file"
      storage_id: "mcap"
      storage_config_uri: ""
```

For a full list of available parameters, you can refer to [`player`](rosbag2_transport/test/resources/player_node_params.yaml) and [`recorder`](rosbag2_transport/test/resources/recorder_node_params.yaml) configurations from the `test` folder of `rosbag2_transport`.

## Storage format plugin architecture

Looking at the output of the `ros2 bag info` command, we can see a field `Storage id:`.
Rosbag2 was designed to support multiple storage formats to adapt to individual use cases.
This repository provides two storage plugins, `mcap` and `sqlite3`.
The default is `mcap`, which is provided to code by [`rosbag2_storage::get_default_storage_id()`](rosbag2_storage/include/rosbag2_storage/default_storage_id.hpp) and defined in [`default_storage_id.cpp`](rosbag2_storage/src/rosbag2_storage/default_storage_id.cpp#L21)

If not specified otherwise, rosbag2 will write data using the default plugin.

In order to use a specified (non-default) storage format plugin, rosbag2 has a command line argument `--storage`:

```
$ ros2 bag record --storage <storage_id>
```

Bag reading commands can detect the storage plugin automatically, but if for any reason you want to force a specific plugin to read a bag, you can use the `--storage` option on any `ros2 bag` verb.

To write your own Rosbag2 storage implementation, refer to [Storage Plugin Development](docs/storage_plugin_development.md)


## Serialization format plugin architecture

Looking further at the output of `ros2 bag info`, we can see another field attached to each topic called `Serialization Format`.
By design, ROS 2 is middleware agnostic and thus can leverage multiple communication frameworks.
The default middleware for ROS 2 is DDS which has `cdr` as its default binary serialization format.
However, other middleware implementation might have different formats.
If not specified, `ros2 bag record -a` will record all data in the middleware specific format.
This however also means that such a bag file can't easily be replayed with another middleware format.

rosbag2 implements a serialization format plugin architecture which allows the user the specify a certain serialization format.
When specified, rosbag2 looks for a suitable converter to transform the native middleware protocol to the target format.
This also allows to record data in a native format to optimize for speed, but to convert or transform the recorded data into a middleware agnostic serialization format.

By default, rosbag2 can convert from and to CDR as it's the default serialization format for ROS 2.

[qos-override-tutorial]: https://docs.ros.org/en/rolling/Guides/Overriding-QoS-Policies-For-Recording-And-Playback.html
[about-qos-settings]: https://docs.ros.org/en/rolling/Concepts/About-Quality-of-Service-Settings.html

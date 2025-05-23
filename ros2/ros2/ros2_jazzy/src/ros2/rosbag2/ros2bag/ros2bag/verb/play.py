# Copyright 2018 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from argparse import FileType

from rclpy.qos import InvalidQoSProfileException
from ros2bag.api import add_standard_multi_reader_args
from ros2bag.api import check_not_negative_float
from ros2bag.api import check_not_negative_int
from ros2bag.api import check_positive_float
from ros2bag.api import convert_service_to_service_event_topic
from ros2bag.api import convert_yaml_to_qos_profile
from ros2bag.api import input_bag_arg_to_storage_options
from ros2bag.api import print_error
from ros2bag.api import print_warn
from ros2bag.verb import VerbExtension
from ros2cli.node import NODE_NAME_PREFIX
from rosbag2_py import Player
from rosbag2_py import PlayOptions
from rosbag2_py import ServiceRequestsSource
from rosbag2_py import StorageOptions
import yaml


def positive_float(arg: str) -> float:
    value = float(arg)
    if value <= 0:
        raise ValueError(f'Value {value} is less than or equal to zero.')
    return value


class PlayVerb(VerbExtension):
    """Play back ROS data from a bag."""

    def add_arguments(self, parser, cli_name):  # noqa: D102
        add_standard_multi_reader_args(parser)
        parser.add_argument(
            '--read-ahead-queue-size', type=int, default=1000,
            help='size of message queue rosbag tries to hold in memory to help deterministic '
                 'playback. Larger size will result in larger memory needs but might prevent '
                 'delay of message playback.')
        parser.add_argument(
            '-r', '--rate', type=check_positive_float, default=1.0,
            help='rate at which to play back messages. Valid range > 0.0.')
        parser.add_argument(
            '--topics', type=str, default=[], metavar='topic', nargs='+',
            help='Space-delimited list of topics to play.')
        parser.add_argument(
            '--services', type=str, default=[], metavar='service', nargs='+',
            help='Space-delimited list of services to play.')
        parser.add_argument(
            '-e', '--regex', default='',
            help='Play only topics and services matches with regular expression.')
        parser.add_argument(
            '-x', '--exclude-regex', default='',
            help='regular expressions to exclude topics and services from replay.')
        parser.add_argument(
            '--exclude-topics', type=str, default=[], metavar='topic', nargs='+',
            help='Space-delimited list of topics not to play.')
        parser.add_argument(
            '--exclude-services', type=str, default=[], metavar='service', nargs='+',
            help='Space-delimited list of services not to play.')
        parser.add_argument(
            '--qos-profile-overrides-path', type=FileType('r'),
            help='Path to a yaml file defining overrides of the QoS profile for specific topics.')
        parser.add_argument(
            '-l', '--loop', action='store_true',
            help='enables loop playback when playing a bagfile: it starts back at the beginning '
                 'on reaching the end and plays indefinitely.')
        parser.add_argument(
            '--remap', '-m', default='', nargs='+',
            help='list of topics to be remapped: in the form '
                 '"old_topic1:=new_topic1 old_topic2:=new_topic2 etc." ')
        parser.add_argument(
            '--storage-config-file', type=FileType('r'),
            help='Path to a yaml file defining storage specific configurations. '
                 'See storage plugin documentation for the format of this file.')
        clock_args_group = parser.add_mutually_exclusive_group()
        clock_args_group.add_argument(
            '--clock', type=positive_float, nargs='?', const=40, default=0,
            help='Publish to /clock at a specific frequency in Hz, to act as a ROS Time Source. '
                 'Value must be positive. Defaults to not publishing.'
                 'If specified, /clock topic in the bag file is excluded to publish.')
        clock_args_group.add_argument(
            '--clock-topics', type=str, default=[], nargs='+',
            help='List of topics separated by spaces that will trigger a /clock update '
                 'when a message is published on them'
        )
        clock_args_group.add_argument(
            '--clock-topics-all', default=False, action='store_true',
            help='Publishes an update on /clock immediately before each replayed message'
        )
        parser.add_argument(
            '-d', '--delay', type=positive_float, default=0.0,
            help='Sleep duration before play (loops are not affected), in seconds.'
                 'Negative durations invalid.')
        parser.add_argument(
            '--playback-duration', type=float, default=-1.0,
            help='Playback duration, in seconds. Negative durations mark an infinite playback. '
                 'Default is %(default)d. '
                 'When positive, the maximum effective time between `playback-until-*` '
                 'and this argument will determine when playback stops.')

        playback_until_arg_group = parser.add_mutually_exclusive_group()
        playback_until_arg_group.add_argument(
            '--playback-until-sec', type=float, default=-1.,
            help='Playback until timestamp, expressed in seconds since epoch. '
                 'Mutually exclusive argument with `--playback-until-nsec`. '
                 'Use when floating point to integer conversion error is not a concern. '
                 'A negative value disables this feature. '
                 'Default is %(default)f. '
                 'When positive, the maximum effective time between `--playback-duration` '
                 'and this argument will determine when playback stops.')
        playback_until_arg_group.add_argument(
            '--playback-until-nsec', type=int, default=-1,
            help='Playback until timestamp, expressed in nanoseconds since epoch.  '
                 'Mutually exclusive argument with `--playback-until-sec`. '
                 'Use when floating point to integer conversion error matters for your use case. '
                 'A negative value disables this feature. '
                 'Default is %(default)s. '
                 'When positive, the maximum effective time between `--playback-duration` '
                 'and this argument will determine when playback stops.')

        parser.add_argument(
            '--disable-keyboard-controls', action='store_true',
            help='disables keyboard controls for playback')
        parser.add_argument(
            '-p', '--start-paused', action='store_true', default=False,
            help='Start the playback player in a paused state.')
        parser.add_argument(
            '--start-offset', type=check_not_negative_float, default=0.0,
            help='Start the playback player this many seconds into the bag file.')
        parser.add_argument(
            '--wait-for-all-acked', type=check_not_negative_int, default=-1,
            help='Wait until all published messages are acknowledged by all subscribers or until '
                 'the timeout elapses in millisecond before play is terminated. '
                 'Especially for the case of sending message with big size in a short time. '
                 'Negative timeout is invalid. '
                 '0 means wait forever until all published messages are acknowledged by all '
                 'subscribers. '
                 "Note that this option is valid only if the publisher\'s QOS profile is "
                 'RELIABLE.',
            metavar='TIMEOUT')
        parser.add_argument(
            '--disable-loan-message', action='store_true', default=False,
            help='Disable to publish as loaned message. '
                 'By default, if loaned message can be used, messages are published as loaned '
                 'message. It can help to reduce the number of data copies, so there is a greater '
                 'benefit for sending big data.')
        parser.add_argument(
            '--publish-service-requests', action='store_true', default=False,
            help='Publish recorded service requests instead of recorded service events')
        parser.add_argument(
            '--service-requests-source', default='service_introspection',
            choices=['service_introspection', 'client_introspection'],
            help='Determine the source of the service requests to be replayed. This option only '
                 'makes sense if the "--publish-service-requests" option is set. By default,'
                 ' the service requests replaying from recorded service introspection message.')
        parser.add_argument(
            '--log-level', type=str, default='info',
            choices=['debug', 'info', 'warn', 'error', 'fatal'],
            help='Logging level.')

    def get_playback_until_from_arg_group(self, playback_until_sec, playback_until_nsec) -> int:
        nano_scale = 1000 * 1000 * 1000
        if playback_until_sec and playback_until_sec >= 0.0:
            return int(playback_until_sec * nano_scale)
        if playback_until_nsec and playback_until_nsec >= 0:
            return playback_until_nsec
        return -1

    def main(self, *, args):  # noqa: D102
        qos_profile_overrides = {}  # Specify a valid default
        if args.qos_profile_overrides_path:
            qos_profile_dict = yaml.safe_load(args.qos_profile_overrides_path)
            try:
                qos_profile_overrides = convert_yaml_to_qos_profile(
                    qos_profile_dict)
            except (InvalidQoSProfileException, ValueError) as e:
                return print_error(str(e))

        storage_config_file = ''
        if args.storage_config_file:
            storage_config_file = args.storage_config_file.name

        topic_remapping = ['--ros-args']
        for remap_rule in args.remap:
            topic_remapping.append('--remap')
            topic_remapping.append(remap_rule)

        # Do not allow using both positional arg and --input option for input bags
        if args.bag_path and args.input:
            return print_error(
                'do not mix the [bag_path] positional argument and the -i,--input option; '
                'for multiple input bags, use -i,--input multiple times')
        # Add bag from optional positional arg, then bag(s) from optional flag
        storage_options = []
        if args.bag_path:
            storage_options.append(StorageOptions(
                uri=args.bag_path,
                storage_id=args.storage,
                storage_config_uri=storage_config_file,
            ))
            if args.storage:
                print(print_warn('--storage option is deprecated, use -i,--input to '
                                 'provide an input bag with a specific storage ID'))
        try:
            storage_options.extend(
                input_bag_arg_to_storage_options(args.input or [], storage_config_file))
        except ValueError as e:
            return print_error(str(e))
        if not storage_options:
            return print_error('no input bags were provided')

        # Users can currently only provide one storage config file, which is storage
        # implementation-specific. Since we can replay bags from different storage
        # implementations, this may lead to errors. For now, just warn if input bags have
        # different explicit storage IDs and a storage config file is provided.
        storage_ids = {options.storage_id for options in storage_options if options.storage_id}
        if storage_config_file and len(storage_ids) > 1:
            print(
                print_warn('a global --storage-config-file was provided, but -i,--input bags are '
                           'using different explicit storage IDs, which may lead to errors with '
                           f'replay: {storage_ids}'))

        play_options = PlayOptions()
        play_options.read_ahead_queue_size = args.read_ahead_queue_size
        play_options.node_prefix = NODE_NAME_PREFIX
        play_options.rate = args.rate
        play_options.topics_to_filter = args.topics

        # Convert service name to service event topic name
        play_options.services_to_filter = convert_service_to_service_event_topic(args.services)

        play_options.regex_to_filter = args.regex

        play_options.exclude_regex_to_filter = args.exclude_regex

        play_options.exclude_service_events_to_filter = \
            convert_service_to_service_event_topic(args.exclude_services)

        play_options.topic_qos_profile_overrides = qos_profile_overrides
        play_options.loop = args.loop
        play_options.topic_remapping_options = topic_remapping
        play_options.clock_publish_frequency = args.clock
        exclude_topics = args.exclude_topics if args.exclude_topics else []
        if play_options.clock_publish_frequency > 0:
            exclude_topics.append('/clock')
        play_options.exclude_topics_to_filter = exclude_topics
        if args.clock_topics_all or len(args.clock_topics) > 0:
            play_options.clock_publish_on_topic_publish = True
        play_options.clock_topics = args.clock_topics
        play_options.delay = args.delay
        play_options.playback_duration = args.playback_duration
        play_options.playback_until_timestamp = self.get_playback_until_from_arg_group(
            args.playback_until_sec, args.playback_until_nsec)
        play_options.disable_keyboard_controls = args.disable_keyboard_controls
        play_options.start_paused = args.start_paused
        play_options.start_offset = args.start_offset
        play_options.wait_acked_timeout = args.wait_for_all_acked
        play_options.disable_loan_message = args.disable_loan_message
        play_options.publish_service_requests = args.publish_service_requests
        if not args.service_requests_source or \
                args.service_requests_source == 'service_introspection':
            play_options.service_requests_source = ServiceRequestsSource.SERVICE_INTROSPECTION
        else:
            play_options.service_requests_source = ServiceRequestsSource.CLIENT_INTROSPECTION

        player = Player(args.log_level)
        try:
            player.play(storage_options, play_options)
        except KeyboardInterrupt:
            pass

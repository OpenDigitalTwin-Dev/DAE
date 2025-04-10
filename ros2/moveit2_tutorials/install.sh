source /opt/ros/jazzy/setup.bash

rosdep install -r --from-paths src --ignore-src --rosdistro jazzy -y

colcon build --event-handlers desktop_notification- status- --cmake-args -DCMAKE_BUILD_TYPE=Release

source install/setup.sh
ros2 launch moveit2_tutorials demo.launch.py

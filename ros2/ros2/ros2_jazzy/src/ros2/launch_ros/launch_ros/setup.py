from setuptools import find_packages
from setuptools import setup

package_name = 'launch_ros'

setup(
    name=package_name,
    version='0.26.6',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/' + package_name, ['package.xml']),
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
    ],
    package_data={'launch_ros': ['py.typed']},
    install_requires=[
        'setuptools',
        'ament_index_python',
        'launch',
        'osrf_pycommon',
        'pyyaml',
    ],
    zip_safe=True,
    author='William Woodall',
    author_email='william@osrfoundation.org',
    maintainer='Aditya Pande, Brandon Ong',
    maintainer_email='aditya.pande@openrobotics.org, brandon@openrobotics.org',
    url='https://github.com/ros2/launch_ros',
    download_url='https://github.com/ros2/launch_ros/releases',
    keywords=['ROS'],
    classifiers=[
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Programming Language :: Python',
        'Topic :: Software Development',
    ],
    description='ROS specific extensions to `launch`.',
    long_description=(
        'This package provides ROS specific extensions to the launch package.'
    ),
    license='Apache License, Version 2.0',
    tests_require=['pytest'],
    entry_points={
        'launch.frontend.launch_extension': [
            'launch_ros = launch_ros',
        ],
    }
)

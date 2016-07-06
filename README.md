# REU_Rover_Simulator

## About
This project contains the simulator of the 2016 OSU Robotics REU multiagent coordination team.
The initial implementation will be a more basic gridworld that will be used for theoretical research.
These models will then be translated to a physical platform, the Pioneer robots.
There is the possibility for further work on more realistic simulator once the basic gridworld is complete.

## Authors
* [Connor Yates](github.com/Sir-Batman), [yatesco@oregonstate.edu](mailto:yatesco@oregonstate.edu)
* Abby Van Soest, [asoest@princeton.edu](mailto:asoest@princeton.edu)
* Tobi Ogunyale, [tobiogunyale@gmail.com](mailto:tobiogunyale@gmail.com)

## Dependencies
This project requires [FANN](http://leenissen.dk/fann/wp/) to be installed on the system to implement the neural network to control the agent policy.

## Building and Running
The provided makefile has been tested so far only on Linux systems.

To run a program that relies on FANN, it is necessary to set the proper environment variables for the runtime linker.
FANN installs its libraries in `/usr/local/lib` by default, so if using the default system installation, the runtime linker may not find the necessary libraries.

This can be fixed temporarily in a terminal with 
```bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
```
or my adding the FANN library path to the ld config directory.
This is traditionally in `/etc/ld.so.conf.d/`, and requires a root user invocation of `ldconfig` after adding the config file. 

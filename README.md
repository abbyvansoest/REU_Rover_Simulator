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
This project will work with either a default global installation of FANN (in `/usr/local/lib`) or via a custom installation.
The custom installation is useful for situations where superuser access is not available.
The disk usage of the installed libraries is minimal, so it can easily be done in a restricted-account in a multi-user server situation.

This code relies on the most recent version of FANN, available directly from the source repository on [GitHub](https://github.com/libfann/fann).
At time of writing, the most recent version was in commit d71d54788bee56ba4cf7522801270152da5209d7.

To do a default installation of FANN, the instructions provided (`cmake . && sudo make install`) work fine.
For a non-default installation, `cmake . && make` will fully build the library.
The library files (`*.a`, `*.so`, `*.so.2`, and `*.so.2.2.0`) will be created in `fann/src`.
The header files (`*.h` and `*.hpp`) can be found in `fann/src/include`.
Copy these files to your desired installation location for the FANN library and include directories, respectively.

## Building and Running
The provided makefile has been tested on x86-64 systems of Arch Linux (Kernel 4.6.4-1), CentOS 7.2.1511 (Kernel 3.10.0-327) and OSX El Capitan systems.
### Non-Default FANN Installation
If the FANN library and headers were installed to a non-default location, the `-L` and `-I` flags must be passed to the compiler (either `g++` or `clang`) with the locations of the library and include directories, respectively.

To run a program that relies on FANN, it is necessary to set the proper environment variables for the runtime linker.
This seems to work automatically in OSX systems, but does not happen automatically on Linux systems, since the default installation directory (`/usr/local/lib`) is not searched by `ld` by default.

This can be fixed temporarily in a terminal with 
```bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
```
replacing `/usr/local/lib` with the location of the locally installed library, if this was the case.
As this is an environment variable, it will not persist past the lifetime of the shell process.
A more permanent solution is to either invoke the above command in the shell config file, or by adding a rule to the `ld` config directory (requires superuser privileges).
This is traditionally in `/etc/ld.so.conf.d/`.
To add a rule, create a new file in the format `*.conf`, such as `fann.conf`.
The contents of this file is the directory to search, such as `/usr/local/lib` (or the local installation directory).
After creating the file, a root user invocation of `ldconfig` will reload the config files, adding the new directory to the search path of the runtime linker.

The compiled simulator can now be run with `./grid_sim.out` (or whatever executable name is specified in the makefile.

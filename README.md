# Randomizer

A UNIX command line utility written in C++ for generating pseudorandom numbers and sequences. A variety of different pseudorandom number generators can be used including XORShift, a Linear-Congruential-Generator, and the famous Mersenne-Twister. This program is not necessarily intended to generate pseudorandom values quickly, in bulk quantities, or for scientific purposes. 

This program was mainly written so that I could learn modern C++ and how to build C++ projects with CMake. The build scripts are setup to package this utility as a `.deb` or a `.rpm` for easy installation alongside the included man page on most popular and modern linux distributions. There are also container files included that come with the utility installed inside of them in case you  just want to try it out without installing it on your host machine. 

For usage instructions, use `randomizer --help` or view the [included man page](./docs/randomizer.1) using `man randomizer` once the application is installed.

## Building

The project is built using CMake. First, make sure you have cmake >= 3.22 installed on your machine:

```shell
cmake --version
```

If you do not have CMake installed, or your version is <3.22, you can install it using [these instructions](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html#getting-and-installing-cmake-on-your-computer). 

Also ensure that you have a modern version of `gcc` or `clang` installed that supports C++ versions 20 or later. CMake will also need to use a build tool such as `make` or `ninja-build`, make sure one of those is installed on your machine. These packages should be included by most Linux distribution app repositories when you install CMake with tools like `apt` or `dnf`.

Once CMake is installed, clone or download this repo and open the root directory of this repo. You can then configure the project using:
```shell
cmake -S . -B build
```

and then build the project using:
```shell
cmake --build build
```

You should now find that there is a binary called `randomizer` in `./build/out/`. This is the binary executable for the program. You can run the program by using `./randomizer`. 


## Testing

When the project was built there should have also been a binary called `RandomizerTests` which contains the executable for unit testing. You can run all of the unit tests by using the command `ctest` from the `./build/` directory.

## Packaging

To package the program in `.deb` and `.rpm` formats change into the `./build/` directory and use the command `cpack`. This will create a `.deb` archive for installing on Debian based systems and a `.rpm` archive for installing on RedHat based systems. Both packages will install the binary executable and the included man page. These archives will be placed in the `./build/out/` directory. 

Once packaged, you can install the `.deb` archive on any modern Debian-based system with:
```shell
sudo dpkg -i ./build/out/randomizer-<version>.deb
```

To install the `.rpm` archive on any modern RedHat-based distribution, use:
```shell
sudo dnf install ./build/out/randomizer-<version>.rpm
```

> These packages were tested on Ubuntu 24.04, Debian 12, and Fedora 42. 

## Containers

If you do not wish to install the application directly on your host machine, container files are included which will install the application to be used in the container. One image is Debian based and installs the `.deb` archive and the other is Fedora based and installs the `.rpm` archive. Make sure you go through the steps to build and package the application with CMake before building the container(s).

The containers can be created using [Podman](https://podman.io/) or [Docker](https://www.docker.com/). You must have the CLI for either program installed on your computer. The commands should be the same regardless of what container application you choose to use, just use the appropriate command name. 

The Debian image can be created using the following command from the project's root directory:
```shell
podman build -f containers/Containerfile.debian -t randomizer:debian
```
and the Fedora image can be created using the command:
```shell
podman build -f containers/Containerfile.fedora -t randomizer:fedora
```
also from the project's root directory. 

The container can then be accessed via shell using the command:
```shell
podman run --rm -it randomizer:debian
```
or
```shell
podman run --rm -it randomizer:fedora
```

Once inside the container, the application can be ran like normal from the command line using the command `randomizer` and the manual page can be viewed using `man randomizer`.

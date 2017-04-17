# Internet Visualizer

Source repository for the Internet Visualizer (inetvis) tool.

This project is based off of the original work by JP van Riel and Barry Irwin of Rhodes University. Their original work can be found here: www.cs.ru.ac.za/research/g02v2468/inetvis.html

# Notes

This project was originally written in Qt3 and compiled to 32-bit code, it has since been ported to native Qt4,
and been set to compile a 64-bit binary.

The instructions below have all been tested on the current version of Ubuntu, 17.04 64-bit.


# Building InetVis

In order to build InetVis in your own VM please consider the following guidelines:

Building InetVis is relatively straight forward. Begin by installing the following dependencies:

1. Update your system:
  * `sudo apt-get update` and `sudo apt-get upgrade`, finally `sudo apt-get dist-upgrade`
2. Install the following dependencies:
  * `sudo apt-get install build-essential g++ libc6 libstdc++6 libc6-dev make libpcap-dev libgl1-mesa-dev libqt4-dev libqt4-dev qt4-dev-tools`

Once the dependencies are installed, clone this repository if you haven't already.

1. Clone the github repo into the `inetvis` directory:
  * git clone git@github.com:yestinj/inetvis.git
2. Change into the `inetvis` directory, and then change to `src`.
3. Checkout whichever branch you want to build, i.e. master or develop.
  * `git checkout develop`

Finally, build the `inetvis` binary:

1. `qmake`
2. `make`

This should result in a new `inetvis` binary being generated within the source directory.

# Running InetVis

To run InetVis do the following:

1. Download the latest InetVis binary from the releases page.
  * Alternatively clone the repository and run the `inetvis` binary in the root directory.
  * Or simply use the binary generated from the build process, in `src/`
2. Install the requirements (if necessary):
  * `sudo apt-get install build-essential g++ libc6 libstdc++6 libc6-dev make libpcap-dev libgl1-mesa-dev libqt4-dev libqt4-dev qt4-dev-tools`
  * Note: These are also the build requirements, installing them may not be necessary for simply running.

Next, locate the binary you downloaded/built and run it as follows: `./inetvis`

If you would like to monitor your own host for network events, instead of replaying capture files, please ensure that you run as root, i.e. `sudo ./inetvis`

Have fun! :-)

# Development

Development is currently done using a combination of Atom, vim, and Qt Creator.

QT Creator allows for simple editing of source code, as well as graphical form editing, and can be
installed in Debian flavours of Linux by installing the `qtcreator` package.

# Versioning

This software will make use of Semantic Versioning 2.0.0 (http://semver.org/) once reaching the v1.0.0 release phase.

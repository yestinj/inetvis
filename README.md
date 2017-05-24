# Internet Visualizer

Source repository for the Internet Visualizer (inetvis) tool.

This project is based off of the original work by JP van Riel and Barry Irwin of Rhodes University. Their original work can be found here: www.cs.ru.ac.za/research/g02v2468/inetvis.html

# Notes

This project was originally written in Qt3 and compiled to 32-bit code, it has since been ported to Qt5,
and been updated to compile a 64-bit binary.

The instructions below have all been tested on the current version of Ubuntu, 17.04 64-bit.

# Installing InetVis (from release archive)

A compiled version of InetVis is available under the releases section of https://github.com/yestinj/inetvis.

In order to install and run the software please do the following:

1. Download the latest release archive from the releases page.
2. Extract the archive which will be called something like `inetvis-2.0.0.tgz`
3. Change into the extracted directory, something like `inetvis-2.0.0`
4. Run the `install_inetvis.sh` shell script to install the software.
  1. This script will: install the software to `/opt/inetvis-<version>`
  2. Create a symlink directory `/opt/inetvis` for convenience
  3. Copy across the relevant files to the new directory under `/opt`.
  4. Place an icon file in `/usr/share/icons/hicolor/48x48/apps/`
  5. Place a `desktop` file in `/usr/share/applications`, allowing inetvis to be found in the menu on Ubuntu systems.
  6. Create a symlink at `/usr/local/bin/inetvis` pointing to the main binary.
  7. Set the `cap_net_raw`, and `cap_net_admin=eip` capabilities on the inetvis binary allowing for monitoring packets on local host without running as root.
5. If the script completes successfully inetvis should now be in your path, and also be in the menu system of your distribution.

# Running InetVis

To run InetVis do the following:

1. Download the latest InetVis release archive from the releases page.
  * Alternatively clone the repository, and follow the build procedure above
2. Install the requirements (if necessary):
  * `sudo apt-get install build-essential g++ libc6 libstdc++6 libc6-dev make libpcap-dev libgl1-mesa-dev libqt4-dev libqt4-dev qt4-dev-tools`
  * Note: These are also the build requirements, installing them may not be necessary for simply running the application.

If you installed InetVis as per the instructions, you should simply be able
to run the `inetvis` command from your command line or menu system.

If you have built the binary yourself, please run it using manually using `./inetvis` from the src directory.

You will need to run your built binary as root should you wish to monitor local host.

Have fun! :-)

# Uninstalling InetVis

A convenience script is included in the release archive, namely `uninstall_inetvis.sh`, which can be used to completely remove inetvis from your system at any time.

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

# Development

Development is currently done using Qt Creator.

QT Creator allows for simple editing of source code, as well as graphical form editing, and can be
installed in Debian flavours of Linux by installing the `qtcreator` package.

# Versioning

This software uses Semantic Versioning 2.0.0 (http://semver.org/) as of release v2.0.0.

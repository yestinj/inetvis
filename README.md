# Internet Visualizer

Source repository for the Internet Visualizer (inetvis) tool.

This project is based off of the original work by JP van Riel and Barry Irwin of Rhodes University. Their original work can be found here: www.cs.ru.ac.za/research/g02v2468/inetvis.html

# Running InetVis

At this time InetVis only works under 32-bit flavours of Ubuntu and requires a few older libraries to be installed for correct operation.

In order to get InetVis running in your own VM please consider the following guidelines:

1. Install your choice of 32-bit Linux in a VM (Tested with Ubuntu 14.04.5 LTS 32-bit desktop edition)
  * You may choose to install the i386 architecture under your current 64-bit distrubition, however this has not been tested.
2. Be sure to update your system fully after installation
  * `sudo apt-get update` and `sudo apt-get upgrade`, finally `sudo apt-get dist-upgrade`
3. Install all of the basic requirements as mentioned in the manual (doc/inetvisdoc.html)
  * `sudo apt-get install build-essential g++ libc6 libstdc++6 libc6-dev make libpcap-dev libgl1-mesa-dev libqt4-dev libqt4-dev qt4-dev-tools libqt4-qt3support`
  * Note: Some of the above dependencies are only required for building and not running, but I haven't sorted out which is which yet. To be safe rather install them all.
4. Install the older 'missing' libraries which aren't there by default
  1. `sudo apt-get install libpq5` - libpg.so.5
  2. libmysqlclient_r.so.15
    1. The version we need is no longer in the Ubuntu repos, and needs to be downloaded and installed manually.
    2. `sudo apt-get install mysql-common`
    3. `wget -O libmysqlclient15off_5.1.30really5.0.75-0ubuntu10.5_i386.deb http://automation.binarysage.net/\?dl_id\=6`
    4. `dpkg -i libmysqlclient15off_5.1.30really5.0.75-0ubuntu10.5_i386.deb`
  3. `sudo apt-get install libsqlite0` - libsqlite.so.0
5. That should be it!

To run InetVis either download the latest binary from the Releases page, or clone the source code and run the `inetvis` binary from the root of the repository.

`./inetvis`

If you would like to monitor your own host for network events, instead of replaying capture files, please ensure that you use `sudo`:

`sudo ./inetvis`

Have fun! :-)

# Building InetVis

Building InetVis is relatively straight forward but comes with the same caveats as running it. Currently only 32-bit flavours of Linux, Ubuntu, are supported.

Before getting started please follow all of the instructions in **Running InetVis** to ensure that you have all of the relevant dependencies installed.

Next you either need to set the environment variable `QT_SELECT=qt4`, wherever you prefe, or you need to run the `qmake` command below with the `-qt=qt4` option. This ensures that the project is built using Qt4, and not Qt5 which will likely be the default. 

Once your system is up and running do the following to build InetVis:

1. `cd src`
2. `qmake`
3. `make`

This should result in a new `inetvis` binary being generated within the source directory.


# Notes

This software will make use of Semantic Versioning 2.0.0 (http://semver.org/) once reaching the v1.0.0 release phase. 

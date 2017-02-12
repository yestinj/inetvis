InetVis 0.9.5 Readme
===================

This file has brief complie instructions for InetVis. Refere to the InetVis html documentation (inetvisdoc.html) for usage notes.



Build dependancies
------------------

To complile InetVis on a Linux platform, make sure the following dependancies are met:

* essential build utilities like:
  * g++
  * libc-dev
  * libstdc++-dev
  * make
* libpcap-dev (old versions may cause compile errors)
* libgl1-mesa-dev
* libqt4-dev
* qt4-dev-tools
* qt4-qt3support

Note,the package names above are for a typical Debian/Ubuntu system may differ for your distribution. 


Complile instructions
---------------------

For linux, it's assumed the devlopment environment for qt4, libpcap and OpenGL is setup. Similarly on Windows, the environment for MinGW, Qt4, and WinPCap must be setup. Note that InetVis was ported from Qt3 to Qt4. See the InetVis manual for more build notes.

To complie inetvis, run:
  qmake
  make

(On debain, be sure to use qmake-qt4, as qmake is a link to qmake-qt3)

'qmake' builds qt related files and generates the Makefile. 'make' then performs the compliation. Note, no automated system install has been configured, so there no point in issuing the command 'make install'. InetVis will simply run as is from the directory it is complied in. Read below for a note on the documentation and help dialog.


Documentation
-------------

The documentation is not included in the source directory. However the InetVis help dialog links to the html documentation and expects it be present at the following location 
  doc/intevisdoc.html
relative to working directory (i.e. the directory the inetvis binary is executed from).


License
-------

InetVis is provided as is and licensed under the GPL v2 (InetVis_GPL2_License.txt should be include with this source package).

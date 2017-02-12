InetVis 0.93 Readme
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
* libqt3-mt-dev
* qt3-dev-tools

Note,the package names above are for a typical Debian/Ubuntu system may differ for your distribution. 


Complile instructions
---------------------

To complie inetvis, run:
  qmake
  make

'qmake' builds qt related files and generates the Makefile. 'make' then performs the compliation. Note, no automated system install has been configured, so there no point in issuing the command 'make install'. InetVis will simply run as is from the directory it is complied in. Read below for a note on the documentation and help dialog.


Documentation
-------------

The documentation is not included in the source directory. However the InetVis help dialog links to the html documentation and expects it be present at the following location 
  doc/intevisdoc.html
relative to working directory (i.e. the directory the inetvis binary is executed from).


License
-------

InetVis is provided as is and licensed under the GPL v2 (InetVis_GPL2_License.txt should be include with this source package).


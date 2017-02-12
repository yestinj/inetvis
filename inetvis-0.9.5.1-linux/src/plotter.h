/*******************************************************************************

InetVis - Internet Visualisation

version: 0.9.5
release date: 2007/11/21

Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7

author: Jean-Pierre van Riel
supervisor: Barry Irwin

License
-------

InetVis - Internet Visaulisation for network traffic.
Copyright (C) 2006, Jean-Pierre van Riel

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA.

*******************************************************************************/

/* plotter.h & plotter.cpp
 *
 * A class of static methods to perform mapping functions onto a set of
 * predefined x, y, and z axis. Also includes colour mapping functions.
 * It is a utility class which other objects can use to perform plotting and
 * colour mapping.
 *
 */

/* REVISION history

2007-11-08: change colour scheme and mapping to be based on protocols instead of 
   global scheme effecting all protocols - this has ramifications for the UI and
   the background colour scheme selection. It also has ramifications for the 
   DataProcessor class and setColourScheme function

*/



#include <iostream>
#include <math.h>
#include <packetevent.h>


// DEGBUG DIRECTIVES ----------------------------------------------------------
//uncomment to enabl debug
//#define DEBUG_PLOTTER
//#define DEBUG_LOG_PLOT
//#define DEBUG_COL_LOG_MAP


// DEFINITIONS ----------------------------------------------------------------

#define DEFAULT_DRAWING_FIELD_SIZE 100.0
   //defines a cubic region of 100.0 x 100.0 x 100.0
   //the plotting and axes should be scaled to filt into this drawing field
   //this value would idealy corrospond to AXIS_LENGTH in glviswidget.h
#define MAX_U_INT32_VAL 4294967295
#define MAX_U_INT16_VAL 65535
#define DEFAULT_LOG_RANGE_BASE 100.0
//define background colour codes
#ifndef BLACK_BACKGROUND
   #define BLACK_BACKGROUND 0
#endif
#ifndef WHITE_BACKGROUND
   #define WHITE_BACKGROUND 1
#endif

enum yPlotScheme { Y_PLOT_LINEAR_DST_PORT, Y_PLOT_LOGARITHMIC_DST_PORT };
   //options to plot spacially along z axis by desitnation port

//x and y are linearly plotted according to destination and source address
//respectivly

//if colour schemes are changed setColourScheme() in DataProcessor validation 
//needs to be upated

//global/generic colour schemes - some to be implimented
//enum colSchemesGen { COL_MAP_TIME, COL_MAP_PAYLOAD_SIZE };

//old schemes
enum colSchemes { COL_MAP_DST_PORT, COL_LOG_MAP_DST_PORT,
                  COL_MAP_SRC_PORT, COL_MAP_SRC_ADR, COL_MAP_PROTO,
                  COL_MAP_PKT_SIZE, COL_LOG_MAP_PKT_SIZE };

//Global colSchemes text descriptions, corrosponding to enum colSchemes
//*
#define STR_COL_MAP_DST_PORT "Destination port"
#define STR_COL_LOG_MAP_DST_PORT "Log destination port"
#define STR_COL_MAP_SRC_PORT "Source port"
#define STR_COL_MAP_SRC_ADR "Source IP address"
#define STR_COL_MAP_PROTO "Protocol"
#define STR_COL_MAP_PKT_SIZE "Packet size"
#define STR_COL_LOG_MAP_PKT_SIZE "Log packet size"
#define STR_COL_MAP_TIME "Timestamp"
//*/

//per protocol colour schemes - there are three options. The data field used for
//colour mapping, the range of the values in the field, and the colour mapping
//function used.

enum colSchemesIP { COL_IP_SRC_ADR, COL_IP_DST_ADR, COL_IP_PROTO, COL_IP_SIZE,
                    COL_IP_TTL };
   //note choice of IP colour scheme should overwrite other colour schemes
   //protocol specific colour schemes can be set indepenantly of others

enum colSchemesICMP { COL_ICMP_PROTO, COL_ICMP_TYPE, COL_ICMP_TYPE_CODE };

enum colSchemesTCP { COL_TCP_PROTO, COL_TCP_DST_PORT, COL_TCP_SRC_PORT, 
                     COL_TCP_FLAG };

enum colSchemesUDP { COL_UDP_PROTO, COL_UDP_DST_PORT, COL_UDP_SRC_PORT };

enum colMapFunct { COL_MAP_RAINBOW, COL_MAP_LOG_RAINBOW, COL_MAP_HEATMAP, 
                   COL_MAP_LOG_HEATMAP, COL_MAP_CUST };






//CLASS DEFINITION ------------------------------------------------------------


class Plotter
{
   public:

      //grpahical plotting members to hold values
      static float drawingFieldSize;
      //spatial ranges
      static unsigned int xRange[2];
      static unsigned int yRange[2];
      static unsigned int zRange[2];
      //colour ranges
      static unsigned int colRange[2];
      static unsigned int ipColRange[2];
      static unsigned int icmpColRange[2];
      static unsigned int tcpColRange[2];
      static unsigned int udpColRange[2];
      //range lengths to hold precomputed values
      static unsigned int xRangeLength;
      static unsigned int yRangeLength;
      static unsigned int zRangeLength;
      static unsigned int colRangeLength;
      static unsigned int ipColRangeLength;
      static unsigned int icmpColRangeLength;
      static unsigned int tcpColRangeLength;
      static unsigned int udpColRangeLength;
      //log predetermined values
      static float yLogRangeBase; //log range and base for plotting to y
      static float yLogRangeBaseDiv; //the value of log(yLogBaseRange)
      static float colLogRangeBase; //log range and base for mapping colour
      static float colLogRangeBaseDiv; //the value of log(colLogRangeBase)

      //plotting/mapping scheme variables to specifify plotting modes
      //other objects may use these values as a refrence for plotting choices
      static int yPlotScheme; //hold currnet plotting scheme, either linear
            //or logarithmic
      //x and y directions have linear plots
      static int colMapScheme; //hold current generic colour scheme, by:
            //dst port, src port, address, protocol, packet size, time, etc
      //protocol specific colour schemes
      static int ipColMapScheme;
      static int icmpColMapScheme;
      static int tcpColMapScheme; 
      static int udpColMapScheme; 
      static int backgroundCol; //the background colour, either black or white
      static bool transDecay;
            //to specify wether transparent decay is on or off

      //the start of an axis should always be greater than the end
      static void setRanges(unsigned int xStart, unsigned int xEnd,
            unsigned int yStart, unsigned int yEnd,
            unsigned int zStart, unsigned int zEnd,
            unsigned int colStart, unsigned int colEnd,
            float drawFieldSize);
         //once off function to set all range values
      //spatial ranges
      static void setXRange(unsigned int  xStart, unsigned int  xEnd);
      static void setYRange(unsigned int yStart, unsigned int  yEnd);
      static void setZRange(unsigned int  zStart, unsigned int zEnd);
      //colour ranges
      static void setColRange(unsigned int colStart, unsigned int colEnd);
      static void setIpColRange(unsigned int start, unsigned int end);
      static void setIcmpColRange(unsigned int start, unsigned int end);
      static void setTcpColRange(unsigned int start, unsigned int end);
      static void setUdpColRange(unsigned int start, unsigned int end);
      //log base
      static void setYLogRangeBase(float rangeBase);
         //specific log base range function parameter for y plot
      static void setColLogRangeBase(float rangeBase);
         //specific log base range function parameter for colour plot

      static float linearMap(unsigned int  val, const unsigned int range[2]);
         //generic function to calculate a fraction along a range
         //returns the fraction between [0.0, 1.0] along axis between range[0]
         //and range[1]. values outside of [0.0, 1.0] indicate plottings beyond
         //the range which the caller can test for. Double precision is used.
      static float linearXMap(unsigned int xVal);
      static float linearYMap(unsigned int yVal);
      static float linearZMap(unsigned int zVal);
         //direction specific map functions to take advantage of precomputed
         //range values improving performance of many itterations

      static float logarithmicMap(unsigned int  val, const unsigned int range[2],
            float logFactorBase=10.0);
         //plots by the following function:
         //    f(x)=log10(1+Lf*(x-x0)/(x1-x0))/log10(Lf)
         //where x is the port in the viewing range of x0 to x1, and Lf is the
         //logaritmic factor that x values are effectivly linearly mapped to
      static float logarithmicYMap(unsigned int  yVal);
         //uses precomputed values to improve performance

      static void rainbowColourMap(float colour[3], unsigned int val,
            const unsigned int range[2]);
         //returns the fraction between [0.0, 1.0] along axis between range[0]
         //and range[1]. linearly maps the ranbow spectrum from red to violet
         //through to rgb colours, returned using the colour[3] parameter
      static void rainbowColourMap(float colour[3], unsigned int val);
         //performs colour map calculation using precomputed values
      static void logRainbowColMap(float colour[3], unsigned int val);
         //similar to rainbowColourMap, but performs log transform on input
         //values and uses precomputed values to improve performance

      static void plot(PacketEvent &pe);
         //used to plot packet events to thier point coordinates and colour
         //and assumes the caller has set appropriate range values to take
         //advantage of precomputed performance gain over many itterations

private:

      static void HSVtoRGB(float rgb[3], const float hsv[3]);

};

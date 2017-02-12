#include <plotter.h>
//#include <glviswidget.h> //for some #defines

using namespace std;

//intialization of static graphical plotting members
float Plotter::drawingFieldSize = DEFAULT_DRAWING_FIELD_SIZE;
unsigned int Plotter::xRange[2] = {0, MAX_U_INT32_VAL}; //full ip range
unsigned int Plotter::yRange[2] = {0, MAX_U_INT16_VAL}; //full iport range
unsigned int Plotter::zRange[2] = {0, MAX_U_INT32_VAL}; //full ip range
unsigned int Plotter::colRange[2] = {0, MAX_U_INT16_VAL}; //full port range
unsigned int Plotter::xRangeLength = MAX_U_INT32_VAL;
unsigned int Plotter::yRangeLength = MAX_U_INT16_VAL;
unsigned int Plotter::zRangeLength = MAX_U_INT32_VAL;
unsigned int Plotter::colRangeLength = MAX_U_INT16_VAL;
float Plotter::yLogRangeBase = DEFAULT_LOG_RANGE_BASE;  //0 represents log base e
float Plotter::yLogRangeBaseDiv = log(1 + DEFAULT_LOG_RANGE_BASE);
float Plotter::colLogRangeBase = 100.0; //log range and base for mapping colour
float Plotter::colLogRangeBaseDiv = log(1 + DEFAULT_LOG_RANGE_BASE); //the value of log(colLogRangeBase)

//default plotting scheme
int Plotter::yPlotScheme = Y_PLOT_LINEAR_DST_PORT;
int Plotter::colMapScheme = COL_MAP_DST_PORT;
int Plotter::backgroundCol = BLACK_BACKGROUND;
bool Plotter::transDecay = false;

//NOTE: other application components may change these (i.e from a config file
//that saves user preferences

void Plotter::setRanges(unsigned int xStart, unsigned int xEnd,
   unsigned int yStart, unsigned int yEnd,
   unsigned int zStart, unsigned int zEnd,
   unsigned int colStart, unsigned int colEnd,
   float drawFieldSize)
{
   //intialization of graphical plotting members
   drawingFieldSize = drawFieldSize;
   setXRange(xStart, xEnd);
   setYRange(yStart, yEnd);
   setZRange(zStart, zEnd);
   setColRange(colStart, colEnd);

   #ifdef DEBUG_PLOTTER
      cout << "DEBUG: Ranges set x=" << xRange[0] << '-' << xRange[1] << ", y="
            << yRange[0] << '-' << yRange[1] << ", z=" << zRange[0] << '-'
            << zRange[1] << "col=" << colRange[0] << '-' << colRange[1] << endl;
   #endif

}


void Plotter::setXRange(unsigned int xStart, unsigned int xEnd)
{
   Plotter::xRange[0] = xStart;
   Plotter::xRange[1] = xEnd;
   Plotter::xRangeLength = Plotter::xRange[1] - Plotter::xRange[0];

   #ifdef DEBUG_PLOTTER
      cout << "DEBUG: Plot: set range x0=" << (int)xRange[0] << ", x1="
            << (int)xRange[1] << ", rangex=" << xRangeLength << "\n";
   #endif

}


void Plotter::setYRange(unsigned int yStart, unsigned int yEnd)
{
   Plotter::yRange[0] = yStart;
   Plotter::yRange[1] = yEnd;
   Plotter::yRangeLength = Plotter::yRange[1] - Plotter::yRange[0];

   #ifdef DEBUG_PLOTTER
      cout << "DEBUG: Plot: set range y0=" << (int)yRange[0] << ", y1="
            << (int)yRange[1] << ", rangey=" << yRangeLength << "\n";
   #endif

}


void Plotter::setZRange(unsigned int zStart, unsigned int zEnd)
{
   Plotter::zRange[0] = zStart;
   Plotter::zRange[1] = zEnd;
   Plotter::zRangeLength = Plotter::zRange[1] - Plotter::zRange[0];

   #ifdef DEBUG_PLOTTER
      cout << "DEBUG: Plot: set range z0=" << zRange[0] << ", z1="
            << zRange[1] << ", rangez=" << zRangeLength << "\n";
   #endif

}


void Plotter::setColRange(unsigned int colStart, unsigned int colEnd)
{
   Plotter::colRange[0] = colStart;
   Plotter::colRange[1] = colEnd;
   Plotter::colRangeLength = Plotter::colRange[1] - Plotter::colRange[0];

   #ifdef DEBUG_PLOTTER
      cout << "DEBUG: Plot: set range col0=" << colRange[0] << ", col1="
            << colRange[1] << ", rangecol=" << colRangeLength << "\n";
   #endif

}


void Plotter::setYLogRangeBase(float rangeBase)
{
   if (rangeBase >= 0)
   {  yLogRangeBase = rangeBase;
      yLogRangeBaseDiv = log(1 + yLogRangeBase);
   }
   else
      cerr << "ERROR: valid log base >= 0\n";

}


void Plotter::setColLogRangeBase(float rangeBase)
{
   if (rangeBase >= 0)
   {  colLogRangeBase = rangeBase;
      colLogRangeBaseDiv = log(1 + colLogRangeBase);
   }
   else
      cerr << "ERROR: valid log base >= 0\n";

}


float Plotter::linearMap(unsigned int val, const unsigned int range[2])
{

   return ((double)(val - range[0])
         / (double)(range[1] - range[0]))
         * Plotter::drawingFieldSize;

}


float Plotter::linearXMap(unsigned int xVal)
{
   return ((double)(xVal - Plotter::xRange[0])
         / (double)Plotter::xRangeLength)
         * Plotter::drawingFieldSize;

}


float Plotter::linearYMap(unsigned int yVal)
{
   return ((double)(yVal - Plotter::yRange[0])
         / (double)Plotter::yRangeLength)
         * Plotter::drawingFieldSize;

}


float Plotter::linearZMap(unsigned int zVal)
{
   return ((double)(zVal - Plotter::zRange[0])
         / (double)Plotter::zRangeLength)
         * Plotter::drawingFieldSize;

}


float Plotter::logarithmicMap(unsigned int val, const unsigned int range[2],
      float logFactorBase)
{
   //assertion that value is in valid domain: (0,infinity]
   if (val <= 0)
   {  cerr << "ERROR: logarithmicMap(): invalid range/domain for transformed value = "
            << val << '\n';
      return 0.0;
   }

   float result = 0.0;

   //f(x)=log10(1+Lf*(x-x0)/(x1-x0))/log10(Lf)
   //where x is the port in the viewing range of x0 to x1, and Lf is the
   //logaritmic factor that x values are effectivly linearly mapped to
   result = log(1 + logFactorBase*(double)(val-range[0])
         / (double)(range[1]-range[0]))
         / log(1 + logFactorBase)*drawingFieldSize;

   #ifdef DEBUG_LOG_PLOT
      cout << "DEBUG: log plot (base " << logFactorBase << "): val=" << val << "(rng="
            << range[0] << ',' << range[1] << ") ==> " << result << '\n';
   #endif

   return result;

}


float Plotter::logarithmicYMap(unsigned int yVal)
{
   //for efficieny, trust that value is in valid domain: (0,infinity]
   //uses precomputed values, and performs same functiona as logarithmicMap
   //specifically for y axis

   #ifdef DEBUG_LOG_PLOT
      cout << "DEBUG: log plot (base " << yLogRangeBase << "): val="
            << yVal << "(rng="
            << yRange[0] << ',' << yRange[1] << ") ==> "
            << log(1 + yLogRangeBase*(double)(yVal-yRange[0])
                  / (double)yRangeLength)
                  / (double)yLogRangeBaseDiv*drawingFieldSize << '\n';
   #endif

   return log(1 + yLogRangeBase*(double)(yVal-yRange[0])
         / (double)yRangeLength)
         / (double)yLogRangeBaseDiv*drawingFieldSize;

}


void Plotter::rainbowColourMap(float colour[3], unsigned int val,
      const unsigned int range[2])
{
   //uses colour map hue values in hsv to rgb to produce rainbow effect
   //generic function where values are specified

   float hsv[3];
      //360 degrees for h values of hsv, to set colour
      //s = saturation, 1.0 is full colour intensity as apposed to pure white
      //at 0.0
      //v = brightness, where 1 is fully bright, 0 black
   hsv[0] = ((double)(val - range[0]) / (double)(range[1] - range[0])) * 330.0;
   //make subtle contrast with hue and saturation against background
   if (backgroundCol == BLACK_BACKGROUND)
   {  hsv[1] = 0.9; //add slight tinge of whiteness
      hsv[2] = 1.0; //make fully bright
   }
   else //white background
   {  hsv[1] = 1.0; //make full hue
      hsv[2] = 0.9; //decrease brightness slightly
   }
   HSVtoRGB(colour, hsv);

}


void Plotter::rainbowColourMap(float colour[3], unsigned int val)
{
   //uses colour map hue values in hsv to rgb to produce rainbow effect
   //alternate function that uses precomputed values to improve performace
   //over many itterations calling the function

   float hsv[3];
   hsv[0] = ((double)(val - colRange[0]) / (double)colRangeLength) * 330.0; //360 degrees for
      //h values of hsv
   hsv[1] = 1.0; //stauration, 1.0 is colour intensity as apposed to purity
      //(whiteness)
   hsv[2] = 1.0; //brightness, where 1 is fully bright, 0 black
   HSVtoRGB(colour, hsv);

}


void Plotter::logRainbowColMap(float colour[3], unsigned int val)
{
   //similar to rainbowColourMap, but first scales input values by logaritmic
   //transfomation to expand the lower end of the data range and compress
   //the higher end of the range

   //transform input value
   unsigned int transVal = (unsigned int)(log(1 +
                  colLogRangeBase*(double)(val-colRange[0])
                  / (double)colRangeLength)
                  / colLogRangeBaseDiv*(double)colRangeLength);
      //log transform the value within the range of colours

   #ifdef DEBUG_COL_LOG_MAP
      cout << "DEBUG: log colour map (base " << colLogRangeBase << "): val=" << val << "(rng="
            << colRange[0] << ',' << colRange[1] << ") ==> "
            << transVal << '\n';
   #endif

   //use ranbowcolour map on transformed values
   rainbowColourMap(colour, transVal);

}


void Plotter::plot(PacketEvent &pe)
{
   //Apply plotting schemes to PacketEvent objects, setting co-ordinates and
   //colour, given supported protocols (ethernet, IP, TCP, UDP, ICMP)
   //NOTE: for improving efficiency, the caller must set the plotting scheme
   //and appropriate corrosponding ranges

   if (pe.headers.dataLinkType == DL_ETHERNET)
   {
      if (pe.headers.networkType == N_IPv4)
      {
         #ifdef DEBUG_PLOTTER
            cout << "DEBUG: Plot:\n";
         #endif

         struct IP_Header *ip;
         ip = (IP_Header*)(pe.headers.pt_networkHeader);

         //plot IP destination address within ranges along X direction
         pe.point.coord[0] = linearXMap(ip->dstAddress);
         //plot IP source address within ranges along Y direction
         pe.point.coord[2] = linearZMap(ip->srcAddress);

         #ifdef DEBUG_PLOTTER
            cout << "   dst IP adr = "
                  << PacketHeaders::ipAdrToDecDotStr(ip->dstAddress) << " = "
                  << ip->dstAddress << " --> x = " << pe.point.coord[0]
                  << endl;
            cout << "   src IP adr = "
                  << PacketHeaders::ipAdrToDecDotStr(ip->srcAddress) << " = "
                  << ip->srcAddress << " --> z = " << pe.point.coord[2]
                  << endl;
         #endif

         //map colour
         if (colMapScheme == COL_MAP_SRC_ADR)
         {  rainbowColourMap(pe.point.colour, ip->srcAddress);

            #ifdef DEBUG_PLOTTER
               cout << "   src IP adr = " << ip->srcAddress
                     << " --> col (rgb) = " << pe.point.colour[0] << ","
                     << pe.point.colour[1] << ","  << pe.point.colour[2]
                     << endl;
            #endif

         }
         else if (colMapScheme == COL_MAP_PKT_SIZE)
         {  rainbowColourMap(pe.point.colour, ip->len);
               //for ethernet len < 1500

            #ifdef DEBUG_PLOTTER
               cout << "   pkt size = " << ip->len  << " --> col (rgb) = "
                     << pe.point.colour[0] << "," << pe.point.colour[1]
                     << ","  << pe.point.colour[2] << endl;
            #endif

         }
         else if (colMapScheme == COL_LOG_MAP_PKT_SIZE)
         {  logRainbowColMap(pe.point.colour, ip->len);
               //for ethernet len < 1500

            #ifdef DEBUG_PLOTTER
               cout << "   pkt size = " << ip->len  << " --> log col (rgb) = "
                     << pe.point.colour[0] << "," << pe.point.colour[1]
                     << ","  << pe.point.colour[2] << endl;
            #endif

         }

         switch (pe.headers.transportType)
         {  case T_ICMP: //ICMP
               struct ICMP_Header *icmp;
               icmp = (struct ICMP_Header*)(pe.headers.pt_transportHeader);
               //plot to ICMP plane
               pe.point.coord[1] = -20.0;

               #ifdef DEBUG_PLOTTER
                  cout << "   ICMP packet --> y = " << pe.point.coord[1]
                        << endl;
               #endif

               //map colour
               if (colMapScheme == COL_MAP_DST_PORT
                     || colMapScheme == COL_LOG_MAP_DST_PORT
                     || colMapScheme == COL_MAP_SRC_PORT
                     || colMapScheme == COL_MAP_PROTO)
               {  //no port info for ICMP

               //for now ICMP is made white or black depending on background
               //(for now... may addd ICMP type colour schemes)
                  if (backgroundCol == BLACK_BACKGROUND) //make ICMP white
                  {  pe.point.colour[0] = 1.0;
                     pe.point.colour[1] = 1.0;
                     pe.point.colour[2] = 1.0;
                  }
                  else //must be white background - make ICMP black
                  {  pe.point.colour[0] = 0.0;
                     pe.point.colour[1] = 0.0;
                     pe.point.colour[2] = 0.0;
                  }

                  #ifdef DEBUG_PLOTTER
                     cout << "   ICMP proto --> col (rgb) = "
                           << pe.point.colour[0] << "," << pe.point.colour[1]
                           << "," << pe.point.colour[2] << ","
                           << endl;
                  #endif

               }
               break;
            case T_TCP: //TCP
               struct TCP_Header *tcp;
               tcp = (struct TCP_Header*)(pe.headers.pt_transportHeader);
               //plot by dst port
               if (yPlotScheme == Y_PLOT_LINEAR_DST_PORT)
                  pe.point.coord[1] = linearYMap(tcp->dstPort);
               else //must be logarythmic
                  pe.point.coord[1] = logarithmicYMap(tcp->dstPort);

               #ifdef DEBUG_PLOTTER
                  cout << "   TCP dst port = " << tcp->dstPort << " --> y = "
                        << pe.point.coord[1] << endl;
               #endif

               //map colour
               if (colMapScheme == COL_MAP_DST_PORT)
               {  rainbowColourMap(pe.point.colour, tcp->dstPort);

                  #ifdef DEBUG_PLOTTER
                  cout << "   TCP dst port = " << tcp->dstPort
                        << " --> col (rgba) = " << pe.point.colour[0] << ","
                        << pe.point.colour[1] << ","  << pe.point.colour[2]
                        << endl;
                  #endif

               }
               else if (colMapScheme == COL_LOG_MAP_DST_PORT)
               {  logRainbowColMap(pe.point.colour, tcp->dstPort);

                  #ifdef DEBUG_PLOTTER
                  cout << "   TCP dst port = " << tcp->dstPort
                        << " --> log col (rgba) = " << pe.point.colour[0] << ","
                        << pe.point.colour[1] << ","  << pe.point.colour[2]
                        << endl;
                  #endif

               }
               else if (colMapScheme == COL_MAP_SRC_PORT)
               {  rainbowColourMap(pe.point.colour, tcp->srcPort);

                  #ifdef DEBUG_PLOTTER
                  cout << "   TCP src port = " << tcp->srcPort
                        << " --> col (rgba) = " << pe.point.colour[0] << ","
                        << pe.point.colour[1] << ","  << pe.point.colour[2]
                        << endl;
                  #endif

               }
               else if (colMapScheme == COL_MAP_PROTO)
               {
                  //yellow
                  if (backgroundCol == BLACK_BACKGROUND)
                  {  //with slight white tinge (hsv = 60/360,0.9,1.0) -> rgb
                     pe.point.colour[0] = 1.0;
                     pe.point.colour[1] = 1.0;
                     pe.point.colour[2] = 0.09765625;
                  }
                  else //white backgorund
                  {  //slight dimmed (hsv = 0.6,1.0,0.9) -> rgb
                     pe.point.colour[0] = 0.8984375;
                     pe.point.colour[1] = 0.8984375;
                     pe.point.colour[2] = 1.0;
                  }

                  #ifdef DEBUG_PLOTTER
                  cout << "   TCP proto = " << tcp->srcPort
                        << " --> col (rgba) = " << pe.point.colour[0] << ","
                        << pe.point.colour[1] << ","  << pe.point.colour[2]
                        << endl;
                  #endif

               }
               break;
            case T_UDP: //UDP
               struct UDP_Header *udp;
               udp = (struct UDP_Header*)(pe.headers.pt_transportHeader);
               //plot by dst port
               if (yPlotScheme == Y_PLOT_LINEAR_DST_PORT)
                  pe.point.coord[1] = linearYMap(udp->dstPort);
               else //must be logarythmic
                  pe.point.coord[1] = logarithmicYMap(udp->dstPort);

               #ifdef DEBUG_PLOTTER
                  cout << "UDP dst port = " << udp->dstPort << " --> y = "
                        << pe.point.coord[1] << endl;
               #endif

               //map colour
               pe.point.colour[3] = 1.0; //fully opacit
               if (colMapScheme == COL_MAP_DST_PORT)
               {  rainbowColourMap(pe.point.colour, udp->dstPort);

                  #ifdef DEBUG_PLOTTER
                  cout << "   UDP dst port = " << udp->dstPort
                        << " --> col (rgba) = " << pe.point.colour[0] << ","
                        << pe.point.colour[1] << ","  << pe.point.colour[2]
                        << "," << pe.point.colour[3] << endl;
                  #endif

               }
               else if (colMapScheme == COL_LOG_MAP_DST_PORT)
               {  logRainbowColMap(pe.point.colour, udp->dstPort);

                  #ifdef DEBUG_PLOTTER
                  cout << "   UDP dst port = " << udp->dstPort
                        << " --> log col (rgba) = " << pe.point.colour[0]
                        << "," << pe.point.colour[1] << ","
                        << pe.point.colour[2] << endl;
                  #endif

               }
               else if (colMapScheme == COL_MAP_SRC_PORT)
               {  rainbowColourMap(pe.point.colour, udp->srcPort);

                  #ifdef DEBUG_PLOTTER
                  cout << "   UDP src port = " << udp->srcPort
                        << " --> col (rgba) = " << pe.point.colour[0] << ","
                        << pe.point.colour[1] << ","  << pe.point.colour[2]
                        << "," << pe.point.colour[3] << endl;
                  #endif

               }
               else if (colMapScheme == COL_MAP_PROTO)
               {  //bright pink
                  if (backgroundCol == BLACK_BACKGROUND)
                  {  //with slight white tinge (hsv = 300/360,0.8,1.0) -> rgb
                     pe.point.colour[0] = 1.0;
                     pe.point.colour[1] = 0.19921875;
                     pe.point.colour[2] = 1.0;
                  }
                  else //white backgorund
                  {  //slighty dimmer (hsv = 300/360,0.9,0.9) -> rgb
                     pe.point.colour[0] = 0.8984375;
                     pe.point.colour[1] = 0.1796875;
                     pe.point.colour[2] = 0.8984375;
                  }


                  #ifdef DEBUG_PLOTTER
                  cout << "   UDP proto = " << udp->srcPort
                        << " --> col (rgba) = " << pe.point.colour[0] << ","
                        << pe.point.colour[1] << ","  << pe.point.colour[2]
                        << endl;
                  #endif

               }
               break;

            #ifdef DEBUG_PLOTTER
            default:
               cout << "   unknown protocol (neither TCP, ICMP, or UDP)\n"
                    << "   transport type code = " << pe.headers.transportType
                     << endl;
            #endif

         }

      }

   }

}


void Plotter::HSVtoRGB(float rgb[3], const float hsv[3])
{
   //hsv to rgb colour mapping algorythm referenced to:
   //http://www.cs.rit.edu/~ncs/color/t_convert.html#RGB%20to%20HSV%20&%20HSV%20to%20RGB

   int i;
   float h, f, p, q, t;

   h = hsv[0];

   if (h == 0) //if (s == 0)
   {  // achromatic (grey)
      rgb[0] = rgb[1] = rgb[2] = hsv[2];
      return;
   }

   h /= 60;       // sector 0 to 5
   i = (int)(floor(h));
   f = h - i; // factorial part of h
   p = hsv[2] * ( 1 - hsv[1] ); //p = v * (1 -s)
   q = hsv[2] * ( 1 - hsv[1] * f ); //q = v * ( 1 - s * f )
   t = hsv[2] * ( 1 - hsv[1] * ( 1 - f ) ); //t = v * ( 1 - s * ( 1 - f ) )

   switch (i)
   {  case 0:
         rgb[0] = hsv[2]; //r = v
         rgb[1] = t; //g = t
         rgb[2] = p; //b = p
         break;
      case 1:
         rgb[0] = q; //r = q
         rgb[1] = hsv[2]; //g = v
         rgb[2] = p; //b = p
         break;
      case 2:
         rgb[0] = p; //r = p
         rgb[1] = hsv[2]; //g = v
         rgb[2] = t; //b = t
         break;
      case 3:
         rgb[0] = p; //r = p
         rgb[1] = q; //g = q
         rgb[2] = hsv[2]; //b = v
         break;
      case 4:
         rgb[0] = t; //r = t
         rgb[1] = p; //g = p
         rgb[2] = hsv[2]; //b = v
         break;
      default:    // case 5:
         rgb[0] = hsv[2]; //r = v
         rgb[1] = p; //g = p
         rgb[2] = q; //b = q
         break;
   }

}

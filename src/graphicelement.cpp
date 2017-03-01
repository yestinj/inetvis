#include "graphicelement.h"


GraphicElement::GraphicElement()
{
   //set graphic element type to undefined
   type = GE_UNDEFINED;

}


GraphicElement::GraphicElement(int t)
{
   //set graphic element type
   switch (t)
   {  case GE_POINT:
         type = GE_POINT;
      case GE_LINE:
         type = GE_LINE;
      case  GE_CUBE:
         type = GE_CUBE;
      default: //if no match found
         type = GE_UNDEFINED;
   }

}


Point::Point()
{
   //set graphic element type
   type = GE_POINT;

   //default to origin
   coord[0] = 0.0;
   coord[1] = 0.0;
   coord[2] = 0.0;
   //default to white, fully opauge
   colour[0] = 0.0;
   colour[1] = 0.0;
   colour[2] = 0.0;
   colour[3] = 1.0;

}


Point::Point(float array_coord[3], float array_col[4])
{
   //set graphic element type
   type = GE_POINT;

   for (int i = 0; i < 3; i++)
      coord[i] = array_coord[i];
   for (int i = 0; i < 4; i++)
      coord[i] = array_col[i];

}


Point::Point(float x, float y, float z, float r, float g, float b, float a)
{
   //set graphic element type
   type = GE_POINT;

   coord[0] = x;
   coord[1] = y;
   coord[2] = z;
   colour[0] = r;
   colour[1] = g;
   colour[2] = b;
   colour[3] = a;

}






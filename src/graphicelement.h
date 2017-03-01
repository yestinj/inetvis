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


/* graphicelement.h & graphicelement.cpp
 *
 * A hirarchical group of objects to represent graphical objects
 *
 */


enum grapElemTypes {GE_UNDEFINED, GE_POINT, GE_LINE, GE_CUBE};


class GraphicElement
{
   public:

      int type;

      GraphicElement(); //undefined type
      GraphicElement(int t); //set to type

};


class Point : public GraphicElement
{
   public:

      float coord[3]; //xyz
      float colour[4]; //rgba

      Point();
      Point(float array_coord[3], float array_col[4]);
      Point(float x, float y, float z, float r, float g, float b, float a);

};


class Line : public GraphicElement
{
   public:

      float coord1[3]; //xyz
      float coord2[3]; //rgba
      float colour[4]; //rgba

};


class Cube : public GraphicElement
{
   public:

      float coord[3]; //xyz
      float colour[4]; //rgba
      float size;

};




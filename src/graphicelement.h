/*******************************************************************
InetVis - Internet Visualisation
Version: 2.1.0
release date: 2017/09/21

Original Authors: Jean-Pierre van Riel, Barry Irwin
Initvis 2.x Authors: Yestin Johnson, Barry Irwin
Rhodes University
Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7
Computer Science Masters Project - 2017
author: Jean-Pierre van Riel
supervisor: Barry Irwin

InetVis - Internet Visualisation for network traffic.
Copyright (C) 2006-2017, Jean-Pierre van Riel, Barry Irwin, Yestin Johnson

*******************************************************************/


/* graphicelement.h & graphicelement.cpp
 *
 * A hirarchical group of objects to represent graphical objects
 *
 */

#ifndef GRAPHICELEMENT_H
#define GRAPHICELEMENT_H

enum grapElemTypes {GE_UNDEFINED, GE_POINT, GE_LINE, GE_CUBE};


class GraphicElement {
public:

    int type;

    GraphicElement(); //undefined type
    GraphicElement(int t); //set to type

};


class Point : public GraphicElement {
public:

    float coord[3]; //xyz
    float colour[4]; //rgba

    Point();
    Point(float array_coord[3], float array_col[4]);
    Point(float x, float y, float z, float r, float g, float b, float a);

};


class Line : public GraphicElement {
public:

    float coord1[3]; //xyz
    float coord2[3]; //rgba
    float colour[4]; //rgba

};


class Cube : public GraphicElement {
public:

    float coord[3]; //xyz
    float colour[4]; //rgba
    float size;

};

#endif // GRAPHICELEMENT_H

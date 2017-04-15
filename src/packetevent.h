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

/* packetevent.h & packetevent.cpp
 *
 * An encapsulate object to represent a packet event occurence, and a link to
 * its graphical metephore. This class leverages off the functionality provided
 * by PacketHeaders (packetheaders.h) and GraphicElement (graphicelement.h)
 *
 */

#ifndef PACKETEVENT_H
#define PACKETEVENT_H

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <pcap.h>
#include <packetheaders.h>

#ifndef CAPTURE_LENGTH
#  define CAPTURE_LENGTH 68
#endif


// DEGBUG DIRECTIVES ----------------------------------------------------------
//uncomment for debugging info
//#define DEBUG_PACKET_EVENT
//#define DEBUG_RAW_PACKET_DATA


//CLASS DEFINITION ------------------------------------------------------------


class PacketEvent
{

public:

    //packet data
    unsigned char rawPacket[CAPTURE_LENGTH + 1];
    //to store the actual packet data in a raw string format
    unsigned int captureLength; //length of capture in bytes
    unsigned int packetLength; //length of packet off wire
    struct OSI_Packet headers; //collection of packet header pointers
    struct timeval timeStamp; //time of observation of packet

    //visual metephor respresenting the packet
    Point point;
    GraphicElement *graphicElement; //to point to data for the visual

    PacketEvent();
    PacketEvent(const pcap_pkthdr *pcapHeader, const unsigned char* snapshot);
    ~PacketEvent();

};
#endif // PACKETEVENT_H

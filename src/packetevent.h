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

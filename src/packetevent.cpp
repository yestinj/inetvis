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

#include "packetevent.h"
#include <qdatetime.h>
#include "timeutil.h"


using namespace std;


//Packet Event -----------------------------------------------------------------


PacketEvent::PacketEvent()
{

    //zero timestamp, equivelent to 00h00 Jan 1970
    timeStamp.tv_sec = 0;
    timeStamp.tv_usec = 0;

    //set empty string
    captureLength = 0;
    //rawPacket = new unsigned char[captureLength + 1];
    rawPacket[0] = '\0'; //set to empty string

    //assign header struct
    headers.dataLinkType = DL_UNKNOWN;
    headers.pt_dataLinkHeader = NULL;
    headers.networkType = N_UNKNOWN;
    headers.pt_networkHeader = NULL;
    headers.transportType = T_UNKNOWN;
    headers.pt_transportHeader = NULL;
    headers.sessionType = S_UNKNOWN;
    headers.pt_sessionHeader = NULL;
    headers.data = &rawPacket[0];

    //graphicElement
    graphicElement = NULL;

}


PacketEvent::PacketEvent(const pcap_pkthdr *pcapHeader, const unsigned char* snapshot)
{
    timeStamp = pcapHeader->ts;
    captureLength = (unsigned int)pcapHeader->caplen;
    packetLength = (unsigned int)pcapHeader->len;

    if (captureLength >= CAPTURE_LENGTH)
    {  captureLength = CAPTURE_LENGTH; //set new shortened capture length
    }

    //copy raw packet data - up to maxIndex
    for (unsigned int i = 0; i < captureLength; i++)
    {  rawPacket[i] = snapshot[i];
    }
    rawPacket[captureLength + 1] = '\0'; //terminate with null;

    //cast to packet header structs
    PacketHeaders::parsePacket(headers, rawPacket, captureLength);
    //at this stage only points are used (and hardcoded)

    //set graphicElement as point
    graphicElement = &point;
    //graphicElement->type = GE_POINT; //no need to set as yet, feature not used

#ifdef DEBUG_PACKET_EVENT
    QDateTime dt;
    TimeUtil::timevalToQDateTime(dt, timeStamp);
    //report packet info
    cout << "DEBUG: PacketEvent created: timestamp = " << dt.toString()
         << " (" << timeStamp.tv_sec << " sec " << timeStamp.tv_usec
         << " usec)" << ", length = " << captureLength << endl;
#ifdef DEBUG_RAW_PACKET_DATA
    //print the raw packet
#define LINE_LEN 16
    cerr << "DEBUG: PacketEvent raw snapshot copy:\n";
    printf("\t"); //tab indent
    for (int i = 1; i <= captureLength + 1; i++)
    {  printf("%.2x ", rawPacket[i-1]);
        if ((i % LINE_LEN) == 0) //new line
            printf("\n\t");
    }
    cout << endl;
#endif
    PacketHeaders::coutHeaders(headers);
    cout << "mapped to: x=" << point.coord[0] << ", y=" << point.coord[1] << ", z="
         << point.coord[2] << endl;
    cout << endl << endl;
#endif

}


PacketEvent::~PacketEvent()
{

}






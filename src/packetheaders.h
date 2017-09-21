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


/* packetheaders.h
 *
 * This class is used for describing packet headers in terms of structures. It
 * The class and underlying structures is (initially) intended for ethernet
 * IP packets of one of three types, ICMP, TCP, or UDP. In terms of capturing
 * packets for use with this class, the implicit minimal BPF filter expression
 * is "ip and (icmp or tcp or udp)". Anything other packets will not be handled.
 * The class and structures are however designed for future extentions to cover
 * more protocols.
 *
 * This class is provides a set of appropriate operations/procedures for casting
 * and parsing raw binary packet info and for pefroming querying and textual
 * output. The parsing function uses the plotter class to perform plotting and
 * and colour mapping on the fly as it parses
 *
 * Packet structures are modeled on the posix include/netinet, with a an
 * abstract OSI network stack packet structure to encapsulate the varied
 * protocols at diffrent layers
 *
 * One consequnece of avoiding platform dependant typedef types is that the
 * underlying systems implimentation of primatives is assumed to be:
 * - unsigned char = 1 byte
 * - unsigned short = 2 bytes
 * - unsigned int = 4 bytes
 * This can be checked at runtime with the function testDataPrimativeSizes()
 *
 * A note about byte order: Intel and AMD based processors are little endian,
 * but network byte order is big endian, so by default we can expect to need
 * to make the appropriate conversions. On machines that use big endian which
 * the conversion would be unessasary, so the conversion function implimented
 * by the host OS would presumably do nothing and not require much overhead
 *
 */


/* REVISION history

2007-11-08: 
   * deal with IP options and drop frag packets
   * fix deparse method that failed to reswitch byte order

*/

#ifndef PACKETHEADERS_H
#define PACKETHEADERS_H

#include <stdio.h>

#include <iostream>
#include <qstring.h>
#include <graphicelement.h>
//#include <plotter.h>


//platform specific headers

#ifdef LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef WIN
#include <Winsock2.h>
#endif


using namespace std;


//IP masks
#define INT_MASK_CLASS_A 0xFF000000
#define INT_MASK_CLASS_B 0x00FF0000
#define INT_MASK_CLASS_C 0x0000FF00
#define INT_MASK_CLASS_D 0x000000FF
//quick reference to int representations of IP address masks

#define MAX_ETHERNET_PACKET_SIZE 1500
#define MIN_ETHERNET_PACKET_SIZE 20
//IP header 20 bytes,

using namespace std;



//Packet types classified according to the OSI network stack
//----------------------------------------------------------


//data link layer (layer 2) types prefixed by DL_
//type codes corrospond to those returned by libpcap
enum DataLinkTypes {DL_UNKNOWN =  65535,
                    DL_ETHERNET = 1
                   };

//network layer (layer 3) types prefixed by N_ and enumerated to match ethernet
//type codes in ehternet headers
enum NetworkTypes {N_UNKNOWN =  0xFFFF,   //65535
                   N_IPv4 = 0x0800,       //2048
                   N_IPv6 = 0x86DD        //34525
                  };

//transport layer (layer 4) types prefixed by T_ and enumerated to match IP
//protocol field codes
enum TransportTypes {T_UNKNOWN =  0xFFFF,    //65535
                     T_ICMP = 0x01,          //1
                     T_TCP = 0x06,           //6
                     T_UDP = 0x11,           //17
                    };
//technically ICMP is a network layer type wrapped by IP, but is placed in the
//next level for simplification of the parsing process


//session layer (layer 4) types prefixed by S_
enum SessionTypes {S_UNKNOWN = 65525};


//Packet abstraction according to OSI layer
//-----------------------------------------

struct OSI_Packet
{
    //protocol identification tags
    unsigned short dataLinkType;
    unsigned short networkType;
    unsigned short transportType;
    unsigned short sessionType;
    //could be extended for futher OSI layers

    //pointers to OSI level headers
    void *pt_dataLinkHeader;
    void *pt_networkHeader;
    void *pt_transportHeader;
    void *pt_sessionHeader;
    //could be extended for futher OSI layers

    unsigned char *data;
    //packet payload beyond the headers

};


//Protocol Specific header structures
//-----------------------------------

//POSIX loke packet data structures (found in include/netinet on unix systems)


//Ethernet header
struct EthernetHeader
{
#define ETHER_ADDR_LEN 6

    unsigned char dstHost[ETHER_ADDR_LEN]; //Destination host address
    unsigned char srcHost[ETHER_ADDR_LEN]; //Source host address
    unsigned short type; //IP? ARP? RARP? etc

};


//IP header
struct IP_Header
{
#define IP_RF 0x8000 //reserved fragment flag
#define IP_DF 0x4000 //dont fragment flag
#define IP_MF 0x2000 //more fragments flag
#define IP_OFFMASK 0x1fff //mask for fragmenting bits

    unsigned char vhl; //version << 4 | header length >> 2
    unsigned char tos; //type of service
    unsigned short len; //total length
    unsigned short id; //identification
    unsigned short fragOffset; //fragment offset field
    unsigned char ttl; //time to live
    unsigned char protocol; //protocol
    unsigned short checksum; //checksum
    unsigned int srcAddress; //source address
    unsigned int dstAddress; //dest address

    //note, this has no field for the uncommon incidence of IP options

};


struct ICMP_Header
{
#define ICMP_ECHOREPLY        0  //Echo Reply
#define ICMP_DEST_UNREACH     3  //Destination Unreachable
#define ICMP_SOURCE_QUENCH    4  ///Source Quench
#define ICMP_REDIRECT         5  //Redirect (change route)
#define ICMP_ECHO             8  //Echo Request
#define ICMP_TIME_EXCEEDED    11 //Time Exceeded
#define ICMP_PARAMETERPROB    12 //Parameter Problem
#define ICMP_TIMESTAMP        13 //Timestamp Request
#define ICMP_TIMESTAMPREPLY   14 //Timestamp Reply
#define ICMP_INFO_REQUEST     15 //Information Request
#define ICMP_INFO_REPLY       16 //Information Reply
#define ICMP_ADDRESS          17 //Address Mask Request
#define ICMP_ADDRESSREPLY     18 //Address Mask Reply
#define NR_ICMP_TYPES         18

    //Codes for UNREACHABLE
#define ICMP_NET_UNREACH      0  //Network Unreachable
#define ICMP_HOST_UNREACH     1  //Host Unreachable
#define ICMP_PROT_UNREACH     2  //Protocol Unreachable
#define ICMP_PORT_UNREACH     3  //Port Unreachable
#define ICMP_FRAG_NEEDED      4  //Fragmentation Needed/DF set
#define ICMP_SR_FAILED        5  //Source Route failed
#define ICMP_NET_UNKNOWN      6
#define ICMP_HOST_UNKNOWN     7
#define ICMP_HOST_ISOLATED    8
#define ICMP_NET_ANO          9
#define ICMP_HOST_ANO         10
#define ICMP_NET_UNR_TOS      11
#define ICMP_HOST_UNR_TOS     12
#define ICMP_PKT_FILTERED     13 //Packet filtered
#define ICMP_PREC_VIOLATION   14 //Precedence violation
#define ICMP_PREC_CUTOFF      15 //Precedence cut off
#define NR_ICMP_UNREACH       15 //instead of hardcoding immediate value

    //Codes for REDIRECT
#define ICMP_REDIR_NET        0  //Redirect Net
#define ICMP_REDIR_HOST       1  //Redirect Host
#define ICMP_REDIR_NETTOS     2  //Redirect Net for TOS
#define ICMP_REDIR_HOSTTOS    3  //Redirect Host for TOS

    //Codes for TIME_EXCEEDED
#define ICMP_EXC_TTL          0  //TTL count exceeded
#define ICMP_EXC_FRAGTIME     1  //Fragment Reass time exceeded

    unsigned char type; //message type
    unsigned char code; //type sub-code
    unsigned short checksum;

    //the data field is commented out for now as it not used and throws off the
    //the sizeof function which is meant to return the minimum of 8 bytes for
    //a mimimal icmp header
    /* char *data; */

    union
    {
        struct
        {
            unsigned short   id;
            unsigned short   sequence;
        } echo; //echo datagram

        unsigned int gateway; //gateway address

        struct
        {
            unsigned short   unused;
            unsigned short   mtu;
        } fragment; //path mtu discovery

    } un;

};


//TCP header
struct TCP_Header
{
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PUSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20
#define TCP_ECE 0x40
#define TCP_CWR 0x80
#define TCP_FLAGS (TCP_FIN|TCP_SYN|TCP_RST|TCP_ACK|TCP_URG|TCP_ECE|TCP_CWR)

    unsigned short srcPort; //source port
    unsigned short dstPort; //destination port
    unsigned int seqNumber; //sequence number
    unsigned int ackNumber; //acknowledgement number
    unsigned char dataOffset; //data offset, rsvd
    unsigned char flags;
    unsigned short window; //window
    unsigned short checksum; //checksum
    unsigned short urgentPointer; //urgent pointer

};


//UDP header
struct UDP_Header
{
    unsigned short srcPort; //source port */
    unsigned short dstPort; //destination port */
    unsigned short length;  //udp length */
    unsigned short checksum; //udp checksum */

};


class PacketHeaders
{

private:

    //values to be set by caller before proper operation of class
    static int dataLinkType;
    static int byteOrder; //BIG_ENDIAN = 0, LITTLE_INDIAN = 1
    //can avoid unessary calling to system functions in the one case

public:

    //size of header pointers for use in pointer arithmetic
    static const int sizeOSI_Headers = sizeof(struct OSI_Packet);
    static const int sizeEthernet = sizeof(struct EthernetHeader);
    static const int sizeIP = sizeof(IP_Header);
    static const int sizeICMP = sizeof(struct ICMP_Header);
    static const int sizeTCP = sizeof(struct TCP_Header);
    static const int sizeUDP = sizeof(struct UDP_Header);

    static bool testDataPrimativeSizes();
    //returns true if char, short and int are 1, 2 and 4 bytes respectivly
    //else returns false

    static void setDataLinkType(int type);
    //calling programs responsibilty to set the data link type

    static void parsePacket(struct OSI_Packet &packet,
                            unsigned char *rawPacket,
                            int length);
    //parses a raw packet char array for header information upto a given
    //length (68 bytes) and casts the packets into header structures for
    //an efficient mechanism to index byte data.
    // - The data link protocol type should be set with setDataLinkType
    //   before using this funtion.
    // - If byte order is to be determined at runtime, the
    //   isLittleEndianByteOrder() function should be called first and the
    //   appropriate byte order set to either SYS_BIG_ENDIAN or
    //   SYS_LITTLE_ENDIAN
    // - The testDataPrimitiveSizes() function should be called to determine
    //   if the underlying platform has the assumed data type bit sizes
    // - It is the callers responsibility to ensure that header captures
    //   are long enough and not corrupt - for efficiency reasons this is
    //   not checked
    // - As a performance enhancer, the parser can use the plotter utility
    //   class to setup plotting and colour mapping on the fly as it
    //   traveres thought protocol data (otherwise the plotter class would
    //   have to repeat this traversal). One consequence is that the
    //   plotter utility must be intialised before parsing. Uncomment the
    //   PLOT_ON_THE_FLY preprossor defintion to enable plotting on the
    //   fly. (Otherwise the plotter function can perform plottin upon
    //   parsed data.)

    static void deParsePacket(struct OSI_Packet &packet);
    //reverse the nework to host order byte conversions done. This is need
    //when recording events back from the parsed data

    static QString ipAdrToDecDotStr(unsigned int ipAdr);

    static void coutHeaders(struct OSI_Packet &packet);


};

#endif // PACKETHEADERS_H

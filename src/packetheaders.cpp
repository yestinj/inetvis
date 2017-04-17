#include "packetheaders.h"

#ifndef NULL
#define NULL 0
#endif

#define MAX_DEC_DOT_LENGTH 16

//intial values of static member
int PacketHeaders::dataLinkType = DL_UNKNOWN;

bool PacketHeaders::testDataPrimativeSizes()
{
    //ensure the correct sizes of primitives
    return (sizeof(unsigned char) == 1
            && sizeof(unsigned short) == 2
            && sizeof(unsigned int) == 4);

}


void PacketHeaders::parsePacket(struct OSI_Packet &packet,
                                unsigned char *rawPacket,
                                int length)
{
    //init to unkown state
    packet.pt_dataLinkHeader = NULL;
    packet.dataLinkType = DL_UNKNOWN;
    packet.pt_networkHeader = NULL;
    packet.networkType = N_UNKNOWN;
    packet.pt_transportHeader = NULL;
    packet.transportType = T_UNKNOWN;
    packet.pt_sessionHeader = NULL;
    packet.sessionType = S_UNKNOWN;

    //just reference start of raw packet data
    packet.data = rawPacket;

    //assume an ethernet header; if not, set to NULL and DL_UNKNOWN; future
    //extentions may want to cater for more data link types
    if ((PacketHeaders::dataLinkType == DL_ETHERNET) && (length >= sizeEthernet))
    {  //note we reley on the caller that the capture was big enough before
        //casting otherwise the cast will result in pointers refrencing invalid
        //memory

        //cast to ethernet struct
        packet.dataLinkType = DL_ETHERNET;
        packet.pt_dataLinkHeader = (struct EthernetHeader*)(rawPacket);
        packet.data += sizeEthernet; //point to end of IP header

        //based on protocol code in ethernet, cast next encapsulated header
        struct EthernetHeader *eth;
        eth = (struct EthernetHeader*)packet.pt_dataLinkHeader;
        //convert between network byte order and machine byte order
        eth->type = ntohs(eth->type);

        //If IP, parse IP header
        if ((eth->type == N_IPv4) && (length >= sizeEthernet + sizeIP))
        {  packet.networkType = N_IPv4;
            packet.pt_networkHeader = (struct IP_Header*)(rawPacket
                                                          + sizeEthernet);
            packet.data += sizeIP;
            //based on protocol types in IP header, cast next encapsulated header
            //NOTE: not checking that raw data is suffiently long or not corrupted
            struct IP_Header *ip;
            ip = (struct IP_Header*)packet.pt_networkHeader;
            //convert between network byte order and machine byte order
            //- IP addresses are used to plot postion
            //- len may be used for colour mapping
            ip->srcAddress = ntohl(ip->srcAddress);
            ip->dstAddress = ntohl(ip->dstAddress);
            ip->len = ntohs(ip->len);
            ip->fragOffset = ntohs(ip->fragOffset);

            //need to check for IP options and skip if necessary. Mask off IP
            //version. standard IP header with no options will have value of 5.
            //5 means 5 x 32 bit words.
            int hdrLen = ip->vhl & 0x0F;
            if (hdrLen > 5)
            {  //indicates IP options are present, calculate length of options
                int optLen = (hdrLen - 5) * 4; //4 bytes in a 32 bit word
                if (length >= sizeEthernet + sizeIP + optLen)
                {   packet.data += optLen;
                }
                else
                {  //abort parsing
                    return;
                    //this would leave packet.data pointing to the 6th 32bit word of
                    //the ip header. Somewhere beyond this, the IP header was
                    //truncated
                }
            }

            //check for fragments and drop if fragment field set

            switch (ip->protocol)
            {  case 0x01: //ICMP
                if (length >= (sizeEthernet + sizeIP + sizeICMP))
                {  packet.transportType = T_ICMP;
                    packet.pt_transportHeader = (struct ICMP_Header*)(rawPacket
                                                                      + sizeEthernet + sizeIP);
                    packet.data += sizeICMP;
                }
                break;
            case 0x06: //TCP
                if (length >= (sizeEthernet + sizeIP + sizeTCP))
                {  packet.transportType = T_TCP;
                    packet.pt_transportHeader = (struct TCP_Header*)(rawPacket
                                                                     + sizeEthernet + sizeIP);
                    packet.data += sizeTCP;
                    struct TCP_Header *tcp;
                    tcp = (struct TCP_Header*)packet.pt_transportHeader;
                    //convert between network byte order and machine byte order
                    //- dst port used for plotting and may be used for colour mapping
                    //- src port may be used for colour mapping
                    tcp->srcPort = ntohs(tcp->srcPort);
                    tcp->dstPort = ntohs(tcp->dstPort);
                }
                break;
            case 0x11: //UDP
                if (length >= (sizeEthernet + sizeIP + sizeUDP))
                {  packet.transportType = T_UDP;
                    packet.pt_transportHeader = (struct UDP_Header*)(rawPacket
                                                                     + sizeEthernet + sizeIP);
                    packet.data += sizeUDP;
                    //set point coord
                    struct UDP_Header *udp;
                    udp = (struct UDP_Header*)packet.pt_transportHeader;
                    //convert between network byte order and machine byte order
                    //- dst port used for plotting and may be used for colour mapping
                    //- src port may be used for colour mapping
                    udp->srcPort = ntohs(udp->srcPort);
                    udp->dstPort = ntohs(udp->dstPort);
                }
                break;
            default:
                packet.data += sizeIP; //point to end of IP header
                break;
            }
        }
        else //cannot parse further, IP header incomplete
        {  packet.data += sizeEthernet; //pointer arithmetic to point to data to
            //end of ethernet header
        }

    }

}


void PacketHeaders::deParsePacket(struct OSI_Packet &headers)
{
    //revert byte orders changed

    if (headers.dataLinkType == DL_ETHERNET)
    {
        struct EthernetHeader *eth =
                (struct EthernetHeader*)headers.pt_dataLinkHeader;
                eth->type = htons(eth->type);

                if (headers.networkType == N_IPv4)
                {
                    struct IP_Header *ip;
                    ip = (IP_Header*)(headers.pt_networkHeader);
                    //switch back byte order changed
                    ip->dstAddress = htonl(ip->dstAddress);
                    ip->srcAddress = htonl(ip->srcAddress);
                    ip->len = htons(ip->len);
                    ip->fragOffset = htons(ip->fragOffset);

                    switch (headers.transportType)
                    {  case T_ICMP: //ICMP
                        /* nothing to do - as yet
               struct ICMP_Header *icmp;
               icmp = (struct ICMP_Header*)(pe.headers.pt_transportHeader);
            */
                        break;
                    case T_TCP: //TCP
                        struct TCP_Header *tcp;
                        tcp = (struct TCP_Header*)(headers.pt_transportHeader);
                        tcp->dstPort = htons(tcp->dstPort);
                        tcp->srcPort = htons(tcp->srcPort);
                    case T_UDP: //UDP
                        struct UDP_Header *udp;
                        udp = (struct UDP_Header*)(headers.pt_transportHeader);
                        udp->dstPort = htons(udp->dstPort);
                        udp->srcPort = htons(udp->srcPort);
                        break;
                    }
                }
    }

}


void PacketHeaders::setDataLinkType(int type)
{
    //check to make sure it is set to a valid type, else set to DL_UNKNOWN
    switch (type)
    {  case DL_ETHERNET :
        PacketHeaders::dataLinkType = DL_ETHERNET;
        break;
    default :
        PacketHeaders::dataLinkType = DL_UNKNOWN;
        break;
    }

}


QString PacketHeaders::ipAdrToDecDotStr(unsigned int ipAdr)
{
    //address segments
    unsigned int a = (ipAdr >> 24) & 0x000000FF;
    unsigned int b = (ipAdr >> 16) & 0x000000FF;
    unsigned int c = (ipAdr >> 8) & 0x000000FF;
    unsigned int d = ipAdr & 0x000000FF;
    //a little bit shifting and logical and with a bit-mask should do the
    //trick :)
    return QString(QString::number(a) + '.' + QString::number(b) + '.' +
                   QString::number(c) + '.' + QString::number(d));

}


void PacketHeaders::coutHeaders(struct OSI_Packet &packet)
{

    if (packet.dataLinkType == DL_ETHERNET)
    {  //datalink layer
        struct EthernetHeader *eth;
        eth = (struct EthernetHeader*)packet.pt_dataLinkHeader;
        cout << "datalink   > ethernet: src=";
        for (int i = 0; i < ETHER_ADDR_LEN - 1; i++)
        {  printf("%.2x-", eth->srcHost[i]);
        }
        printf("%.2x", eth->dstHost[ETHER_ADDR_LEN - 1]);
        cout << ", dst=";
        for (int i = 0; i < ETHER_ADDR_LEN - 1; i++)
        {  printf("%.2x-", eth->dstHost[i]);
        }
        printf("%.2x", eth->dstHost[ETHER_ADDR_LEN - 1]);
        //check host type, and be weary of network vs host byte order
        cout << ", type=" << ntohs(eth->type) << endl;
        if (packet.networkType == N_IPv4)
        {  struct IP_Header *ip;
            ip = (struct IP_Header*)packet.pt_networkHeader;
            cout << "network    > IPv4: src = " << ipAdrToDecDotStr(ip->srcAddress).toStdString()
                 << " (" << (ip->srcAddress) << "), dst = "
                 << ipAdrToDecDotStr(ip->dstAddress).toStdString() << " (" << (ip->dstAddress)
                 << "), proto=" << (int)ip->protocol << endl;
            //ICMP
            if (packet.transportType == T_ICMP)
            {  struct ICMP_Header *icmp;
                icmp = (struct ICMP_Header*)packet.pt_transportHeader;
                cout << "transport  > ICMP: type=" << (int)icmp->type << endl;
            }
            //TCP
            if (packet.transportType == T_TCP)
            {  struct TCP_Header *tcp;
                tcp = (struct TCP_Header*)packet.pt_transportHeader;
                cout << "transport  > TCP: src port=" << ntohs(tcp->srcPort)
                     << ", dst port=" << ntohs(tcp->dstPort) << ", flags=";
                //flags: CP_FIN|TCP_SYN|TCP_RST|TCP_ACK|TCP_URG|TCP_ECE|TCP_CWR
                if (tcp->flags & TCP_FIN)
                    cout << "FIN ";
                if (tcp->flags & TCP_SYN)
                    cout << "SYN ";
                if (tcp->flags & TCP_RST)
                    cout << "RST ";
                if (tcp->flags & TCP_ACK)
                    cout << "ACK ";
                if (tcp->flags & TCP_URG)
                    cout << "URG ";
                if (tcp->flags & TCP_ECE)
                    cout << "ECE ";
                if (tcp->flags & TCP_CWR)
                    cout << "CWR ";
                cout << endl;
            }
            //UDP
            if  (packet.transportType == T_UDP)
            {  struct UDP_Header *udp;
                udp = (struct UDP_Header*)packet.pt_transportHeader;
                cout << "transport  > UDP: src port=" << ntohs(udp->srcPort)
                     << ", dst port=" << ntohs(udp->dstPort) << endl;
            }
        }
        //network layer

    }
    else
        cout << "datalink > unsupported protocol\n";

}

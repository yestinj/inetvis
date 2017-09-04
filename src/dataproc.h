#ifndef DATAPROC_H
#define DATAPROC_H

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


/* dataproc.h & dataproc.cpp
 *
 * A main DataProcessor class with functional subclasses, DataExtractor and
 * Plotter which together with the DataProcessor handle the background
 * processing of data. In conjuntion with the DataProcessors time settings:
 * - the DataExtractor injects data from file, live local or remote network
 *   monitor, by constructing PacketEvents and placing them in a buffer of
 *   PacketEvents
 * - the Plotter is used to map the data in a PacketEvent to its visual
 *   metaphore, which at base is held as a GraphicalElement
 *
 */

/* revision history

2007-11-08: 
   * added implicit filter to drop fragmented packets. PacketHeaders can parse
     packets with IP options, but not frag packets.

*/


#include <cstring>
#include <math.h>
#include <deque>
#include <qobject.h>
#include <qthread.h>
#include <qmutex.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qdir.h>
#include <pcap.h>
#include <QSettings>

#ifdef MAC
#include <OpenGL/gl.h>
#endif

#ifdef LINUX
#include <GL/gl.h>
#endif

#ifdef WINDOWS
#include <GL/gl.h>
#endif

#include <timeutil.h>
#include <plotter.h>
//packetevent.h and in turn, packetheaders.h are linked via plotter


// DEGBUG DIRECTIVES ----------------------------------------------------------
//un-commnet to compile with debug info
//#define DEBUG_DATA_PROCESSOR
//#define DEBUG_DATA_EXTRACTOR
//#define DEBUG_PACKETS_PROCESSED
//#define DEBUG_CAPTURE_INSTANCE
//#define DEBUG_LOOKUP_HOME_NET
//#define DEBUG_IMPLICIT_FILTER
//#define DEBUG_FILTER
//#define DEBUG_DATA_UPDATE_TIMEING
//#define DEBUG_DATA_RENDER
//#define DEBUG_DATA_RENDER_PLOT
//#define DEBUG_TRANS_DECAY
//#define DEBUG_RECORD_PACKETS
//#define DEBUG_NEW_POINT_BULGE
#define OUTPUT_ERRORS_STD_OUT
#define OUTPUT_ERRORS_GUI


// DEFINITIONS ----------------------------------------------------------------

#define _BSD_SOURCE 1
//to avoid libpcap compatibility problems on some linux systems

#define IMPLICIT_BPF_FILTER_EXP "(ip[6:2] & 0x3fff = 0 and (icmp or tcp or udp))"
//only support certain protcols and non-fraged IP packets. 0x2fff checks that
//the more fragment bit is zero and that the fragement offset is zero
#define OPTIMISE_BPF_FILTER 1
#define DONT_OPTIMISE_BPF_FILTER 0

#define CAPTURE_LENGTH 68
//should be enough to cover ethernet IP - TCP, UDP or ICMP packets
#define MAX_CAPTURE_LENGTH 65535

#define PROMISC_MODE 1
#define NON_PROMISC_MODE 0
#define NO_READ_TIMEOUT 0

#define DATA_LINK_UNSET -1

#define DEFAULT_DUMP_FILE_NAME "dump.cap"

#define UPDATE_TIMER_ID 1
#define DEFAULT_UPDATE_RATE 25
//every 40 msec
#define MAX_UPDATE_RATE 200
//5msec
#define MIN_UPDATE_RATE 1
//200msec

#define DEFAULT_REPLAY_RATE 1.0
#define MAX_REPLAY_RATE 86400.0
//1 day in a second
#define MIN_REPLAY_RATE 0.001
//time factor, 1 millisec in a sec

#define EARLIEST_REPLAY_POSITION 0
//sec value for timeval tv.sec Jan 1970, 2am

#define DEFAULT_TIME_WINDOW 86400
//1 day
#define MAX_TIME_WINDOW 157766400.0
//5 years
#define MIN_TIME_WINDOW 0.04
// 1/25 of a second - anything lower should be parctically unoticible for
//instant events

#define DEFAULT_BUFFER_UNDERUN_PROTECTION 1.0
#define MIN_BUFFER_UNDERUN_PROTECTION 0.0
#define MAX_BUFFER_UNDERUN_PROTECTION 10.0
//number of seconds to buffer ahead - not really needed unless the
//application were converted to a threaded application where the
//data processor and extraction ran on a seperate thread to the
//visualization and gui

#define MAX_INT 2147483647
//size of integer field
//2^(32-1)-1 = 2147483646 seconds = about 68 years (else overflow problem
//with int)

#define DEFAULT_CHAR_FIELD_SIZE 256
//size of char buffers for c string storage

#define NEW_POINT_BULGE_USEC 500000
//this value is the number of microseconds the new points should bulge for

// Settings key definitions
#define RECORD_DEFAULT_DIR_KEY "dataproc/recording/default_dir"
#define RECORD_PCAPS_SUBDIR_KEY "dataproc/recording/pcaps_subdir"
#define RECORD_FRAMES_SUBDIR_KEY "dataproc/recording/frames_subdir"
#define RECORD_SNAPSHOTS_SUBDIR_KEY "dataproc/recording/snapshots_subdir"
#define RECORD_LIVE_SUBDIR_KEY "dataproc/recording/live_subdir"
#define RECORD_REPLAY_SUBDIR_KEY "dataproc/recording/replay_subdir"
#define DEFAULT_HOME_NETWORK_KEY "dataproc/home_network/default_home_network"
#define SHOW_HOME_NETWORK_NOT_SET_ERROR_KEY "dataproc/home_network/show_not_set_error"
#define DEFAULT_MONITOR_INTERFACE_KEY "dataproc/home_network/monitor_interface"
#define SCREENSHOT_FORMAT_KEY "dataproc/screenshot/screenshot_format"
#define SCREENSHOT_QUALITY_KEY "dataproc/screenshot/screenshot_quality"
#define SCREENSHOT_EXTENSION_KEY "dataproc/screenshot/screenshot_extension"

// Default settings values
#define RECORD_DEFAULT_DIR_DEFAULT "inetvis-recorded"
#define RECORD_PCAPS_SUBDIR_DEFAULT "inetvis-recorded/pcaps"
#define RECORD_FRAMES_SUBDIR_DEFAULT "inetvis-recorded/frames"
#define RECORD_SNAPSHOTS_SUBDIR_DEFAULT "inetvis-recorded/snapshots"
#define RECORD_LIVE_SUBDIR_DEFAULT "live"
#define RECORD_REPLAY_SUBDIR_DEFAULT "replayed"
#define DEFAULT_HOME_NETWORK_DEFAULT "0.0.0.0/0"
#define SHOW_HOME_NETWORK_NOT_SET_ERROR_DEFAULT true
#define DEFAULT_MONITOR_INTERFACE_DEFAULT ""
#define SCREENSHOT_FORMAT_DEFAULT "png"
#define SCREENSHOT_QUALITY_DEFAULT -1
#define SCREENSHOT_EXTENSION_DEFAULT "png"


enum replayModes {NOT_READY, MONITOR_LOCAL, MONITOR_REMOTE, REPLAY_FILE};
//NOT_READY indicates that the mode is not yet set

//enum replayState {PAUSED, PLAYING, LIVE, REPLAY_AT_END};
enum replayState {PAUSED, PLAYING, REPLAY_AT_END};

enum dumpFileRecMode {CONTINUOUS, SESSION};


class GLVisWidget;


//CLASS DEFINITIONS ------------------------------------------------------------

class DataProcessor : public QObject {
    Q_OBJECT //macro for use of signals and slots

private:
    //DataExtractor subclass -------------------------------------------------
    class DataExtractor {
        /* Internal class to impliment non blocking input into buffer by
          * extracting data using the libpcap library
          * - may in future be revised to operate as a seperate thread
          *   therefore control functions are required to protect data members
          *   from simultaneous access i.e. functions that lock and unlock a
          *   mutex
          *
          */

    private:

        DataProcessor *dataProcessor;
        //need a static pointer for pcap callback function
        static DataProcessor *statDataProcessor;

        //data source references
        QString strReplayFileReference;
        QString strRecordFileReference;
        QString strLocalInterface;
        QString strServerAddress;

        //settings
        int packetCaptureLength; //number of starting bytes to capture for
        //packet packet event buffer
        int promiscuousMode; //1 = promisc mode, 0 = non-promisc mode
        int readTimeout; //in millisec

        //libpcap components
        int pcapRet; //to hold return values from pcap
        int dataLinkType;
        char pcapErrorBuffer[PCAP_ERRBUF_SIZE];
        pcap_if_t *allDevices;
        pcap_if_t *device;
        //capture instances
        pcap_t *fileCaptureInstance;
        pcap_t *liveCaptureInstance;
        pcap_dumper_t *dumpFileDescriptor;
        int recMode;

        //filter
        QString implicit_bpf_FilterExpr;
        QString bpfFilterExpr;
        struct bpf_program bpfFilterProg;

        //network info
        bpf_u_int32 netAddress; //IP network address
        bpf_u_int32 netMask; //IP subnet mask

        //processing methodes, designed to run in a loop
        void readCaptureFile();
        void monitorLocalHost();
        void monitorRemoteServer();

        //loopback function called by pcap
        static void processPacket(u_char *args,
                                  const struct pcap_pkthdr *header, const u_char *snapshot);

        //error reporting
        bool checkPcapErrorBuffer(); //returns true if thier is an error
        //present
        void resetPcapErrorBuffer(); //resets error buffer to null string
        QString copyPcapError(); //returns a copy of the pcap error
        void reportError(const QString &errMsg,
                         const QString &function = NULL);

    public:

        //char pcapErrorBuffer[PCAP_ERRBUF_SIZE]; //store pcap error message
        //made private in case of threaded access

        DataExtractor();
        ~DataExtractor();

        void setDataProcLink(DataProcessor *ptr);

        void run(); //method to run process loop
        //- if extended to a thread, this method is called for thread
        //  execution and should be defined as virtual

        bool findAllDevices(QStringList& strLstAllDevs); //creates an itterator to network devices
        bool setDataLinkType(); //identifies and sets the dataLinkType
        bool lookupNetwork(); //sets up network address and mask
        void setHomeNetwork(const unsigned int netAdr,
                            const unsigned int mask);
        void getHomeNetwork(unsigned int &netAdr, unsigned int &mask);
        //returns currently set netwrok address and mask as ints

        //functions return true if succesful, else false
        //bool getAllInterfaces(QStringList &strList);
        bool openCaptureFile(const QString file = "");
        bool reopenCaptureFile(); //used when
        //the replay position or tiint a, int b, int c, int d, int slashMaskmewindow is changed such that
        //backtracking in the file is required
        QString getReplayFileName();
        bool openInterface(const QString netInterface = "");
        bool openDefaultInterface(); //set to default capture interface
        bool connectServer(const QString server = "");
        bool openDumpFile(const QString file = "");
        void recordToDumpFile(bool record); //record packets to file
        void closeDumpFile();
        bool setImplicitFilter(const QString impFilter);
        bool setFilterExpr(const QString filter);
        bool applyFilter();
        QString getImplicitFilter();
        //return values through parameters

    };


    //DataProcessor members and operations --------------------------------------

    //private internal members -----------------------------------------------

    //object states - enumerators defined previously
    int replayMode; //current mode: NULL_MODE, MONITOR_LOCAL, MONITOR_REMOTE,
    //REPLAY_FILE,
    int replayState; //current state: NOT_READY, PLAYING, PAUSED
    //transparent decay varaibles
    bool transDecay; //turn on fading effect as events become older
    bool transDecayNextRender; //notify to decrement transparency on next
    //render - not all renders are triggered by updates
    double transDecayFrac; //amount to decriment opacity
    //point bulge variables
    bool newPointBulge;
    //record state
    bool recordToFile;

    //Timming factors
    double timeFactor; //rate of playback as a time factor
    int updateRate; //rate in msec which data processor updates time values
    //no point in higher accuracy, since Timer operates at msec
    //resolution

    //relative time values - not to be interpreted as conventional timevals
    struct timeval timeWindow; //time window in sec and millisec
    struct timeval bufferAhead; //time in sec and millisec - realtive time

    //absolute time values - time elapsed since 02:00:00 Jan 01 1970
    struct timeval currentTime; //current time updated by a resolution
    //determined by updateRate
    struct timeval replayPosition; //current reference position of play back,
    //in absolute terms reference to Jan 01 1970, and updated at rate of
    //time factor - indicates time at begging of replay buffer
    struct timeval timeWindowEnd; //depends on timeWindow and end time
    struct timeval updateTimeInc; //to store update time step in microsec
    // - relative time - replay file mode
    struct timeval timeLapse; //to store time lapsed in microsec
    // - relative time - live mode
    struct timeval bufferPosition; //buffer position ahead of replay position
    //by bufferAhead
    struct timeval captureStartTime; //time indicating the beging of a capture
    //changes when a replay file is loaded or live monitor initiated
    struct timeval captureEndTime; //time indicating the end of a capture,
    //which will be the last capture timestamp in a capture file, or the
    //current time (less buffer underun protection) if it is a live
    //capture
    struct timeval lastRecordTime; //the time that the last packet was
    //recorded at
    struct timeval bulgeTime; //the relative amount of time to bulge points for
    struct timeval stopBulgeTime; //the time to stop bulging a new point (with
    //respect to the replay position and replay rate

    //Strings to hold implicit filter settings
    QString dstNetFilter;
    QString srcNetFilter;
    QString portFilter;

    //Qt QDateTime objects for updating gui display
    QDateTime qdt_replayPosition; //qt equivelent of replayPosition
    QDateTime qdt_twEnd; //qt equivelent of timeWindowEnd

    //Sting list of interface names
    QStringList strAllLocalInterfaces;

    QTimer *processTimer;
    QTimer *statusTimer; //update rate for reporting status messages

    DataExtractor dataExtractor; //consituent component to fetch packet data
    //place in packet event buffer
    //GLVisWidget *glVisWidget;

    //display list identifier - fost the case of static view to store the
    //points in a diplay list if playback is not running
    GLuint staticSatterplotList;

    static QSettings settings;

    //private internal operations --------------------------------------------

    void updateTimeIncrimental(int tmLapse); //function to set the
    //incrimental period in replay time between updates according to update
    //rate and replay rate (timeFactor)
    void updateTimevals(); //updates timeWindowEndTime, bufferEndTime and
    //bufferLength
    void updateTimeRange();
    bool updateImplicitFilter();
    bool checkDefaultDirs();

public:

    std::deque <PacketEvent> packetEventBuffer; //buffer with public access
    //for direct reference and improved access
    //- this will work fine provided only single thread access is required,
    //  otherwise it should be declared private with thread-safe access
    //  functions
    //- newly processed data is pushed on the front of the deque whilst
    //  the back can be checked to remove expired data

    //environment
    QString strAppDir; //application startup directory

    DataProcessor();
    ~DataProcessor();
    void init();
    //void addPacketEvent(const PacketEvent &pe);
    DataProcessor *getDataProcessorPtr();
    //get a pointer to the data processor
    //void setGLVisWidgetLink(GLVisWidget* ptr);

    //error reporting
    void reportError(const QString &errMsg, const QString &function = NULL);

    // configuration
    static QString getRecordDir();
    static void setRecordDir(QString recordDir);
    static bool isRecordDirSet();

    static QString getPcapsDir();
    static void setPcapsDir(QString pcapsDir);
    static bool isPcapsDirSet();

    static QString getFramesDir();
    static void setFramesDir(QString framesDir);
    static bool isFramesDirSet();

    static QString getSnapshotsDir();
    static void setSnapshotsDir(QString snapshotsDir);
    static bool isSnapshotDirSet();

    static QString getLiveSubdir();
    static void setLiveSubdir(QString liveSubdir);
    static bool isLiveSubdirSet();

    static QString getReplaySubdir();
    static void setReplaySubdir(QString replaySubdir);
    static bool isReplaySubdirSet();

    static QString getDefaultHomeNetwork();
    static bool setDefaultHomeNetwork(int, int, int, int, int);
    static bool setDefaultHomeNetwork(QString);
    static bool isDefaultHomeNetworkSet();

    static bool getShowHomeNetworkNotSetError();
    static void setShowHomeNetworkNotSetError(bool show);
    static bool isShowHomeNetworkNotSetError();

    static QString getDefaultMonitorInterface();
    static void setDefaultMonitorInterface(QString monitorInterface);
    static bool isDefaultMonitorInterfaceSet();

    static QString getScreenshotFormat();
    static void setScreenshotFormat(QString screenshotFormat);
    static bool isScreenshotFormatSet();

    static QString getScreenshotExtension();
    static void setScreenshotExtension(QString screenshotExtension);
    static bool isScreenshotExtensionSet();

    static int getScreenshotQuality();
    static void setScreenshotQuality(int screenshotQuality);
    static bool isScreenshotQualitySet();


public slots:

    //signals recived from gui control panel - set playback options
    void setMode(int mode);
    void selectReplayFile(const QString file);
    void selectNetworkInterface(const QString netInterface);
    void selectRemoteServer(const QString server);
    void setFilter(const QString filterExpression);
    void setReplayPosition(const struct timeval newReplayPos);
    void setReplayPosition(const QDateTime newReplayPos);
    void play();
    void pause();
    void togglePlayPause();
    void record(bool rec);
    void setReplayRate(double rate); //dependant on sufficient update rate
    void setTimeWindow(double seconds);
    void setBufferAhead(double seconds);
    void setUpdateRate(int timesPerSecond);
    void enablePointBulge(bool on);

    //report functions
    void getBufferStatus(); //report the size of the buffer in terms of the
    //number of elements, its size in bytes, and its length in time
    int getMode();
    int getState();
    QDateTime *getReplayPosition();
    QDateTime *getTimeWindowEnd();
    QString getReplayFileName();
    void getListLocalInterfaces(QStringList *interfaces);

    //set network ranges
    void reportHomeNetwork(); //to update gui display
    void setHomeNetwork(int a, int b, int c, int d, int slashMask);
    void setHomeNetwork(unsigned int netAdr, unsigned int netMask);
    void guessHomeNetwork();
    void setSourceNetwork(int a, int b, int c, int d, int slashMask);
    void setPortRange(int start, int end,
                      int plotType=Y_PLOT_LINEAR_DST_PORT,
                      int logRangeBase=DEFAULT_LOG_RANGE_BASE);

    //plotting and rendering
    void setColourScheme(int scheme);
    void setTransparentDecay(bool on = true);
    void changeBackgroundColour(int colCode);
    void rePlot();
    void update(); //called by updateTimer to update various timming values
    void updateStatus(); //called by statusTimer
    void generatePointDispList(); //to generate the static scatteplot list
    void renderData(); //called by VisDisplay
    //when playing/live, dynamic change of points
    //else when paused, a static display list improves performance
    void renderDataDynamic();

signals:

    void playing();
    void paused();
    void updateGLVisWidget();
    void setStatusBarMessage(QString message);
    void updateReplayTimeDisplay(const QDateTime &repPos,
                                 const QDateTime &timeWinEnd);
    void updateReplayRateDisplay(double rate);
    void updateTimeWindowDisplay(double timeWin);
    void setTimeRange(const QDateTime start, const QDateTime end);
    void updateHomeNetworkDisplay(int a, int b, int c, int d, int slashMask,
                                  QString range);
    void updateSourceNetworkDisplay(int a, int b, int c, int d, int slashMask,
                                    QString range);
    void updatePortRangeDisplay(int start, int end);
    void errorFound(const QString errMsg);
    void setXAxisLabels(const QString x0, const QString x1);
    void setYAxisLabels(const QString y0, const QString y1);
    void setZAxisLabels(const QString z0, const QString z1);
    void setDateTimeReference(QDateTime* dt);
    void setRecordButton(bool buttonOn);
    void sendErrMsg(QString errMsg);

};
#endif DATAPROC_H

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

/* glviswidget.h & glviswidget.cpp
 *
 * Header file for custom OpenGLWidget. The basis of the for this class and the
 * the OpenGL techniques employed owe due credit to the 2005 Honours Graphics
 * course run by Shaun Bangay at Rhodes University.
 *
 * This class handles the OpenGL framework for rendering out to the screen using
 * a Qt OpenGL widget extention. It calls and relies on the render methods of other
 * objects, such as the DataProcessor.
 *
 */


/* CHANGELOG

20071112: remove lastTime and dependancy on timeofday due to lack of support 
    in win32. Instead we rely on the accuracy of the timer

*/

//To use nvidia development GL header
//#include "/usr/share/doc/nvidia-glx-dev/include/GL/gl.h"

#include <qgl.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <sys/time.h>
#include <QCloseEvent>

// DEGBUG DIRECTIVES -----------------------------------------------------------
//un-commnet to compile with debug info
//#define DEBUG_FRAME_CAPTURE
//#define DEBUG_REF_FRAME_GRID
//


// DEFINITIONS -----------------------------------------------------------------


#define AXIS_LENGTH 100.0
//this value would idealy corrospond DEFAULT_DRAWING_FIELD_SIZE in plotter.h
#define CUBE_CENTER_POINT 50.0
#define VERICAL_PLANE_OFFSET -20.0
#define MARKER_LENGTH 1.0
#define MARKER_TILT_COMPONENT 0.707106781
//to get marker lines at 45 degrees ... 1 / sqrt(2)
#define DEFAULT_PARTITIONS_X 8
#define DEFAULT_PARTITIONS_Y 4
#define DEFAULT_PARTITIONS_Z 16
//for grid lines and markers
#define DEFAULT_TRANSPARENCY 0.2
//for grid lines
#define DEFAULT_POINT_SIZE 2.0
#define CAPTURE_BUFFER_INCREASE_FACTOR 1.5
//to calculate capture buffer size incriments
#define CAPTURE_BUFFER_DECREASE_FACTOR 2.0
//threshold trigger to decrease memory

#define DEFAULT_FPS_SAMPLE_RATE 1000
//default number of milliseconds to wait before calculating average
//framerate

//text to display if home range is cloaked
#define CLOAKED_X0 "the.drk.net.0"
#define CLOAKED_X1 "the.drk.net.255"

//define background colour codes
#ifndef BLACK_BACKGROUND
#define BLACK_BACKGROUND 0
#endif
#ifndef WHITE_BACKGROUND
#define WHITE_BACKGROUND 1
#endif

enum projectionModes {GLVW_PERSPECTIVE, GLVW_ORTHOGRAPHIC};


//forward decleration
class DataProcessor;


//CLASS DEFINITION -------------------------------------------------------------


class GLVisWidget : public QGLWidget
{
    Q_OBJECT //macro fro use of signals and slots

protected:

    QWidget *parentWidget;
    DataProcessor *dataProcessor;

    //variables to hold properties of display effects
    int projectionMode; //GLVW_PERSPECTIVE or GLVW_ORTHOGRAPHIC
    double globalPointSize, globalLineWidth;
    bool pointSmoothing;
    bool blending; //for point smoothing or transparency, blending must be enabled
    bool transDecay;

    //varibles related to background colour
    int backgroundCol; //either black (0) or white (1)
    float xLabelCol[3];
    float yLabelCol[3];
    float zLabelCol[3];
    float otherLabelCol[3];

    //list identifiers
    GLuint referenceAxesList, referenceMarkersList, referenceGridList;

    //reference frame drawing controls
    bool primaryAxesOn, boundingAxesOn, cubeReferenceOn, planeReferenceOn,
    referenceMarkersOn;
    //reference grids around cube and plane
    bool referenceGridFrontOn, referenceGridBackOn, referenceGridLeftOn,
    referenceGridRightOn, referenceGridTopOn, referenceGridBottomOn,
    referenceGridPlaneOn;
    float gridTransparency;
    //partitions
    int xPartitions, yPartitions, zPartitions;
    //'in-display' text output options
    bool axisLabelsOn, dateTimeDisplayOn, fpsTextOn, hideHomeRange;

    //state variable for frames to file
    bool captureFrameToFile; //current frame is to be captured
    bool captureSingleFrameToFile; //take a single snapshot, as apposed to
    //recording multiple frames
    //variable to count frames
    unsigned int recFrameCount; //used to name files during frame capture
    unsigned int fpsFrameCount; //used for framerate calculation
    int fps; //the last framerate achieved
    int fpsSampleRate; //number of milliseconds to wait before calculating
    //average framerate
    QTimer *fpsTimer; //time to sample frames rendered
    int snapshotCount; //to count number of snapshots taken at a specfic time

    //file path and file name stirngs
    QString frameCaptureDir;
    char frameFileName[512];
    int frameDirCharIndex; //number of characters in the dir

    //strings to hold labels
    //axes labels - intersections XYZ primary axes and ranges
    QString strX0, strX1, strY0, strY1, strZ0, strZ1;
    QString homeRange0, homeRange1;
    QString fpsText;
    QFont fontLabels;
    //time label pointer
    QDateTime nullTime;
    QDateTime *currentTime;

    //frame buffer for capturing images to file
    int captureBufferSize;
    char* captureBuffer;

    //variables to keep track of current window size - important for frame
    //capture buffer size and memory managment
    int width, height, resComp;
    //resComp used for calculating scalling effects, normally assigned as the
    //smaller of height or width, since smallest size determines cubes
    //dimentions

    //variables to track mouse position
    int mouseox; //offset x
    int mouseoy; //offset y

    //variables to allow interactive dynamic mainpulation of perspective
    float scale;
    float resScale; //resrrticted scale
    float rotx;
    float roty;
    float offsetx;
    float offsety;
    float offsetz;
    //restricted offsets
    float resOffsetx;
    float resOffsety;
    float resOffsetz;

    //implimented virtual functions
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //internal functions
    void updateProjection(); //modularised to be called both by resizeGL and
    //when setting the perspectives

    //create display lists for static refrence frame
    void compileReferenceAxesList();
    void compileReferenceMarkersList();
    void compileReferenceGridList();

    //user interaction
    void checkControlBounds(); //ensure navigation within bounds
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent (QWheelEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    //internal methods
    void checkFrameCaptureBufferSize();

    // for Harlem shake mode
    float harlemCount;
    bool harlemToggle;
    
    // for rotation
    bool rotateToggle;
    float rotateAmount;

public:

    //GLVisWidget(QWidget *parent, const char *name);
    GLVisWidget(QWidget *parent);
    ~GLVisWidget();

    //object references
    void setDataProcLink(DataProcessor* ptr);

public slots:

    //to set projection mode
    void setPerspectiveProjection();
    void setOrthographicProjection();

    //set orientation
    void setFrontView();
    void setBackView();
    void setLeftView();
    void setRightView();
    void setTopView();
    void setBottomView();

    //set reference frame values
    void setReferenceFrame(bool primAxes, bool boundAxes, bool cube, bool plane,
                           bool markers);
    void setGridLines(bool front, bool back, bool left, bool right, bool top,
                      bool bottom, bool plane, float transparency);
    void displayAxisLabels(bool axisOn);
    void displayDateTime(bool timeOn);
    void setDateTimeReference(QDateTime* dt);
    void displayFramerate(bool fpsOn);
    void setXAxisLabels(const QString x0, const QString x1);
    void setYAxisLabels(const QString y0, const QString y1);
    void setZAxisLabels(const QString z0, const QString z1);
    void hideHomeRangeLabels(bool on);
    void setPartitions(int x, int y, int z);
    void setPointSize(int size);
    void enablePointSmoothing(bool on);
    void enableTransDecay(bool on);

    //togle frame capture
    void captureFrames(bool);
    void captureCurrentFrame();

    //framerate reporting function
    void calculateFPS(); //calculates and average since the last time it was
    //called

    //set display options
    void showEvent();
    void setFullScreen(bool);
    void setBackgroundCol(int colCode); //1 = white, 0 = black

    //get values
    double getGlobalPointSize();

signals:

    void showControlPanel();
    void showPlotterSettings();
    void showReferenceFrameSettings();
    void showGeneralSettings();
};

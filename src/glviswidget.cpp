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

#include <glviswidget.h>
#include <math.h>
#include <iostream>
#include <dataproc.h>
#include <logui.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

using namespace std;


//GLVisWidget::GLVisWidget(QWidget *parent, char const *name) : QGLWidget(parent, name)
GLVisWidget::GLVisWidget(QWidget *parent) : QGLWidget(parent) {
    parentWidget = parent;
    dataProcessor = NULL;

    //set defualt projection mode
    projectionMode = GLVW_PERSPECTIVE;

    //set global properties for size to defaults
    globalPointSize = 2.0;
    globalLineWidth = 1.0;

    //set state variables effecting blending
    pointSmoothing = false;
    transDecay = false;
    blending = false;

    //set background related colours
    setBackgroundCol(BLACK_BACKGROUND); //default

    //set reference for list id
    referenceAxesList = 1;
    referenceMarkersList = 2;
    referenceGridList = 3;

    //set reference frame rendering options
    primaryAxesOn = true;
    boundingAxesOn = true;
    cubeReferenceOn = true;
    planeReferenceOn = true;
    referenceMarkersOn = false;
    //grids
    referenceGridFrontOn = false;
    referenceGridBackOn = true;
    referenceGridLeftOn = true;
    referenceGridRightOn = false;
    referenceGridTopOn = false;
    referenceGridBottomOn = false;
    referenceGridPlaneOn = true;
    gridTransparency = DEFAULT_TRANSPARENCY;

    //default partitions
    xPartitions = DEFAULT_PARTITIONS_X;
    yPartitions = DEFAULT_PARTITIONS_Y;
    zPartitions = DEFAULT_PARTITIONS_Z;

    //'in-display' text output defauls
    axisLabelsOn = true;
    hideHomeRange = false;
    dateTimeDisplayOn = true;
    fpsTextOn = false;

    //default setting for capture frame to file
    captureFrameToFile = false;
    captureSingleFrameToFile = false;

    //framecount and framerate variables
    recFrameCount = 0;
    fpsFrameCount = 0;
    fps = 0;
    fpsSampleRate = DEFAULT_FPS_SAMPLE_RATE; //number of milliseconds to wait
    //before calculating average framerate
    //setup timer
    fpsTimer = new QTimer(this); //timer to sample frames rendered
    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(calculateFPS()));
    snapshotCount = 0;

    //file capture dirs
    QString frameCaptureDir = ""; //default to the current dir
    frameFileName[0] = '\0'; //init empty string
    frameDirCharIndex = 0;

    //text labels for axes
    strX0 = "x0";
    strX1 = "x1";
    strY0 = "y0";
    strY1 = "y1";
    strZ0 = "z0";
    strZ1 = "z1";
    homeRange0 = "homeRange0";
    homeRange1 = "homeRange1";
    //for fps
    fpsText = "fps: off";
    //font
    fontLabels.setPointSize(12);
    //time label
    nullTime = QDateTime(QDate(1970, 1, 1));
    nullTime.time().setHMS(2, 0, 0);
    currentTime = &nullTime;

    //init values for frame capture buffer - start larger to accomodate screen
    //size growth
    captureBufferSize = 0;
    captureBuffer = NULL; //cannot be setup at this stage becuase window
    //size not yet intialized

    //init mouse offset values
    mouseox = -1;
    mouseoy = -1;

    //init dynamic transformation variables
    scale = 1.0; //the size
    resScale = 1.0; //the restricted representation of scale
    rotx = 0.0; //horrizontal direction
    roty = 0.0; //verical direction
    offsetx = 0.0; //horrizontal position
    offsety = 0.0; //verical position
    offsetz = 0.0; //depth position

    //init offset values for restricted values
    resOffsetx = 0.0;
    resOffsety = 0.0;
    resOffsetz = 0.0;

    // init Harlem shake counter
    harlemCount = 0;
    harlemToggle = false;

    // init rotate
    rotateToggle = false;
    rotateAmount = 0.05;

    setFocusPolicy(Qt::StrongFocus);
}


GLVisWidget::~GLVisWidget() {
    makeCurrent();
    delete[] captureBuffer;

}


void GLVisWidget::setDataProcLink(DataProcessor *ptr) {
    dataProcessor = ptr;
}


void GLVisWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST); //to draw overlapping points in correct order
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //for best results to
    //draw transparently
    glDisable(GL_BLEND); //by default, don't blend, unless required
    glDisable(GL_LIGHTING); //no use for lighting in this visualization
    glShadeModel(GL_FLAT);
    glDisable(GL_POINT_SMOOTH);

    //set default point and line sizes
    glPointSize(globalPointSize);
    glLineWidth(globalLineWidth);

    //set background colour
    if (backgroundCol == BLACK_BACKGROUND)
        glClearColor(0.0, 0.0, 0.0, 0.0);
    else
        glClearColor(1.0, 1.0, 1.0, 0.0);

    //set memmory data alignment for capturing frames
    glPixelStorei(GL_PACK_ALIGNMENT, 1); //align to byte rather than the default
    //of a word, for writting frame out to a buffer and then an image file

    //create lists
    compileReferenceAxesList();
    compileReferenceMarkersList();
    compileReferenceGridList();

}


void GLVisWidget::resizeGL(int w, int h) {
    //update width and height values
    width = w;
    height = h;

    //resComp for calaculating scalling effects on text labels, the smaller
    //component determines the cube dimentions
    if (width > height) {
        resComp = height;
    } else {
        resComp = width;
    }

    //setup projection matrix
    glViewport(0, 0, (GLint) width, (GLint) height);

    if (captureFrameToFile) {
        checkFrameCaptureBufferSize();
    }

    updateProjection();
}


void GLVisWidget::updateProjection() {
    int w, h; //width and height
    GLfloat scaleAspect; //for calculating bounding box according to resize

    w = this->size().width();
    h = this->size().height();

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    //keep aspect ratio by checking width vs height
    if (w >= h) //more width than hight, scale down height
    {
        scaleAspect = (GLfloat) w / (GLfloat) h;
        if (projectionMode == GLVW_PERSPECTIVE) {
            glFrustum(-1.0 * scaleAspect, 1.0 * scaleAspect, -1.0, 1.0, 1.5, 200.0);
        } else //orthographic
        {
            glOrtho(-1.0 * scaleAspect, 1.0 * scaleAspect, -1.0, 1.0, 1.5, 200.0);
        }
    } else //more height than width, scale down width
    {
        scaleAspect = (GLfloat) h / (GLfloat) w;
        if (projectionMode == GLVW_PERSPECTIVE) {
            glFrustum(-1.0, 1.0, -1.0 * scaleAspect, 1.0 * scaleAspect, 1.5, 200.0);
        } else //orthographic
        {
            glOrtho(-1.0, 1.0, -1.0 * scaleAspect, 1.0 * scaleAspect, 1.5, 200.0);
        }
    }
    //set to modelview matrix
    glMatrixMode(GL_MODELVIEW);

}


void GLVisWidget::paintGL() {
    
    /* Hacky code start */
    
    if(harlemToggle)
    {
        rotx += sin(harlemCount) * 0.25;
        roty += cos(harlemCount) * 0.25;
        harlemCount = (harlemCount + 0.1);
        
        if(harlemCount == 360)
        {
            harlemCount = 0.0;
        }
    }
    
    if(rotateToggle)
    {
        rotx += rotateAmount;
    }
    
    /*  Hacky code end  */
    
    glLoadIdentity();

    //clear the previous frame rendered
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //scale to fit into frustrum and set z axis positive coming out of screen
    if (projectionMode == GLVW_PERSPECTIVE) {
        glScalef(0.03, 0.03, -0.03);
    } else //orthographic
    {
        glScalef(0.0166, 0.0166, -0.03); //we need to zoom out a little more since
        //no diminishing perspective
    }

    //move the world out so we see the visualization almost sqaure on,
    //and can view the icmp plane
    glTranslatef(0.0, 10.0, 150.0);

    //INTERACTIVE TRANSFORMATIONS -------------------------

    //do dynamic transformation according to mouse manipulations

    //SCALE the object about its center
    glScalef(resScale, resScale, resScale);

    //OFFSET the center of the object horizontally and vertically according
    glTranslatef(resOffsetx, resOffsety, resOffsetz);

    //ROTATE the object about its center
    //horizontal movements to rotate about the y axis
    glRotatef(roty, 1.0, 0.0, 0.0);
    //vertical movements to rotatereferenceGridOn about the x axis
    glRotatef(rotx, 0.0, 1.0, 0.0);

    //let the origin (0,0,0) be the bottom left back corner of the cube
    //this is so that the cube spins about its center
    glTranslatef(-CUBE_CENTER_POINT, -CUBE_CENTER_POINT, -CUBE_CENTER_POINT); //this is the center point of the cube

    //draw reference frame and axis labeling
    if (cubeReferenceOn || planeReferenceOn) {
        glLineWidth(globalLineWidth * 2.0);
        glCallList(referenceAxesList);
        //text labels for axes
        if (axisLabelsOn) {  //TODO: calculate scale and res spacing factor
            //double spacingFact = (480 / resComp) + (1 / scale);
            //x axis
            glColor3fv(xLabelCol);
            renderText(-2.5, -5.5, -2.5, strX0, fontLabels);
            renderText(AXIS_LENGTH + 2.5, -2.5, -2.5, strX1, fontLabels);
            //y axis
            glColor3fv(yLabelCol);
            renderText(-2.5, -2.5, -2.5, strY0, fontLabels);
            renderText(-2.5, AXIS_LENGTH + 2.5, -2.5, strY1, fontLabels);
            //z axis
            glColor3fv(zLabelCol);
            renderText(-2.5, -8.5, -2.5, strZ0, fontLabels);
            renderText(-2.5, -2.5, AXIS_LENGTH + 2.5, strZ1, fontLabels);
        }

        if (referenceMarkersOn) {
            glLineWidth(globalLineWidth * 2.0);
            glCallList(referenceMarkersList);
        }

        if (referenceGridFrontOn || referenceGridBackOn || referenceGridLeftOn
            || referenceGridRightOn || referenceGridTopOn || referenceGridBottomOn
            || referenceGridPlaneOn) {
            glLineWidth(globalLineWidth * 1.0);
            glCallList(referenceGridList);

        }
    }

    //draw other labels
    glColor3fv(otherLabelCol);

    int localDevicePixelRatio = devicePixelRatio();

    //draw time label
    if (dateTimeDisplayOn) {  //glColor3f(1.0, 1.0, 1.0);
        renderText(10, height/localDevicePixelRatio - 10, currentTime->toString("yyyy/MM/dd - hh:mm:ss:zzz"), fontLabels);
    }

    //draw fps
    if (fpsTextOn) {  //glColor3f(1.0, 1.0, 1.0);
        renderText(width/localDevicePixelRatio - 60, height/localDevicePixelRatio - 10, fpsText, fontLabels);
        fpsFrameCount++;
    }

    //render data
    if (blending) {
        glEnable(GL_BLEND); //blending is required to smooth points
        //or make them transparent
        dataProcessor->renderData();
        glDisable(GL_BLEND);
    } else
        dataProcessor->renderData();

    glFlush();

    //dump frames into raw bitmaps
    if (captureFrameToFile) {
        //create file name
        QString screenshotExtension = DataProcessor::getScreenshotExtension();
        if (captureSingleFrameToFile) {
            //grabbed the single frame, don't want any more
            captureSingleFrameToFile = false;
            captureFrameToFile = false;            
            //create filename
            QString fname_part = "/" + currentTime->toString("/yyyyMMdd-hhmmsszzz") + "_snap%02d." + screenshotExtension;
            sprintf(&frameFileName[frameDirCharIndex], fname_part.toLatin1(), snapshotCount);
            QString fmtExt = QString("%02d.").append(screenshotExtension);
            //check if filename exists - in case of multiple snapshots at same position in file
            if (QFile::exists(frameFileName)) {
                snapshotCount++;
                //reprint the end of the files name
                sprintf(&frameFileName[frameDirCharIndex + 25], fmtExt.toStdString().c_str(), snapshotCount);
            } else {
                snapshotCount = 0;
                //reprint the end of the files name
                sprintf(&frameFileName[frameDirCharIndex + 25], fmtExt.toStdString().c_str(), snapshotCount);
            }
        } else {
            //capturing multiple frames to files
            recFrameCount++;
            QString fname_part = "/frame%09d - " + currentTime->toString("yyyyMMdd-hhmmsszzz") + "." + screenshotExtension;
            sprintf(&frameFileName[frameDirCharIndex], fname_part.toLatin1(), recFrameCount);
            //here we cater for framecount of up to 9 digits
        }

        QString screenshotFormat = DataProcessor::getScreenshotFormat();

        // Handle each screenshot format differently as they may require some unique tweaking..
        // The null check is to act as a failsafe incase config is not set for some reason.
        if (screenshotFormat.toStdString() == "png" || screenshotFormat == "") {
            // Read the GL screen pixels into the captureBuffer
            glReadPixels (0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, captureBuffer);

            // Create and write out a PNG image screenshot.
            QImage image((const unsigned char*)captureBuffer, width, height, QImage::Format_ARGB32);
            // For some reason it's flipped, need to fix that here before outputting.
            image = image.mirrored();
            int screenshotQuality = DataProcessor::getScreenshotQuality();
            bool success = image.save(frameFileName, "PNG", screenshotQuality);

            if (!success) {
                Log::logError(QString("ERROR: Failed to output frame capture ").append(frameFileName));
            }
        } else if (screenshotFormat.toStdString() == "ppm") {
            // Read the GL screen pixels into the captureBuffer
            glReadPixels (0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, captureBuffer);

            // Create and write out a screenshot.
            QImage image((const unsigned char*)captureBuffer, width, height, QImage::Format_ARGB32);
            // For some reason it's flipped, need to fix that here before outputting.
            image = image.mirrored();
            int screenshotQuality = DataProcessor::getScreenshotQuality();
            bool success = image.save(frameFileName, "PPM", screenshotQuality);

            if (!success) {
                Log::logError(QString("ERROR: Failed to output frame capture ").append(frameFileName));
            }
        }  else if (screenshotFormat.toStdString() == "bmp") {
            // Read the GL screen pixels into the captureBuffer
            glReadPixels (0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, captureBuffer);

            // Create and write out a screenshot.
            QImage image((const unsigned char*)captureBuffer, width, height, QImage::Format_ARGB32);
            // For some reason it's flipped, need to fix that here before outputting.
            image = image.mirrored();
            int screenshotQuality = DataProcessor::getScreenshotQuality();
            bool success = image.save(frameFileName, "BMP", screenshotQuality);

            if (!success) {
                Log::logError(QString("ERROR: Failed to output frame capture ").append(frameFileName));
            }
        }
        else {
            Log::logError(QString("ERROR: Screenshot format not supported! ").append(screenshotFormat));
        }
    }
}


void GLVisWidget::compileReferenceAxesList() {
    //function called in a way to minimise state changes when 2rendered

    //REFERENCE AXES LIST CREATION ---------------

    glNewList(referenceAxesList, GL_COMPILE);

    glBegin(GL_LINES);

    if (cubeReferenceOn) {
        //x axis, horrizontal component
        if (primaryAxesOn) {
            glColor3f(0.0, 0.0, 1.0); //blue
            //x axis, bottom front
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(AXIS_LENGTH, 0.0, 0.0);
            if (boundingAxesOn) {  //x axis, top front
                glVertex3f(0.0, AXIS_LENGTH, 0.0);
                glVertex3f(AXIS_LENGTH, AXIS_LENGTH, 0.0);
                //x axis, bottom back
                glVertex3f(0.0, 0.0, AXIS_LENGTH);
                glVertex3f(AXIS_LENGTH, 0.0, AXIS_LENGTH);
                //x axis, top back
                glVertex3f(0.0, AXIS_LENGTH, AXIS_LENGTH);
                glVertex3f(AXIS_LENGTH, AXIS_LENGTH, AXIS_LENGTH);
            }
        }

        //y axis, veriticle component
        if (primaryAxesOn) {
            glColor3f(0.0, 1.0, 0.0); //green
            //y axis, left front
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, AXIS_LENGTH, 0.0);
            if (boundingAxesOn) {  //y axis, left back
                glVertex3f(0.0, 0.0, AXIS_LENGTH);
                glVertex3f(0.0, AXIS_LENGTH, AXIS_LENGTH);
                //y axis, right front
                glVertex3f(AXIS_LENGTH, 0.0, 0.0);
                glVertex3f(AXIS_LENGTH, AXIS_LENGTH, 0.0);
                //y axis, right back
                glVertex3f(AXIS_LENGTH, 0.0, AXIS_LENGTH);
                glVertex3f(AXIS_LENGTH, AXIS_LENGTH, AXIS_LENGTH);
            }
        }

        //z axis, depth component
        if (primaryAxesOn) {
            glColor3f(1.0, 0.0, 0.0); //red
            //z axis, left bottom
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, AXIS_LENGTH);
            if (boundingAxesOn) {  //z axis, left top
                glVertex3f(0.0, AXIS_LENGTH, 0.0);
                glVertex3f(0.0, AXIS_LENGTH, AXIS_LENGTH);
                //z axis, right bottom
                glVertex3f(AXIS_LENGTH, 0.0, 0.0);
                glVertex3f(AXIS_LENGTH, 0.0, AXIS_LENGTH);
                //z axis, right top
                glVertex3f(AXIS_LENGTH, AXIS_LENGTH, 0.0);
                glVertex3f(AXIS_LENGTH, AXIS_LENGTH, AXIS_LENGTH);
            }
        }

    }

    if (planeReferenceOn) {
        //bottom icmp plane
        if (primaryAxesOn) {
            glColor3f(0.5, 0.5, 0.5); //50% grey
            //front
            glVertex3f(0.0, VERICAL_PLANE_OFFSET, 0.0);
            glVertex3f(AXIS_LENGTH, VERICAL_PLANE_OFFSET, 0.0);
            //left
            glVertex3f(0.0, VERICAL_PLANE_OFFSET, 0.0);
            glVertex3f(0.0, VERICAL_PLANE_OFFSET, AXIS_LENGTH);
            if (boundingAxesOn) {  //right
                glVertex3f(AXIS_LENGTH, VERICAL_PLANE_OFFSET, AXIS_LENGTH);
                glVertex3f(AXIS_LENGTH, VERICAL_PLANE_OFFSET, 0.0);
                //back
                glVertex3f(0.0, VERICAL_PLANE_OFFSET, AXIS_LENGTH);
                glVertex3f(AXIS_LENGTH, VERICAL_PLANE_OFFSET, AXIS_LENGTH);
            }
        }

    }

    glEnd();

    glEndList();


}


void GLVisWidget::compileReferenceMarkersList() {

    //REFERENCE MARKER LIST CREATION -------------

    //precaculated values
    double xPartitionSize = AXIS_LENGTH / xPartitions;
    double yPartitionSize = AXIS_LENGTH / yPartitions;
    double zPartitionSize = AXIS_LENGTH / zPartitions;
    const double MARKER_OFFSET = MARKER_LENGTH / MARKER_TILT_COMPONENT;
    //to make 45 degree line from axis edge - to find 45 degree protruding
    //angle for marker from axis, r arcos (theta) = x ... r/sqrt(2

    glNewList(referenceMarkersList, GL_COMPILE);

    if (referenceMarkersOn) {
        glBegin(GL_LINES);

        if (cubeReferenceOn) {
            if (primaryAxesOn) {
                //x component marker lines
                glColor3f(0.0, 0.0, 1.0); //blue
                for (int i = 1; i < xPartitions; i++) //since the main axes will act
                    //as the end lines for the partitions, we draw one less lines than
                    //the number of partitions specified
                {  //along x axis - front face
                    glVertex3f(i * xPartitionSize, 0.0, 0.0);
                    glVertex3f(i * xPartitionSize, -MARKER_OFFSET, -MARKER_OFFSET);
                }
                if (boundingAxesOn) {
                    for (int i = 1; i < xPartitions; i++) {
                        glVertex3f(i * xPartitionSize, AXIS_LENGTH, 0.0);
                        glVertex3f(i * xPartitionSize, AXIS_LENGTH + MARKER_OFFSET, -MARKER_OFFSET);
                        //along x axis - back face
                        glVertex3f(i * xPartitionSize, 0.0, AXIS_LENGTH);
                        glVertex3f(i * xPartitionSize, -MARKER_OFFSET, AXIS_LENGTH + MARKER_OFFSET);
                        glVertex3f(i * xPartitionSize, AXIS_LENGTH, AXIS_LENGTH);
                        glVertex3f(i * xPartitionSize, AXIS_LENGTH + MARKER_OFFSET, AXIS_LENGTH + MARKER_OFFSET);
                    }
                }

                //y component marker lines
                glColor3f(0.0, 1.0, 0.0); //green
                for (int i = 1; i < yPartitions; i++) {   //alony y axis - front face
                    glVertex3f(0.0, i * yPartitionSize, 0.0);
                    glVertex3f(-MARKER_OFFSET, i * yPartitionSize, -MARKER_OFFSET);
                }
                if (boundingAxesOn) {
                    for (int i = 1; i < yPartitions; i++) {
                        glVertex3f(AXIS_LENGTH, i * yPartitionSize, 0.0);
                        glVertex3f(AXIS_LENGTH + MARKER_OFFSET, i * yPartitionSize, -MARKER_OFFSET);
                        //alony y axis - back face
                        glVertex3f(0.0, i * yPartitionSize, AXIS_LENGTH);
                        glVertex3f(-MARKER_OFFSET, i * yPartitionSize, AXIS_LENGTH + MARKER_OFFSET);
                        glVertex3f(AXIS_LENGTH, i * yPartitionSize, AXIS_LENGTH);
                        glVertex3f(AXIS_LENGTH + MARKER_OFFSET, i * yPartitionSize, AXIS_LENGTH + MARKER_OFFSET);
                    }
                }

                //z component marker lines
                glColor3f(1.0, 0.0, 0.0); //red
                //along z axis - left side
                for (int i = 1; i < zPartitions; i++) {
                    glVertex3f(0.0, 0.0, i * zPartitionSize);
                    glVertex3f(-MARKER_OFFSET, -MARKER_OFFSET, i * zPartitionSize);
                }
                if (boundingAxesOn) {
                    for (int i = 1; i < zPartitions; i++) {
                        glVertex3f(0.0, AXIS_LENGTH, i * zPartitionSize);
                        glVertex3f(-MARKER_OFFSET, AXIS_LENGTH + MARKER_OFFSET, i * zPartitionSize);
                        //along z axis - right size
                        glVertex3f(AXIS_LENGTH, 0.0, i * zPartitionSize);
                        glVertex3f(AXIS_LENGTH + MARKER_OFFSET, -MARKER_OFFSET, i * zPartitionSize);
                        glVertex3f(AXIS_LENGTH, AXIS_LENGTH, i * zPartitionSize);
                        glVertex3f(AXIS_LENGTH + MARKER_OFFSET, AXIS_LENGTH + MARKER_OFFSET, i * zPartitionSize);
                    }
                }
            }
        }

        if (planeReferenceOn) {  //bottom icmp plane
            if (primaryAxesOn) {
                glColor3f(0.5, 0.5, 0.5); //50% grey
                for (int i = 1; i < xPartitions; i++) {  //front
                    glVertex3f(i * xPartitionSize, VERICAL_PLANE_OFFSET, 0.0);
                    glVertex3f(i * xPartitionSize, VERICAL_PLANE_OFFSET, -MARKER_LENGTH);
                }
                for (int i = 1; i < zPartitions; i++) {  //left
                    glVertex3f(0.0, VERICAL_PLANE_OFFSET, i * zPartitionSize);
                    glVertex3f(-MARKER_LENGTH, VERICAL_PLANE_OFFSET, i * zPartitionSize);
                }
                if (boundingAxesOn) {
                    for (int i = 1; i < zPartitions; i++) {  //right
                        glVertex3f(AXIS_LENGTH, VERICAL_PLANE_OFFSET, i * zPartitionSize);
                        glVertex3f(AXIS_LENGTH + MARKER_LENGTH, VERICAL_PLANE_OFFSET, i * zPartitionSize);
                    }
                    for (int i = 1; i < zPartitions; i++) {  //back
                        glVertex3f(i * zPartitionSize, VERICAL_PLANE_OFFSET, AXIS_LENGTH);
                        glVertex3f(i * zPartitionSize, VERICAL_PLANE_OFFSET, AXIS_LENGTH + MARKER_LENGTH);
                    }
                }
            }
        }

        glEnd();

    }

    glEndList();

}


void GLVisWidget::compileReferenceGridList() {

    //REFERENCE GRID CREATION -------------

    //precaculated values
    double xPartitionSize = AXIS_LENGTH / xPartitions;
    double yPartitionSize = AXIS_LENGTH / yPartitions;
    double zPartitionSize = AXIS_LENGTH / zPartitions;

    glNewList(referenceGridList, GL_COMPILE);

    if (referenceGridFrontOn || referenceGridBackOn || referenceGridLeftOn
        || referenceGridRightOn || referenceGridTopOn || referenceGridBottomOn
        || referenceGridPlaneOn) {
        glEnable(GL_BLEND); //grid lines may be transparent

        glColor4f(0.5, 0.5, 0.5, gridTransparency);

        glBegin(GL_LINES);

        if (cubeReferenceOn) {
            if (referenceGridFrontOn) {  //front grid lines
                //horrizontal lines
                for (int i = 1; i < yPartitions; i++) {
                    glVertex3f(0.0, i * yPartitionSize, 0.0);
                    glVertex3f(AXIS_LENGTH, i * yPartitionSize, 0.0);
                }
                //vertical lines
                for (int i = 1; i < xPartitions; i++) {
                    glVertex3f(i * xPartitionSize, 0.0, 0.0);
                    glVertex3f(i * xPartitionSize, AXIS_LENGTH, 0.0);
                }
            }

            if (referenceGridBackOn) {  //back grid lines
                //horrizontal lines
                for (int i = 1; i < yPartitions; i++) {
                    glVertex3f(0.0, i * yPartitionSize, AXIS_LENGTH);
                    glVertex3f(AXIS_LENGTH, i * yPartitionSize, AXIS_LENGTH);
                }
                //vertical lines
                for (int i = 1; i < xPartitions; i++) {
                    glVertex3f(i * xPartitionSize, 0.0, AXIS_LENGTH);
                    glVertex3f(i * xPartitionSize, AXIS_LENGTH, AXIS_LENGTH);
                }
            }

            if (referenceGridLeftOn) {  //left grid lines
                //horrizontal lines
                for (int i = 1; i < yPartitions; i++) {
                    glVertex3f(0.0, i * yPartitionSize, 0.0);
                    glVertex3f(0.0, i * yPartitionSize, AXIS_LENGTH);
                }
                //vertical lines
                for (int i = 1; i < zPartitions; i++) {
                    glVertex3f(0.0, 0.0, i * zPartitionSize);
                    glVertex3f(0.0, AXIS_LENGTH, i * zPartitionSize);
                }
            }

            if (referenceGridRightOn) {  //right grid lines
                //horrizontal lines
                for (int i = 1; i < yPartitions; i++) {
                    glVertex3f(AXIS_LENGTH, i * yPartitionSize, 0.0);
                    glVertex3f(AXIS_LENGTH, i * yPartitionSize, AXIS_LENGTH);
                }
                //vertical lines
                for (int i = 1; i < zPartitions; i++) {
                    glVertex3f(AXIS_LENGTH, 0.0, i * zPartitionSize);
                    glVertex3f(AXIS_LENGTH, AXIS_LENGTH, i * zPartitionSize);
                }
            }

            if (referenceGridTopOn) {  //top grid lines
                //lines across x - i.e. parrallel to z
                for (int i = 1; i < xPartitions; i++) {
                    glVertex3f(i * xPartitionSize, AXIS_LENGTH, 0.0);
                    glVertex3f(i * xPartitionSize, AXIS_LENGTH, AXIS_LENGTH);
                }
                //lines down z
                for (int i = 1; i < zPartitions; i++) {
                    glVertex3f(0.0, AXIS_LENGTH, i * zPartitionSize);
                    glVertex3f(AXIS_LENGTH, AXIS_LENGTH, i * zPartitionSize);
                }
            }

            if (referenceGridBottomOn) {  //bottom grid lines
                //lines across x
                for (int i = 1; i < xPartitions; i++) {
                    glVertex3f(i * xPartitionSize, 0.0, 0.0);
                    glVertex3f(i * xPartitionSize, 0.0, AXIS_LENGTH);
                }
                //lines down z
                for (int i = 1; i < zPartitions; i++) {
                    glVertex3f(0.0, 0.0, i * zPartitionSize);
                    glVertex3f(AXIS_LENGTH, 0.0, i * zPartitionSize);
                }
            }
        }

        if (planeReferenceOn) {
            if (referenceGridPlaneOn) {  //plane grid lines for icmp plane
                //lines across x
                for (int i = 1; i < xPartitions; i++) {
                    glVertex3f(i * xPartitionSize, VERICAL_PLANE_OFFSET, 0.0);
                    glVertex3f(i * xPartitionSize, VERICAL_PLANE_OFFSET, AXIS_LENGTH);
                }
                //lines down z
                for (int i = 1; i < zPartitions; i++) {
                    glVertex3f(0.0, VERICAL_PLANE_OFFSET, i * zPartitionSize);
                    glVertex3f(AXIS_LENGTH, VERICAL_PLANE_OFFSET, i * zPartitionSize);
                }
            }
        }

        glEnd();

        glDisable(GL_BLEND);

    }

    glEndList();

}


void GLVisWidget::checkControlBounds() {
    //- put restrictions on scaling so the user cannot loose sight of the cube
    //- the limit is 0.5 - a zoom out, and 5.0 - a zoom in
#define MIN_SCALE 0.5
#define MAX_SCALE 5.0
    if (scale > MIN_SCALE && scale < MAX_SCALE)
        resScale = scale;
    else
        scale = resScale; //don't let scale run away, similar to offset issue
    //- put restrictions on movement so the user cannot loose sight of the cube
    //- amplify the offset 20X, else movement is too slow
    //- the limit is TRANS_LIMIT in any direction
#define TRANS_LIMIT 55.0
    if (offsetx > -TRANS_LIMIT && offsetx < TRANS_LIMIT)
        resOffsetx = offsetx;
    else
        offsetx = resOffsetx; //keep the mouse movement correlating with the
    //restriction of the movement in the display. As soon as user moves the
    //mouse back, it will immediately affect the view - otherwise offsetx
    //can get far public:beyond 100, and will be unresponsive untill it gets back
    //in the restricted range
    if (offsety > -TRANS_LIMIT && offsety < TRANS_LIMIT)
        resOffsety = offsety;
    else
        offsety = resOffsety; //dito, as for offsetx
#define Z_TRANS_LIMIT 100.0
    if (offsetz > -Z_TRANS_LIMIT && offsetz < Z_TRANS_LIMIT)
        resOffsetz = offsetz;
    else
        offsetz = resOffsetz; //dito, as for offsetx

}

void GLVisWidget::mouseMoveEvent(QMouseEvent *ev) {

    //get the current mouse coordinates
    int x = ev->x();
    int y = ev->y();

    // No change
    if (mouseox < 0) {
        mouseox = x;
        mouseoy = y;
        return;
    }

    double diffx = (double) (x - mouseox) / (double) (width);
    double diffy = (double) (y - mouseoy) / (double) (height);
    mouseox = x;
    mouseoy = y;

    switch (ev->buttons()) {
        //depending on mouse acction, update dynamic transformation variables

#define ROTATION_RATIO 180.0
#define SCALE_EXPONENT_RATIO 4.0
#define OFFSET_MOVE_RATIO 133.0

        case Qt::LeftButton:
            rotx += (diffx * ROTATION_RATIO);
            roty += (diffy * ROTATION_RATIO);
            break;
        case Qt::MidButton:
            scale *= pow(2.0, diffx * SCALE_EXPONENT_RATIO);
            break;
        case Qt::RightButton:
            offsetx += (OFFSET_MOVE_RATIO * diffx);
            offsety -= (OFFSET_MOVE_RATIO * diffy);
            break;
            //for completness, handle all other events by doing nothing
        default:
            break;
    }

    checkControlBounds();
    //update(); //else framerate increases
}

void GLVisWidget::mouseReleaseEvent(QMouseEvent *ev) {
    mouseox = -1;
    mouseoy = -1;
}


void GLVisWidget::wheelEvent(QWheelEvent *ev) {
    //ev->accept();

    //get the delta change
#define DELTA_RATIO 0.041666667
    //5*(1/120) for the resolution at which values are returned from the wheel event
    offsetz -= DELTA_RATIO * ev->delta();

    checkControlBounds();
}


void GLVisWidget::keyPressEvent(QKeyEvent *ev) {
    switch (ev->key()) {
        case Qt::Key_Plus: //zoom in
            scale += 0.01;
            checkControlBounds();
            break;
        case Qt::Key_Minus: //zoom out
            scale -= 0.01;
            checkControlBounds();
            break;
        case Qt::Key_Left: // left arrow
            if (ev->modifiers() & Qt::ControlModifier) {
                offsetx -= 0.5;
                checkControlBounds();
            } else
                rotx += 1.0;
            break;
        case Qt::Key_Right: //down arrow
            if (ev->modifiers() & Qt::ControlModifier) {
                offsetx += 0.5;
                checkControlBounds();
            } else
                rotx -= 1.0;
            break;
        case Qt::Key_Up: //up arrow
            if (ev->modifiers() & Qt::ControlModifier) {
                offsety += 0.5;
                checkControlBounds();
            } else if (ev->modifiers() & Qt::AltModifier) {
                offsetz -= 0.5;
                checkControlBounds();
            } else
                roty += 1.0;
            break;
        case Qt::Key_Down: //right arrow
            if (ev->modifiers() & Qt::ControlModifier) {
                offsety -= 0.5;
                checkControlBounds();
            } else if (ev->modifiers() & Qt::AltModifier) {
                offsetz += 0.5;
                checkControlBounds();
            } else
                roty -= 1.0;
            break;
            
        case Qt::Key_QuoteLeft:  //Toggle Harlem
            harlemToggle = !harlemToggle;
            break;
            
        case Qt::Key_1:  // Rotation Toggle
            rotateToggle = !rotateToggle;
            break;
            
        case Qt::Key_2:  // Decrease Rotation
            rotateAmount -= 0.01;
            break;
            
        case Qt::Key_3: //Increase Rotation
            rotateAmount += 0.01;
            break;

        case Qt::Key_9: //Turn on the FPS display
            toggleDisplayFramerate();
            break;

        case Qt::Key_Z: // take single frame snapshot
            //captureSingleFrame();
            captureCurrentFrame();
            break;
        case Qt::Key_Space: // pause / play
            emit togglePlayPause();
            break;
    }
}

void GLVisWidget::keyReleaseEvent(QKeyEvent *ev) {
    switch (ev->key()) {
        case Qt::Key_F: //front view and full screen
            if (ev->modifiers() & Qt::ControlModifier) //ctrl F
            {
                setFrontView();
            } else // just F //toggle full screen
            {
                if (parentWidget->isFullScreen())
                    setFullScreen(false);
                else
                    setFullScreen(true);
            }
            break;
        case Qt::Key_L:
            if (ev->modifiers() & Qt::ControlModifier) //ctrl L
            {
                setLeftView();
            }
            break;
        case Qt::Key_T:
            if (ev->modifiers() & Qt::ControlModifier) //ctrl T
            {
                setTopView();
            }
            break;
        case Qt::Key_B:
            if (ev->modifiers() & Qt::ControlModifier) //ctrl B
            {
                setBottomView();
            }
            break;
        case Qt::Key_Home: //reset view
            setFrontView();
            break;
        case Qt::Key_End: //reset view
            setBackView();
            break;
        case Qt::Key_Escape: //exit full screen
            if (parentWidget->isFullScreen())
                setFullScreen(false);
            break;
        case Qt::Key_C: // Control Panel
            emit showControlPanel();
            break;
        case Qt::Key_P: // Plotter Settings
            emit showPlotterSettings();
            break;
        case Qt::Key_R: // Reference Frame Settings
            if (ev->modifiers() & Qt::ControlModifier) //ctrl R
            {
                setRightView();
            } else
                emit showReferenceFrameSettings();
            break;
        case Qt::Key_G: // General Settings
            emit showGeneralSettings();
            break;
        case Qt::Key_H: // toggle hide home range
            if (ev->modifiers() & Qt::ControlModifier) //ctrl H
            {
                if (hideHomeRange) {
                    hideHomeRangeLabels(false);
                    strX0 = homeRange0;
                    strX1 = homeRange1;
                } else {
                    hideHomeRangeLabels(true);
                    strX0 = CLOAKED_X0;
                    strX1 = CLOAKED_X1;
                }
            }
            break;
        case Qt::Key_0: // Reset the visualisation plane
            emit resetGLVisWidget();
        break;
    case Qt::Key_Q: // Ctrl + Q override to close the window
        if (ev->modifiers() & Qt::ControlModifier) {
            parentWidget->close();
            emit closeAllWindows();
            break;
        }
    }
}


void GLVisWidget::checkFrameCaptureBufferSize() {
    //ensure capture buffer is appropriate size, or decrease it if its
    //wasting too much memory

    int requiredSize = width * height * 3;

    if (captureBufferSize <= requiredSize
        || captureBufferSize >= (int) (CAPTURE_BUFFER_DECREASE_FACTOR * requiredSize)) {
        //reallocate memory as needed allowing a cussion for small expansion or
        //decrease
        if (captureBuffer != NULL) {
            delete[] captureBuffer;
        }
        captureBufferSize = (int) (CAPTURE_BUFFER_INCREASE_FACTOR * requiredSize);
        captureBuffer = new char[captureBufferSize];

#ifdef DEBUG_FRAME_CAPTURE
        cerr << "DEBUG: Resize capture buffer\n";
        cerr << "   width = " << this->size().width() << ", height = "
             << this->size().height() << "\n";
        cerr << "   Required size  = " << requiredSize << "\n";
        cerr << "   Current size = " << captureBufferSize << "\n";
#endif
    }
}


void GLVisWidget::setPerspectiveProjection() {
    projectionMode = GLVW_PERSPECTIVE;
    updateProjection();
    //update();

}


void GLVisWidget::setOrthographicProjection() {
    projectionMode = GLVW_ORTHOGRAPHIC;
    updateProjection();
    //update();

}


void GLVisWidget::setFrontView() {
    //front view standard scale
    scale = 1.0;
    resScale = 1.0;
    rotx = 0.0;
    roty = 0.0;
    offsetx = 0.0;
    offsety = 0.0;
    offsetz = 0.0;
    checkControlBounds();

}


void GLVisWidget::setBackView() {
    //front view standard scale
    scale = 1.0;
    resScale = 1.0;
    rotx = 180.0;
    roty = 0.0;
    offsetx = 0.0;
    offsety = 0.0;
    offsetz = 0.0;
    checkControlBounds();

}


void GLVisWidget::setLeftView() {
    //fleft side view standard scale
    scale = 1.0;
    resScale = 1.0;
    rotx = -90.0; //270
    roty = 0.0;
    offsetx = 0.0;
    offsety = 0.0;
    offsetz = 0.0;
    checkControlBounds();

}


void GLVisWidget::setRightView() {
    //fleft side view standard scale
    scale = 1.0;
    resScale = 1.0;
    rotx = 90.0;
    roty = 0.0;
    offsetx = 0.0;
    offsety = 0.0;
    offsetz = 0.0;
    checkControlBounds();

}


void GLVisWidget::setTopView() {
    //fleft side view standard scale
    scale = 1.0;
    resScale = 1.0;
    rotx = 0.0;
    roty = -90.0; //270
    offsetx = 0.0;
    offsety = -10.0;
    offsetz = 0.0;
    checkControlBounds();

}


void GLVisWidget::setBottomView() {
    //fleft side view standard scale
    scale = 1.0;
    resScale = 1.0;
    rotx = 0.0;
    roty = 90.0;
    offsetx = 0.0;
    offsety = -10.0;
    offsetz = 0.0;
    checkControlBounds();

}


void GLVisWidget::setReferenceFrame(bool primAxes, bool boundAxes, bool cube,
                                    bool plane, bool markers) {
    primaryAxesOn = primAxes;
    boundingAxesOn = boundAxes;
    cubeReferenceOn = cube;
    planeReferenceOn = plane;
    referenceMarkersOn = markers;
    //recreate lists
    glDeleteLists(referenceAxesList, 0);
    compileReferenceAxesList();
    glDeleteLists(referenceMarkersList, 0);
    compileReferenceMarkersList();
    //update();
    glDeleteLists(referenceGridList, 0);
    compileReferenceGridList();

}


void GLVisWidget::displayAxisLabels(bool axisOn) {
    axisLabelsOn = axisOn;

}


void GLVisWidget::displayDateTime(bool timeOn) {
    dateTimeDisplayOn = timeOn;

}


void GLVisWidget::setDateTimeReference(QDateTime *dt) {
    currentTime = dt;

}


void GLVisWidget::toggleDisplayFramerate() {
    // If the text is already on, switch it off,
    // otherwise, if its off, switch it on.
    if (fpsTextOn) {
        displayFramerate(false);
    } else {
        displayFramerate(true);
    }
}

void GLVisWidget::displayFramerate(bool fpsOn) {
    if (fpsTextOn == fpsOn) {
        return; //state already set
    }

    if (fpsOn) {  //setup framerate calculation values
        fps = 0;
        fpsFrameCount = 0;
        fpsTimer->start(fpsSampleRate);
        fpsText = "fps: calc...";
        fpsTextOn = true;
    } else {
        fpsTextOn = false;
        fpsTimer->stop();
        fps = 0; //to indicate its not being measured
        fpsText = "fps: off";
    }

}

void GLVisWidget::setXAxisLabels(const QString x0, const QString x1) {
    homeRange0 = x0; //line return to position below port lable
    homeRange1 = x1;

    if (hideHomeRange) {
        strX0 = CLOAKED_X0;
        strX1 = CLOAKED_X1;
    } else {
        strX0 = homeRange0; //line return to position below port lable
        strX1 = homeRange1;
    }
}

void GLVisWidget::setYAxisLabels(const QString y0, const QString y1) {
    strY0 = y0;
    strY1 = y1;
}

void GLVisWidget::setZAxisLabels(const QString z0, const QString z1) {
    strZ0 = z0;
    strZ1 = z1;
}

void GLVisWidget::hideHomeRangeLabels(bool on) {
    if (hideHomeRange != on)
        hideHomeRange = on;
}

void GLVisWidget::setGridLines(bool front, bool back, bool left, bool right,
                               bool top, bool bottom, bool plane, float transparency) {
    referenceGridFrontOn = front;
    referenceGridBackOn = back;
    referenceGridLeftOn = left;
    referenceGridRightOn = right;
    referenceGridTopOn = top;
    referenceGridBottomOn = bottom;
    referenceGridPlaneOn = plane;
    gridTransparency = transparency;
    //recreate list
    glDeleteLists(referenceGridList, 0);
    compileReferenceGridList();
    //update();

#ifdef DEBUG_REF_FRAME_GRID
    cerr << "DEBUG: reference frame grid set:\n";
    cerr << "   front=" << referenceGridFrontOn << ", back="
         << referenceGridBackOn << ", left=" << referenceGridLeftOn
         << ", right=" << referenceGridRightOn << ", top="
         << referenceGridTopOn << ", bottom=" << referenceGridBottomOn
         << ", plane=" << referenceGridPlaneOn << endl;
    cerr << "   transparency=" << gridTransparency << endl;
#endif

}

void GLVisWidget::setPartitions(int x, int y, int z) {
    xPartitions = x;
    yPartitions = y;
    zPartitions = z;
    //recreate list
    glDeleteLists(referenceGridList, 0);
    compileReferenceGridList();
    glDeleteLists(referenceMarkersList, 0);
    compileReferenceMarkersList();
    //update();

}

void GLVisWidget::setPointSize(int size) {
    globalPointSize = size;
    glPointSize(globalPointSize);
    //update();

    //HACK
    //bug resolve - if point bulge enabled, changes to point size in static
    //render mode does not relect since the display list is not updated - so
    //bug occurs in paused mode. Need to trigger regeneration of point list.
    // TODO: Commented this out, added line without trailing ;, investigate..
    //if (dataProcessor->getState() != PLAYING);
    if (dataProcessor->getState() != PLAYING) {
        dataProcessor->generatePointDispList();
    }
}

void GLVisWidget::enablePointSmoothing(bool on) {
    if (on == pointSmoothing)
        return; //no state change

    if (on) {
        pointSmoothing = true;
        blending = true; //else points wont be smoothed
        glEnable(GL_POINT_SMOOTH);
    } else {
        pointSmoothing = false;
        glDisable(GL_POINT_SMOOTH);
        //check if blending can be disabled to improve performance
        if (transDecay)
            blending = true; //else points wont be transparent
        else
            blending = false;
    }
}

void GLVisWidget::enableTransDecay(bool on) {
    if (on == transDecay)
        return; //no state change

    if (on) {
        transDecay = true;
        blending = true; //else points wont be transparent
    } else {
        transDecay = false;
        //check if blending can be disabled to improve performance
        if (pointSmoothing)
            blending = true; //else points wont be smoothed
        else
            blending = false;
    }
}


void GLVisWidget::captureFrames(bool on) {
    QDir dir; //to mainpulate dirs

    //avoid repeating a currenlty activate state
    if (captureFrameToFile == on) {
        if (captureFrameToFile) {
            cerr << "ERROR: already capturing frames\n";
        } else {
            cerr << "ERROR: was not capturing frames, already set to off\n";
        }
        return;
    }

    if (on) {  //reset frame counter
        recFrameCount = 0;
        //check that capture buffer is large enough
        checkFrameCaptureBufferSize();
        //setup directory to save to
        if (dataProcessor->getMode() == REPLAY_FILE) {  //create first sub dir bassed on replay file name
            QString replayFileName = dataProcessor->getReplayFileName();
            frameCaptureDir = QString(DataProcessor::getFramesDir()) + QString("/")
                              + QString(DataProcessor::getReplaySubdir()) + QString("/")
                              + replayFileName;
            if (!dir.exists(frameCaptureDir)) {
                if (!dir.mkdir(frameCaptureDir)) {
                    QString errMsg = QString("could not create subdirectory \"");
                    errMsg += frameCaptureDir;
                    errMsg += "\" for frame dumps";
                    dataProcessor->reportError(errMsg,
                                               "GLVisWidget::capturetFrames()");
                    return;
                }
            }
            //create second subdir bassed on replay time
            frameCaptureDir += QString("/")
                               + dataProcessor->getReplayPosition()->toString("yyyyMMdd-hhmmss");
            if (!dir.exists(frameCaptureDir)) {
                if (!dir.mkdir(frameCaptureDir)) {
                    QString errMsg = QString("could not create subdirectory \"");
                    errMsg += frameCaptureDir;
                    errMsg += "\" for frame dumps";
                    dataProcessor->reportError(errMsg,
                                               "GLVisWidget::capturetFrames()");
                    return;
                }
            }
        } else if (dataProcessor->getMode() == MONITOR_LOCAL) {
            frameCaptureDir = QString(DataProcessor::getFramesDir()) + QString("/")
                              + QString(DataProcessor::getLiveSubdir()) + QString("/")
                              + dataProcessor->getReplayPosition()->toString("yyyyMMdd-hhmmss");
            if (!dir.exists(frameCaptureDir)) {
                if (!dir.mkdir(frameCaptureDir)) {
                    QString errMsg = QString("could not create subdirectory \"");
                    errMsg += frameCaptureDir;
                    errMsg += "\" for frame dumps";
                    dataProcessor->reportError(errMsg,
                                               "GLVisWidget::capturetFrames()");
                    return;
                }
            }
        } else {
            frameCaptureDir = QString(DataProcessor::getFramesDir());
        }
        //init frameFileName reference
        strcpy(frameFileName, frameCaptureDir.toLatin1());
        frameDirCharIndex = frameCaptureDir.length();
        //set capture frames flag
        captureFrameToFile = true;
    } else {
        captureFrameToFile = false;
        //if capturing frames add end time to subdir
        if (frameCaptureDir != QString(DataProcessor::getFramesDir())) {  //was replay file or monitor local mode during frame dump
            dir = QDir(frameCaptureDir);
            //navigate into dir and rename file
            if (dir.exists()) {
                QString subDir = dir.dirName();
                QString newDirName = subDir + QString(" - ")
                                     + dataProcessor->getReplayPosition()->toString("yyyyMMdd-hhmmss");
                QDir navDir = QDir(dataProcessor->strAppDir);
                if (navDir.cd(frameCaptureDir.remove(subDir))) {  //rename file
                    if (!navDir.rename(subDir, newDirName)) {
                        dataProcessor->reportError(
                                "could not rename subdirectory for frame dumps",
                                "GLVisWidget::captureFrames()");
                    }
                } else {
                    dataProcessor->reportError(
                            "could not navigate into sub-directory for frame dumps",
                            "GLVisWidget::captureFrames()");
                }
            } else {
                dataProcessor->reportError(
                        "frame dump directory does not exist",
                        "GLVisWidget::captureFrames()");
            }
        }
    }
}

void GLVisWidget::captureCurrentFrame() {
    if (!captureFrameToFile) {
        //check that capture buffer is large enough
        checkFrameCaptureBufferSize();
        //setup directory to save to
        if (dataProcessor->getMode() == REPLAY_FILE) {
            QString replayFileName = dataProcessor->getReplayFileName();
            frameCaptureDir = QString(DataProcessor::getSnapshotsDir()) + QString("/")
                              + QString(DataProcessor::getReplaySubdir()) + QString("/") + replayFileName;
            //create sub dir
            QDir dir;
            if (!dir.exists(frameCaptureDir)) {
                if (!dir.mkdir(frameCaptureDir)) {
                    dataProcessor->reportError(
                            "could not create subdirectory for frame snapshot file",
                            "GLVisWidget::captureCurrentFrame()");
                    return;
                }
            }
        } else if (dataProcessor->getMode() == MONITOR_LOCAL) {
            frameCaptureDir = QString(DataProcessor::getSnapshotsDir()) + QString("/")
                              + QString(DataProcessor::getLiveSubdir());
        } else {
            frameCaptureDir = QString(DataProcessor::getSnapshotsDir());
        }
        //init frameFileName reference
        strcpy(frameFileName, frameCaptureDir.toLatin1());
        frameDirCharIndex = frameCaptureDir.length();
        //set flags
        captureSingleFrameToFile = true;
        captureFrameToFile = true; //so that the next frame render will be caught
        update(); //update now, so that the current frame is redrawn and captured
        //before more data is processed
    }
    //otherwise already capturing every frames to file, so do nothing
}


void GLVisWidget::calculateFPS() {
    //calculate the fps - note fpsSampleRate specified in msec
    fps = (int) (fpsFrameCount / (fpsSampleRate / 1000));
    //be conservative and truncate value (+ more efficient than rounding off)
    fpsText = "fps: " + QString::number(fps);

    //prepare for next call
    fpsFrameCount = 0; //reset
}

void GLVisWidget::showEvent() {
    activateWindow();

}

void GLVisWidget::setFullScreen(bool on) {
    if (on)
        parentWidget->showFullScreen();
    else
        parentWidget->showNormal();

}

void GLVisWidget::setBackgroundCol(int colCode) {
    if (colCode == BLACK_BACKGROUND) {
        backgroundCol = BLACK_BACKGROUND;
        //set x label colour - blue with 50/100 sat, and 90/100 val -> rgb
        xLabelCol[0] = 0.45;
        xLabelCol[1] = 0.45;
        xLabelCol[2] = 0.9;
        //set y label colour - green with 50/100 sat, and 90/100 val -> rgb
        yLabelCol[0] = 0.45;
        yLabelCol[1] = 0.9;
        yLabelCol[2] = 0.45;
        //set z label colour - red with with 50/100 sat, and 90/100 val -> rgb
        zLabelCol[0] = 0.9;
        zLabelCol[1] = 0.45;
        zLabelCol[2] = 0.45;
        //set other label colour - white
        otherLabelCol[0] = 1.0;
        otherLabelCol[1] = 1.0;
        otherLabelCol[2] = 1.0;
        //set the clearing colour
        glClearColor(0.0, 0.0, 0.0, 0.0); //black
    } else  {
        //white background
        backgroundCol = WHITE_BACKGROUND;
        //set x label colour - blue with 66/100 val, and 90/100 sat -> rgb
        xLabelCol[0] = 0.066;
        xLabelCol[1] = 0.066;
        xLabelCol[2] = 0.695;
        //set y label colour - green with 66/100 val, and 90/100 sat ->rgb
        yLabelCol[0] = 0.066;
        yLabelCol[1] = 0.695;
        yLabelCol[2] = 0.066;
        //set z label colour - red with 66/100 val, and 90/100 sat -> rgb
        zLabelCol[0] = 0.695;
        zLabelCol[1] = 0.066;
        zLabelCol[2] = 0.066;
        //set other label colour - black
        otherLabelCol[0] = 0.0;
        otherLabelCol[1] = 0.0;
        otherLabelCol[2] = 0.0;
        //set the clearing colour
        glClearColor(1.0, 1.0, 1.0, 0.0); //white
    }
}


double GLVisWidget::getGlobalPointSize() {
    return globalPointSize;
}

#pragma once

#include "library.h"

//GLWidget class is inherited of the QGLWidget class to render the oscillograph
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    //Construction and Destruction functions
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    //An override function, which is used to initialize all the parameters for the GL
    void initializeGL();

    //An override function, it is a rendering function which would be called every time the updateGL function is called
    void paintGL();

    //An override function, everytime the size of the rendering interface is changed, this function would be called
    void resizeGL(int width, int height);

public:
    //Initialize some variables in this class
    void initializeValue(double* data[], double* minmaxVal[], int pointsNum, vector<pair<int, string>>& displayChannels);

    //Get the top-left-corner position of different oscillograph
    void makeGeneral(int width, int height);

    //Construct the oscillograph which would show the data of every sensor
    GLuint makeObject();

    //Set the channel in current page, startIndex, and beginChannel of the data which are used to render
    void setParameter(int tabChannelCount, int arrayStartIndex, int beginChannel);

private:

    //The width and height of every quadrilateral
    double quadraWidth, quadraHeight;

    //The object is used to store all the rendering oscillographs on one page
    GLuint object;

    //Store all the rendering data
    double** data;

    //An array stored the minimum and maximum data from every channel's data
    double** minmaxVal;

    //The position of the text appeared on the screen
    GLdouble* textPosition;

    //The position information about every oscillograph
    GLdouble* position;

    //The number of sample points which would show on the screen
    int pointsNum;

    //All the selected channels which are used to render
    vector<pair<int, string>> displayChannels;

    //The rotate angle along the X/Y axis
    int xRot, yRot;

    //The distance the oscillograph has moved along the Z axis
    float moveZ;

    //The increment to moveZ
    float iIncrement;

    //Take a record of the mouse position
    QPoint lastPos;

    //The number of channels displayed on one page
    int tabChannelCount;

    //The startIndex which is used to render
    int arrayStartIndex;

    //The beginning index of the channel which should be displayed in one page
    int beginChannel;

private:

    //Normalize all the angles
    void normalizeAngle(int *angle);

    //Construct a oscillograph from right to left
     /*Parameters:
      *  renderData:         Data used to render on then screen.
      *  start:              The start index of the data used to render
      *  pointsNum:          From the 'start', there are 'pointsNum' new points to render
      *  numPhysicalChannels The number of physical channels on the DAQ
      *  activeChannels:     List of channels that have sensors connected to them. First parameter is the index of the channel in the list of channels on the DAQ.
      *                          The second parameter is the channel ID
      */
     void Oscill(double* renderData, int pointsNum, double coordX, double coordY, double pointXGap, double pointYGap,double minY);

     //Construct the qurdrilateral to display single oscillograph
     void quadra(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4);

};


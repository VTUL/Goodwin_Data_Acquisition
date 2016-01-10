#pragma once

#include "library.h"

//GLStereo class is inherited of the QGLWidget class to render the stereograph
class GLStereo : public QGLWidget
{
    Q_OBJECT

public:
    //Construction and Destruction functions
    explicit GLStereo(QWidget *parent = 0);
    ~GLStereo();

protected:
    //An override function, which is used to initialize all the parameters for the GL
    void initializeGL();

    //The rendering function which would be called every time the updateGL function is called
    void paintGL();

    //Everytime the size of the rendering interface is changed, this function would be called
    void resizeGL(int width, int height);

    //Call the function to respond to the mouse press event
    void mousePressEvent(QMouseEvent *event);

    //Call the function to respond to the mouse move event
    void mouseMoveEvent(QMouseEvent *event);

    //Call the function to respond to the key press event
    void keyPressEvent(QKeyEvent *event);

public slots:
    //Set the rotate angle along the X axis
    void setXRotation(int angle);

    //Set the rotate angle along the Y axis
    void setYRotation(int angle);

public:
    //Initialize some variables in this class
    void initializeValue(double* data[], vector<pair<int, string>>& displayChannels);

    //Set the position of the corners of every background image
    void makeGeneral();

    //Construct the cylinders which show the data of every sensor
    GLuint makeObject();

    //Set the startIndex of the data which is used to render
    void setParameter(int arrayStartIndex);

private:

    //The object is used to store all the rendering cylinders
    GLuint object;

    //The rendering data read from HDF5 file
    double** data;

    //The position of the corners of every background image
    GLdouble* bgPosition;

    //The position of every sensor on the background image
    GLdouble* sensorPosition;

    //All the selected channels
    vector<pair<int, string>> displayChannels;

    //The rotate angle along the X/Y axis
    int xRot, yRot;

    //The distance the stereograph has moved along the X/Y/Z axis
    float moveX, moveY, moveZ;

    //The increment to moveX/moveY/moveZ
    float iIncrement;

    //Take a record of the mouse position
    QPoint lastPos;

    //The ratio of background image = ImageWidth/ImageHeight
    GLdouble imageRatio;

    //The width,height,ratio of rendering screen
    int screenWidth;
    int screenHeight;
    GLdouble screenRatio;

    //The startIndex which is used to render
    int arrayStartIndex;

    //The ID of all the textures
    GLuint texture[5];

    //The GLUquadric class is used to draw cylinders
    GLUquadric* quadric;

    //The number of all the channels
    int displayChannelCount;

    //The color array of all the cylinders
    GLfloat* cylinderColor;

    //Whether the screen has been clicked
    bool isClicked;

    //Whether the position of the clicked point has been calculated
    bool isCalculated;

    //The X/Y position of the mouse
    float mouseX, mouseY;

    //The viewport, modelviewMatrix, projectionMatrix of the rendering inderface
    GLint *viewport;
    GLdouble *modelview;
    GLdouble *projection;

    //Calculated result about the position of the click point
    GLdouble *result;

    //The compared result about the index of selected sensor
    int cResult;

    //The information that should be displayed on the screen
    QString displayInfo;

protected:

    //Load the textures using the background images
    void loadGLTextures();

    //Generate colors for every cylinders
    void generateColor();

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
     void Stereo(double* renderData[], int index);

     //Construct the qurdrilateral to display single background image
     void quadra(GLdouble* vertex, int quadraNum);

     //Draw one circle for every cylinder to cover it
     void DrawCircleArea(float cx, float cy, float cz, float r, int num_segments);

     //Get the viewport, modelviewMatrix, projectionMatrix of the rendering inderface
     void UnProjection();

     //Get the clicked position
     void GetClickPosition(float x, float y);

     //Compare the position with every sensor's position, to get the index of selected sensor
     int ComparePosition();
};


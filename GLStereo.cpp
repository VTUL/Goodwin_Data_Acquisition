
#include "GLStereo.h"

//Construction function
GLStereo::GLStereo(QWidget *parent)
    : QGLWidget(parent)
{
    object = 0;

    //Set the initial value for the parameters used to change the position of the graphics
    xRot = 0;
    yRot = 0;
    iIncrement = 0.05f;
    moveX = 0.0f;
    moveY = 0.0f;
    moveZ = 2.5f;

    isClicked = false;
    isCalculated = true;

    mouseX = 0;
    mouseY = 0;

    cResult = -1;

    //Allocate memory for the four matrices
    viewport = new GLint[4];
    modelview = new GLdouble[16];
    projection = new GLdouble[16];
    result = new GLdouble[3];
}

//Destruction function
GLStereo::~GLStereo()
{
    glDeleteLists(object, 1);

    //Release all the memory allocated in this class
    if(bgPosition != NULL)
    {
        delete []bgPosition;
        bgPosition = NULL;
    }

    if(sensorPosition != NULL)
    {
        delete []sensorPosition;
        sensorPosition = NULL;
    }
    if(cylinderColor != NULL)
    {
        delete []cylinderColor;
        cylinderColor = NULL;
    }
    if(viewport != NULL)
    {
        delete []viewport;
        viewport = NULL;
    }
    if(modelview != NULL)
    {
        delete []modelview;
        modelview = NULL;
    }
    if(projection != NULL)
    {
        delete []projection;
        projection = NULL;
    }
    if(result != NULL)
    {
        delete []result;
        result = NULL;
    }
    makeCurrent();
}

//Initialize the openGL
void GLStereo::initializeGL()
{
    QGLWidget::initializeGL();

    glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculation

    //Use gluNewQuadric to render cylinders
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    //Load the texture as the background of the graphics
    loadGLTextures();

    glEnable(GL_DEPTH_TEST); // enables depth testing
    glDepthFunc(GL_LEQUAL);  // the type of depth test to do

    //Set two light sources
    const GLfloat lightAmbientValue[] = {0.0f, 0.1f, 0.1f, 1.0f};
    const GLfloat lightDiffuseValue[] = {0.6f, 0.6f, 0.6f, 1.0f};
    const GLfloat lightSpecularValue[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat lightDiffusePosition[] = {0.0f, 0.0f, 5.0f, 1.0f};

    const GLfloat lightAmbientValue1[] = {0.0f, 0.1f, 0.1f, 1.0f};
    const GLfloat lightDiffuseValue1[] = {0.6f, 0.6f, 0.6f, 1.0f};
    const GLfloat lightSpecularValue1[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat lightDiffusePosition1[] = {0.0f, 0.0f, -6.0f, 1.0f};

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,  GL_TRUE);
    // Set up lighting
    glLightfv(GL_LIGHT0,  GL_AMBIENT, lightAmbientValue); // Set the ambient lighting for LIGHT0
    glLightfv(GL_LIGHT0,  GL_DIFFUSE, lightDiffuseValue); // Set the diffuse lighting for LIGHT0
    glLightfv(GL_LIGHT0,  GL_SPECULAR, lightSpecularValue);
    glLightfv(GL_LIGHT0,  GL_POSITION, lightDiffusePosition); // Set the position for LIGHT0
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1,  GL_AMBIENT, lightAmbientValue1); // Set the ambient lighting for LIGHT0
    glLightfv(GL_LIGHT1,  GL_DIFFUSE, lightDiffuseValue1); // Set the diffuse lighting for LIGHT0
    glLightfv(GL_LIGHT1,  GL_SPECULAR, lightSpecularValue1);
    glLightfv(GL_LIGHT1,  GL_POSITION, lightDiffusePosition1); // Set the position for LIGHT0
    glEnable(GL_LIGHT1);

    glDisable( GL_LIGHTING); // Enable Lighting

    // Used blending function based On source alpha value
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    //Enable the material for the cylinders, to make them more realistic
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial( GL_FRONT_AND_BACK,  GL_AMBIENT_AND_DIFFUSE);

    //Set the value for cylinder material
    const GLfloat matAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    const GLfloat matDiffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
    const GLfloat matSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    const GLfloat matEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat matshinniess = 50.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matshinniess);
}

//Using this function to render graphics on the screen
void GLStereo::paintGL()
{
    //Clear the color and depth buffer leaving from last frame, to draw a new frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Load the matrix
    glLoadIdentity();

    //Move the graph to an specified position
    glTranslated(-moveX, -moveY, -moveZ);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);

    //Get the viewport, modelviewMatrix, projectionMatrix of the rendering inderface
    UnProjection();

    glDisable(GL_LIGHTING); // Since we use blending, we disable Lighting
    glDisable(GL_DEPTH_TEST);  // Enable depth testing
    glEnable(GL_BLEND); //Enable the blending rendering
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Blending based on source alpha and 1 minus dest alpha

    //Render the background quadrilateral
    quadra(bgPosition, 5);

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);  // Enable lighting
    glEnable(GL_DEPTH_TEST);  // Enable depth testing

    //Render the cylinders
    object = makeObject();
    glCallList(object);

    //Click th mouse, get the space coordinate according to the cursor's position, compare with the location of the sensors, get which sensor is selected and show the information
//    if(isClicked)
//    {
//        if(!isCalculated)
//        {
//            GetClickPosition(mouseX, mouseY);
//            cResult = ComparePosition();
//            isCalculated = true;
//        }
//        if(cResult > -1)
//        {
//            int channelID = displayChannels.at(cResult).first;
//            string channelName = displayChannels.at(cResult).second;
//            displayInfo = QString::fromStdString(channelName) + QString::fromStdString("    ") + QString::number(data[channelID][arrayStartIndex]);
//            renderText(screenWidth - TAB_FONTSIZE * 8, TAB_FONTSIZE, displayInfo);
//        }
//    }
}

//If the size of the screen is modified, we use this function to reset some parameters
void GLStereo::resizeGL(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    screenRatio = (GLdouble)width/(GLdouble)height;
    // set the viewport
    glViewport(0, 0, (GLint)width, (GLint)height);
    // select projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Set the modelview matrix
    gluPerspective(45.0, screenRatio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    isClicked = false;
}

//Initialize some variables in this class
void GLStereo::initializeValue(double* data[], vector<pair<int, string>>& displayChannels)
{
    //Set the rending data and channels
    this->data = data;
    this->displayChannels = displayChannels;

    //Get the number of all the channels
    displayChannelCount = displayChannels.size();

    //Allocate a memory space for the background images' positions
    bgPosition = new GLdouble[5 * 4 * 3];

    //Allocate a memory space to save the color of all the cylinders
    cylinderColor = new GLfloat[displayChannelCount * 3];
    //Generate the color for every cylinder and store them in cylinderColor
    generateColor();

    //Allocate and initialize a memory space for the position of all the sensors
    sensorPosition = new GLdouble[displayChannelCount * 3];
    memset(sensorPosition, 0, displayChannelCount * 3 * sizeof(GLdouble));

    //Read a csv file to get the distribution of all the sensors
    QFile file(SENSOR_DISTRIBUTION_FILE);
    if(!file.open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this,"Error", "Cannot open file storing the sensor distribution information!");
        return;
    }
    QStringList list;
    list.clear();
    QTextStream in(&file);

    int i = 0, j = 0;
    QString channelName;
    string strChannal;
    double prevCoordX = 0, prevCoordY = 0;
    double xCoord, yCoord, zCoord;

    //Get the ratio of the texture image
    imageRatio = (GLdouble)IMAGE_WIDTH / (GLdouble)IMAGE_HEIGHT;

    //Iteration in this csv file
    while(!in.atEnd())
    {
        //Get every line from the csv file
       QString fileLine = in.readLine();
       list = fileLine.split(",", QString::SkipEmptyParts);

       //distribution information

       if(i > 0)
       {
           //Get every channel's name information
           channelName = list.at(0);
           xCoord = atoi(list.at(1).toStdString().c_str());
           if(xCoord >= 0)
           {
               //Compare the channel's name in this csv file with all the channels'name in a global vector, to get the index of this channel in the vector
               for(j = 0; j < displayChannelCount; j++)
               {
                   strChannal = displayChannels.at(j).second;
                   if(strChannal == channelName.toStdString())
                   {
                       yCoord = atoi(list.at(2).toStdString().c_str());
                       zCoord = atoi(list.at(3).toStdString().c_str());
                       sensorPosition[j * 3] = (xCoord * 2 - IMAGE_WIDTH) /IMAGE_HEIGHT;
                       sensorPosition[j * 3 + 1] = (IMAGE_HEIGHT - yCoord * 2) /IMAGE_HEIGHT;
                       sensorPosition[j * 3 + 2] = zCoord - 5;
                       prevCoordX = xCoord; prevCoordY = yCoord;
                       break;
                   }
               }
           }
       }
       i++;
    }
    list.clear();
    file.close();

    makeGeneral();
}

//Generate color for every cylinders
void GLStereo::generateColor()
{
    int i, index;
    srand((unsigned)time(NULL));
    for(i = 0; i < displayChannelCount; i++)
    {
        index = i * 3;
        cylinderColor[index] = (GLfloat)i / (GLfloat)displayChannelCount;   //R
        cylinderColor[index + 1] = (GLfloat)(displayChannelCount - i) / (GLfloat)displayChannelCount;   //G
        cylinderColor[index + 2] = (GLfloat)rand()/65536;   //B
    }
}

//Set the position information for all the background images
void GLStereo::makeGeneral()
{
    int i, index;

    for(i = 0; i < 5; i++)
    {
        index = i * 12;
        bgPosition[index] = -imageRatio; bgPosition[index + 1] = -1; bgPosition[index + 2] = i - 4;
        bgPosition[index + 3] = -imageRatio; bgPosition[index + 4] = 1; bgPosition[index + 5] = i - 4;
        bgPosition[index + 6] = imageRatio; bgPosition[index + 7] = 1; bgPosition[index + 8] = i - 4;
        bgPosition[index + 9] = imageRatio; bgPosition[index + 10] = -1; bgPosition[index + 11] = i - 4;
    }
}

//Set the arrayStartIndex
void GLStereo::setParameter(int arrayStartIndex)
{
    this->arrayStartIndex = arrayStartIndex;
}

//Construct the GL object for rendering
GLuint GLStereo::makeObject()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    Stereo(data, arrayStartIndex);

    glEndList();
    return list;
}

//Construct cylinders to show the data from every sensor
void GLStereo::Stereo(double* renderData[], int arrayIndex)
{
    int i, index;

    int channelID;
    string channelName;
    double scaledHeight;
    for(i = 0; i < displayChannelCount; i++)
    {
        index = i * 3;
        //If the position information of the sensor is valid
        if(sensorPosition[index] != 0 && sensorPosition[index + 1] != 0)
        {
            //Get the channel information
            channelID = displayChannels.at(i).first;
            channelName = displayChannels.at(i).second;
            //Get the current sensor data * DISPLAY_SCALER
            scaledHeight = renderData[channelID][arrayIndex] * DISPLAY_SCALER;

            //The position of every cylinder should be stored in a stack
            glPushMatrix();

            //Move every cylinder to the right position
            glTranslatef(sensorPosition[index], sensorPosition[index + 1], sensorPosition[index + 2]);
            glColor4f(cylinderColor[index], cylinderColor[index + 1], cylinderColor[index + 2], 0.9f);
            gluCylinder(quadric, CYLINDER_RADIUS, CYLINDER_RADIUS, scaledHeight, 20, 20); // draw cylinder
            DrawCircleArea(0.0, 0.0, scaledHeight, CYLINDER_RADIUS, 20);

            glPopMatrix();
        }
    }
}

//Construct a circle for every cylinder
void GLStereo::DrawCircleArea(float cx, float cy, float cz, float r, int num_segments)
{
    GLfloat vertex[4];

    //Get the angle of every segment
    const GLfloat delta_angle = 2.0*M_PI/num_segments;
    glBegin(GL_TRIANGLE_FAN);

    vertex[0] = cx;
    vertex[1] = cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);

    //draw the vertex on the contour of the circle
    for(int i = 0; i < num_segments ; i++)
    {
        vertex[0] = std::cos(delta_angle*i) * r + cx;
        vertex[1] = std::sin(delta_angle*i) * r + cy;
        vertex[2] = cz;
        vertex[3] = 1.0;
        glVertex4fv(vertex);
    }

    vertex[0] = 1.0 * r + cx;
    vertex[1] = 0.0 * r + cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
    glEnd();
}

//Constructing a quadrilateral, using the coordinates of four vertex of the quadrilateral
void GLStereo::quadra(GLdouble* vertex, int quadraNum)
{
    int i, index;
    for(i = 0; i < quadraNum; i++)
    {
        index = i * 12;
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        //The graph is a quadrilateral
        glBegin(GL_QUADS);

        glColor4f(0.5f, 0.5f, 0.5f, 0.8f);
        glTexCoord2d(0.0f, 0.0f); glVertex3d(vertex[index], vertex[index + 1], vertex[index + 2]);
        glTexCoord2d(0.0f, 1.0f); glVertex3d(vertex[index + 3], vertex[index + 4], vertex[index + 5]);
        glTexCoord2d(1.0f, 1.0f); glVertex3d(vertex[index + 6], vertex[index + 7], vertex[index + 8]);
        glTexCoord2d(1.0f, 0.0f); glVertex3d(vertex[index + 9], vertex[index + 10], vertex[index + 11]);

        glEnd();
    }
}

//Load the textures
void GLStereo::loadGLTextures()
{
    int i;
    char indexInfo[8];
    //Generate five textures
    glGenTextures(5, texture);

    for(i = 0; i < 5; i++)
    {
        QImage img;
        QImage buf;

        //Get the path of every background image
        sprintf_s(indexInfo, "%i", i + 1);
        string filename = TEXTURE_FILE_PATH;
        filename = filename + indexInfo + ".jpg";

        //Load the image into memory buffer
        if(!buf.load(QString::fromStdString(filename)))
        {
            qWarning( "Could not read image file, using single-color instead." );
            QImage dummy( 128, 128, QImage::Format_RGB32 );
            buf = dummy;
        }
        img = QGLWidget::convertToGLFormat(buf);

        //Bind the textureID with the buffer
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

//Set the rotate angle along the X axis and refresh the screen
void GLStereo::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        updateGL();
    }
}

//Set the rotate angle along the Y axis and refresh the screen
void GLStereo::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        updateGL();
    }
}

//Get the viewport, modelviewMatrix, projectionMatrix of the rendering inderface
void GLStereo::UnProjection()
{
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
}

//Get the space position of the cursor
void GLStereo::GetClickPosition(float x, float y)
{
    float tmpResult[3];
    GLdouble winX = x;
    GLdouble winY = viewport[3] - y;

    glReadPixels((int)winX, (int)winY, screenRatio, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, result);
    GLdouble winZ = result[2];
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &result[0], &result[1], &result[2]);
}

//Compare the clicked position with the position of all the sensor to get corresponding index
int GLStereo::ComparePosition()
{
    int index;
    for(int i = 0; i < displayChannelCount; i++)
    {
        index = i * 3;
        if(abs(sensorPosition[index] - result[0]) < CYLINDER_RADIUS * 10)
        {
            if(abs(sensorPosition[index + 1] - result[1]) < CYLINDER_RADIUS * 10)
            {
                return i;
            }
        }
    }
    return -1;
}

//Call the function to respond to the mouse press event
void GLStereo::mousePressEvent(QMouseEvent *event)
{
    //If we click the left button of the mouse
    if (event->buttons() & Qt::LeftButton)
    {
        //Get the position of the cursor
        mouseX = event->x();
        mouseY = event->y();

        //We could calculate the position of the cursor
        isClicked = true;
        isCalculated = false;
    }
    //If we click the right button of the mouse
    else if (event->buttons() & Qt::RightButton)
    {
        //We disable the calculation of the position of the cursor
        isClicked = false;
    }

    //Save the position of the mouse
    lastPos = event->pos();
}

//Call the function to respond to the mouse move event
void GLStereo::mouseMoveEvent(QMouseEvent *event)
{

    //Get the latest position the mouse
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    //If we click the left button
    if (event->buttons() & Qt::LeftButton)
    {
        //Set the rotation angle
        setXRotation(xRot + 3 * dy);
        setYRotation(yRot + 3 * dx);
    }
    //If we click the right button
    else if (event->buttons() & Qt::RightButton)
    {
        //Set zoom in/out
        if(dy > 0)
            moveZ += iIncrement;
        else
            moveZ -= iIncrement;
        updateGL();//refresh the screen
    }
    //Save the position of the mouse
    lastPos = event->pos();
}

//Call the function to respond to the key press event
void GLStereo::keyPressEvent(QKeyEvent *event)
{
    int keyCode = event->key();
    switch (keyCode)
    {
         case Qt::Key_PageUp:   // zoom-out
             moveZ += iIncrement;
            break;
         case Qt::Key_PageDown: // zoom-in
             moveZ -= iIncrement;
            break;
         case Qt::Key_Up:  //move up
             moveY -= iIncrement;
            break;
         case Qt::Key_Down: //move down
             moveY += iIncrement;
            break;
         case Qt::Key_Left: //move left
             moveX += iIncrement;
            break;
         case Qt::Key_Right: //move right
            moveX -= iIncrement;
            break;
    }
    updateGL();//refresh the screen
}

//Confine the angle in a designated range
void GLStereo::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}


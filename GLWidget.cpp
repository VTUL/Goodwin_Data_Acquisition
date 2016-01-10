
#include "GLWidget.h"

//Construction function
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    object = 0;

    //Initialize the position for the text and the quadrilateral
    textPosition = new GLdouble[TAB_CHANNEL_COUNT * 6];
    position = new GLdouble[TAB_CHANNEL_COUNT * 4];
    
    //Set the incremental coefficient
    iIncrement = 0.05f;
    
    //Set the initial z value
    moveZ = 2.5f;
}

//Destruction
GLWidget::~GLWidget()
{
    glDeleteLists(object, 1);
    //Release the memory used to store the positions of the text and the quadrilateral
    if(textPosition != NULL)
    {
        delete []textPosition;
        textPosition = NULL;
    }
    if(position != NULL)
    {
        delete []position;
        position = NULL;
    }
    makeCurrent();
}

//Initialize the OpenGL
void GLWidget::initializeGL()
{
    //Set the shade model
    glShadeModel(GL_FLAT);

    //Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);

    //Enable the depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

//Using this function to render graph on the screen
void GLWidget::paintGL()
{
    //Clear the color and depth buffer leaving from last frame, to draw a new frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Load the matrix
    glLoadIdentity();

    //Move the graph to a specified position
    glTranslated(0.0f, 0.0f, -moveZ);
    
    //Construct the rendering object
    object = makeObject();
    glCallList(object);
}

//Change the size of the rendering screen
void GLWidget::resizeGL(int width, int height)
{
    //According to the width and height of the screen, set the parameter array
    makeGeneral(width, height);

    //Set the viewport
    glViewport(0, 0, (GLint)width, (GLint)height);
    //Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Set the modelview matrix
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//Initialize the rendering data array, minmaxval array, pointsNum, displayChannels
void GLWidget::initializeValue(double* data[], double* minmaxVal[], int pointsNum, vector<pair<int, string>>& displayChannels)
{
    this->data = data;
    this->minmaxVal = minmaxVal;
    this->pointsNum = pointsNum;
    this->displayChannels = displayChannels;
}

//Set the position of text and quadrilateral
void GLWidget::makeGeneral(int width, int height)
{
    int i, j, tpCount = 0, pCount = 0;

    //The ratio of screen width and height
    double ratio = (double)width / height;
    
    //Set these variables to fulfill the page layout
    double doubleGap = (double)TAB_FONTSIZE / height;

    GLdouble xWidth = 2 * ratio / (TAB_CHANNEL_COL + TAB_CHANNEL_GAP);
    GLdouble yHeight = 2 / (TAB_CHANNEL_ROW + TAB_CHANNEL_GAP);

    GLdouble txWidth = width / (TAB_CHANNEL_COL + TAB_CHANNEL_GAP * 1.7);
    GLdouble tyHeight = height / (TAB_CHANNEL_ROW + TAB_CHANNEL_GAP * 1.7);

    quadraWidth = xWidth - doubleGap * 6;
    quadraHeight = yHeight - doubleGap * 4;

    GLdouble startX, startY;
    GLdouble texStartX = TAB_FONTSIZE * 1.2, texStartY;
    
    for(i = 0; i < TAB_CHANNEL_ROW; i++)
    {
        startX = -ratio + i * xWidth + doubleGap * 5;
        texStartY = TAB_FONTSIZE * 1.8;
        for(j = 0; j < TAB_CHANNEL_COL; j++)
        {
            startY = 1 - j * yHeight - doubleGap * 2;

            //Set the position of the quadrilateral, the range of the whole screen would be x:(-ratio, ratio) y:(-1 1)
            position[pCount] = startX; pCount++;
            position[pCount] = startY; pCount++;
            position[pCount] = position[pCount - 2] + quadraWidth; pCount++;
            position[pCount] = position[pCount - 2] - quadraHeight; pCount++;

            //Set the position of the text on this screen, the range of the whole screen would be x:(0, width) y:(0 height)
            textPosition[tpCount++] = texStartX; textPosition[tpCount++] = texStartY;
            textPosition[tpCount++] = texStartX + TAB_FONTSIZE * 3; textPosition[tpCount++] = texStartY + TAB_FONTSIZE;
            textPosition[tpCount++] = texStartX + txWidth; textPosition[tpCount++] = texStartY + tyHeight - TAB_FONTSIZE;

            texStartY += tyHeight;
        }
        texStartX += txWidth;
    }
}

//Set the updatable parameters
void GLWidget::setParameter(int tabChannelCount, int arrayStartIndex, int beginChannel)
{
    this->tabChannelCount = tabChannelCount;
    this->arrayStartIndex = arrayStartIndex;
    this->beginChannel = beginChannel;
}

//Construct the rendering object
GLuint GLWidget::makeObject()
{
    int i, index;
    int textPositionIndex;
    int positionIndex;

    QString coordXBegin = QString::number(0);
    QString coordXEnd = QString::number(pointsNum);

    double* renderData;

    //Set the gap between two neighboring points from one channel
    double pointXGap = quadraWidth / (MAX_OSCILL_POINTS_NUM - 1);
    double pointYGap;

    int channelID;
    string channelName;

    //Use this list to pack all the GL objects
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    for(i = 0; i < tabChannelCount; i++)
    {
        index = i;
        textPositionIndex = index * 6;
        positionIndex = index * 4;

        //Get the channelInfo
        channelID = displayChannels.at(beginChannel + index).first;
        channelName = displayChannels.at(beginChannel + index).second;

        //Render the text about the information of channel
        renderText(textPosition[textPositionIndex], textPosition[textPositionIndex + 1], QString::fromStdString(channelName));
        
        //Set the quadrilateral's coordination of four vertex
        quadra(position[positionIndex], position[positionIndex + 1], position[positionIndex + 2], position[positionIndex + 1],
                position[positionIndex + 2], position[positionIndex + 3],position[positionIndex], position[positionIndex + 3]);

        //Render the text about the pointsNum information
        renderText(textPosition[textPositionIndex + 2], textPosition[textPositionIndex + 5], coordXBegin);
        renderText(textPosition[textPositionIndex + 4] - TAB_FONTSIZE * 2.5, textPosition[textPositionIndex + 5], coordXEnd);

        //Render the text about the minimun and maximun value of data from one channel
        renderText(textPosition[textPositionIndex], textPosition[textPositionIndex + 3], QString::number(minmaxVal[1][channelID], 'f', 3));
        renderText(textPosition[textPositionIndex], textPosition[textPositionIndex + 5] - TAB_FONTSIZE, QString::number(minmaxVal[0][channelID], 'f', 3));

        renderData = data[channelID];

        //Set the Oscillograph using the rendering data
        pointYGap = quadraHeight / (minmaxVal[1][channelID] - minmaxVal[0][channelID]);
        Oscill(renderData, pointsNum, position[positionIndex], position[positionIndex + 3], pointXGap, pointYGap, minmaxVal[0][channelID]);
    }

    glEndList();

    return list;
}

//Construct the oscillograph
void GLWidget::Oscill(double* renderData, int pointsNum, double coordX, double coordY, double pointXGap, double pointYGap, double minY)
{
    int i;
    //Get the last data index that needs to be rendered
    int rend = arrayStartIndex + pointsNum - 1;
    //Get the first data index that needs to be rendered on the screen
    int rstart = rend - MAX_OSCILL_POINTS_NUM + 1;
    GLdouble cy;

    //Using GL_LINE_STRIP to draw an oscillograph
    glBegin(GL_LINE_STRIP);

    //If the index doesn't exceed the bound
    if (rend < MAX_OSCILL_POINTS_NUM)
    {
        GLdouble firstCY = coordY + (renderData[0] - minY) * pointYGap;
        //Add every point into this oscillograph
        for(i = 0; i < MAX_OSCILL_POINTS_NUM; i++)
        {
            if(i < MAX_OSCILL_POINTS_NUM - rend)
                cy = firstCY;
            else
                cy = coordY + (renderData[i + rend - MAX_OSCILL_POINTS_NUM] - minY) * pointYGap;
            glVertex2d(coordX + pointXGap * i, cy);
        }
    }
    //If the index exceeds the bound
    else if (rend > FILE_SAMPLE_NUM)
    {
        //Reset the index
        rend = FILE_SAMPLE_NUM - 1;
        
        //Set the start index
        rstart = rend - MAX_OSCILL_POINTS_NUM + 1;
        
        //Add every point into this oscillograph
        for(i = 0; i < MAX_OSCILL_POINTS_NUM; i++)
        {
            cy = coordY + (renderData[rstart + i] - minY) * pointYGap;
            glVertex2d(coordX + pointXGap * i, cy);
        }
    }
    else
    {
        //Add every point into this oscillograph
        for(i = 0; i < MAX_OSCILL_POINTS_NUM; i++)
        {
            cy = coordY + (renderData[rstart + i] - minY) * pointYGap;
            glVertex2d(coordX + pointXGap * i, cy);
        }
    }

    glEnd();
}


//Constructing a quadrilateral using the coordinates of four vertex
void GLWidget::quadra(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
          GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4)
{
    //The graph is a quadrilateral
    glBegin(GL_LINES);

    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(x1, y1); glVertex2f(x2, y2);
    glVertex2f(x2, y2); glVertex2f(x3, y3);
    glVertex2f(x3, y3); glVertex2f(x4, y4);
    glVertex2f(x4, y4); glVertex2f(x1, y1);

    glEnd();
}

//Confine the angle wouldn't exceed the bound
void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

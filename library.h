#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>         // this_thread::sleep_for
#include <chrono>
#include <conio.h> //CM 5/13/15 - include for kbhit to catch keyboard input without pausing the program
#include <vector>
#include <string>
#include <list>
#include <time.h>

#include <ctime>
#include <stdlib.h>
#include <stdio.h>

#include <mutex>
#include <QFileDialog>
#include <QMessageBox>

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include <QMainWindow>
#include <QWidget>

using namespace std;

#include "H5Cpp.h"

#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL/GLU.h>

#ifndef TIMER_VARS
#define TIMER_VARS

extern time_t startTime,endTime; //Global timing variables for clocking user specified sections of code.
                                 //     NOTE: see note below about current usage.
extern int ALL_CHANNEL_DATA; //Pull all the samples on all channels despite whether or not a channel has a connected sensor
                             //   Value = 0: only pull sensor data from channels with sensors
                             //   Value = 1: pull all sensor data

extern int PROGRESS_LINE_LENGTH; //CM 8/18/15 - during each collection phase, print out progress feedback (i.e., how much progress has passed)
                                 //              this variable sets the length of the progress bar (so no matter how long a collection cycle
                                 //              is, it will always take the same space. The longer the cycle, the slower the progress will show
extern int CHUNK_SIZE;	//Using this variable while compressing the HDF5 File

extern int FILE_SAMPLE_NUM;  //The sample number of every channel stored in one HDF5 file

extern int MAX_OSCILL_POINTS_NUM;  //The fastest speed one oscillograph could take
#endif

//The maximum length of a name of string type
#define     MAX_NAME_LENGTH         128

/* Some parameters which would be used while drawing the oscillograph */
//The row of channels displayed in one page
#define     TAB_CHANNEL_ROW         4
//The column of channels displayed in one page
#define     TAB_CHANNEL_COL         4
//The number of all the channels displayed in one page
#define     TAB_CHANNEL_COUNT       (TAB_CHANNEL_ROW * TAB_CHANNEL_COL)
//The default size of font in one page, which is used to modify the position of the text and image
#define     TAB_FONTSIZE            15
//The gap between neighbouring channels' images, which is used to modify the position of the text and image
#define     TAB_CHANNEL_GAP         0.2


/* Some parameters which would be used while drawing the stereograph */
//The time interval(ms) between two neighbouring frames
#define     STEREO_TIME_INTERVAL    50
//The number of frames per second
#define     STEREO_FPS              (1000/STEREO_TIME_INTERVAL)
//The gap between index of data which would be used in two neighbouring frames
#define     STEREO_INDEX_GAP        1

//The width of the background image
#define     IMAGE_WIDTH             289.3
//The height of the background image
#define     IMAGE_HEIGHT            216

//In the stereograph, the DISPLAY_SCALER is used to magnify the value of every sample data
#define     DISPLAY_SCALER          5

//The radius of the cylinder drawn in the stereograph
#define     CYLINDER_RADIUS         0.008

//The file path of the texture files(background images)
#define TEXTURE_FILE_PATH "AccelerometerLocationsMAP_20150902_Page_"

//The file path of the distribution of the sensors
#define SENSOR_DISTRIBUTION_FILE "VTSIL_Sensor_Distribution.csv"


//NOTE: I have not gotten the global start and end timers to work properly. I can get local ones to work just fine.
void startTimer(); //CM 5/18/15 - start global timer for timing how long operations take
double endTimer(); //CM 5/18/15 - end global timer for timing how long operations take

//to_bool function courteousy of Chris Jester-Young at: http://stackoverflow.com/questions/2165921/converting-from-a-stdstring-to-bool
inline bool to_bool(string const& s) {
    return s != "0";
}



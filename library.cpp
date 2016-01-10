

/*
 *  library.cpp
 *
 *
 *  Created by Chreston Miller on 3/17/15.
 *  Copyright 2011 Virginia Tech. All rights reserved.
 *
 */

#include "library.h"

time_t startTime,endTime;

//CM 6/5/15 - configuration parameters =================================

int ALL_CHANNEL_DATA = 1; //CM 6/4/15 - testing chaninge a #define to a regular global variable so it can be dynamically changed
                          //    by the config file
//The chunk size is used for compression
int CHUNK_SIZE = 80;

//The number of sample data from one channel in one HDF5 file
int FILE_SAMPLE_NUM = 96000;

//The maximum number of the points that could be shown in the oscillograph in one frame
int MAX_OSCILL_POINTS_NUM = 1000;

//Start the timer
void startTimer()
{
    time(&startTime);
}

double endTimer()
{
    double timeForCollection = 0.0;
    time(&endTime);

    timeForCollection = difftime(endTime,startTime);

    return timeForCollection;
}


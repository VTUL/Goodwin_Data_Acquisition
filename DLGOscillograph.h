#pragma once

#include <QDialog>
#include "GLWidget.h"


namespace Ui
{
class DLGOscillograph;
}

//The diglog which is used to render oscillograph
class DLGOscillograph : public QDialog
{
    Q_OBJECT

public:
    //Construction and destruction functions
    explicit DLGOscillograph(QWidget *parent = 0);
    ~DLGOscillograph();

signals:
    //A function which is used to send information to the Main Frame to inform that this dialog is to be closed
    //This function is called when the dialog is to be closed
    void sent_oscil_data(int);

private:
    //The beginning index of the data which is used to render
    //Example: 0-500-1000-1500......
    int startIndex;

    //When switching to different pages, the beginning Channel Index would be changed
    int beginChannel;

    //The number of pages which would be displayed in the screen
    int tabCount;

    //The number of channels which would be displayed in one page
    int pageChannelCount;

    //The number of all the channels which would be rendered on the screen
    int displayChannelCount;

    //The number of sample points rendering on the screen in one frame
    int pointsPerFrame;

    //An array stored the minimum and maximum data from every channel's data
    double** minmaxVal;

    //A timer which is used to call updateGL function, refresh the rendering screen
    QTimer *timer;

    //To activate or deactivate the use of timer
    bool timerActivate;

private slots:

    //Set the beginChannel for the GLWidget class which could be used to render
    void setStartChannelIndex(int page);

    //Stop/Resume the render of the oscillograph
    void on_btnStop_clicked();

    //Return to the main frame
    void on_btnReturn_clicked();

    //Set the startIndex for the GLWidget class which could be used to render
    void setBeginArrayIndex();

private:
    Ui::DLGOscillograph *ui;

    //The pointer pointing to a GLWidget class, which is used to show the oscillograph
    GLWidget *glOscill;

public:
    //This function would be automatically called whence the dialog is closed
    void closeEvent(QCloseEvent *event);

    //Provide a function for the main frame, to enable the transmission of all the parameters
    //Call the function in GLWidget class and render the oscillograph
    void renderOscillo(double** data, int dataCol, int totalChannelCount, vector<pair<int, string>>& displayChannels, int pointsPerFrame);
};

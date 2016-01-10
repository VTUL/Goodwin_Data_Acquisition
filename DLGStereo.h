#pragma once

#include <QDialog>
#include "GLStereo.h"

namespace Ui {
class DLGStereo;
}

//The dialog which is used to render stereograph
class DLGStereo : public QDialog
{
    Q_OBJECT

public:
    //Construction and destruction functions
    explicit DLGStereo(QWidget *parent = 0);
    ~DLGStereo();

signals:
    //A function which is used to send information to the Main Frame to inform that this dialog is to be closed
    //This function is called in the closeEvent function
    void sent_stereo_data(int);

private slots:
    //Stop/Resume the render of the stereograph
    void on_btnStop_clicked();

    //Close this dialog and return to the main frame
    void on_btnReturn_clicked();

    //Set the startIndex for the GLStereo class which could be used to render
    void setBeginArrayIndex();

private:
    //The beginning index of the data which is used to render
    int startIndex;

    //The number of all the channels which would be rendered on the screen
    int displayChannelCount;

    //A timer which is used to call updateGL function, refresh the rendering screen
    QTimer *timer;

    //To activate or deactivate the use of timer
    bool timerActivate;

private:
    Ui::DLGStereo *ui;

    //The pointer pointing to a GLStereo class, which is used to show the stereograph
    GLStereo *glStereo;

public:
    //This function would be automatically called whence the dialog is closed
    void closeEvent(QCloseEvent *event);

    //Provide a function for the main frame, to enable the transmission of all the parameters
    //Call the function in GLStereo class and render the stereograph
    void renderStereo(double** data, vector<pair<int, string>> displayChannels);
};

#include "DLGStereo.h"
#include "ui_dlgstereo.h"

//Constructor
DLGStereo::DLGStereo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLGStereo)
{
    ui->setupUi(this);

    //Set the initial value of the startIndex
    startIndex = 0;

    //Set the fixed size of this dialog
    setFixedSize(1318, 931);

    timerActivate = true;
}

//Destructor
DLGStereo::~DLGStereo()
{
    delete timer;
    delete glStereo;
    delete ui;
}

//Set the startIndex for GLStereo
void DLGStereo::setBeginArrayIndex()
{
    //Set the startIndex parameter in GLStereo class
    glStereo->setParameter(startIndex);

    //In two neighboring frame, the difference between the index of the the data that is rendered in corresponding frame
    int indexGap = STEREO_INDEX_GAP;

    //Update the startIndex
    startIndex += indexGap;

    //Make sure that the startIndex wouldn't excess the bound
    if(startIndex + indexGap > FILE_SAMPLE_NUM)
        startIndex = FILE_SAMPLE_NUM - indexGap;

    //Refresh the GLStereo and render a new graph
    glStereo->updateGL();
}

//Set the parameters in GLStereo class
void DLGStereo::renderStereo(double** data, vector<pair<int, string>> displayChannels)
{
    //Initilize the pointer pointing to GLStereo
    glStereo = new GLStereo;

    //Set the parameters for GLStereo
    glStereo->initializeValue(data, displayChannels);
    glStereo->setParameter(0);

    //Add the GLStereo to this dialog for rendering
    ui->verticalLayout->addWidget(glStereo);

    //We use timer to send signal to refresh the GLStereo
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setBeginArrayIndex()));
    timer->start(STEREO_TIME_INTERVAL);
}

//Activate of deactivate the timer to start or stop the rendering
void DLGStereo::on_btnStop_clicked()
{
    if(timerActivate)
    {
        timer->stop();
        ui->btnStop->setText("Resume");
        timerActivate = false;
    }
    else
    {
        timer->start(20);
        ui->btnStop->setText("Stop");
        timerActivate = true;
    }
}

//Close this dialog
void DLGStereo::on_btnReturn_clicked()
{
    this->close();
}

//Send a signal '1' to MainFrame to inform that this dialog is closed
void DLGStereo::closeEvent(QCloseEvent *event)
{
    emit sent_stereo_data(1);
}

#include "DLGOscillograph.h"
#include "ui_dlgoscillograph.h"

//Constructor of the dialog
DLGOscillograph::DLGOscillograph(QWidget *parent): QDialog(parent),
    ui(new Ui::DLGOscillograph)
{
    ui->setupUi(this);

    //Set the initial value of the startIndex and beginChannel
    startIndex = 0;
    beginChannel = 0;

    //Set the fixed size of this dialog
    setFixedSize(1275, 912);

    //Set the pointer pointing to the minmax value array be NULL
    minmaxVal = NULL;

    //We need the timer to
    timerActivate = true;
}

//Destruction function
DLGOscillograph::~DLGOscillograph()
{
    //Release the memory storing the minmaxval
    if(minmaxVal != NULL)
    {
        for(int i = 0; i < 2; i++)
        {
            delete []minmaxVal[i];
            minmaxVal[i] = NULL;
        }
        delete []minmaxVal;
        minmaxVal = NULL;
    }
    delete timer;
    delete glOscill;
    delete ui;
}

//Set the startIndex
void DLGOscillograph::setBeginArrayIndex()
{
    //Set the parameters in GLWidget class
    glOscill->setParameter(pageChannelCount, startIndex, beginChannel);

    //Change the startIndex by adding the pointsPerFrame
    startIndex += pointsPerFrame;
    if(startIndex + pointsPerFrame > FILE_SAMPLE_NUM)
        startIndex = FILE_SAMPLE_NUM - pointsPerFrame;

    //Refresh the GLWidget and render a new graph
    glOscill->updateGL();
}

//Set the beginChannel
void DLGOscillograph::setStartChannelIndex(int page)
{
    //Set the beginChannel
    beginChannel = page * TAB_CHANNEL_COUNT;

    //The number of channels that would be shown in one page
    pageChannelCount = TAB_CHANNEL_COUNT;

    //If we switch to the last page, the pageChannelCount could only be calculated
    if(page == tabCount - 1)
        pageChannelCount = displayChannelCount - TAB_CHANNEL_COUNT * (tabCount - 1);

    //Set the corresponding parameters in GLWidget class
    glOscill->setParameter(pageChannelCount, startIndex, beginChannel);

    //Refresh the GLWidget and render a new graph
    glOscill->updateGL();
}

//Set the parameters in GLWidget class
void DLGOscillograph::renderOscillo(double** data, int dataCol, int totalChannelCount, vector<pair<int, string>>& displayChannels, int pointsPerFrame)
{
    int i, j;
    QString tabName;

    //The number of the channels that would be used in rendering
    displayChannelCount = displayChannels.size();

    //The number of the tabs
    tabCount = ceil((double)displayChannelCount / TAB_CHANNEL_COUNT);

    //Initilize the pointer pointing to GLWidget
    glOscill = new GLWidget;

    //Store the minimum and maximum value of the data from each sensor
    double minVal, maxVal;
    minmaxVal = new double*[2];
    minmaxVal[0] = new double[totalChannelCount];
    minmaxVal[1] = new double[totalChannelCount];
    for(i = 0; i < totalChannelCount; i++)
    {
        minVal = 10000;
        maxVal = -10000;
        for(j = 0; j < dataCol; j++)
        {
            if(minVal > data[i][j]) minVal = data[i][j];
            if(maxVal < data[i][j]) maxVal = data[i][j];
        }
        minmaxVal[0][i] = minVal;
        minmaxVal[1][i] = maxVal;
    }

    //Set the number of the points rendering on the screen in one frame
    this->pointsPerFrame = pointsPerFrame;

    //Set the number of channels shown in one tab page
    pageChannelCount = TAB_CHANNEL_COUNT;

    //If we only need to render on one page, the the number of channels shown in this page would be the number of all the displayChannels
    if(tabCount == 1) pageChannelCount = displayChannelCount;

    //Set the parameter for GLWidget
    glOscill->initializeValue(data, minmaxVal, pointsPerFrame, displayChannels);
    glOscill->setParameter(pageChannelCount, 0, 0);

    //Add the tabs' index to the listWidget
    for(i = 1; i <= tabCount; i++)
    {
        tabName = QString::number(i);
        ui->listWidget->insertItem(i - 1, tabName);
    }

    //Add the GLWidget to this dialog to render on this GLWidget
    ui->horizontalLayout->addWidget(glOscill);

    //We use timer to refresh the GLWidget after every 0.5s
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setBeginArrayIndex()));
    timer->start(500);

    //We use listWidget to choose different tabs
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setStartChannelIndex(int)));
}

//Activate of deactivate the timer to stop the rendering
void DLGOscillograph::on_btnStop_clicked()
{
    if(timerActivate)
    {
        timer->stop();
        ui->btnStop->setText("Resume");
        timerActivate = false;
    }
    else
    {
        timer->start(500);
        ui->btnStop->setText("Stop");
        timerActivate = true;
    }
}

//Close this dialog
void DLGOscillograph::on_btnReturn_clicked()
{
    this->close();
}

//Send a signal '0' to MainFrame to inform that this dialog is closed
void DLGOscillograph::closeEvent(QCloseEvent *event)
{
    emit sent_oscil_data(0);
}

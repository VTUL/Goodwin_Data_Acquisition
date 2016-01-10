#include "DLGChannels.h"
#include "ui_dlgchannels.h"

//Construction fuction
DLGChannels::DLGChannels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLGChannels)
{
    ui->setupUi(this);
    channelCount = 0;
}

//Destruction function
DLGChannels::~DLGChannels()
{
    int i = 0;
    //Remove all the dynamic QCheckBoxes from the Widget
    for(i = 0; i != channelCount; i++)
    {
        QCheckBox *qcb = qcbList.at(i);
        if(qcb != NULL)
        {
            ui->gridLayout->removeWidget(qcb);
            delete qcb;
        }
    }
    qcbList.clear();

    //Clear the vector storing the channels' infomation
    selectedChannels.clear();

    delete ui;
}

//Lauch the informaion of all the channels into Checkbox List
void DLGChannels::launchChannels(vector<pair<int, string>>& activeChannels)
{
    //The number of all the channels
    channelCount = activeChannels.size();
    QString channelName;

    for(int i = 0; i < channelCount; i++)
    {
        //Initilize a pointer pointing to a new QCheckBox
        QCheckBox *qcb = new QCheckBox;

        //Get the channelName of the ith channel in the list
        channelName = QString::fromStdString(activeChannels.at(i).second);

        //Set the text of the QCheckBox as the channelName
        qcb->setText(channelName);

        //Set the size of each QCheckBox
        qcb->setGeometry(0,0,80,35);

        //Add this new QCheckBox to the list
        qcbList.push_back(qcb);

        //Add this new QCheckBox to the Widget
        ui->gridLayout->addWidget(qcb, i%30, i/30);
    }
}

//Whether or not to select all the channels
void DLGChannels::on_cBSelectAll_clicked()
{
    int i;
    //If all the channels have been selected
    if(ui->cBSelectAll->isChecked())
    {
        //Set the state of this QCheckBox as ture(selected)
        ui->cBSelectAll->setChecked(true);
        //Set the state of the channels' QCheckBoxes as ture(selected)
        for(i = 0; i < channelCount; i++)
        {
            qcbList.at(i)->setChecked(true);
        }
    }
    //If not all the channels have been selected
    else
    {
        //Set the state of this QCheckBox as false(not selected)
        ui->cBSelectAll->setChecked(false);
        //Set the state of the channels' QCheckBoxes as false(not selected)
        for(i = 0; i < channelCount; i++)
        {
            qcbList.at(i)->setChecked(false);
        }
    }
}

//Click this button, we will return to the MainFrame, and this dialog would be invisible
void DLGChannels::on_btnReturn_clicked()
{
    int i;

    //Clear the channel list
    selectedChannels.clear();
    for(i = 0; i < channelCount; i++)
    {
        //Get the (i+1)th QCheckBox from the list
        QCheckBox *qcb = qcbList.at(i);
        //If this QCheckBox is selected
        if(qcb->isChecked())
        {
            //Get the ID and Name of this channel
            pair<int, string> sChannel = pair<int, string>(i, qcb->text().toStdString());
            //Save the information of this channel into the channel list
            selectedChannels.push_back(sChannel);
        }
    }
    //Set this dialog to be invisible
    this->setVisible(false);

    //Send the signal -1 to the MainFrame
    emit sent_data(-1);
}

//This function would be automatically called whence the dialog is closed
void DLGChannels::closeEvent(QCloseEvent *event)
{
    int i;

    //Clear the channel list
    selectedChannels.clear();
    for(i = 0; i < channelCount; i++)
    {
        //Get the (i+1)th QCheckBox from the list
        QCheckBox *qcb = qcbList.at(i);
        //If this QCheckBox is selected
        if(qcb->isChecked())
        {
            //Get the ID and Name of this channel
            pair<int, string> sChannel = pair<int, string>(i, qcb->text().toStdString());
            //Save the information of this channel into the channel list
            selectedChannels.push_back(sChannel);
        }
    }
    //Send the signal -1 to the MainFrame
    emit sent_data(-1);
}

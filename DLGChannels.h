#pragma once

#include <QDialog>
#include "library.h"

namespace Ui
{
class DLGChannels;
}

//The dialog which is used to show all the channel names
class DLGChannels : public QDialog
{
    Q_OBJECT

public:
    //Construction and destruction functions
    explicit DLGChannels(QWidget *parent = 0);

    ~DLGChannels();

signals:
    //A function which is used to send information to the Main Frame
    //This function is called when the dialog is to be closed
    void sent_data(int);

private:
    //The number of all the channels
    int channelCount;

    //A vector stored pointers pointing to all the QCheckBoxes
    //It is used to dynamically add the widgets
    vector<QCheckBox*> qcbList;
public:

    //This function would be automatically called whence the dialog is closed
    void closeEvent(QCloseEvent *event);

    //All the selected channels' channelIDs and channelNames have to be stored in a vector
    vector<pair<int, string>> selectedChannels;

    //Lauch the informaion of all the channels into Checkbox List
    void launchChannels(vector<pair<int, string>>& activeChannels);

private slots:
    //Click this button, we will return to the MainFrame, and this dialog would be invisible
    void on_btnReturn_clicked();

    //Select all the channels displayed on the dialog
    void on_cBSelectAll_clicked();

private:
    //The user interface which could be used to do anything to the dialog
    Ui::DLGChannels *ui;
};

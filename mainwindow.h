#pragma once

#include "library.h"
#include "DLGChannels.h"
#include "DLGOscillograph.h"
#include "DLGStereo.h"

namespace Ui
{
    class MainWindow;
}

//The main frame which should be displayed when the program is launched
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Construction and Destruction functions
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Initialize some parameters
    void initialize();

private:

    //A pointer pointing to the DLGChannels class which would show all the channels
    DLGChannels* dlgChannels;

    //A pointer pointing to the DLGOscillograph class(render the oscillograph using the data)
    DLGOscillograph* dlgOscillo;

    //A pointer pointing to the DLGStereo class(render the stereograph using the data)
    DLGStereo* dlgStereo;

private:
    //Whether this is the first time to show all the channels
    bool firstTimeShowChannels;

    //The file path of the HDF5 file
    string hdf5FilePath;
    //The number of points that would be used to render in the oscillograph in one frame
    int renderPoints;
    //If the channels haven't been selected in the DLGChannels, then it is not permitted to render
    bool selectedChannels;

    //The number of all the selected channels
    int totalChannels;

    //This variable store all the <channelID, channelName> pair of active channels(channels with sensors)
    vector<pair<int, string>> activeChannels;
    //This variable store all the <channelID, channelName> pair of selected channels(channels used to display)
    vector<pair<int, string>> displayChannels;

    //The row number of the rendering data
    int totalRow;
    //The column number of the rendering data
    int totalCol;
    //This array is used to store all the rendering data
    double** renderData;

private slots:

    //A checkBox associated with the oscillograph rendering
    void on_cBOscill_clicked();

    //A checkBox associated with the stereograph rendering
    void on_cBStereo_clicked();

    //Enable the user to select single file using a dialog
    void on_btnSelect_clicked();

    //A checkBox which is used to indicate that whether all the channels would be selected
    void on_cBAChannels_clicked();

    //A checkBox which is used to indicate that some of the channels would be selected
    void on_cBSChannels_clicked();

    //To show the dialog displaying all the channels(DLGChannels)
    void on_btnAllChannels_clicked();

    //Show the dialog rendering oscillograph or stereograph
    void on_btnRender_clicked();

    //Close the whole program
    void on_btnCancel_clicked();

    //Process the information sent from the children QWidget
    void dataprocess(int value);

private:
    //Read a HDF5 file, and get all the data into the renderData array
    void getFileData(string filename);

private:
    Ui::MainWindow *ui;
};

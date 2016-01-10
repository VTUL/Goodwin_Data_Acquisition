#include "mainwindow.h"
#include "ui_mainwindow.h"

//Construction of the MainWindow
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set the fixed size of this window
    setFixedSize(901, 559);

    //The HDF5 file path
    hdf5FilePath= "";

    //How many points would be rendered in the ocillograph in one frame
    renderPoints = 0;

    //Whether we have selected channels for rendering
    selectedChannels = false;

    //A 2-dimensional array to store data for rendering
    renderData = NULL;

    //Whether this is the first time to show all the channels on a dialog
    firstTimeShowChannels = true;

    //Initialize the pointer pointing to the DLGChannels class
    dlgChannels = new DLGChannels;

    //Connect the function "dataprocess" with the signal "sent_data" sent from the DLGChannels class
    connect(dlgChannels,SIGNAL(sent_data(int)),this,SLOT(dataprocess(int)));

    dlgOscillo = NULL;
    dlgStereo = NULL;
}

//Destruction
MainWindow::~MainWindow()
{
    //Release the memory allocated to store the rendering data
    if(renderData != NULL)
    {
        for (int i = 0; i < totalRow; i++)
        {
            if(renderData[i] != NULL)
            {
                delete []renderData[i];
                renderData[i] = NULL;
            }
        }
    }
    delete []renderData;
    renderData = NULL;

    delete ui;
}

//Initialize the parameters in this class
void MainWindow::initialize()
{
    //Set the following widgets to be invisible
    ui->labelArrange->setVisible(false);

    ui->btnAllChannels->setVisible(false);

    ui->labelRenderPoints->setVisible(false);
    ui->labelPointsRange->setVisible(false);
    ui->lineEdit->setVisible(false);

    ui->cBAChannels->setVisible(false);
    ui->cBSChannels->setVisible(false);

    //We could only input number between [0, 10000) in lineEdit widget
    QRegExp rx("^[0-9]{4}$");
    QRegExpValidator *pRevalidotor = new QRegExpValidator(rx, this);
    ui->lineEdit->setValidator(pRevalidotor);

    //We set the value in lineEdit to be 500 as the initial value
    ui->lineEdit->setText(QString::number(500));
}

//Whether to render the oscillograph or not
void MainWindow::on_cBOscill_clicked()
{
    //If this QCheckbox is checked
    if(ui->cBOscill->isChecked())
    {
        //Set cBOscill to be selected
        ui->cBOscill->setChecked(true);

        //Set cBStereo to be unselected
        ui->cBStereo->setChecked(false);

        //Set these widgets to be visible
        ui->labelRenderPoints->setVisible(true);
        ui->lineEdit->setVisible(true);
        ui->labelPointsRange->setVisible(true);
    }
    else
    {
        //Set cBOscill to be unselected
        ui->cBOscill->setChecked(false);

        //Set these widgets to be invisible
        ui->labelRenderPoints->setVisible(false);
        ui->lineEdit->setVisible(false);
        ui->labelPointsRange->setVisible(false);
    }
}

//Whether to render the stereograph or not
void MainWindow::on_cBStereo_clicked()
{
    //If this cBStereo is checked
    if(ui->cBStereo->isChecked())
    {
        //Set cBStereo to be selected
        ui->cBStereo->setChecked(true);

        //Set these widgets to be invisible
        ui->cBOscill->setChecked(false);
        ui->labelRenderPoints->setVisible(false);
        ui->lineEdit->setVisible(false);
        ui->labelPointsRange->setVisible(false);
    }
    else
    {
        //Set cBStereo to be unselected
        ui->cBStereo->setChecked(false);
    }
}

//Select a HDF5 file
void MainWindow::on_btnSelect_clicked()
{
    //Open a dialog for the user to select HDF5 file from the file system
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("HDF5 File (*.h5)"));

    //Get the filepath of this HDF5 file
    hdf5FilePath = filepath.toStdString();
    //Set the filepath to textFilePath
    ui->textFilePath->setText(filepath);

    //Read the HDF5 file and get the data and channels from the file
    getFileData(hdf5FilePath);

    //Set these widgets to be invisible
    ui->cBAChannels->setVisible(true);
    ui->cBSChannels->setVisible(true);
}

//Whether to select all the channels or not
void MainWindow::on_cBAChannels_clicked()
{
    //If this QCheckbox is checked
    if(ui->cBAChannels->isChecked())
    {
        //Get the number of all the channels
        char channelNum[32];
        totalChannels = activeChannels.size();
        sprintf_s(channelNum, "%i", totalChannels);
        //Show the information about the number of all the channels
        string str = "";
        str = str + "Selected sensor data from " + channelNum + " channels";
        QString qstr = QString::fromStdString(str);
        ui->labelArrange->setText(qstr);

        //Set the status of the QCheckBoxes to be true or false
        ui->cBAChannels->setChecked(true);
        ui->cBSChannels->setChecked(false);

        //Set these widgets to be invisible
        ui->btnAllChannels->setVisible(false);
        ui->labelArrange->setVisible(true);

        //Set the channels to be rendered as all of the channels
        displayChannels = activeChannels;
    }
    //If this QCheckbox is checked
    else
    {
        //Set the status of cBAChannels to be false
        ui->cBAChannels->setChecked(false);
        //Set this widgets to be invisible
        ui->labelArrange->setVisible(false);
    }
}

//Whether to select some channels or not
void MainWindow::on_cBSChannels_clicked()
{
    if(ui->cBSChannels->isChecked())
    {
        ui->cBAChannels->setChecked(false);
        ui->cBSChannels->setChecked(true);
        ui->btnAllChannels->setVisible(true);
        ui->labelArrange->setVisible(false);
        selectedChannels = false;
    }
    else
    {
        ui->cBSChannels->setChecked(false);
        ui->btnAllChannels->setVisible(false);
        ui->labelArrange->setVisible(false);
    }
}

//Open the dialog to show all the channels for the user to select
void MainWindow::on_btnAllChannels_clicked()
{
    if(firstTimeShowChannels)
    {
        //Launch all the channels and
        dlgChannels->launchChannels(activeChannels);
        firstTimeShowChannels = false;
    }
    dlgChannels->setModal(true);
    dlgChannels->show();
    //this->setEnabled(false);
}

//Click this button, to judge whether the conditions hold, and render corresponding graphs using dialogs
void MainWindow::on_btnRender_clicked()
{
    //The graph type must be selected
    if(!ui->cBOscill->isChecked() && !ui->cBStereo->isChecked())
    {
        QMessageBox::information(this,"Error", "The Graph Type could not be empty!");
        return;
    }
    //The HDF5 file path could not be empty
    if(ui->textFilePath->toPlainText() == "")
    {
        QMessageBox::information(this,"Error", "The HDF5 file path could not be empty!");
        return;
    }
    //The Rendering Sample Number could not be empty
    if(ui->cBOscill->isChecked() && ui->lineEdit->text() == "")
    {
        QMessageBox::information(this,"Error", "The Rendering Sample Number could not be empty!");
        return;
    }
    //If we select to render oscillograph
    if(ui->cBOscill->isChecked())
    {
        //Get the rendering sample number in one frame from one channel
        renderPoints = atoi(ui->lineEdit->text().toStdString().c_str());
        //The rendering sample number could not be less than 20
        if(renderPoints < 20)
        {
            QMessageBox::information(this,"Error", "The Rendering Sample Number Per Frame could not be less than 20!");
            return;
        }
        //The rendering sample number could not be greater than 1000
        else if(renderPoints >1000)
        {
            QMessageBox::information(this,"Error", "The Rendering Sample Number Per Frame could not be larger than 1000!");
            return;
        }
    }
    //The channels' selection could not be empty
    if(!ui->cBAChannels->isChecked() && !ui->cBSChannels->isChecked())
    {
        QMessageBox::information(this,"Error", "The channels' selection could not be empty!");
        return;
    }
    //The selected channels could not be empty
    if(ui->cBSChannels->isChecked() && !selectedChannels)
    {
        QMessageBox::information(this,"Error", "The selected channels could not be empty!");
        return;
    }
    //If we choose to render oscillograph
    if(ui->cBOscill->isChecked())
    {
        //Initialize the pointer pointing to a DLGOscillograph class
        dlgOscillo = new DLGOscillograph;
        
        //Connect the function "dataprocess" with the signal "sent_oscil_data" sent from the DLGOscillograph class
        connect(dlgOscillo, SIGNAL(sent_oscil_data(int)), this, SLOT(dataprocess(int)));

        //Show the dialog, and set the parameters of this dialog
        dlgOscillo->setModal(true);
        dlgOscillo->show();
        //this->hide();
        dlgOscillo->renderOscillo(renderData, totalCol, activeChannels.size(), displayChannels, renderPoints);
        //this->setEnabled(false);
    }
    //If we choose to render stereo graph
    else if(ui->cBStereo->isChecked())
    {
        //Initialize the pointer pointing to a DLGStereo class
        dlgStereo = new DLGStereo;
        
        //Connect the function "dataprocess" with the signal "sent_stereo_data" sent from the DLGStereo class
        connect(dlgStereo, SIGNAL(sent_stereo_data(int)), this, SLOT(dataprocess(int)));

        //Show the dialog, and set the parameters of this dialog
        dlgStereo->setModal(true);
        dlgStereo->show();
        //this->hide();
        dlgStereo->renderStereo(renderData, displayChannels);
        //this->setEnabled(false);
    }
}

//Read file data from a HDF5 file
void MainWindow::getFileData(string filename)
{
    // Turn off the auto-printing when failure occurs
    Exception::dontPrint();
//    // -----------------------------------------------
//    // Re-open the file and dataset, retrieve filter
//    // information for dataset, read the data back and
//    // test whether we have created the right dataset.
//    // -----------------------------------------------

    hid_t file;
    hid_t gid;
    hsize_t nobj;
    herr_t err;
    ssize_t len;
    hid_t dsid;
    int otype;

    char ds_name[MAX_NAME_LENGTH];
    char memb_name[MAX_NAME_LENGTH];

    hid_t sid;
    hid_t rank;
    hid_t status_n;
    hsize_t dims_out[2];
    string channelName;

    // Open the file and the dataset in the file.
    file = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    gid = H5Gopen(file, "/Data", H5P_DEFAULT);

    err = H5Gget_num_objs(gid, &nobj);

    //Allocating a memory buffer for the Sensor Data in one DAQ
    //The number of Channels with sensors
    totalRow = nobj;

    totalCol = 96000;//The number of total Samples in one session
    //Storing the Sensor Data in a 2-dimentional array, allocating the memory space
    renderData = new double*[totalRow];
    for (int i = 0; i < totalRow; i++)
    {
        renderData[i] = new double[totalCol];
        memset(renderData[i], 0, sizeof(double)* totalCol);
    }

    //To iterate all the datasets in one HDF5 file
    for (int i = 0; i < nobj; i++)
    {
        //Get the length of the file
        len = H5Gget_objname_by_idx(gid, (hsize_t)i, memb_name, (size_t)MAX_NAME_LENGTH);
        otype = H5Gget_objtype_by_idx(gid, (size_t)i);

        //process a dataset type
        if (otype == H5G_DATASET)
        {
            dsid = H5Dopen(gid, memb_name, H5P_DEFAULT);

            //H5Iget_name(dsid, ds_name, MAX_NAME_LENGTH);
            //sid = H5Dget_space(dsid); //dataspace identifier
            //rank = H5Sget_simple_extent_ndims(sid);
            //status_n = H5Sget_simple_extent_dims(sid, dims_out, NULL);
            //Get the filename of the dataset
            channelName = memb_name;
            channelName.replace(0, 5, "");

            //Push the channelName into a vector
            pair<int, string> channel = pair<int, string>(i, channelName);
            activeChannels.push_back(channel);

            //Read HDF5 file and write the data into renderData array
            H5Dread(dsid, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, renderData[i]);
            H5Dclose(dsid);
        }
    }

    H5Gclose(gid);
    H5Fclose(file);
}

//Use this function to deal with all the signals sent by other dialogs
void MainWindow::dataprocess(int value)
{
    //If the signal is -1, then it comes from the DLGChannels classs
    if(value == -1)
    {
        //We have selected channels in DLGChannels diglog, then return the vector of all the channels
        displayChannels = dlgChannels->selectedChannels;
        //The size of the channels
        totalChannels = displayChannels.size();
        selectedChannels = true;

        //Show the information about how many channels have been selected
        char channelNum[32];
        sprintf_s(channelNum, "%i", totalChannels);
        string str = "";
        str = str + "Selected sensor data from " + channelNum + " channels";
        QString qstr = QString::fromStdString(str);

        //Set the status of some other widgets
        ui->labelArrange->setText(qstr);
        ui->labelArrange->setVisible(true);
        this->setEnabled(true);
    }
    //If the signal is 0, then it comes from the DLGOscillograph class
    else if(value == 0)
    {
        //Set the MainWindow to be enabled, delete the dialog
        this->setEnabled(true);
        delete dlgOscillo;
        dlgOscillo = NULL;
    }
    //If the signal is 1, then it comes from the DLGStereo class
    else
    {
        //Set the MainWindow to be enabled, delete the dialog
        this->setEnabled(true);
        delete dlgStereo;
        dlgStereo = NULL;
    }
}

//Delete all the dialogs, and close the MainFrame
void MainWindow::on_btnCancel_clicked()
{
    if(dlgChannels != NULL)
        delete dlgChannels;
    if(dlgOscillo != NULL)
        delete dlgOscillo;
    if(dlgStereo != NULL)
        delete dlgStereo;
    this->close();
}

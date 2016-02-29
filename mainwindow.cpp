#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cpu.h"
#include <unistd.h> // sysconf
#include <sstream>
#include <vector>
#include <iostream>
#include <QProgressBar>
#include <QTimer>

const int n_coreCount = sysconf( _SC_NPROCESSORS_CONF );
int n_coresOnline = sysconf( _SC_NPROCESSORS_ONLN );
std::vector<cpu> core;
std::vector<QProgressBar*> CPUInfoProgressBars;
std::vector<QLabel*> CPUInfoLables;
QTimer *timer = new QTimer();

void updateInterfaceInformation(Ui::MainWindow* ui)
{
    int n_curRow = ui->listWidget->currentRow();
    n_coresOnline = sysconf( _SC_NPROCESSORS_ONLN );
    std::stringstream sstm;

    sstm << n_coreCount;
    ui->label_19->setText(sstm.str().c_str());
    sstm.str(std::string());
    sstm.clear();

    sstm << n_coresOnline;
    ui->label_20->setText(sstm.str().c_str());
    sstm.str(std::string());
    sstm.clear();

    sstm << "Core " << n_curRow << " information:";
    ui->label_6->setText(sstm.str().c_str());
    sstm.str(std::string());
    sstm.clear();
    if (core[n_curRow].b_isOnline)
    {
        ui->label_21->setText("Online");

        ui->label_22->setText(core[n_curRow].str_governor);

        sstm << int(core[n_curRow].f_curFreq/1000) << " MHz";
        ui->label_23->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();

        sstm << int(core[n_curRow].f_maxFreq/1000) << " MHz";
        ui->label_24->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();

        sstm << int(core[n_curRow].f_minFreq/1000) << " MHz";
        ui->label_25->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();

        sstm << int(core[n_curRow].f_scalingMaxFreq/1000) << " MHz";
        ui->label_26->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();

        sstm << int(core[n_curRow].f_scalingMinFreq/1000) << " MHz";
        ui->label_27->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();
    }
    else
    {
        ui->label_21->setText("Offline");
        ui->label_22->setText("Offline");
        ui->label_23->setText("Offline");
        ui->label_24->setText("Offline");
        ui->label_25->setText("Offline");
        ui->label_26->setText("Offline");
        ui->label_27->setText("Offline");
    }
}

void updateCPUinfo(Ui::MainWindow* ui)
{
    std::stringstream sstm;
    sstm << "CPUs online: " << n_coresOnline;
    ui->label->setText(sstm.str().c_str());
    sstm.str(std::string());
    sstm.clear();
    for (int i = 0; i < n_coreCount; i++)
    {
        CPUInfoProgressBars[i]->setMaximum(int(core[i].f_maxFreq/1000));
        CPUInfoProgressBars[i]->setMinimum(int(core[i].f_minFreq/1000));
        CPUInfoProgressBars[i]->setValue(int(core[i].f_curFreq/1000));

        sstm << "CPU " << i;
        CPUInfoLables[i]->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();
    }
}

void updateInterfaceSet(Ui::MainWindow* ui)
{
    int n_curRow = ui->listWidget->currentRow();
    n_coresOnline = sysconf( _SC_NPROCESSORS_ONLN );

    if (core[n_curRow].b_isOnline)
    {
        std::stringstream sstm;

        ui->checkBox->setChecked(core[n_curRow].b_isOnline);
        ui->horizontalSlider->setMinimum(int(core[n_curRow].f_minFreq/1000));
        ui->horizontalSlider->setMaximum(int(core[n_curRow].f_maxFreq/1000));
        ui->horizontalSlider->setValue(int(core[n_curRow].f_scalingMaxFreq/1000));
        sstm << int(core[n_curRow].f_scalingMaxFreq/1000) << " MHz";
        ui->label_15->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();
        ui->horizontalSlider_2->setMinimum(int(core[n_curRow].f_minFreq/1000));
        ui->horizontalSlider_2->setMaximum(int(core[n_curRow].f_maxFreq/1000));
        ui->horizontalSlider_2->setValue(int(core[n_curRow].f_scalingMinFreq/1000));
        sstm << int(core[n_curRow].f_scalingMinFreq/1000) << " MHz";
        ui->label_17->setText(sstm.str().c_str());
        sstm.str(std::string());
        ui->comboBox->clear();
        for (unsigned int i = 0; i < core[n_curRow].vec_governorList.size(); i++)
        {
            ui->comboBox->addItem(core[n_curRow].vec_governorList[i].c_str());
        }
        int index = ui->comboBox->findText(core[n_curRow].str_governor);
        ui->comboBox->setCurrentIndex(index);
    }
    else
    {
        ui->checkBox->setChecked(core[n_curRow].b_isOnline);
        ui->horizontalSlider->setMinimum(int(core[n_curRow].f_minFreq/1000));
        ui->horizontalSlider->setMaximum(int(core[n_curRow].f_maxFreq/1000));
        ui->horizontalSlider->setValue(int(core[n_curRow].f_scalingMaxFreq/1000));
        ui->horizontalSlider_2->setMinimum(int(core[n_curRow].f_minFreq/1000));
        ui->horizontalSlider_2->setMaximum(int(core[n_curRow].f_maxFreq/1000));
        ui->horizontalSlider_2->setValue(int(core[n_curRow].f_scalingMinFreq/1000));
    }
}

void MainWindow::timerUpdate()
{
    n_coresOnline = sysconf( _SC_NPROCESSORS_ONLN );
    for(int i = 0; i < n_coreCount; i ++)
    {
        core[i].updateInformation();
    }
    updateInterfaceInformation(ui);
    updateCPUinfo(ui);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Qt::WindowFlags flags;
    flags = Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
    setWindowFlags( flags );
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1500);
    //std::cout << "Yep!" << std::endl;

    for(int i = 0; i < n_coreCount; i ++)
    {
        core.push_back(cpu(i));
        core[i].updateInformation();
        std::stringstream sstm;
        sstm << "CPU " << i;
        ui->listWidget->addItem(sstm.str().c_str());
        ui->listWidget_2->addItem(sstm.str().c_str());
    }
    std::stringstream sstm;

    sstm << n_coreCount;
    ui->label_19->setText(sstm.str().c_str());
    sstm.str(std::string());
    sstm.clear();

    sstm << n_coresOnline;
    ui->label_20->setText(sstm.str().c_str());
    sstm.str(std::string());
    sstm.clear();

    ui->listWidget->setCurrentRow(0);
    ui->listWidget_2->setCurrentRow(0);
    updateInterfaceInformation(ui);
    updateInterfaceSet(ui);
    sstm << "CPUs online: " << n_coresOnline;
    ui->label->setText(sstm.str().c_str());
    sstm.str(std::string());
    sstm.clear();
    for (int i = 0; i < n_coreCount; i++)
    {
        CPUInfoProgressBars.push_back(new QProgressBar(ui->tab));
        CPUInfoProgressBars[i]->setTextVisible(true);
        CPUInfoProgressBars[i]->setGeometry(80, 40+i*40, 390, 30);
        CPUInfoProgressBars[i]->setMaximum(int(core[i].f_maxFreq/1000));
        CPUInfoProgressBars[i]->setMinimum(int(core[i].f_minFreq/1000));
        CPUInfoProgressBars[i]->setValue(int(core[i].f_curFreq/1000));

        sstm << "CPU " << i;
        CPUInfoLables.push_back(new QLabel(ui->tab));
        CPUInfoLables[i]->setGeometry(10, 45+i*40, 60, 20);
        CPUInfoLables[i]->setText(sstm.str().c_str());
        sstm.str(std::string());
        sstm.clear();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_clicked()
{
    for(unsigned int i = 0; i < core.size(); i++)
    {
        core[i].updateInformation();
    }
    updateInterfaceInformation(ui);
}

void MainWindow::on_listWidget_2_clicked()
{
    for(unsigned int i = 0; i < core.size(); i++)
    {
        core[i].updateInformation();
    }
    updateInterfaceSet(ui);
}

void MainWindow::on_pushButton_2_clicked()
{
    int n_curRow = ui->listWidget_2->currentRow();
    core[n_curRow].setOnline(ui->checkBox->checkState());
    core[n_curRow].setMaxScalingFreq(ui->horizontalSlider->value()*1000);
    core[n_curRow].setMinScalingFreq(ui->horizontalSlider_2->value()*1000);
    core[n_curRow].setGovernor(ui->comboBox->currentText());
}

void MainWindow::on_pushButton_clicked()
{
    for (int i = 0; i < n_coreCount; i++)
    {
        core[i].setOnline(ui->checkBox->checkState());
        core[i].setMaxScalingFreq(ui->horizontalSlider->value()*1000);
        core[i].setMinScalingFreq(ui->horizontalSlider_2->value()*1000);
        core[i].setGovernor(ui->comboBox->currentText());
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    if (!ui->checkBox_2->checkState())
    {
        timer->stop();
        return;
    }
    if (ui->checkBox_2->checkState() && timer->isActive())
    {
        timer->start(ui->spinBox->value());
        return;
    }
    timer->setInterval(ui->spinBox->value());
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    std::stringstream sstm;

    sstm << value << " MHz";
    ui->label_15->setText(sstm.str().c_str());
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    std::stringstream sstm;

    sstm << value << " MHz";
    ui->label_17->setText(sstm.str().c_str());
}

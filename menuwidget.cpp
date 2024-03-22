#include "menuwidget.hpp"

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setLayout(&layout);
    /*options.addItem("First Come First Served");
    options.addItem("Shortest Job First");*/

    //layout.addWidget(&options);
    //start_simulation_btn.setText("Start Simulation");
    //layout.addWidget(&start_simulation_btn);

    int width = 300, height = 50;

    QLabel *label_1 = new QLabel("Process Planning Algorithms");
    label_1->setStyleSheet("font-size: 30px; color: #333; font-weight: bold;");
    layout.addWidget(label_1, 0, Qt::AlignCenter);

    QLabel *label_2 = new QLabel("Non-Expulsive");
    label_2->setStyleSheet("font-size: 25px; color: #333; font-weight: bold;");
    layout.addWidget(label_2, 0, Qt::AlignCenter);

    layout.addWidget(&FCFS, 0, Qt::AlignCenter);
    FCFS.setText("First Come First Served");
    FCFS.setFixedSize(width, height);

    layout.addWidget(&RS, 0, Qt::AlignCenter);
    RS.setText("Random Selection");
    RS.setFixedSize(width, height);

    layout.addWidget(&PNE, 0, Qt::AlignCenter);
    PNE.setText("Planning Based on Priorities (Not Expulsive)");
    PNE.setFixedSize(width, height);

    layout.addWidget(&SJF, 0, Qt::AlignCenter);
    SJF.setText("Shortest Job First");
    SJF.setFixedSize(width, height);

    QLabel *label_3 = new QLabel("Expulsive");
    label_3->setStyleSheet("font-size: 25px; color: #333; font-weight: bold;");
    layout.addWidget(label_3, 0, Qt::AlignCenter);

    layout.addWidget(&RR, 0, Qt::AlignCenter);
    RR.setText("Round Robin");
    RR.setFixedSize(width, height);

    layout.addWidget(&SRTF, 0, Qt::AlignCenter);
    SRTF.setText("Shortest Remaining");
    SRTF.setFixedSize(width, height);

    layout.addWidget(&PE, 0, Qt::AlignCenter);
    PE.setText("Planning Based on Priorities (Expulsive)");
    PE.setFixedSize(width, height);

    connect(&FCFS, SIGNAL(clicked(bool)), this, SLOT(on_FCFS_pressed()));
    connect(&RS, SIGNAL(clicked(bool)), this, SLOT(on_RS_pressed()));
    connect(&PNE, SIGNAL(clicked(bool)), this, SLOT(on_PNE_pressed()));
    connect(&SJF, SIGNAL(clicked(bool)), this, SLOT(on_SJF_pressed()));
    connect(&RR, SIGNAL(clicked(bool)), this, SLOT(on_RR_pressed()));
    connect(&SRTF, SIGNAL(clicked(bool)), this, SLOT(on_SRTF_pressed()));
    connect(&PE, SIGNAL(clicked(bool)), this, SLOT(on_PE_pressed()));
}
void MenuWidget::on_FCFS_pressed()
{
    emit FCFS_pressed();
}
void MenuWidget::on_RS_pressed()
{
    emit RS_pressed();
}
void MenuWidget::on_PNE_pressed()
{
    emit PNE_pressed();
}
void MenuWidget::on_SJF_pressed()
{
    emit SJF_pressed();
}
void MenuWidget::on_RR_pressed()
{
    emit RR_pressed();
}
void MenuWidget::on_SRTF_pressed()
{
    emit SRTF_pressed();
}
void MenuWidget::on_PE_pressed()
{
    emit PE_pressed();
}

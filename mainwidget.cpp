/*
    2024
    This file contains the definition of the class MainWidget.
*/
#include <mainwidget.hpp>

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    this->resize(1000,500);
    this->setLayout(&layout);
    title_widget = new TitleWidget{this};
    this->setWindowTitle("PROCESS PLANNING SIMULATOR");
    layout.addWidget(title_widget);
    connect(title_widget,SIGNAL(button_pressed()),this,SLOT(on_button_start_pressed()));
}


void MainWidget::disconnect_slots()
{
    disconnect(menu_widget,SIGNAL(FCFS_pressed()),this,SLOT(on_FCFS_pressed()));
    disconnect(menu_widget,SIGNAL(RS_pressed()),this,SLOT(on_RS_pressed()));
    disconnect(menu_widget,SIGNAL(PNE_pressed()),this,SLOT(on_PNE_pressed()));
    disconnect(menu_widget,SIGNAL(SJF_pressed()),this,SLOT(on_SJF_pressed()));
    disconnect(menu_widget,SIGNAL(RR_pressed()),this,SLOT(on_RR_pressed()));
    disconnect(menu_widget,SIGNAL(SRTF_pressed()),this,SLOT(on_SRTF_pressed()));
    disconnect(menu_widget,SIGNAL(PE_pressed()),this,SLOT(on_PE_pressed()));
}

void MainWidget::connect_slots()
{
    connect(menu_widget,SIGNAL(FCFS_pressed()),this,SLOT(on_FCFS_pressed()));
    connect(menu_widget,SIGNAL(RS_pressed()),this,SLOT(on_RS_pressed()));
    connect(menu_widget,SIGNAL(PNE_pressed()),this,SLOT(on_PNE_pressed()));
    connect(menu_widget,SIGNAL(SJF_pressed()),this,SLOT(on_SJF_pressed()));
    connect(menu_widget,SIGNAL(RR_pressed()),this,SLOT(on_RR_pressed()));
    connect(menu_widget,SIGNAL(SRTF_pressed()),this,SLOT(on_SRTF_pressed()));
    connect(menu_widget,SIGNAL(PE_pressed()),this,SLOT(on_PE_pressed()));
}

void MainWidget::on_button_start_pressed()
{
    layout.removeWidget(title_widget);
    disconnect(title_widget,SIGNAL(button_pressed()),this,SLOT(on_button_start_pressed()));
    delete title_widget;
    title_widget = nullptr;
    menu_widget = new MenuWidget{this};
    layout.addWidget(menu_widget);
    connect_slots();
}
void MainWidget::on_FCFS_pressed()
{
    layout.removeWidget(menu_widget);
    disconnect_slots();
    delete menu_widget;
    menu_widget = nullptr;
    simulation_widget = new SimulationWidget{AlgorithmType::FCFS,this};
    layout.addWidget(simulation_widget);
    connect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
}

void MainWidget::on_RS_pressed()
{
    layout.removeWidget(menu_widget);
    disconnect_slots();
    delete menu_widget;
    menu_widget = nullptr;
    simulation_widget = new SimulationWidget{AlgorithmType::RS,this};
    layout.addWidget(simulation_widget);
    connect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
}

void MainWidget::on_PNE_pressed()
{
    layout.removeWidget(menu_widget);
    disconnect_slots();
    delete menu_widget;
    menu_widget = nullptr;
    simulation_widget = new SimulationWidget{AlgorithmType::PNE,this};
    layout.addWidget(simulation_widget);
    connect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
}

void MainWidget::on_SJF_pressed()
{
    layout.removeWidget(menu_widget);
    disconnect_slots();
    delete menu_widget;
    menu_widget = nullptr;
    simulation_widget = new SimulationWidget{AlgorithmType::SJF,this};
    layout.addWidget(simulation_widget);
    connect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
}

void MainWidget::on_RR_pressed()
{
    layout.removeWidget(menu_widget);
    disconnect_slots();
    delete menu_widget;
    menu_widget = nullptr;
    simulation_widget = new SimulationWidget{AlgorithmType::RR,this};
    layout.addWidget(simulation_widget);
    connect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
}

void MainWidget::on_SRTF_pressed()
{
    layout.removeWidget(menu_widget);
    disconnect_slots();
    delete menu_widget;
    menu_widget = nullptr;
    simulation_widget = new SimulationWidget{AlgorithmType::SRTF,this};
    layout.addWidget(simulation_widget);
    connect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
}

void MainWidget::on_PE_pressed()
{
    layout.removeWidget(menu_widget);
    disconnect_slots();
    delete menu_widget;
    menu_widget = nullptr;
    simulation_widget = new SimulationWidget{AlgorithmType::PE,this};
    layout.addWidget(simulation_widget);
    connect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
}

void MainWidget::on_button_close_pressed()
{
    layout.removeWidget(simulation_widget);
    disconnect(simulation_widget,SIGNAL(button_close_pressed()),this,SLOT(on_button_close_pressed()));
    delete simulation_widget;
    simulation_widget = nullptr;
    menu_widget = new MenuWidget{this};
    layout.addWidget(menu_widget);
    connect_slots();
}

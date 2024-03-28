/*
    2024
    This file contains the declaration of the class MainWidget.
    this class is for managing the diferent widgets(let's say scenes) in the app
*/
#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>

#include <titlewidget.hpp>
#include <menuwidget.hpp>
#include <simulationwidget.hpp>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
private:
    QVBoxLayout layout;

    TitleWidget* title_widget;
    MenuWidget* menu_widget;
    SimulationWidget* simulation_widget;

    void disconnect_slots();
    void connect_slots();

private slots:
    void on_button_start_pressed();
    void on_FCFS_pressed();
    void on_RS_pressed();
    void on_PNE_pressed();
    void on_SJF_pressed();
    void on_RR_pressed();
    void on_SRTF_pressed();
    void on_PE_pressed();
    void on_button_close_pressed();
signals:
};

#endif // MAINWIDGET_HPP

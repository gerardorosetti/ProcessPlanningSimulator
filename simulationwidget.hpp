#ifndef SIMULATIONWIDGET_HPP
#define SIMULATIONWIDGET_HPP

#include <algorithms.hpp>
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>

class SimulationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationWidget(AlgorithmType, QWidget *parent = nullptr);
    void create_threads();
    ~SimulationWidget();
private:
    QVBoxLayout layout;
    QListWidget process;
    QPushButton button_close;
    std::shared_ptr<Algorithm> algorithm;
    //Algorithm& a;
    std::thread processes;
    std::thread processes_creator;
    //std::shared_ptr<Algorithm> algorithm;
    bool going;
private slots:
    void on_button_close_pressed();
signals:
    void button_close_pressed();
};

#endif // SIMULATIONWIDGET_HPP

#ifndef SIMULATIONWIDGET_HPP
#define SIMULATIONWIDGET_HPP

#include <algorithms.hpp>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QFrame>
#include <sstream>
#include <unordered_set>

class SimulationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationWidget(AlgorithmType, QWidget *parent = nullptr);
    void create_threads();
    ~SimulationWidget();

private:
    QVBoxLayout layout;
    QHBoxLayout processes_layout;
    QHBoxLayout buttons_layout;
    QListWidget process;
    QPushButton button_close;
    QPushButton button_stop;
    QListWidget processes_list;
    QListWidget current_list;
    QListWidget compleated_list;
    QListWidget blocked_list;

    std::shared_ptr<Algorithm> algorithm;

    std::thread processes;
    std::thread processes_creator;
    std::thread modify_lists;
    std::thread blocked_thread;

    //bool going;
    bool simulation_closed{false};
    bool has_blocked_list;

    void sleep_for(ulong);

private slots:
    void on_button_close_pressed();
    void on_button_stop_pressed();

signals:
    void button_close_pressed();
    void button_stop_pressed();
};

#endif // SIMULATIONWIDGET_HPP

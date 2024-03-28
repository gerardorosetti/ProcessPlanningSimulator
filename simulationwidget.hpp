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
#include <QLabel>
#include <unordered_set>

class SimulationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationWidget(AlgorithmType, QWidget *parent = nullptr);
    void create_threads();
    ~SimulationWidget();

private:
    //layouts
    QVBoxLayout layout;
    QHBoxLayout processes_layout;
    QHBoxLayout processes_tittle_layout;
    QHBoxLayout report_layout;
    QHBoxLayout buttons_layout;

    QVBoxLayout time_layout;
    QVBoxLayout cpu_layout;
    QVBoxLayout created_layout;
    QVBoxLayout compleated_layout;
    QVBoxLayout blocked_layout;
    QVBoxLayout waited_layout;
    QVBoxLayout execute_layout;
    QVBoxLayout blocked_t_layout;

    //lists
    QListWidget process;
    QListWidget processes_list;
    QListWidget current_list;
    QListWidget compleated_list;
    QListWidget blocked_list;

    //buttons
    QPushButton button_close;
    QPushButton button_stop;

    //labels for tittle report
    QLabel *total_time_t = new QLabel("Time: ");
    QLabel *CPU_usage_t = new QLabel("CPU usage");
    QLabel *total_process_created_t = new QLabel("Process Created: ");
    QLabel *total_process_compleated_t = new QLabel("Process Compleated: ");
    QLabel *total_process_blocked_t = new QLabel("Process Blocked: ");
    QLabel *average_waited_time_t = new QLabel("Average Waited Time: ");
    QLabel *average_executed_time_t = new QLabel("Average Executed Time: ");
    QLabel *average_blocked_time_t = new QLabel("average blocked time");


    //labels for reports
    QLabel *total_time = new QLabel("Time: ");
    QLabel *CPU_usage = new QLabel("CPU usage");
    QLabel *total_process_created = new QLabel("Process Created: ");
    QLabel *total_process_compleated = new QLabel("Process Compleated: ");
    QLabel *total_process_blocked = new QLabel("Process Blocked: ");
    QLabel *average_waited_time = new QLabel("Average Waited Time: ");
    QLabel *average_executed_time = new QLabel("Average Executed Time: ");
    QLabel *average_blocked_time = new QLabel("Average blocked time");

    //algorithm
    std::shared_ptr<Algorithm> algorithm;

    //threads
    std::thread processes;
    std::thread processes_creator;
    std::thread modify_lists;
    std::thread blocked_thread;

    bool simulation_closed{false};

    std::chrono::time_point<std::chrono::high_resolution_clock> start_point;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_point;

    void sleep_for(ulong);

private slots:
    void on_button_close_pressed();
    void on_button_stop_pressed();

signals:
    void button_close_pressed();
    void button_stop_pressed();
};

#endif // SIMULATIONWIDGET_HPP

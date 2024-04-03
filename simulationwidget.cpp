/*
    2024
    This file contains the definition of the class SimulationWidget.
*/
#include <simulationwidget.hpp>

SimulationWidget::SimulationWidget(AlgorithmType type, QWidget *parent)
    : QWidget{parent}
{
    GlobalVariables::going = true;
    this->setLayout(&layout);

    //Creation of Algorithm
    switch (type)
    {
        case AlgorithmType::FCFS:
            title_l->setText("First Come First Served");
            algorithm = std::make_shared<FirstComeFirstServed>();
            break;
        case AlgorithmType::RS:
            title_l->setText("Random Selection");
            algorithm = std::make_shared<RandomSelection>();
            break;
        case AlgorithmType::SJF:
            title_l->setText("Shortest Job First");
            algorithm = std::make_shared<ShortestJobFirst>();
            break;
        case AlgorithmType::PNE:
            title_l->setText("Priority");
            algorithm = std::make_shared<PrioritySelectionNonExpulsive>();
            break;
        case AlgorithmType::RR:
            title_l->setText("Round Robin");
            algorithm = std::make_shared<RoundRobin>();
            break;
        case AlgorithmType::SRTF:
            title_l->setText("Shortest Remaining Time First");
            algorithm = std::make_shared<ShortestRemainingTimeFirst>();
            break;
        case AlgorithmType::PE:
            title_l->setText("Priority");
            algorithm = std::make_shared<PrioritySelectionExpulsive>();
            break;
        default:
            break;
    }
    title_l->setStyleSheet("font-size: 30px; font-family: Times New Roman; color: #0D1321; font-weight: bold;");
    style_layouts();

    start_point = std::chrono::high_resolution_clock::now();
    create_threads();
}

void SimulationWidget::create_threads()
{
    ConcurrentQueue& processes_queue = algorithm->get_process_queue();
    ConcurrentQueue& blocked_queue = algorithm->get_blocked_queue();
    ConcurrentQueue& completed_queue = algorithm->get_completed_process();
    const Process& current_process = algorithm->get_current_process();
    /*
        The "processes_creator" thread its mean to be creating new process
        and adding them to the processes queue
    */
    auto process_creation = [this, &processes_queue] ()
    {
        sleep_for(1000);
        std::random_device rd;
        std::mt19937 gen(rd());
        Process::counter = 0;
        std::exponential_distribution<double> exp_dist(1.0 / GlobalVariables::lambda);
        Process random_process = Process::build_random_process(gen);
        while(GlobalVariables::going)
        {
            processes_queue.push(random_process);

            ++GlobalVariables::total_processes_created;

            double interval = exp_dist(gen);
            sleep_for(static_cast<int64_t>(interval));
            random_process = Process::build_random_process(gen);
        }
    };
    processes_creator = std::thread(process_creation);

    /*
        The "blocked_thread" thread its mean to be cheking the blocked queue
        pop them when their time ends and push them back to the processes queue
    */
    auto blocked = [this, &processes_queue, &blocked_queue] ()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int64_t> time_dist(1000, 3000);
        while(GlobalVariables::going)
        {
            if(!blocked_queue.empty())
            {
                int64_t random_time = time_dist(gen);
                sleep_for(random_time);
                Process curr = blocked_queue.pop();
                GlobalVariables::total_blocked_time += random_time;
                ++GlobalVariables::total_processes_blocked;
                curr.update_status(STATUS::CREATED);
                curr.update_creation_time(std::chrono::system_clock::now().time_since_epoch().count());
                processes_queue.push(curr);
            }
        }
    };
    blocked_thread = std::thread(blocked);

    /*
        The "modify lists" threads its mean to be updating the visual part
        of the application
    */
    auto mod_1 = [this, &processes_queue] ()
    {
        std::list<Process> tempList = processes_queue.toList();
        while (GlobalVariables::going)
        {
            if(!(tempList.size() == processes_queue.size()))
            {
                tempList = processes_queue.toList();
                processes_list.clear();

                for (const Process& process : tempList) {
                    std::stringstream ss;
                    ss << "Process ID: " << process.get_id() << " Time: " << process.get_time() <<" Priority: " << process.get_priority();
                    QString QStr = QString::fromStdString(ss.str());// Iterar sobre la cola de procesos y mostrar la información relevante en el QListWidget
                    processes_list.addItem(QStr);
                }
            }
        }
    };
    modify_process_list = std::thread(mod_1);

    auto mod_2 = [this, &current_process] ()
    {
        Process current = current_process;
        while (GlobalVariables::going)
        {
            if(current_process.get_id() != current.get_id())
            {
                current = current_process;
                current_list.clear();
                std::stringstream ss;
                ss << "Process ID: " << current.get_id() << " Time: " << current.get_time() <<" Priority: " << current.get_priority();
                QString QStr = QString::fromStdString(ss.str());
                current_list.addItem(QStr);
            }
            if(current_process.get_status() != STATUS::IN_EXECUTION) current_list.clear();
        }
    };
    modify_current_list = std::thread(mod_2);

    auto mod_3 = [this, &completed_queue] ()
    {
        std::list<Process> tempList = completed_queue.toList();
        while (GlobalVariables::going)
        {
            if(!(tempList.size() == completed_queue.size()))
            {
                tempList = completed_queue.toList();
                compleated_list.clear();

                for (const Process& process : tempList) {
                    std::stringstream ss;
                    ss << "Process ID: " << process.get_id() << " Time: " << process.get_time() <<" Priority: " << process.get_priority();
                    QString QStr = QString::fromStdString(ss.str());
                    compleated_list.addItem(QStr);
                }
            }
        }
    };
    modify_completed_list = std::thread(mod_3);

    auto mod_4 = [this, &blocked_queue] ()
    {
        std::list<Process> tempList = blocked_queue.toList();
        while (GlobalVariables::going)
        {
            if(tempList.size() != blocked_queue.size())
            {
                tempList = blocked_queue.toList();
                blocked_list.clear();

                for (const Process& process : tempList) {
                    std::stringstream ss;
                    ss << "Process ID: " << process.get_id() << " Time: " << process.get_time() <<" Priority: " << process.get_priority();
                    QString QStr = QString::fromStdString(ss.str());
                    blocked_list.addItem(QStr);
                }
            }
        }
    };
    modify_blocked_list = std::thread(mod_4);

    /*
        The "processes" thread its mean to be calling the method process algorithm
        and this method will start processing the process as long there are in the process queue
    */
    auto pred = [this] ()
    {
        algorithm->process_algorithm();
    };
    processes = std::thread{pred};
}

void SimulationWidget::on_button_close_pressed()
{
    GlobalVariables::going = false;
    simulation_closed = true;
    processes_creator.join();
    processes.join();
    modify_process_list.join();
    modify_current_list.join();
    modify_completed_list.join();
    modify_blocked_list.join();
    blocked_thread.join();

    GlobalVariables::reset();

    emit button_close_pressed();
}

void SimulationWidget::on_button_stop_pressed()
{
    /*
        its added the labels that shows the results of the simulation
        the method "GlobalVariables::update() calculate some of these results
    */
    if(GlobalVariables::going)
    {
        //this part if for calculate the time of the simulation
        end_point = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed_time = end_point - start_point;
        GlobalVariables::total_time = elapsed_time.count();
    }
    GlobalVariables::going = false;
    GlobalVariables::update();

    time_layout.addWidget(total_time_t);
    time_layout.addWidget(total_time);

    cpu_layout.addWidget(CPU_usage_t);
    cpu_layout.addWidget(CPU_usage);

    created_layout.addWidget(total_process_created_t);
    created_layout.addWidget(total_process_created);

    compleated_layout.addWidget(total_process_compleated_t);
    compleated_layout.addWidget(total_process_compleated);

    blocked_layout.addWidget(total_process_blocked_t);
    blocked_layout.addWidget(total_process_blocked);

    waited_layout.addWidget(average_waited_time_t);
    waited_layout.addWidget(average_waited_time);

    execute_layout.addWidget(average_executed_time_t);
    execute_layout.addWidget(average_executed_time);

    blocked_t_layout.addWidget(average_blocked_time_t);
    blocked_t_layout.addWidget(average_blocked_time);

    report_layout.addLayout(&time_layout);
    report_layout.addLayout(&cpu_layout);
    report_layout.addLayout(&created_layout);
    report_layout.addLayout(&compleated_layout);
    report_layout.addLayout(&blocked_layout);
    report_layout.addLayout(&waited_layout);
    report_layout.addLayout(&execute_layout);
    report_layout.addLayout(&blocked_t_layout);

    total_time->setText(QString::number(GlobalVariables::total_time));
    CPU_usage->setText(QString::number(GlobalVariables::CPU_usage));
    total_process_created->setText(QString::number(GlobalVariables::total_processes_created));
    total_process_compleated->setText(QString::number(GlobalVariables::total_processes_compleated));
    total_process_blocked->setText(QString::number(GlobalVariables::total_processes_blocked));
    average_waited_time->setText(QString::number(GlobalVariables::average_waited_time));
    average_executed_time->setText(QString::number(GlobalVariables::average_executed_time));
    average_blocked_time->setText(QString::number(GlobalVariables::average_blocked_time));
}

void SimulationWidget::style_layouts()
{
    /*
        in here we added the widget, labels and layouts that styles de simulation
    */
    title.addWidget(title_l);
    title.addLayout(&processes_title_layout);

    QWidget *containerWidget = new QWidget;
    containerWidget->setFixedSize(1020, 100);
    containerWidget->setLayout(&title);

    QLabel *ready = new QLabel("Ready");
    ready->setStyleSheet("font-size: 20px; color: rgba(0, 51, 102, 0.8); font-weight: bold;");
    processes_title_layout.addWidget(ready);

    QLabel *cpu = new QLabel("CPU");
    cpu->setStyleSheet("font-size: 20px; color: rgba(85, 107, 47, 0.8); font-weight: bold;");
    processes_title_layout.addWidget(cpu);

    QLabel *compleated = new QLabel("Compleated");
    compleated->setStyleSheet("font-size: 20px; color: rgba(0, 100, 0, 0.8); font-weight: bold;");
    processes_title_layout.addWidget(compleated);

    QLabel *blocked = new QLabel("Blocked");
    blocked->setStyleSheet("font-size: 20px; color: rgba(139, 0, 0, 0.8); font-weight: bold;");
    processes_title_layout.addWidget(blocked);

    layout.addWidget(containerWidget);

    processes_list.setFixedSize(230, 300);
    processes_list.setStyleSheet(
        "QListWidget {"
        "background-color: #ABB4B4;"
        "border: 2px solid #1a1a1a;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-family: Arial;"
        "}"
        "QListWidget::item {"
        "color: rgba(0, 51, 102, 0.8);"
        "text-align: center;"
        "font-weight: bold;"
        "}"
        "QListWidget::item:selected {"
        "background-color: #007bff;"
        "color: #fff;"
        "}"
        "QScrollBar:vertical {"
        "border: none;"
        "background: #f0f0f0;"
        "width: 10px;"
        "margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "background: #ccc;"
        "min-height: 20px;"
        "border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "background: #999;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::sub-page:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-page:vertical {"
        "background: none;"
        "}"
        );

    processes_layout.addWidget(&processes_list, 0, Qt::AlignCenter);
    current_list.setFixedSize(230, 50);
    current_list.setStyleSheet(
        "QListWidget {"
        "background-color: #ABB4B4;"
        "border: 2px solid #556b2f;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-family: Arial;"
        "}"
        "QListWidget::item {"
        "color: rgba(85, 107, 47, 0.8);"
        "text-align: center;"
        "font-weight: bold;"
        "}"
        "QListWidget::item:selected {"
        "background-color: #007bff;"
        "color: #fff;"
        "}"
        "QScrollBar::handle:vertical {"
        "background: #ccc;"
        "min-height: 20px;"
        "border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "background: #999;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::sub-page:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-page:vertical {"
        "background: none;"
        "}"
        );

    processes_layout.addWidget(&current_list, 0, Qt::AlignTop);
    compleated_list.setFixedSize(230, 300);
    compleated_list.setStyleSheet(
        "QListWidget {"
        "background-color: #ABB4B4;"
        "border: 2px solid #006400;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-family: Arial;"
        "}"
        "QListWidget::item {"
        "color: rgba(0, 100, 0, 0.8);"
        "text-align: center;"
        "font-weight: bold;"
        "}"
        "QListWidget::item:selected {"
        "background-color: #007bff;"
        "color: #fff;"
        "}"
        "QScrollBar:vertical {"
        "border: none;"
        "background: #f0f0f0;"
        "width: 10px;"
        "margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "background: #ccc;"
        "min-height: 20px;"
        "border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "background: #999;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::sub-page:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-page:vertical {"
        "background: none;"
        "}"
        );

    processes_layout.addWidget(&compleated_list, 0, Qt::AlignCenter);

    blocked_list.setFixedSize(230, 300);
    blocked_list.setStyleSheet(
        "QListWidget {"
        "background-color: #ABB4B4;"
        "border: 2px solid #8b0000;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-family: Arial;"
        "}"
        "QListWidget::item {"
        "color: rgba(139, 0, 0, 0.8);"
        "text-align: center;"
        "font-weight: bold;"
        "}"
        "QListWidget::item:selected {"
        "background-color: #007bff;"
        "color: #fff;"
        "}"
        "QScrollBar:vertical {"
        "border: none;"
        "background: #f0f0f0;"
        "width: 10px;"
        "margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "background: #ccc;"
        "min-height: 20px;"
        "border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "background: #999;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-line:vertical {"
        "background: none;"
        "}"
        "QScrollBar::sub-page:vertical {"
        "background: none;"
        "}"
        "QScrollBar::add-page:vertical {"
        "background: none;"
        "}"
        );
    processes_layout.addWidget(&blocked_list, 0, Qt::AlignCenter);

    layout.addLayout(&processes_layout);

    total_time_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");
    CPU_usage_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");
    total_process_created_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");
    total_process_compleated_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");
    total_process_blocked_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");
    average_waited_time_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");
    average_executed_time_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");
    average_blocked_time_t->setStyleSheet("font-size: 11px; color: #333; font-weight: bold;");

    layout.addLayout(&report_layout);

    button_stop.setFixedSize(250, 50);
    button_stop.setText("Stop Simulation");
    buttons_layout.addWidget(&button_stop, 0, Qt::AlignCenter);

    button_close.setFixedSize(250, 50);
    button_close.setText("Stop Simulation and Go to the Menu");
    buttons_layout.addWidget(&button_close, 0, Qt::AlignCenter);

    layout.addLayout(&buttons_layout);

    connect(&button_close, SIGNAL(clicked(bool)), this, SLOT(on_button_close_pressed()));
    connect(&button_stop, SIGNAL(clicked(bool)), this, SLOT(on_button_stop_pressed()));
}

//destructor for the simulationwidget
SimulationWidget::~SimulationWidget()
{
    if (!simulation_closed)
    {
        GlobalVariables::going = false;
        processes_creator.join();
        processes.join();
        modify_process_list.join();
        modify_current_list.join();
        modify_completed_list.join();
        modify_blocked_list.join();
        blocked_thread.join();
    }
    GlobalVariables::reset();
}

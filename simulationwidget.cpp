#include "simulationwidget.hpp"

SimulationWidget::SimulationWidget(AlgorithmType type, QWidget *parent)
    : QWidget{parent}
{
    GlobalVariables::going = true;
    Process::counter = 1;
    //Creation of Algorithm
    switch (type)
    {
        case AlgorithmType::FCFS:
            algorithm = std::make_shared<FirstComeFirstServed>();
            has_blocked_list = false;
            break;
        case AlgorithmType::RS:
            algorithm = std::make_shared<RandomSelection>();
            has_blocked_list = false;
            break;
        case AlgorithmType::SJF:
            algorithm = std::make_shared<ShortestJobFirst>();
            has_blocked_list = false;
            break;
        case AlgorithmType::PNE:
            algorithm = std::make_shared<PrioritySelectionNonExpulsive>();
            has_blocked_list = false;
            break;
        case AlgorithmType::RR:
            algorithm = std::make_shared<RoundRobin>();
            has_blocked_list = true;
            break;
        case AlgorithmType::SRTF:
            algorithm = std::make_shared<ShortestRemainingTimeFirst>();
            has_blocked_list = true;
            break;
        case AlgorithmType::PE:
            algorithm = std::make_shared<PrioritySelectionExpulsive>();
            has_blocked_list = true;
            break;
        default:
            break;
    }

    this->setLayout(&layout);

    processes_list.setFixedSize(200, 300);
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
    current_list.setFixedSize(200, 50);
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
    compleated_list.setFixedSize(200, 300);
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

    if (has_blocked_list)
    {
        blocked_list.setFixedSize(200, 300);
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
    }

    layout.addLayout(&processes_layout);

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

    //layout.addLayout(&report_layout);

    button_stop.setFixedSize(250, 50);
    button_stop.setText("Stop Simulation");
    buttons_layout.addWidget(&button_stop, 0, Qt::AlignCenter);

    button_close.setFixedSize(250, 50);
    button_close.setText("Stop Simulation and Go to the Menu");
    buttons_layout.addWidget(&button_close, 0, Qt::AlignCenter);

    layout.addLayout(&buttons_layout);

    connect(&button_close, SIGNAL(clicked(bool)), this, SLOT(on_button_close_pressed()));
    connect(&button_stop, SIGNAL(clicked(bool)), this, SLOT(on_button_stop_pressed()));

    create_threads();
    GlobalVariables::total_time = std::chrono::system_clock::now().time_since_epoch().count();
}

void SimulationWidget::sleep_for(ulong time)
{
    auto tp1 = std::chrono::high_resolution_clock::now();
    uint64_t time_counter = 0;
    while(GlobalVariables::going)
    {
        auto tp2 = std::chrono::high_resolution_clock::now();
        time_counter += std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
        if(time_counter >= time)
        {
            break;
        }
        tp1 = tp2;
        std::this_thread::sleep_for(std::chrono::milliseconds(GlobalVariables::tick));
    }
}

void SimulationWidget::create_threads()
{

    auto pred = [this] () {
        algorithm->process_algorithm();
    };
    processes = std::thread{pred};

    sleep_for(150);

    ConcurrentQueue& processes_queue = algorithm->get_process_queue();
    ConcurrentQueue& blocked_queue = algorithm->get_blocked_queue();

    auto process_creation = [this, &processes_queue] ()  {
        std::random_device rd;
        std::mt19937 gen(rd());
        Process::counter = 1;
        std::exponential_distribution<double> exp_dist(1.0 / GlobalVariables::lambda);

        //sleep_for(250);
        Process random_process = Process::build_random_process(gen);
        //sleep_for(250);
        while(GlobalVariables::going)
        {
            std::stringstream ss;
            ss << "Process ID: " << random_process.get_id();
            QString qstr = QString::fromStdString(ss.str());
            QListWidgetItem* newItem = new QListWidgetItem(qstr);
            newItem->setTextAlignment(Qt::AlignHCenter);
            processes_list.addItem(newItem);
            processes_queue.push(random_process);

            ++GlobalVariables::total_processes_created;

            double interval = exp_dist(gen);
            sleep_for(static_cast<ulong>(interval));
            random_process = Process::build_random_process(gen);
        }
        Process::counter = 1;
    };
    processes_creator = std::thread(process_creation);
    auto mod = [this] ()
    {
        const Process& current = algorithm->get_current_process();
        bool waiting = false;
        while (GlobalVariables::going)
        {
            std::stringstream ss;
            ss << "Process ID: " << current.get_id();
            QString QStr = QString::fromStdString(ss.str());
            if (current.get_status() == STATUS::IN_EXECUTION && !waiting)
            {
                waiting = true;
                for (int i = 0; i < processes_list.count(); ++i)
                {
                    QListWidgetItem* item = processes_list.item(i);
                    QString str1 = item->text();
                    if(str1 == QStr)
                    {
                        processes_list.removeItemWidget(processes_list.takeItem(i));
                        break;
                    }
                }
                QListWidgetItem* newItem = new QListWidgetItem(QStr);
                newItem->setTextAlignment(Qt::AlignHCenter);
                current_list.addItem(newItem);
            }


            if (current.get_status() == STATUS::COMPLETED && waiting)
            {

                ++GlobalVariables::total_processes_compleated;

                waiting = false;
                current_list.removeItemWidget(current_list.takeItem(0));
                QListWidgetItem* newItem = new QListWidgetItem(QStr);
                newItem->setTextAlignment(Qt::AlignHCenter);
                compleated_list.addItem(newItem);
                for (int var = 0; var < processes_list.count(); ++var)
                {
                    QListWidgetItem* item = processes_list.item(var);
                    QString str1 = item->text();
                    if(str1 == QStr)
                    {
                        processes_list.removeItemWidget(processes_list.takeItem(var));
                        break;
                    }
                }
            }
            if(current.get_status() == STATUS::BLOCKED && waiting && has_blocked_list)
            {
                waiting = false;
                current_list.removeItemWidget(current_list.takeItem(0));
                QListWidgetItem* newItem = new QListWidgetItem(QStr);
                newItem->setTextAlignment(Qt::AlignHCenter);
                blocked_list.addItem(newItem);
            }
        }
    };
    modify_lists = std::thread(mod);
    if (has_blocked_list)
    {

        auto blocked = [this, &processes_queue, &blocked_queue] ()
        {           
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<size_t> time_dist(1000, 3000);
            while(GlobalVariables::going)
            {
                if(!blocked_queue.empty())
                {
                    Process curr = blocked_queue.pop();
                    size_t random_time = time_dist(gen);
                    sleep_for(random_time);
                    GlobalVariables::total_blocked_time += random_time;
                    ++GlobalVariables::total_processes_blocked;

                    blocked_list.removeItemWidget(blocked_list.takeItem(0));
                    std::stringstream ss;
                    ss << "Process ID: " << curr.get_id();
                    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(ss.str()));
                    newItem->setTextAlignment(Qt::AlignHCenter);
                    processes_list.addItem(newItem);
                    curr.update_status(STATUS::READY);
                    curr.update_creation_time(std::chrono::system_clock::now().time_since_epoch().count());
                    processes_queue.push(curr);
                }
            }
        };

        blocked_thread = std::thread(blocked);
    }
}

void SimulationWidget::on_button_close_pressed()
{
    GlobalVariables::going = false;
    simulation_closed = true;
    if (has_blocked_list)
    {
        blocked_thread.join();
    }
    processes_creator.join();
    processes.join();
    modify_lists.join();

    GlobalVariables::reset();

    emit button_close_pressed();
}

void SimulationWidget::on_button_stop_pressed()
{
    GlobalVariables::going = false;
    GlobalVariables::update();

    total_time->setText(QString::number(GlobalVariables::total_time));
    CPU_usage->setText(QString::number(GlobalVariables::CPU_usage));
    total_process_created->setText(QString::number(GlobalVariables::total_processes_created));
    total_process_compleated->setText(QString::number(GlobalVariables::total_processes_compleated));
    total_process_blocked->setText(QString::number(GlobalVariables::total_processes_blocked));
    average_waited_time->setText(QString::number(GlobalVariables::average_waited_time));
    average_executed_time->setText(QString::number(GlobalVariables::average_executed_time));
    average_blocked_time->setText(QString::number(GlobalVariables::average_blocked_time));
    layout.addLayout(&report_layout);
}

SimulationWidget::~SimulationWidget()
{
    if (!simulation_closed)
    {
        GlobalVariables::going = false;
        processes_creator.join();
        processes.join();
        modify_lists.join();
        if (has_blocked_list)
        {
            blocked_thread.join();
        }
    }
    GlobalVariables::reset();
}

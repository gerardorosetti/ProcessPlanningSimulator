#include "simulationwidget.hpp"

SimulationWidget::SimulationWidget(AlgorithmType type, QWidget *parent)
    : QWidget{parent}/*, GlobalVariables::going{true}*/
{
    GlobalVariables::going = true;
    Process::counter = 1;
    //Creation of Algorithm
    switch (type)
    {
        case AlgorithmType::FCFS:
            std::cout << "FCFS" << std::endl;
            algorithm = std::make_shared<FirstComeFirstServed>();
            has_blocked_list = false;
            break;
        case AlgorithmType::RS:
            std::cout << "RS" << std::endl;
            algorithm = std::make_shared<RandomSelection>();
            has_blocked_list = false;
            break;
        case AlgorithmType::SJF:
            std::cout << "SJF" << std::endl;
            algorithm = std::make_shared<ShortestJobFirst>();
            has_blocked_list = false;
            break;
        case AlgorithmType::PNE:
            std::cout << "PNE" << std::endl;
            algorithm = std::make_shared<PrioritySelectionNonExpulsive>();
            has_blocked_list = false;
            break;
        case AlgorithmType::RR:
            std::cout << "RR" << std::endl;
            algorithm = std::make_shared<RoundRobin>();
            has_blocked_list = true;
            break;
        case AlgorithmType::SRTF:
            std::cout << "SRTF" << std::endl;
            algorithm = std::make_shared<ShortestRemainingTimeFirst>();
            has_blocked_list = true;
            break;
        case AlgorithmType::PE:
            std::cout << "PE" << std::endl;
            algorithm = std::make_shared<PrioritySelectionExpulsive>();
            has_blocked_list = true;
            break;
        default:
            std::cout << "DEFAULT" << std::endl;
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
    //processes_list.itemAlignment()
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
        algorithm->process_algorithm(/*&GlobalVariables::going*/);
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
            //processes_set.insert(qstr);
            //Qt::AlignHCenter
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
        //std::unordered_set<QString> compleated_set;
        //std::unordered_set<QString> processes_set;
        const Process& current = algorithm->get_current_process();
        bool waiting = false;
        //sleep_for(500);
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
                    //const auto& item = processes_list.takeItem(i);
                    QListWidgetItem* item = processes_list.item(i);
                    QString str1 = item->text();
                    if(str1 == QStr)
                    {
                        processes_list.removeItemWidget(processes_list.takeItem(i));
                        break;
                    }
                    //processes_list.insertItem(i, item);
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
                //compleated_set.insert(QStr);
                for (int var = 0; var < processes_list.count(); ++var)
                {
                    //const auto& item = processes_list.takeItem(var);
                    QListWidgetItem* item = processes_list.item(var);
                    QString str1 = item->text();
                    if(str1 == QStr)
                    {
                        processes_list.removeItemWidget(processes_list.takeItem(var));
                        break;
                    }
                    //processes_list.insertItem(var, item);
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
            //std::this_thread::sleep_for(std::chrono::milliseconds{1});
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
            //sleep_for(time_dis);
            while(GlobalVariables::going/* || !blocked_queue.empty()*/ /*|| blocked_queue.size() > 0*/)
            {
                if(!blocked_queue.empty())
                {
                    Process curr = blocked_queue.pop();
                    size_t random_time = time_dist(gen);
                    //sleep_for(random_time);
                    sleep_for(100000000);
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
                //std::this_thread::sleep_for(std::chrono::milliseconds{1});
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
    //simulation_closed = true;
    //if (has_blocked_list)
    //{
    //    blocked_thread.join();
    //}
    //processes_creator.join();
    //processes.join();
    //modify_lists.join();

    //emit button_stop_pressed();
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

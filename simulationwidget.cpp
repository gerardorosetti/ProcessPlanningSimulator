#include "simulationwidget.hpp"

SimulationWidget::SimulationWidget(AlgorithmType type, QWidget *parent)
    : QWidget{parent}, going{true}
{

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

    processes_list.setFixedSize(120, 300);
    processes_layout.addWidget(&processes_list, 0, Qt::AlignCenter);
    current_list.setFixedSize(120, 50);
    processes_layout.addWidget(&current_list, 0, Qt::AlignTop);
    compleated_list.setFixedSize(120, 300);
    processes_layout.addWidget(&compleated_list, 0, Qt::AlignCenter);
    if (has_blocked_list)
    {
        blocked_list.setFixedSize(120, 300);
        processes_layout.addWidget(&blocked_list, 0, Qt::AlignCenter);
    }

    layout.addLayout(&processes_layout);

    button_close.setFixedSize(250, 50);
    button_close.setText("Stop Simulation and Go to the Menu");
    layout.addWidget(&button_close, 0, Qt::AlignCenter);

    connect(&button_close, SIGNAL(clicked(bool)), this, SLOT(on_button_close_pressed()));

    create_threads();
}

void SimulationWidget::sleep_for(ulong time)
{
    auto tp1 = std::chrono::high_resolution_clock::now();
    uint64_t time_counter = 0;
    while(going)
    {
        auto tp2 = std::chrono::high_resolution_clock::now();
        time_counter += std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
        if(time_counter >= time)
        {
            break;
        }
        tp1 = tp2;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void SimulationWidget::create_threads()
{
    ConcurrentQueue& processes_queue = algorithm->get_process_queue();
    ConcurrentQueue& blocked_queue = algorithm->get_blocked_queue();

    auto pred = [this] () {
        algorithm->process_algorithm(&going);
    };
    processes = std::thread{pred};
    auto process_creation = [this, &processes_queue] ()  {
        std::random_device rd;
        std::mt19937 gen(rd());
        while(going)
        {
            Process random_process = Process::build_random_process(gen);
            std::stringstream ss;
            ss << "Process ID: " << random_process.get_id();
            processes_list.addItem(QString::fromStdString(ss.str()));
            processes_queue.push(random_process);
            //std::this_thread::sleep_for(std::chrono::milliseconds{1200});
            sleep_for(1200);
        }
        Process::counter = 0;
    };
    processes_creator = std::thread(process_creation);
    auto mod = [this] ()
    {
        const Process& current = algorithm->get_current_process();
        bool waiting = false;
        while (going)
        {
            std::stringstream ss;
            ss << "Process ID: " << current.get_id();
            QString QStr = QString::fromStdString(ss.str());
            if (current.get_status() == STATUS::IN_EXECUTION && !waiting)
            {
                waiting = true;
                for (int i = 0; i < processes_list.count(); ++i)
                {
                    const auto& item = processes_list.takeItem(i);
                    QString str1 = item->text();
                    if(str1 == QStr)
                    {
                        processes_list.removeItemWidget(item);
                        break;
                    }
                    processes_list.insertItem(i, item);
                }
                current_list.addItem(QStr);
            }
            if (current.get_status() == STATUS::COMPLETED && waiting)
            {
                waiting = false;
                current_list.removeItemWidget(current_list.takeItem(0));
                compleated_list.addItem(QStr);
            }
            if(current.get_status() == STATUS::BLOCKED && waiting && has_blocked_list)
            {
                waiting = false;
                current_list.removeItemWidget(current_list.takeItem(0));
                blocked_list.addItem(QStr);
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
    };
    modify_lists = std::thread(mod);
    if (has_blocked_list)
    {
        auto blocked = [this, &processes_queue, &blocked_queue] ()
        {
            while(going/* || !blocked_queue.empty()*/ /*|| blocked_queue.size() > 0*/)
            {
                if(!blocked_queue.empty())
                {
                    Process curr = blocked_queue.pop();
                    sleep_for(1500);
                    blocked_list.removeItemWidget(blocked_list.takeItem(0));
                    std::stringstream ss;
                    ss << "Process ID: " << curr.get_id();
                    processes_list.addItem(QString::fromStdString(ss.str()));
                    curr.update_status(STATUS::READY);
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
    going = false;
    simulation_closed = true;
    if (has_blocked_list)
    {
        blocked_thread.join();
    }
    processes_creator.join();
    processes.join();
    modify_lists.join();

    emit button_close_pressed();
}

SimulationWidget::~SimulationWidget()
{
    if (!simulation_closed)
    {
        going = false;
        processes_creator.join();
        processes.join();
        modify_lists.join();
        if (has_blocked_list)
        {
            blocked_thread.join();
        }
    }
}

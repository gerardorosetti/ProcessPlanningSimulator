#include "simulationwidget.hpp"

SimulationWidget::SimulationWidget(AlgorithmType type, QWidget *parent)
    : QWidget{parent}, going{true}/*, process_queue{nullptr}, blocked_queue{nullptr}*/
{

    switch (type)
    {
        case AlgorithmType::FCFS:
        std::cout << "FCFS" << std::endl;
            algorithm = std::make_shared<FirstComeFirstServed>();
            break;
        case AlgorithmType::RS:
            std::cout << "RS" << std::endl;
            algorithm = std::make_shared<RandomSelection>();
            break;
        case AlgorithmType::SJF:
            std::cout << "SJF" << std::endl;
            algorithm = std::make_shared<ShortestJobFirst>();
            break;
        case AlgorithmType::PNE:
            std::cout << "PNE" << std::endl;
            algorithm = std::make_shared<PrioritySelectionNonExpulsive>();
            break;
        case AlgorithmType::RR:
            std::cout << "RR" << std::endl;
            algorithm = std::make_shared<RoundRobin>();
            break;
        case AlgorithmType::SRTF:
            std::cout << "SRTF" << std::endl;
            algorithm = std::make_shared<ShortestRemainingTimeFirst>();
            break;
        case AlgorithmType::PE:
            std::cout << "PE" << std::endl;
            algorithm = std::make_shared<PrioritySelectionExpulsive>();
            break;
        default:
            std::cout << "DEFAULT" << std::endl;
            break;
    }

    //process_queue = algorithm->get_process_queue();
    //blocked_queue = algorithm->get_blocked_queue();
    //algorithm = std::make_shared<RandomSelection>();
    this->setLayout(&layout);

    processes_list.setFixedSize(120, 300);
    processes_layout.addWidget(&processes_list, 0, Qt::AlignCenter);
    current_list.setFixedSize(120, 50);
    processes_layout.addWidget(&current_list, 0, Qt::AlignTop);
    compleated_list.setFixedSize(120, 300);
    processes_layout.addWidget(&compleated_list, 0, Qt::AlignCenter);
    blocked_list.setFixedSize(120, 300);
    processes_layout.addWidget(&blocked_list, 0, Qt::AlignCenter);

    layout.addLayout(&processes_layout);

    button_close.setFixedSize(250, 50);
    button_close.setText("Stop Simulation and Go to the Menu");
    layout.addWidget(&button_close, 0, Qt::AlignCenter);

    connect(&button_close, SIGNAL(clicked(bool)), this, SLOT(on_button_close_pressed()));

    create_threads();
    //layout.addWidget(process);
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
        //auto& processes_queue = algorithm->get_process_queue();
        while(going)
        {
            Process random_process = Process::build_random_process(gen);
            std::stringstream ss;
            ss << "Process ID: " << random_process.get_id();
            processes_list.addItem(QString::fromStdString(ss.str()));
            processes_queue.push(random_process);
            std::this_thread::sleep_for(std::chrono::milliseconds{1000});
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
                //std::stringstream ss;
                //ss << "Process ID: " << current.get_id();
                //QString QStr = QString::fromStdString(ss.str());
                waiting = true;
                for (int i = 0; i < processes_list.count(); ++i)
                {
                    const auto& item = processes_list.takeItem(i);
                    QString str1 = item->text();
                    //if(QString::compare(str1, QStr, Qt::CaseSensitive))
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
                //std::stringstream ss;
                //ss << "Process ID: " << current.get_id();
                waiting = false;
                current_list.removeItemWidget(current_list.takeItem(0));
                compleated_list.addItem(QStr);
            }
            if(current.get_status() == STATUS::BLOCKED && waiting)
            {
                waiting = false;
                current_list.removeItemWidget(current_list.takeItem(0));
                blocked_list.addItem(QStr);
            }
            /*if(current.get_status() == STATUS::READY)
            {

            }*/
        }
    };
    modify_lists = std::thread(mod);

    auto blocked = [this, &processes_queue, &blocked_queue] ()
    {
        //ConcurrentQueue& blocked_queue = algorithm->get_blocked_queue();
        //ConcurrentQueue& process_queue = algorithm->get_process_queue();
        while(going)
        {
            Process curr = blocked_queue.pop();
            std::this_thread::sleep_for(std::chrono::milliseconds{2000});
            /*
            if (curr.get_status() == STATUS::BLOCKED)
            {
                curr.update_status(STATUS::READY);
                processes_queue.push(curr);
                std::cout << "cualquier cosa\n";
                blocked_list.removeItemWidget(blocked_list.takeItem(0));
                std::stringstream ss;
                ss << "Process ID: " << curr.get_id();
                processes_list.addItem(QString::fromStdString(ss.str()));
            }
            */

            blocked_list.removeItemWidget(blocked_list.takeItem(0));
            std::stringstream ss;
            ss << "Process ID: " << curr.get_id();
            processes_list.addItem(QString::fromStdString(ss.str()));
            curr.update_status(STATUS::READY);
            processes_queue.push(curr);
        }
    };
    blocked_thread = std::thread(blocked);
}

void SimulationWidget::on_button_close_pressed()
{
    going = false;
    simulation_closed = true;
    //std::cout << "\ntest3\n";
    processes_creator.join();
    //processes_creator.detach();
    //std::cout << "\ntest2\n";
    processes.join();
    blocked_thread.join();
    modify_lists.join();
    //processes.detach();
    //std::cout << "\ntest1\n";
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
        blocked_thread.join();
    }
}

#include "simulationwidget.hpp"

SimulationWidget::SimulationWidget(AlgorithmType type, QWidget *parent)
    : QWidget{parent}, going{true}
{
    switch (type)
    {
        case AlgorithmType::FCFS:
            algorithm = std::make_shared<FirstComeFirstServed>();
            break;
        case AlgorithmType::RS:
            algorithm = std::make_shared<RandomSelection>();
            break;
        case AlgorithmType::SJF:
            algorithm = std::make_shared<ShortestJobFirst>();
            break;
        case AlgorithmType::PNE:
            algorithm = std::make_shared<PrioritySelectionNonExpulsive>();
            break;
        case AlgorithmType::RR:
            algorithm = std::make_shared<RoundRobin>();
            break;
        case AlgorithmType::SRTF:
            algorithm = std::make_shared<ShortestRemainingTimeFirst>();
            break;
        case AlgorithmType::PE:
            algorithm = std::make_shared<PrioritySelectionExpulsive>();
            break;
        default:
            break;
    }

    this->setLayout(&layout);

    button_close.setFixedSize(100, 50);
    button_close.setText("Close");
    layout.addWidget(&button_close, 0, Qt::AlignCenter);

    connect(&button_close, SIGNAL(clicked(bool)), this, SLOT(on_button_close_pressed()));

    create_threads();
    //layout.addWidget(process);
}

void SimulationWidget::create_threads()
{

    auto pred = [this] () {
        algorithm->process_algorithm(&going);
    };
    processes = std::thread{pred};
    auto process_creation = [this] ()  {
        std::random_device rd;
        std::mt19937 gen(rd());
        auto& processes_queue = algorithm->get_process_queue();
        while(going)
        {
            Process random_process = Process::build_random_process(gen);
            //std::stringstream ss;
            //ss << "Process ID: " << random_process.get_id();
            //QString QStr = QString::fromStdString(ss.str());
            //ui->ProcessesListWidget->addItem(QStr);
            //int id = random_process.get_id();
            //ui->ProcessesListWidget->insertItem(id, QStr);
            processes_queue.push(random_process);
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }
        Process::counter = 0;
        std::cout << "\nMaincra\n";
    };
    processes_creator = std::thread(process_creation);
}

void SimulationWidget::on_button_close_pressed()
{
    going = false;
    //std::cout << "\ntest3\n";
    processes_creator.join();
    //processes_creator.detach();
    //std::cout << "\ntest2\n";
    processes.join();
    //processes.detach();
    //std::cout << "\ntest1\n";
    emit button_close_pressed();
}

SimulationWidget::~SimulationWidget()
{
    //going = false;
    //processes_creator.join();
    //processes.join();
}

/*
    2024
    This file contains the definition of the class TitleWidget.
*/
#include <titlewidget.hpp>

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle("Title Widget");  // Set the title of the widget
    this->setFixedSize(750, 550);
    this->setLayout(&layout);

    QLabel *label = new QLabel("PROCESS PLANNING SIMULATOR");
    label->setStyleSheet("font-size: 30px; color: #333; font-weight: bold;");
    label->setAlignment(Qt::AlignCenter);
    layout.addWidget(label);

    button.setFixedSize(100, 50);
    button.setText("Start");
    layout.addWidget(&button, 0, Qt::AlignCenter);

    connect(&button, SIGNAL(clicked(bool)), this, SLOT(on_button_pressed()));
}

void TitleWidget::on_button_pressed()
{
    emit button_pressed();
}

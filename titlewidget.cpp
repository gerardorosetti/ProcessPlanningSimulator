/*
    2024
    This file contains the definition of the class TitleWidget.
*/
#include <titlewidget.hpp>

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle("Title Widget");  // Set the title of the widget
    this->setLayout(&layout);
    QLabel *label = new QLabel("PROCESS PLANNING SIMULATOR");
    label->setStyleSheet("font-size: 36px; font-family: Times New Roman; color: #0D1E32; font-weight: bold;");
    label->setAlignment(Qt::AlignCenter);
    layout.addWidget(label);

    button.setFixedSize(140, 50);
    button.setText("Start");
    button.setStyleSheet("QPushButton {"
                            "background-color: #98BEEC; "
                            "font-size: 20; "
                            "color: black; "
                            "text-align: center; "
                            "font-family: Times New Roman; "
                            "border-radius: 4px; "
                            "}"
                         "QPushButton:hover {"
                            "background-color: #648AB9; "
                            "color: black; "
                            "}"
                         "QPushButton:pressed {"
                            "background-color: #0D1E32; "
                            "color: white; "
                            "}");
    layout.addWidget(&button, 0, Qt::AlignCenter);

    connect(&button, SIGNAL(clicked(bool)), this, SLOT(on_button_pressed()));
}

void TitleWidget::on_button_pressed()
{
    emit button_pressed();
}

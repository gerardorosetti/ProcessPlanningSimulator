/*
    2024
    This file contains the declaration of the class TittleWidget.
    This widget shows the first scene whit the tittle and a button start
*/
#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = nullptr);
private:
    QVBoxLayout layout;
    QPushButton button;
private slots:
    void on_button_pressed();
signals:
    void button_pressed();
};

#endif // TITLEWIDGET_H

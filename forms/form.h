//
// Created by lionos on 8/27/24.
//
#ifndef FORM_H
#define FORM_H

#include <QWidget>

class QLabel;
class QPushButton;

class Form : public QWidget {
    Q_OBJECT

    public:
        Form(QWidget *parent = nullptr);

        private slots:
            void showMessage();

    private:
        QLabel *label;
        QPushButton *button;
};

#endif
#include "form.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

Form::Form(QWidget *parent) : QWidget(parent)
{
    label = new QLabel("Presiona el botón para ver el mensaje.");
    button = new QPushButton("Presionar");

    connect(button, &QPushButton::clicked, this, &Form::showMessage);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(button);

    setLayout(layout);
    setWindowTitle("Ejemplo de Interfaz con Qt");
    resize(300, 150);
}

void Form::showMessage()
{
    label->setText("¡Hola Mundo!");
}

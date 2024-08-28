#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Ejemplo de Interfaz con Qt");

    QLabel* label = new QLabel("Presiona el botón para ver el mensaje.");
    QPushButton* button = new QPushButton("Presionar");

    QObject::connect(button, &QPushButton::clicked, [&](){
        label->setText("¡Hola Mundo!");
    });

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(button);

    window.setLayout(layout);
    window.resize(300, 150);
    window.show();

    return app.exec();
}
#include <QApplication>
#include "Frame.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Frame window;
    window.show();

    return app.exec();
}

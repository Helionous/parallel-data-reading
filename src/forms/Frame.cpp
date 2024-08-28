#include "Frame.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QScreen>
#include <QTimer>
#include <QMessageBox>

Frame::Frame(QWidget *parent) : QWidget(parent)
{
    labelCriteria = new QLabel("Métodos de busqueda");
    labelEnterRUC = new QLabel("Ingrese el RUC");
    lineEditRUC = new QLineEdit;

    radioButtonParallelSearch = new QRadioButton("BUSQUEDA POR PARALELISMO");
    radioButtonRAMSearch = new QRadioButton("BUSQUEDA DESDE LA RAM");
    radioButtonSimpleSearch = new QRadioButton("BUSQUEDA SIMPLE");

    buttonSearch = new QPushButton("BUSCAR");

    progressBar = new QProgressBar;
    progressBar->setVisible(false);

    labelResult = new QLabel;

    connect(buttonSearch, &QPushButton::clicked, this, &Frame::onSearchClicked);

    QHBoxLayout *layoutRadioButtons = new QHBoxLayout;
    layoutRadioButtons->addWidget(radioButtonParallelSearch);
    layoutRadioButtons->addWidget(radioButtonRAMSearch);
    layoutRadioButtons->addWidget(radioButtonSimpleSearch);

    lineEditRUC->setFixedSize(200, 30);
    buttonSearch->setFixedSize(200, 30);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(labelCriteria);
    centralLayout->addLayout(layoutRadioButtons);
    centralLayout->addWidget(labelEnterRUC, 0, Qt::AlignCenter);
    centralLayout->addWidget(lineEditRUC, 0, Qt::AlignCenter);
    centralLayout->addWidget(buttonSearch, 0, Qt::AlignCenter);
    centralLayout->addWidget(progressBar);
    centralLayout->addWidget(labelResult);

    setLayout(centralLayout);
    setWindowTitle("BUSQUEDA POR RUC");
    resize(700, 300);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void Frame::onSearchClicked()
{
    if (!radioButtonParallelSearch->isChecked() &&
        !radioButtonRAMSearch->isChecked() &&
        !radioButtonSimpleSearch->isChecked()) {
        QMessageBox::warning(this, "Advertencia", "Por favor, seleccione un método de búsqueda.");
        return;
    }

    progressBar->setVisible(true);
    progressBar->setRange(0, 0);

    buttonSearch->setEnabled(false);
    radioButtonParallelSearch->setEnabled(false);
    radioButtonRAMSearch->setEnabled(false);
    radioButtonSimpleSearch->setEnabled(false);

    if (radioButtonParallelSearch->isChecked()) {
        labelResult->setText("Executing parallel search...");
        // Call the specific function for parallel search
    } else if (radioButtonRAMSearch->isChecked()) {
        labelResult->setText("Executing RAM search...");
        // Call the specific function for RAM search
    } else if (radioButtonSimpleSearch->isChecked()) {
        labelResult->setText("Executing simple search...");
        // Call the specific function for simple search
    }

    QTimer::singleShot(5000, [this]() {
        progressBar->setVisible(false);
        buttonSearch->setEnabled(true);
        radioButtonParallelSearch->setEnabled(true);
        radioButtonRAMSearch->setEnabled(true);
        radioButtonSimpleSearch->setEnabled(true);
    });
}

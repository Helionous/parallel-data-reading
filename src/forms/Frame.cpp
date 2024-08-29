#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QScreen>
#include <QTimer>
#include <QMessageBox>
#include <QIntValidator>
#include <QLineEdit>

#include "Frame.h"
#include "Person.h"
#include "SearchParallelism.h"
#include "SearchRamMemory.h"

Frame::Frame(QWidget *parent) : QWidget(parent) {
    labelCriteria = new QLabel("Métodos de búsqueda");
    labelEnterRUC = new QLabel("Ingrese el RUC:");

    lineEditRUC = new QLineEdit();
    QRegularExpression regex("\\d{11}");
    validator = new QRegularExpressionValidator(regex, this);
    lineEditRUC->setValidator(validator);

    radioButtonParallelSearch = new QRadioButton("Búsqueda por Paralelismo");
    radioButtonRAMSearch = new QRadioButton("Búsqueda desde la RAM");
    radioButtonSimpleSearch = new QRadioButton("Búsqueda Simple");

    buttonSearch = new QPushButton("Buscar");

    progressBar = new QProgressBar;
    progressBar->setVisible(false);

    labelResult = new QLabel;

    connect(buttonSearch, &QPushButton::clicked, this, &Frame::onSearchClicked);

    layoutRadioButtons = new QHBoxLayout;
    layoutRadioButtons->addWidget(radioButtonParallelSearch);
    layoutRadioButtons->addWidget(radioButtonRAMSearch);
    layoutRadioButtons->addWidget(radioButtonSimpleSearch);

    lineEditRUC->setFixedSize(200, 30);
    buttonSearch->setFixedSize(200, 30);

    centralLayout = new QVBoxLayout;
    centralLayout->addWidget(labelCriteria);
    centralLayout->addLayout(layoutRadioButtons);
    centralLayout->addWidget(labelEnterRUC, 0, Qt::AlignCenter);
    centralLayout->addWidget(lineEditRUC, 0, Qt::AlignCenter);
    centralLayout->addWidget(buttonSearch, 0, Qt::AlignCenter);
    centralLayout->addWidget(progressBar);
    centralLayout->addWidget(labelResult);

    setLayout(centralLayout);
    setWindowTitle("Consulta RUC");
    resize(700, 300);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void Frame::onSearchClicked() {
    QString rucText = lineEditRUC->text();

    if (rucText.size() != 11) {
        QMessageBox::warning(this, "Advertencia", "El RUC debe tener exactamente 11 dígitos.");
        return;
    }

    if (!radioButtonParallelSearch->isChecked() &&
        !radioButtonRAMSearch->isChecked() &&
        !radioButtonSimpleSearch->isChecked()) {
        QMessageBox::warning(this, "Advertencia", "Por favor, seleccione un método de búsqueda.");
        return;
    }

    progressBar->setVisible(true);
    progressBar->setValue(0);

    buttonSearch->setEnabled(false);
    radioButtonParallelSearch->setEnabled(false);
    radioButtonRAMSearch->setEnabled(false);
    radioButtonSimpleSearch->setEnabled(false);

    QString rucString = lineEditRUC->text();
    std::string ruc = rucString.toStdString();
    QString resultText;

    if (radioButtonParallelSearch->isChecked()) {
        labelResult->setText("Executing parallel search...");

        auto [person, elapsedTime] = performParallelSearch(ruc);
        if (person.getRuc().empty()) {
            resultText = "No person found with RUC: " + rucString;
        } else {
            resultText = QString::fromStdString(person.toString());
        }
        resultText += "\nTiempo transcurrido: " + QString::number(elapsedTime) + " ms";
        labelResult->setText(resultText);

    } else if (radioButtonRAMSearch->isChecked()) {
        labelResult->setText("Executing RAM search...");

        auto [person, elapsedTime] = performParallelMemorySearch(ruc);
        if (person.getRuc().empty()) {
            resultText = "No person found with RUC: " + rucString;
        } else {
            resultText = QString::fromStdString(person.toString());
        }
        resultText += "\nTiempo transcurrido: " + QString::number(elapsedTime) + " ms";
        labelResult->setText(resultText);

    } else if (radioButtonSimpleSearch->isChecked()) {
        labelResult->setText("Executing simple search...");
    }

    QTimer::singleShot(0, [this]() {
        progressBar->setValue(progressBar->maximum());
        progressBar->setVisible(false);
        buttonSearch->setEnabled(true);
        radioButtonParallelSearch->setEnabled(true);
        radioButtonRAMSearch->setEnabled(true);
        radioButtonSimpleSearch->setEnabled(true);
    });
}

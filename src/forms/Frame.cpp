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
#include <QApplication>
#include <QProgressDialog>
#include <QFutureWatcher>

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

    radioButtonParallelSearch = new QRadioButton("Búsqueda por Paralelismo", this);
    radioButtonRAMSearch = new QRadioButton("Búsqueda desde la RAM", this);
    radioButtonSimpleSearch = new QRadioButton("Búsqueda Simple", this);

    buttonSearch = new QPushButton("Buscar", this);

    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);

    labelResult = new QLabel;

    connect(buttonSearch, &QPushButton::clicked, this, &Frame::onSearchClicked);
    connect(radioButtonRAMSearch, &QRadioButton::clicked, this, &Frame::clickedRadioButton);

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
}
void Frame::clickedRadioButton()
{
    if (radioButtonRAMSearch->isChecked()) {
        if (SearchRamMemory::persons.empty()) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Mensaje");
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setText("Cargar datos a la memoria ram.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            QObject::connect(&msgBox, &QMessageBox::buttonClicked, this, [this](QAbstractButton* button) {
                if (button->text() == "OK") {
                    QProgressDialog progressDialog("Cargando datos...", "Cancelar", 0, 0, this);
                    progressDialog.setWindowModality(Qt::WindowModal);
                    progressDialog.setMinimumDuration(0);
                    progressDialog.setCancelButton(nullptr);
                    progressDialog.show();

                    std::future<void> future = std::async(std::launch::async, []() {
                        SearchRamMemory searchRamMemory;
                        searchRamMemory.loadData();
                    });

                    while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
                        qApp->processEvents();
                    }

                    future.get();

                    progressDialog.hide();
                    QMessageBox::information(this, "Éxito", "Datos cargados correctamente.");
                }
            });
            msgBox.exec();
        }
    }
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
    progressBar->setRange(0, 0);
    QApplication::processEvents();

    buttonSearch->setEnabled(false);
    radioButtonParallelSearch->setEnabled(false);
    radioButtonRAMSearch->setEnabled(false);
    radioButtonSimpleSearch->setEnabled(false);

    if (radioButtonParallelSearch->isChecked()) {
        PerformParallelSearch(rucText);
    } else if (radioButtonRAMSearch->isChecked()) {
        PerformParallelMemorySearch(rucText);
    } else if (radioButtonSimpleSearch->isChecked()) {
        labelResult->setText("Executing simple search...");
    }

    QTimer::singleShot(0, [this]() {
        progressBar->setVisible(false);
        buttonSearch->setEnabled(true);
        radioButtonParallelSearch->setEnabled(true);
        radioButtonRAMSearch->setEnabled(true);
        radioButtonSimpleSearch->setEnabled(true);
    });
}

QString resultText;

void Frame::PerformParallelSearch(const QString &ruc) const
{
    labelResult->setText("Executing parallel search...");

    auto [person, elapsedTime] = performParallelSearch(ruc.toStdString());
    if (person.getRuc().empty()) {
        resultText = "No person found with RUC: " + ruc;
    } else {
        resultText = QString::fromStdString(person.toString());
    }
    resultText += "\nTiempo transcurrido: " + QString::number(elapsedTime) + " ms";
    labelResult->setText(resultText);
}

void Frame::PerformParallelMemorySearch(const QString &ruc) const
{
    labelResult->setText("Executing RAM search...");

    SearchRamMemory searchRamMemory;
    auto [person, elapsedTime] = searchRamMemory.SearchByRuc(ruc.toStdString());
    if (person.getRuc().empty()) {
        resultText = "No person found with RUC: " + ruc;
    } else {
        resultText = QString::fromStdString(person.toString());
    }
    resultText += "\nTiempo transcurrido: " + QString::number(elapsedTime) + " ms";
    labelResult->setText(resultText);
}

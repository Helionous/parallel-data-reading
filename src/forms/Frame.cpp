#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QApplication>
#include <QFutureWatcher>
#include <QProgressDialog>

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

    labelResult = new QLabel;
    labelResult->setWordWrap(true);

    scrollArea = new QScrollArea();
    scrollWidget = new QWidget;
    scrollLayout = new QVBoxLayout;
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(400,250);
    scrollLayout->addWidget(labelResult);
    scrollLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

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
    centralLayout->addSpacing(30);
    centralLayout->addWidget(scrollArea, 0, Qt::AlignCenter);

    setLayout(centralLayout);
    setWindowTitle("Consulta RUC");
    resize(900, 500);
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
            QObject::connect(&msgBox, &QMessageBox::buttonClicked, this,[this, &msgBox](QAbstractButton* button) {
                if (button->text() == "OK") {
                    msgBox.accept();
                    QProgressDialog progressDialog("Cargando datos...", "", 0, 0, this);
                    progressDialog.setWindowFlag(Qt::FramelessWindowHint);
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

    if (radioButtonParallelSearch->isChecked()) {
        QProgressDialog progressDialog("Buscando directo del archivo...", "", 0, 0, this);
        progressDialog.setWindowFlag(Qt::FramelessWindowHint);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setMinimumDuration(0);
        progressDialog.setCancelButton(nullptr);
        progressDialog.show();

        handleSearchProcess(true);
        std::future<void> future = std::async(std::launch::async, [this,rucText]() {
            PerformParallelSearch(rucText);
        });
        while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
            qApp->processEvents();
        }
        future.get();
        progressDialog.hide();
        handleSearchProcess(false);

    } else if (radioButtonRAMSearch->isChecked()) {
        QProgressDialog progressDialog("Buscando desde la memoria ram...", "", 0, 0, this);
        progressDialog.setWindowFlag(Qt::FramelessWindowHint);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setMinimumDuration(0);
        progressDialog.setCancelButton(nullptr);
        progressDialog.show();

        handleSearchProcess(true);
        std::future<void> future = std::async(std::launch::async, [this,rucText]() {
            PerformParallelMemorySearch(rucText);
        });
        while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
            qApp->processEvents();
        }
        future.get();
        progressDialog.hide();
        handleSearchProcess(false);
    } else if (radioButtonSimpleSearch->isChecked()) {
        QProgressDialog progressDialog("Buscando...", "", 0, 0, this);
        progressDialog.setWindowFlag(Qt::FramelessWindowHint);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setMinimumDuration(0);
        progressDialog.setCancelButton(nullptr);
        progressDialog.show();

        handleSearchProcess(true);
        std::future<void> future = std::async(std::launch::async, [this,rucText]() {
            PerformSimpleSearch(rucText);
        });
        while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
            qApp->processEvents();
        }
        future.get();
        progressDialog.hide();
        handleSearchProcess(false);
    }
}

void Frame::handleSearchProcess(bool start) {
    buttonSearch->setEnabled(!start);
    radioButtonParallelSearch->setEnabled(!start);
    radioButtonRAMSearch->setEnabled(!start);
    radioButtonSimpleSearch->setEnabled(!start);
}

QString resultText;
void Frame::PerformParallelSearch(const QString &ruc) const
{
    auto [person, elapsedTime] = performParallelSearch(ruc.toStdString());
    QString newResult;
    if (person.getRuc().empty()) {
        newResult = QString("RUC NO ENCONTRADO: %1\n").arg(ruc);
    } else {
        newResult = QString("RUC: %1\n"
                            "RAZON SOCIAL: %2\n"
                            "ESTADO DEL CONTRIBUYENTE: %3\n"
                            "CONDICION DE DOMICILIO: %4\n")
                     .arg(QString::fromStdString(person.getRuc()))
                     .arg(QString::fromStdString(person.getName()))
                     .arg(QString::fromStdString(person.getStatus()))
                     .arg(QString::fromStdString(person.getDomicileCondition()));
    }
    newResult += QString("\nTiempo de busqueda: %1 ms").arg(elapsedTime);
    newResult += QString("\n============================================\n");

    resultText = newResult + resultText; // Prepend new result
    labelResult->setText(resultText);
    labelResult->adjustSize();
}

void Frame::PerformParallelMemorySearch(const QString &ruc) const
{
    SearchRamMemory searchRamMemory;
    auto [person, elapsedTime] = searchRamMemory.SearchByRuc(ruc.toStdString());
    QString newResult;
    if (person.getRuc().empty()) {
        newResult = QString("RUC NO ENCONTRADO: %1\n").arg(ruc);
    } else {
        newResult = QString("RUC: %1\n"
                            "RAZON SOCIAL: %2\n"
                            "ESTADO DEL CONTRIBUYENTE: %3\n"
                            "CONDICION DE DOMICILIO: %4\n")
                     .arg(QString::fromStdString(person.getRuc()))
                     .arg(QString::fromStdString(person.getName()))
                     .arg(QString::fromStdString(person.getStatus()))
                     .arg(QString::fromStdString(person.getDomicileCondition()));
    }
    newResult += QString("\nTiempo de busqueda: %1 ms").arg(elapsedTime);
    newResult += QString("\n============================================\n");

    resultText = newResult + resultText; // Prepend new result
    labelResult->setText(resultText);
    labelResult->adjustSize();
}

void Frame::PerformSimpleSearch(const QString &ruc) const
{

}
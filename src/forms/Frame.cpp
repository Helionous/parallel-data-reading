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
#include <QtConcurrent>

#include "Frame.h"
#include "Person.h"
#include "SearchParallelism.h"
#include "SearchRamMemory.h"
#include "SearchSimple.h"
#include "GlobalVar.h"

Frame::Frame(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #121212; color: #e0e0e0;");

    lineEditRUC = new QLineEdit();
    QRegularExpression regex("\\d{11}");
    validator = new QRegularExpressionValidator(regex, this);
    lineEditRUC->setValidator(validator);
    lineEditRUC->setFixedSize(350, 40);
    lineEditRUC->setStyleSheet("background-color: #333333; border: 2px solid #4caf50; border-radius: 8px; padding: 5px; color: #e0e0e0;");

    buttonSearch = new QPushButton("Buscar", this);
    buttonSearch->setFixedSize(100, 40);
    buttonSearch->setStyleSheet("background-color: #4caf50; color: #ffffff; border: 2px solid #4caf50; border-radius: 8px;");

    QFont font = lineEditRUC->font();
    font.setPointSize(16);
    lineEditRUC->setFont(font);
    buttonSearch->setFont(font);

    QHBoxLayout *searchRowLayout = new QHBoxLayout;
    searchRowLayout->addWidget(lineEditRUC);
    searchRowLayout->addWidget(buttonSearch);
    searchRowLayout->setSpacing(10);
    searchRowLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout *radioButtonsRowLayout = new QHBoxLayout;
    radioButtonParallelSearch = new QRadioButton("Lectura paralela", this);
    radioButtonRAMSearch = new QRadioButton("Desde la memoria ram", this);
    radioButtonSimpleSearch = new QRadioButton("Lectura Simple", this);

    radioButtonParallelSearch->setStyleSheet("color: #bb86fc;");
    radioButtonRAMSearch->setStyleSheet("color: #03dac6;");
    radioButtonSimpleSearch->setStyleSheet("color: #ff80ab;");

    radioButtonsRowLayout->addWidget(radioButtonParallelSearch);
    radioButtonsRowLayout->addWidget(radioButtonRAMSearch);
    radioButtonsRowLayout->addWidget(radioButtonSimpleSearch);
    radioButtonsRowLayout->setSpacing(5);
    radioButtonsRowLayout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *searchAndRadioLayout = new QVBoxLayout;
    searchAndRadioLayout->addLayout(searchRowLayout);
    searchAndRadioLayout->addLayout(radioButtonsRowLayout);

    labelResult = new QLabel;
    labelResult->setWordWrap(true);
    scrollArea = new QScrollArea();
    scrollWidget = new QWidget;
    scrollLayout = new QVBoxLayout;
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(500, 380);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollLayout->addWidget(labelResult);
    scrollLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    scrollArea->setVisible(false);

    scrollArea->setStyleSheet(
        "QScrollArea {"
        "  border: none;"
        "  background-color: #1e1e1e;"
        "}"
        "QScrollBar:vertical {"
        "  border: none;"
        "  background: #2c2c2c;"
        "  width: 12px;"
        "  border-radius: 6px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #4caf50;"
        "  border-radius: 6px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  background: #2c2c2c;"
        "  border: none;"
        "  border-radius: 6px;"
        "}"
        "QScrollBar::add-line:vertical {"
        "  border-image: url(:/icons/arrow-down.png) 0;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "  border-image: url(:/icons/arrow-up.png) 0;"
        "}"
    );

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addLayout(searchAndRadioLayout);
    centralLayout->addWidget(scrollArea);
    setLayout(centralLayout);

    setWindowTitle("Buscador por RUC");
    resize(1100, 400);

    connect(buttonSearch, &QPushButton::clicked, this, &Frame::onSearchClicked);
    connect(radioButtonRAMSearch, &QRadioButton::clicked, this, &Frame::clickedRadioButton);
    connect(this, &Frame::searchCompleted, this, &Frame::updateResult);
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
                    progressDialog.setStyleSheet(
                        "QProgressDialog { "
                        "    background-color: #1e1e1e; "
                        "    border: 2px solid #27ae60; "
                        "    border-radius: 3px; "
                        "    padding: 20px; "
                        "}"
                        "QLabel { "
                        "    color: #ffffff; "
                        "    background-color: transparent; "
                        "    padding: 5px; "
                        "}"
                        "QProgressBar { "
                        "    border: none; "
                        "    border-radius: 8px; "
                        "    height: 10px; "
                        "    background-color: #333333; "
                        "}"
                        "QProgressBar::chunk { "
                        "    background-color: #4caf50; "
                        "    border-radius: 8px; "
                        "}"
                    );
                    progressDialog.show();

                    std::future<void> future = std::async(std::launch::async, []() {
                        SearchRamMemory searchRamMemory(filePathToProcess);
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

void Frame::handleSearchProcess(bool start) {
    buttonSearch->setEnabled(!start);
    radioButtonParallelSearch->setEnabled(!start);
    radioButtonRAMSearch->setEnabled(!start);
    radioButtonSimpleSearch->setEnabled(!start);
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

    QProgressDialog progressDialog("Buscando...", "", 0, 0, this);
    progressDialog.setWindowFlag(Qt::FramelessWindowHint);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setMinimumDuration(0);
    progressDialog.setCancelButton(nullptr);
    progressDialog.setStyleSheet(
        "QProgressDialog { "
        "    background-color: #1e1e1e; "
        "    border: 2px solid #27ae60; "
        "    border-radius: 3px; "
        "    padding: 20px; "
        "}"
        "QLabel { "
        "    color: #ffffff; "
        "    background-color: transparent; "
        "    padding: 5px; "
        "}"
        "QProgressBar { "
        "    border: none; "
        "    border-radius: 8px; "
        "    height: 10px; "
        "    background-color: #333333; "
        "}"
        "QProgressBar::chunk { "
        "    background-color: #4caf50; "
        "    border-radius: 8px; "
        "}"
    );
    progressDialog.show();

    handleSearchProcess(true);

    std::future<void> future;

    if (radioButtonParallelSearch->isChecked()) {
        future = std::async(std::launch::async, [this, rucText]() {
            PerformParallelSearch(rucText);
        });
    } else if (radioButtonRAMSearch->isChecked()) {
        future = std::async(std::launch::async, [this, rucText]() {
            PerformParallelMemorySearch(rucText);
        });
    } else if (radioButtonSimpleSearch->isChecked()) {
        future = std::async(std::launch::async, [this, rucText]() {
            PerformSimpleSearch(rucText);
        });
    }

    while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
        qApp->processEvents();
    }
    future.get();
    progressDialog.hide();
    handleSearchProcess(false);
}


void Frame::updateResult(const QString &result) {
    resultText = result + resultText;
    labelResult->setText(resultText);
    labelResult->adjustSize();
}

void Frame::PerformParallelSearch(const QString &ruc){
    scrollArea->setVisible(true);
    SearchParallelism searchParallelism(filePathToProcess);
    auto [person, elapsedTime] = searchParallelism.performParallelSearch(ruc.toStdString());

    QString newResult;
    if (person.getRuc().empty()) {
        newResult = QString("<font color='#bb86fc'>RUC NO ENCONTRADO: %1</font><br>").arg(ruc);
    } else {
        QString statusColor = (person.getStatus() == "ACTIVO") ? "#4caf50" : "orange";
        QString domicileColor = (person.getDomicileCondition() == "HABIDO") ? "#4caf50" : "orange";
        newResult = QString("<font color='#bb86fc'>"
                            "RUC: <font color='#4caf50'>%1</font><br>"
                            "RAZON SOCIAL: <font color='#4caf50'>%2</font><br>"
                            "ESTADO DEL CONTRIBUYENTE: <font color='%3'>%4</font><br>"
                            "CONDICION DE DOMICILIO: <font color='%5'>%6</font><br></font>")
                     .arg(QString::fromStdString(person.getRuc()))
                     .arg(QString::fromStdString(person.getName()))
                     .arg(statusColor)
                     .arg(QString::fromStdString(person.getStatus()))
                     .arg(domicileColor)
                     .arg(QString::fromStdString(person.getDomicileCondition()));
    }

    newResult += QString("<br>Tiempo de búsqueda (lectura en paralelo): %1 ms<br>").arg(elapsedTime);
    newResult += QString("<br>────────────────────────────<br>");

    emit searchCompleted(newResult);
}

void Frame::PerformParallelMemorySearch(const QString &ruc){
    scrollArea->setVisible(true);
    SearchRamMemory searchRamMemory(filePathToProcess);
    auto [person, elapsedTime] = searchRamMemory.SearchByRuc(ruc.toStdString());

    QString newResult;
    if (person.getRuc().empty()) {
        newResult = QString("<font color='#03dac6'>RUC NO ENCONTRADO: %1</font><br>").arg(ruc);
    } else {
        QString statusColor = (person.getStatus() == "ACTIVO") ? "#4caf50" : "orange";
        QString domicileColor = (person.getDomicileCondition() == "HABIDO") ? "#4caf50" : "orange";
        newResult = QString("<font color='#03dac6'>"
                            "RUC: <font color='#4caf50'>%1</font><br>"
                            "RAZON SOCIAL: <font color='#4caf50'>%2</font><br>"
                            "ESTADO DEL CONTRIBUYENTE: <font color='%3'>%4</font><br>"
                            "CONDICION DE DOMICILIO: <font color='%5'>%6</font><br></font>")
                     .arg(QString::fromStdString(person.getRuc()))
                     .arg(QString::fromStdString(person.getName()))
                     .arg(statusColor)
                     .arg(QString::fromStdString(person.getStatus()))
                     .arg(domicileColor)
                     .arg(QString::fromStdString(person.getDomicileCondition()));
    }

    newResult += QString("<br>Tiempo de búsqueda (lectura en paralelo): %1 ms<br>").arg(elapsedTime);
    newResult += QString("<br>────────────────────────────<br>");

    emit searchCompleted(newResult);
}

void Frame::PerformSimpleSearch(const QString &ruc){
    scrollArea->setVisible(true);
    SearchSimple searchSimple(filePathToProcess);
    auto [person, elapsedTime] = searchSimple.searchSimpleByRuc(ruc.toStdString());

    QString newResult;
    if (person.getRuc().empty()) {
        newResult = QString("<font color='#ff80ab'>RUC NO ENCONTRADO: %1</font><br>").arg(ruc);
    } else {
        QString statusColor = (person.getStatus() == "ACTIVO") ? "#4caf50" : "orange";
        QString domicileColor = (person.getDomicileCondition() == "HABIDO") ? "#4caf50" : "orange";
        newResult = QString("<font color='#ff80ab'>"
                            "RUC: <font color='#4caf50'>%1</font><br>"
                            "RAZON SOCIAL: <font color='#4caf50'>%2</font><br>"
                            "ESTADO DEL CONTRIBUYENTE: <font color='%3'>%4</font><br>"
                            "CONDICION DE DOMICILIO: <font color='%5'>%6</font><br></font>")
                     .arg(QString::fromStdString(person.getRuc()))
                     .arg(QString::fromStdString(person.getName()))
                     .arg(statusColor)
                     .arg(QString::fromStdString(person.getStatus()))
                     .arg(domicileColor)
                     .arg(QString::fromStdString(person.getDomicileCondition()));
    }

    newResult += QString("<br>Tiempo de búsqueda (lectura en paralelo): %1 ms<br>").arg(elapsedTime);
    newResult += QString("<br>────────────────────────────<br>");

    emit searchCompleted(newResult);
}

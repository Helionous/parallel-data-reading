#ifndef FRAME_H
#define FRAME_H

#include <QWidget>
#include <QScrollArea>

class QLabel;
class QPushButton;
class QLineEdit;
class QRadioButton;
class QVBoxLayout;
class QProgressBar;
class QRegularExpressionValidator;

class Frame : public QWidget {
    Q_OBJECT

    public:
        explicit Frame(QWidget *parent = nullptr);

    signals:
        void searchCompleted(const QString& result);

    private slots:
        void onSearchClicked();
        void updateResult(const QString& result);
        void handleSearchProcess(bool start);

    private:
        QString resultText;
        void clickedRadioButton();
        void PerformParallelSearch(const QString &ruc);
        void PerformParallelMemorySearch(const QString &ruc);
        void PerformSimpleSearch(const QString &ruc);

        QLabel *labelEnterRUC;
        QLabel *labelResult;
        QLineEdit *lineEditRUC;
        QRadioButton *radioButtonParallelSearch;
        QRadioButton *radioButtonRAMSearch;
        QRadioButton *radioButtonSimpleSearch;
        QPushButton *buttonSearch;
        QProgressBar *progressBar;
        QVBoxLayout *centralLayout;
        QRegularExpressionValidator *validator;

        QScrollArea *scrollArea;
        QWidget *scrollWidget;
        QVBoxLayout *scrollLayout;
};

#endif

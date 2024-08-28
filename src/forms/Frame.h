#ifndef FRAME_H
#define FRAME_H

#include <QWidget>

class QLabel;
class QPushButton;
class QLineEdit;
class QRadioButton;
class QProgressBar;

class Frame : public QWidget {
    Q_OBJECT

public:
    explicit Frame(QWidget *parent = nullptr);

    private slots:
        void onSearchClicked();

private:
    QLabel *labelCriteria;
    QLabel *labelEnterRUC;
    QLabel *labelResult;
    QLineEdit *lineEditRUC;
    QRadioButton *radioButtonParallelSearch;
    QRadioButton *radioButtonRAMSearch;
    QRadioButton *radioButtonSimpleSearch;
    QPushButton *buttonSearch;
    QProgressBar *progressBar;
};

#endif

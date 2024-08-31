#ifndef FRAME_H
#define FRAME_H

#include <QWidget>

class QLabel;
class QPushButton;
class QLineEdit;
class QRadioButton;
class QHBoxLayout;
class QVBoxLayout;
class QProgressBar;
class QRegularExpressionValidator;

class Frame : public QWidget {
    Q_OBJECT

public:
    explicit Frame(QWidget *parent = nullptr);

private slots:
    void onSearchClicked();
    void clickedRadioButton();
    void PerformParallelSearch(const QString &ruc) const;
    void PerformParallelMemorySearch(const QString &ruc) const;

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
    QHBoxLayout *layoutRadioButtons;
    QVBoxLayout *centralLayout;
    QRegularExpressionValidator *validator;
};

#endif

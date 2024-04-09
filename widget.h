#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include "fileparser.h"
#include "plotmatrix.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    ~Widget();

private:
    Ui::Widget *ui;
    QPushButton *loadFileButton;
    QLabel *fileLabel;
    QPainter *painter;
    FileParser parser;
    PlotMatrix plotmatrix;
    void drawMatrix(std::vector<std::vector<int>> matrix);

private slots:
    void handleLoadFileButton(void);
};
#endif // WIDGET_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include "fileparser.h"
#include "plotmatrix.h"

/* Defines geometry of dispayed application. */
#define APP_WINDOW_WIDTH 800
#define APP_WINDOW_HEIGHT 600
#define PLOT_TOP_OFFSET 75
#define PLOT_MATRIX_FULL_WIDTH 800
#define PLOT_MATRIX_FULL_HEIGHT 500

/* TODO: This is depends on the shape of dot int the plotmatrix.
 * Margin is added so no out of boundaries access happens when matrix is updated.
 * Better to decouple somehow. */
#define PLOT_MATRIX_PLOTTING_MARGIN 5
#define PLOT_MATRIX_WIDTH (PLOT_MATRIX_FULL_WIDTH - 2 * PLOT_MATRIX_PLOTTING_MARGIN)
#define PLOT_MATRIX_HIGHT (PLOT_MATRIX_FULL_HEIGHT - 2 * PLOT_MATRIX_PLOTTING_MARGIN)

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
    std::vector<std::vector<int>> matrix;
    void drawMatrix();

private slots:
    void handleLoadFileButton(void);
};
#endif // WIDGET_H

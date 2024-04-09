#include "widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include "fileparser.h"
#include "plotmatrix.h"
#include "ui_widget.h"

#define TOP_OFFSET 75

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    loadFileButton = new QPushButton(this);
    loadFileButton->setText("Load");
    loadFileButton->setGeometry(QRect(QPoint(1, 1), QSize(75, 25)));

    fileLabel = new QLabel(this);
    fileLabel->setText("File:");
    fileLabel->setGeometry(QRect(QPoint(101, 1), QSize(500, 25)));

    painter = new QPainter(this);

    connect(loadFileButton, &QPushButton::released, this, &Widget::handleLoadFileButton);
}

void Widget::handleLoadFileButton(void)
{
    /* 1. Get file */
    /* Only allows Sample Files */
    QString path = QFileDialog::getOpenFileName(this, NULL, NULL, "Sample Files (*.ssd *.rsd)");
    if (!path.isEmpty()) {
        QString file = "File: ";
        fileLabel->setText(file + path);
    }

    /* 2. Call parser on selected file */

    FileParserResultStatus result = Widget::parser.parseLogFile(path.toStdString());

    /* 3. Get results if parsing was successfull */

    if (result == FILE_PARSER_RESULT_STATUS_FAILED_TO_OPEN_FILE) {
        QMessageBox msgBox;
        msgBox.setText("No such file: " + path);
        msgBox.exec();
        /* Nothing to do anymore return */
        return;
    }

    std::vector<LogEntry> parsedLogEntries = Widget::parser.getParsedLogEntries();

    /* 4. Send results to the matrix */
    Widget::plotmatrix.updatePlotMatrixWithLogEntries(parsedLogEntries);
}

void Widget::paintEvent(QPaintEvent *event)
{
    drawMatrix(plotmatrix.getPlotMatrix());
}

void Widget::drawMatrix(std::vector<std::vector<int>> matrix)
{
    painter->begin(this);
    for (int i = 0; i < PLOT_MATRIX_FULL_WIDTH; i++) {
        for (int j = 0; j < PLOT_MATRIX_FULL_HEIGHT; j++) {
            if (matrix[i][j] == 1) {
                QColor color(0, 0, 0);
                painter->setPen(color);
            } else {
                QColor color(255, 255, 255);
                painter->setPen(color);
            }
            /* Painter is upside down, so flip while drawing */
            painter->drawPoint(i, PLOT_MATRIX_FULL_HEIGHT - 1 - j + TOP_OFFSET);
        }
    }

    painter->end();
}

Widget::~Widget()
{
    delete ui;
}

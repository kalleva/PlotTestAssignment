#include "widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include "fileparser.h"
#include "plotmatrix.h"
#include "ui_widget.h"


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

    /* Fills canvas with zeroes initially. */
    matrix = std::vector<std::vector<int>>(PLOT_MATRIX_FULL_WIDTH,
                                           std::vector<int>(PLOT_MATRIX_FULL_HEIGHT, 0));

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

    /* 2. Call parser on selected file parsedLogEntries will contain  */
    std::vector<LogEntry> parsedLogEntries;
    FileParserResultStatus result = Widget::parser.parseLogFile(path.toStdString(),
                                                                parsedLogEntries);

    /* 3. Get results if parsing was successfull */
    if (result == FILE_PARSER_RESULT_STATUS_FAILED_TO_OPEN_FILE) {
        QMessageBox msgBox;
        msgBox.setText("No such file: " + path);
        msgBox.exec();
        /* Nothing to do anymore return */
        return;
    } else if (result == FILE_PARSER_RESULT_STATUS_BAD_FILE) {
        /* TODO: Currently function parseLogFile doesn't return this status */
        return;
    }

    /* 4. Clear current matrix */
    /* Fill canvas with zeroes before updatin it with new values */
    for (auto &i : matrix)
        std::fill(i.begin(), i.end(), 0);

    /* 5. Update matrix with new entries */
    Widget::plotmatrix.updatePlotMatrixWithLogEntries(parsedLogEntries,
                                                      matrix,
                                                      PLOT_MATRIX_WIDTH,
                                                      PLOT_MATRIX_HIGHT,
                                                      PLOT_MATRIX_PLOTTING_MARGIN);
}

void Widget::paintEvent(QPaintEvent *event)
{
    drawMatrix();
}

void Widget::drawMatrix(void)
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
            painter->drawPoint(i, PLOT_MATRIX_FULL_HEIGHT - 1 - j + PLOT_TOP_OFFSET);
        }
    }

    painter->end();
}

Widget::~Widget()
{
    delete ui;
}

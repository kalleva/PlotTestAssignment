#ifndef PLOTMATRIX_H
#define PLOTMATRIX_H

#include "main.h"
#include "point.h"
#include <vector>

#define PLOT_MATRIX_FULL_WIDTH 800
#define PLOT_MATRIX_FULL_HEIGHT 500
#define PLOT_MATRIX_MARGIN 5
#define PLOT_MATRIX_WIDTH (PLOT_MATRIX_FULL_WIDTH - 2 * PLOT_MATRIX_MARGIN)
#define PLOT_MATRIX_HIGHT (PLOT_MATRIX_FULL_HEIGHT - 2 * PLOT_MATRIX_MARGIN)

class PlotMatrix
{
public:
    PlotMatrix();
    void updatePlotMatrixWithLogEntries(std::vector<LogEntry> parsedLogEntries);
    std::vector<std::vector<int>> getPlotMatrix(void);

private:
    std::vector<std::vector<int>> matrix;
    std::vector<Point> tranformLogEntriesToPoints(std::vector<LogEntry> logEntries);
    void plotPoints(std::vector<Point> points);
    void putBresenhamLine(int x0, int y0, int x1, int y1);
    void putVerticalLine(int x, int y0, int y1);
    void putDot(int x, int y);
};

#endif // PLOTMATRIX_H

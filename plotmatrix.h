#ifndef PLOTMATRIX_H
#define PLOTMATRIX_H

#include "main.h"
#include "point.h"
#include <vector>

class PlotMatrix
{
public:
    PlotMatrix();
    void updatePlotMatrixWithLogEntries(std::vector<LogEntry> &parsedLogEntries,
                                        std::vector<std::vector<int>> &matrix,
                                        std::size_t matrixWidth,
                                        std::size_t matrixHeight,
                                        std::size_t matrixPlottingMargin);

private:
    std::vector<Point> tranformLogEntriesToPoints(std::vector<LogEntry> &logEntries,
                                                  std::size_t matrixWidth,
                                                  std::size_t matrixHeight,
                                                  std::size_t matrixPlottingMargin);
    void plotPoints(std::vector<Point> points, std::vector<std::vector<int>> &matrix);
    void putBresenhamLine(int x0, int y0, int x1, int y1, std::vector<std::vector<int>> &matrix);
    void putVerticalLine(int x, int y0, int y1, std::vector<std::vector<int>> &matrix);
    void putDot(int x, int y, std::vector<std::vector<int>> &matrix);
};

#endif // PLOTMATRIX_H

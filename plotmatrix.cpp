#include "plotmatrix.h"
#include "point.h"
#include <limits>
#include <stdlib.h>
#include <string.h>

PlotMatrix::PlotMatrix()
{
}

void PlotMatrix::updatePlotMatrixWithLogEntries(std::vector<LogEntry> &parsedLogEntries,
                                                std::vector<std::vector<int>> &matrix,
                                                std::size_t matrixWidth,
                                                std::size_t matrixHeight,
                                                std::size_t matrixPlottingMargin)
{
    /* 1. Transform input data to points on the matrix */
    std::vector<Point> p = tranformLogEntriesToPoints(parsedLogEntries,
                                                      matrixWidth,
                                                      matrixHeight,
                                                      matrixPlottingMargin);
    /* 2. Plot the points */
    plotPoints(p, matrix);
}

std::vector<Point> PlotMatrix::tranformLogEntriesToPoints(std::vector<LogEntry> &logEntries,
                                                          std::size_t matrixWidth,
                                                          std::size_t matrixHeight,
                                                          std::size_t matrixPlottingMargin)
{
    /* 1. Find min and max timestamp and value in logEntries */

    double minTimestamp = std::numeric_limits<double>::max();
    double minValue = std::numeric_limits<double>::max();
    double maxTimestamp = std::numeric_limits<double>::min();
    double maxValue = std::numeric_limits<double>::min();
    for (auto i : logEntries) {
        if (i.timestamp > maxTimestamp) {
            maxTimestamp = i.timestamp;
        }
        if (i.timestamp < minTimestamp) {
            minTimestamp = i.timestamp;
        }
        if (i.value > maxValue) {
            maxValue = i.value;
        }
        if (i.value < minValue) {
            minValue = i.value;
        }
    }

    /* 2. Compute range of one matrix cell for timestapms and values from logEntries. 
     * Range for timestamps is based on PLOT_MATRIX_WIDTH. 
     * Range for values is based on PLOT_MATRIX_HIGHT. */
    double timestampCellRange = (maxTimestamp - minTimestamp) / matrixWidth;
    double valueCellRange = (maxValue - minValue) / matrixHeight;

    /* 3. Go through the logEntries and for each entry compute point.
     * Some points will be the same, so filter them out with set*/
    std::unordered_set<Point, Point::HashFunction> uniquePoints;
    std::vector<Point> vp;

    double timestampZeroAjustment = int(minTimestamp / timestampCellRange);
    double valueZeroAjustment = int(minValue / valueCellRange);
    for (LogEntry entry : logEntries) {
        int x = matrixPlottingMargin + int(entry.timestamp / timestampCellRange)
                - timestampZeroAjustment;
        int y = matrixPlottingMargin + int(entry.value / valueCellRange) - valueZeroAjustment;
        Point p(x, y);

        /* Only insert in result vector if this is the new Point */
        auto pr = uniquePoints.insert(p);
        if (pr.second) {
            vp.push_back(p);
        }
    }

    return vp;
}

void PlotMatrix::putDot(int x, int y, std::vector<std::vector<int>> &matrix)
{
    /* Shape of the dot
     *    ###
     *   #####
     *   #####
     *   #####
     *    ###
     */

    matrix[x - 1][y + 2] = 1;
    matrix[x][y + 2] = 1;
    matrix[x + 1][y + 2] = 1;
    matrix[x - 2][y + 1] = 1;
    matrix[x - 1][y + 1] = 1;
    matrix[x][y + 1] = 1;
    matrix[x + 1][y + 1] = 1;
    matrix[x + 2][y + 1] = 1;
    matrix[x - 2][y] = 1;
    matrix[x - 1][y] = 1;
    matrix[x][y] = 1;
    matrix[x + 1][y] = 1;
    matrix[x + 2][y] = 1;
    matrix[x - 2][y - 1] = 1;
    matrix[x - 1][y - 1] = 1;
    matrix[x][y - 1] = 1;
    matrix[x + 1][y - 1] = 1;
    matrix[x + 2][y - 1] = 1;
    matrix[x - 1][y - 2] = 1;
    matrix[x][y - 2] = 1;
    matrix[x + 1][y - 2] = 1;
}

void PlotMatrix::putBresenhamLine(
    int x0, int y0, int x1, int y1, std::vector<std::vector<int>> &matrix)
{
    /* https://gist.github.com/bert/1085538#file-plot_line-c
     * Works when x1 > x0 which is alway the case in these timeseries
     * When x1 == x0 I call verticalLine for performance
     * This function works ok with 1000 lines.
     * The plot width is less than 1000 so it's ok */

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;) { /* loop */
        matrix[x0][y0] = 1;
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

void PlotMatrix::putVerticalLine(int x, int y0, int y1, std::vector<std::vector<int>> &matrix)
{
    if (y0 > y1) {
        std::swap(y0, y1);
    }
    for (int i = y0; i <= y1; i++) {
        matrix[x][i] = 1;
    }
}

void PlotMatrix::plotPoints(std::vector<Point> points, std::vector<std::vector<int>> &matrix)
{
    /* Print first point */
    putDot(points[0].x, points[0].y, matrix);
    for (size_t i = 1; i < points.size(); i++) {
        putDot(points[i].x, points[i].y, matrix);

        /* Find if line should be drawn or points are connecting */
        /* Dumb way to check is just to check if squares in the center
         * touch each other. It depends on point shape.
         * 
         * ###
         * #0#
         * ###
         *    ###
         *    #1#
         *    ###
         *    
         *  abs(x1 - x0) <= 3 and abs(y1 - y0)
         * TODO: better collision check.
         */
        if (!((abs(points[i].x - points[i - 1].x) <= 3)
              && (abs(points[i].y - points[i - 1].y) <= 3))) {
            /* Find if vertical line should be drawn */
            if (points[i].x == points[i - 1].x) {
                putVerticalLine(points[i].x, points[i - 1].y, points[i].y, matrix);
            } else {
                /* Bresenham line should be drawn */
                putBresenhamLine(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y, matrix);
            }
        }
    }
}

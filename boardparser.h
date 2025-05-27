#ifndef BOARDPARSER_H
#define BOARDPARSER_H

#include <QString>
#include <vector>
using namespace std;

struct Cell {
    int id;
    int row;
    int col;
    char value;
    QString type;
};

struct ParsedMap {
    vector<Cell> allCells;
    vector<Cell> fiveCellRows;
    vector<Cell> fourCellRows;
};

class BoardParser {
public:
    ParsedMap parse(const QString& filename);
};

#endif // BOARDPARSER_H

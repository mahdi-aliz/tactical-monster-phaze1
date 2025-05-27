#include "boardparser.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ParsedMap BoardParser::parse(const QString& filename) {
    ParsedMap parsed;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open map file:" << filename;
        return parsed;
    }

    QTextStream in(&file);
    QVector<QString> lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
    file.close();


    int cellId = 0;
    int logicalRow = 0;

    for (int i = 0; i < lines.size() - 1; ++i) {
        const QString& line1 = lines[i];
        const QString& line2 = lines[i + 1];

        int maxLen = std::min(line1.length(), line2.length());

        int logicalCol = 0;
        for (int j = 0; j < maxLen - 3; ++j) {
            if (line1[j] == '/' && line1[j + 2] == ' ' && line1[j + 3] == '\\' &&
                line2[j] == '\\' && line2[j + 1] == '_' && line2[j + 2] == '_' && line2[j + 3] == '/') {

                QChar value = line1[j + 1];

                Cell cell;
                cell.id = cellId++;
                cell.row = logicalRow;
                cell.col = logicalCol;
                cell.value = value.toLatin1();
                cell.type = "";

                parsed.allCells.push_back(cell);
                if (logicalRow % 2 == 0)
                    parsed.fiveCellRows.push_back(cell);
                else
                    parsed.fourCellRows.push_back(cell);

                logicalCol++;
                j += 5;

                if (cellId >= 41) break;
            }
        }

        if (logicalCol > 0) {
            logicalRow++;
        }

        if (cellId >= 41) break;
    }

    return parsed;
}

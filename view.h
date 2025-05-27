
#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>

class View : public QWidget {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);

private slots:
    void onTurnChanged(int currentPlayer);

private:
    void setupUI();
    void loadBoard();
    void promptPlayerNames();

    QWidget *boardContainer;
    QVBoxLayout *boardLayout;

    QString player1Name;
    QString player2Name;
    QLabel* turnLabel;
};

#endif // VIEW_H


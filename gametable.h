#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <QHash>
#include <QList>
#include <QAbstractListModel>

#include <memory>
#include <utility>
#include <vector>
#include <set>

#include <gameitem.h>

template<typename T>
struct sPoint_t{
    T row;
    T col;
    T index;
};

using Point = sPoint_t<int>;

class GameTable : public QAbstractListModel
{
    Q_OBJECT

    static const int ROWS = 18;             // total rows
    static const int VROWS = 9;             // visible rows
    static const int COLS = 1;              // columns (TODO: ~ width)
    static const int TOTAL = ROWS * COLS;   // total items in the grid

    Q_PROPERTY(int first READ getFirst())
    Q_PROPERTY(int second READ getSecond())
    Q_PROPERTY(int points READ getGameScore() NOTIFY SigPointChanged)

public:
    GameTable(QObject *parent = 0);
    ~GameTable();

    // QAbstractListModel
    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * Проверяет всю map на наличие '3 в ряд'
     * помечает любую найденную тройку на удаление
     * return >0 - если было хотябы одно удаление
     *         0 - если удалений небыло вообще
     */
    Q_INVOKABLE int checkAllMap();

    /**
     * Когда пользователь нажал на левую кнопку мыши
     * нужно перевести координату в индекс вектора
     * и если у нас есть 2 рядом стоящих кандидата
     * сделать swap() + checkMap()
     *
     * Если кандидат первый
     * - занести в m_First
     * Если кандидат второй и стоит рядом
     * - занести в m_Second и делать проверки
     * Если кандидат плохой
     * - занести в m_First
     * Если кандидат тотже, что и m_First
     * - очистить m_First
     *
     * @brief handleMouse
     * @param index
     * @return
     */
    Q_INVOKABLE void handleMouse(int index);

    int getGameScore() const;

private:
    void clearSelectedPoints();
    void swapPoints();
    int getFirst();
    int getSecond();

    int checkRow(int rowNumber);
    int checkColumn(int columnNumber);
    void handleDeletedNodes();
    void deleteMarked(int index);
    void appendItem(int index);
    void updateScore(int count);

signals:
    void SigPointChanged();

public slots:

private:
    QList<std::shared_ptr<GameItem>> m_Items;
    std::set<int> m_Marked;

    Point m_First;
    Point m_Second;

    int m_GameScore;    // Количество очков
};

#endif // GAMETABLE_H

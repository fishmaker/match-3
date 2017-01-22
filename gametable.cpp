#include "gametable.h"

#include <QDebug>

#include <cstdlib>

GameTable::GameTable(QObject *parent)
    :QAbstractListModel(parent)
{
    //qInfo() << Q_FUNC_INFO;

    qsrand(QTime::currentTime().msec());

    m_GameScore = 0;

    for (int i = 0; i < TOTAL; ++i) {
        m_Items << std::shared_ptr<GameItem>(new GameItem(qrand() % GameItem::HIGH_INDEX));
    }

    clearSelectedPoints();

}

GameTable::~GameTable()
{
    //qInfo() << Q_FUNC_INFO;
}

int GameTable::rowCount(const QModelIndex &) const
{
    //qInfo() << Q_FUNC_INFO;

    return GameTable::TOTAL;
}

QHash<int, QByteArray> GameTable::roleNames() const
{
    //qInfo() << Q_FUNC_INFO;

    QHash<int, QByteArray> roles;

    roles[GameItem::RR_TILEROLE] = "r_tile";
    roles[GameItem::RR_IMAGEPATH] = "r_image";

    return roles;
}

QVariant GameTable::data(const QModelIndex &index, int role) const
{
    //qInfo() << Q_FUNC_INFO;

    if (index.row() < 0 || index.row() >= m_Items.size()) {
        qWarning() << "GameTable::data() - Index out of scope";
        return QVariant();
    }

    const GameItem * tile = m_Items[index.row()].get();

    if (role == GameItem::RR_TILEROLE) {
        return tile->getItemType();
    }
    if (role == GameItem::RR_IMAGEPATH) {
        return tile->getImagePath();
    }

    qWarning() << "GameTable::data() - role not found";
    return QVariant();
}

int GameTable::checkAllMap()
{
    int res;

    for (int i = 0; i < GameTable::VROWS; ++i)
        checkRow(i);
    for (int i = 0; i < GameTable::COLS; ++i)
        checkColumn(i);

    for (auto it = m_Marked.cbegin(); it != m_Marked.cend(); ++it) {
        deleteMarked(*it);
        appendItem(*it);
    }

    res = m_Marked.size();
    updateScore(res);
    m_Marked.clear();

    return res;
}

void GameTable::handleMouse(int index)
{
    qInfo() << "index is = " << index;

    if (index < 0 || index >= GameTable::TOTAL) {
        qWarning() << "Index out of bounds!";
        return;
    }

    if (index % 18 >= GameTable::VROWS) {
        qInfo() << "Disabled for this area";
        return;
    }

    int row, col;

    row = index % 18;
    col = index / 18;

    qInfo() << "row = " << row << "col = " << col;

    if (m_First.row == -1) {
        qInfo() << "first candidate";
        m_First.row = row;
        m_First.col = col;
        m_First.index = index;
        return;
    }

    if (m_First.row == row && m_First.col == col) {
        qInfo() << "Та же точка";
        clearSelectedPoints();
        return;
    }

    if (abs(m_First.row - row) + abs(m_First.col - col) > 1) {
        // Второй кандидат слишком далеко
        // Выбираем как первого
        m_First.row = row;
        m_First.col = col;
        m_First.index = index;
        return;
    }

    qInfo() << "We found second candidate";
    m_Second.row = row;
    m_Second.col = col;
    m_Second.index = index;

    swapPoints();
    if (!checkAllMap()) {
        // backswap
        swapPoints();
    }

    clearSelectedPoints();
    //qInfo() << "Point cleared!";
}

void GameTable::clearSelectedPoints()
{
    m_First.row = m_First.col = m_First.index = -1;
    m_Second.row = m_Second.col = m_Second.index = -1;
}

void GameTable::swapPoints()
{
    if (m_First.row < m_Second.row) {
        //qInfo() << "m_Second выше, чем m_First";

        beginMoveRows(QModelIndex(), m_First.index, m_First.index, QModelIndex(), m_Second.index + 1);
        endMoveRows();

        goto g_swap;
    }

    if (m_First.row > m_Second.row) {
        //qInfo() << "m_First выше, чем m_Second";

        beginMoveRows(QModelIndex(), m_Second.index, m_Second.index, QModelIndex(), m_First.index + 1);
        endMoveRows();

        goto g_swap;
    }

    // Дальше, если m_First.row == m_Second.row (на одном уровне)

    if (m_First.col < m_Second.col) {
        //qInfo() << "m_First левее m_Second";

        beginMoveRows(QModelIndex(), m_First.index, m_First.index, QModelIndex(), m_Second.index + 1);
        endMoveRows();

        beginMoveRows(QModelIndex(), m_Second.index - 1, m_Second.index - 1, QModelIndex(), m_First.index);
        endMoveRows();

        goto g_swap;
    }

    if (m_First.col > m_Second.col) {
        //qInfo() << "m_First правее m_Second";

        beginMoveRows(QModelIndex(), m_Second.index, m_Second.index, QModelIndex(), m_First.index + 1);
        endMoveRows();

        beginMoveRows(QModelIndex(), m_First.index - 1, m_First.index - 1, QModelIndex(), m_Second.index);
        endMoveRows();

        goto g_swap;
    }

    qWarning() << "[W] - unknown situation";

g_swap:
    qSwap(m_Items[m_First.index], m_Items[m_Second.index]);
}

int GameTable::getFirst()
{
    return m_First.index;
}

int GameTable::getSecond()
{
    return m_Second.index;
}

int GameTable::checkRow(int rowNumber)
{
    int count = 1;
    int offset = rowNumber * GameTable::ROWS;
    int begin = offset;
    int current = m_Items[offset]->getItemType();

    for (int i = 1; i < GameTable::VROWS; ++i) {
        if (current == m_Items[offset + i]->getItemType()) {
            ++count;
        } else {
            count = 1;
            current = m_Items[offset + i]->getItemType();
            begin = offset + i;
        }

        if (count == 3) {
            for (int j = 0; j < 3; ++j)
                m_Marked.insert(begin + j);
        }

        if (count > 3) {
            m_Marked.insert(begin + i);
        }
    }
}

int GameTable::checkColumn(int columnNumber)
{

}

void GameTable::handleDeletedNodes()
{

}

void GameTable::deleteMarked(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_Items.removeAt(index);
    endRemoveRows();
}

void GameTable::appendItem(int index)
{
    int position = index / GameTable::ROWS + GameTable::VROWS;
    beginInsertRows(QModelIndex(), position, position);
    m_Items.insert(position, std::shared_ptr<GameItem>(new GameItem(qrand() % GameItem::HIGH_INDEX)));
    endInsertRows();

    emit dataChanged(createIndex(GameTable::ROWS, 0), createIndex(GameTable::TOTAL - GameTable::ROWS + 1, 0));
}

void GameTable::updateScore(int count)
{
    m_GameScore += count * 1000;
    emit SigPointChanged();
}

int GameTable::getGameScore() const
{
    return m_GameScore;
}

#include "gameitem.h"

#include <QDebug>

const QString GameItem::IMG_CLOCK = "images/clock.png";
const QString GameItem::IMG_COAL = "images/coal.png";
const QString GameItem::IMG_DIAMOND = "images/diamond.png";
const QString GameItem::IMG_PICTURE = "images/picture.png";
const QString GameItem::IMG_HOME = "images/home.png";

GameItem::GameItem()
{
    qWarning() << "GameItem::GameItem() - Default ctor called";
}

GameItem::GameItem(int value)
{
    m_ItemType = value;
    updateImagePath();
}

GameItem::GameItem(const GameItem &cpy)
{
    m_ItemType = cpy.getItemType();
    updateImagePath();
}

GameItem& GameItem::operator=(const GameItem &cpy)
{
    m_ItemType = cpy.getItemType();
    updateImagePath();

    return *this;
}

GameItem::~GameItem()
{

}

int GameItem::getItemType() const
{
    //qInfo() << Q_FUNC_INFO;

    return m_ItemType;
}

void GameItem::setItemType(const int &ItemType)
{
    //qInfo() << Q_FUNC_INFO;

    m_ItemType = ItemType;
}

const QString GameItem::getImagePath() const
{
    //qInfo() << Q_FUNC_INFO;

    return *m_ImagePath;
}

void GameItem::updateImagePath()
{
    switch (m_ItemType) {
    case 0: m_ImagePath = &GameItem::IMG_CLOCK; break;
    case 1: m_ImagePath = &GameItem::IMG_COAL; break;
    case 2: m_ImagePath = &GameItem::IMG_DIAMOND; break;
    case 3: m_ImagePath = &GameItem::IMG_PICTURE; break;
    case 4: m_ImagePath = &GameItem::IMG_HOME; break;
    default:
        qWarning() << "GameItem::GameItem() - wrong value!";
    }
}

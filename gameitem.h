#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QtCore>

class GameItem
{
public:
    enum Roles {
        RR_TILEROLE = Qt::UserRole + 1,
        RR_IMAGEPATH,
    };

    static const int HIGH_INDEX = 4;

    static const QString IMG_CLOCK;
    static const QString IMG_COAL;
    static const QString IMG_DIAMOND;
    static const QString IMG_PICTURE;
    static const QString IMG_HOME;

public:
    explicit GameItem();
    explicit GameItem(int value);
    ~GameItem();

    int getItemType() const;
    void setItemType(const int &ItemType);

    const QString getImagePath() const;
    void setImagePath(const QString * ImagePath);

private:
    int m_ItemType;
    const QString * m_ImagePath;
};

#endif // GAMEITEM_H

#ifndef QBATTICON_T_H
#define QBATTICON_T_H

#include <QIcon>
#include <QString>

class QBattIcon_T : public QIcon {
  public:
    /**
     * @brief min lower limit, percent
     */
    int min;

    /**
     * @brief QBattIcon_T main constructor
     * @param fileName icon fileName
     * @param min lower limit for this icon
     */
    QBattIcon_T (QString fileName, int min);

    bool operator< (const QBattIcon_T &qbattIcon);
    friend bool operator > (const QBattIcon_T &a, const QBattIcon_T &b);
};

#endif // QBATTICON_T_H

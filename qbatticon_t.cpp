#include "qbatticon_t.h"

QBattIcon_T::QBattIcon_T (QString fileName, int min) : QIcon (fileName) {
  this->min = min;
}

bool QBattIcon_T::operator < (const QBattIcon_T &qbattIcon) {
  return this->min < qbattIcon.min;
}

bool operator > (const QBattIcon_T &a, const QBattIcon_T &b) {
  return a.min > b.min;
}

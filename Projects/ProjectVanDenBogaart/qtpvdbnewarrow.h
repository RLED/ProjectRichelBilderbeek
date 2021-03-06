#ifndef QTPVDBNEWARROW_H
#define QTPVDBNEWARROW_H

#include <string>
#include <vector>

#include <QBrush>

#include "qtarrowitem.h"

#ifdef PVDB_USE_FORWARD_DECLARATIONS_248738
#include "pvdbfwd.h"
#else
#include "qtpvdbnodeitem.h"
#endif

///QtPvdbNewArrow is the arrow shown before a new one is added
struct QtPvdbNewArrow: public QtArrowItem
{
  QtPvdbNewArrow(
    const QtPvdbNodeItem * const from,
    const QPointF& current_to);

  ///Obtain the source node
  const QtPvdbNodeItem * GetFrom() const { return m_from; }
  //      QtPvdbNodeItem * GetFrom()       { return m_from; }

  private:
  ///Must be suppplied
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option = 0, QWidget* widget = 0);

  ///The source node
  ///Cannot be const as the user might want to edit it
  const QtPvdbNodeItem * const m_from;
};

#endif // QTPVDBNEWARROW_H

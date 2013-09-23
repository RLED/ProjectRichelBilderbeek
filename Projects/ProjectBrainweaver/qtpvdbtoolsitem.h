#ifndef QTPVDBTOOLSITEM_H
#define QTPVDBTOOLSITEM_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QBrush>
#include <QGraphicsPixmapItem>

#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

#include "pvdbnode.h"

#include "pvdbfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace pvdb {

///Tools item is a class that is displayed above a selected node or edge
///and displays clickable icons to perform actions, for example
///adding a relation or modifying arrow edges
///QtPvdbToolsItem cannot have its visibility set directly:
///- if it has a buddy, it is visible
///- if it has no buddy, it is invisible
struct QtPvdbToolsItem : public QGraphicsPixmapItem
{
  explicit QtPvdbToolsItem();
  QtPvdbToolsItem(const QtPvdbToolsItem& other) = delete;
  QtPvdbToolsItem& operator=(const QtPvdbToolsItem& other) = delete;
  virtual ~QtPvdbToolsItem() {}

  ///Get the item the tools item floats above
  ///Return type cannot be const, as the user might want to modify it
  const QtPvdbNodeItem * GetBuddyItem() const;
        QtPvdbNodeItem * GetBuddyItem();

  ///Set the position from the widget it floats above
  ///item cannot be const, as the user might want to modify it
  void SetBuddyItem(const QtPvdbNodeItem * const item);

  ///Signalled when a tool is clicked
  boost::signals2::signal<void ()> m_signal_clicked;

  protected:
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *);

  private:
  ///Make private to ensure use of other setPos member function
  void setPos(qreal ax, qreal ay) { QGraphicsPixmapItem::setPos(ax,ay); }

  ///The item the tools item floats above
  ///m_item cannot be const, as the user might want to modify it
  const QtPvdbNodeItem * m_item;

  void setVisible(bool visible) { QGraphicsPixmapItem::setVisible(visible); }
};

} //~namespace pvdb
} //~namespace ribi

#endif // QTPVDBTOOLSITEM_H

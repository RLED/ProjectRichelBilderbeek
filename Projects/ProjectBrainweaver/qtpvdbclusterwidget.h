 #ifndef QTPVDBCLUSTERWIDGET_H
#define QTPVDBCLUSTERWIDGET_H


#include <map>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>

#include <QTreeWidget>

#include "conceptmapcompetency.h"
#include "pvdbcluster.h"
#include "pvdbfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace pvdb {

class QtPvdbClusterWidget : public QTreeWidget
{
  Q_OBJECT

public:
  explicit QtPvdbClusterWidget(
    boost::shared_ptr<pvdb::Cluster> cluster,
    QWidget* parent = 0);

  ///Add a top-level item
  void Add(const std::string& text);

  ///Do something random
  void DoRandomStuff();

  ///Get the cluster (after reading from the dialog)
  const boost::shared_ptr<pvdb::Cluster> GetCluster();

protected:
  void dropEvent(QDropEvent *event);

  void keyPressEvent(QKeyEvent *event);

private slots:
  void RemoveEmptyItem(QTreeWidgetItem*,int);

  ///Block: QtPvdbTreeWidgetItem must be added instead
  void addTopLevelItem(QTreeWidgetItem *item) { QTreeWidget::addTopLevelItem(item); }

private:
  ///The cluster
  const boost::shared_ptr<pvdb::Cluster> m_cluster;

  ///Build the widget from the cluster
  void BuildCluster();

  ///Find out the depth of an item
  int GetDepth(const QTreeWidgetItem * const item) const;

  ///Test this class
  static void Test() noexcept;

  ///Write the widget its data to the cluster
  void WriteToCluster();

  void SetCorrectFlags() noexcept;
};

} //~namespace pvdb
} //~namespace ribi

#endif // QTPVDBCLUSTERWIDGET_H

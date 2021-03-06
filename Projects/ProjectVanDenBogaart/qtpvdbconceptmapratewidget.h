#ifndef QTPVDBCONCEPTMAPRATEWIDGET_H
#define QTPVDBCONCEPTMAPRATEWIDGET_H

#include "qtpvdbconceptmapwidget.h"

#ifdef PVDB_USE_FORWARD_DECLARATIONS_248738
#include "pvdbfwd.h"
#else
#include "qtpvdbrateconcept.h"
#endif

///QtPvdbConceptMapWidget for creation and editing of a ConceptMap
struct QtPvdbConceptMapRateWidget : public QtPvdbConceptMapWidget
{
  typedef QtPvdbConceptMapRateWidget This_t;

  QtPvdbConceptMapRateWidget(QWidget* parent = 0);
  ~QtPvdbConceptMapRateWidget() {}
  QtPvdbConceptMapRateWidget(const QtPvdbConceptMapRateWidget& other) = delete;
  QtPvdbConceptMapRateWidget& operator=(const QtPvdbConceptMapRateWidget& other) = delete;

  ///Creates a new derived class
  ///A simpler alternative to Clone (see above)
  std::unique_ptr<QtPvdbConceptMapWidget> CreateNewDerived() const;

  ///Do something random
  void DoRandomStuff();

  ///Initialize the widget with a concept map
  ///Note that ReadFromConceptMap changes the copy of the map entered, by changing some GUI
  ///elements (coordinats of the nodes, for example). Therefore, the following test will fail:
  ///  conceptmapwidget->ReadFromConceptMap(m);
  ///  const pvdb::ConceptMapPtr n = w->WriteToConceptMap();
  ///  assert(m == n);
  ///instead, use
  ///  assert(HasSameContent(m,n));
  //void ReadFromConceptMap(const boost::shared_ptr<const pvdb::ConceptMap>& map);

private:

  ///Adds an Edge and connects (some of) its signals to slots present in the derived classes
  void AddEdge(
    const boost::shared_ptr<pvdb::Edge>& edge,
    const std::vector<QtPvdbNodeItem*>& qtnodes);

  ///Adds a node and connects (some of) its signals to slots present in the derived classes
  ///For rating and editing, the node must be non-cost
  ///For display, the node should best be const
  QtPvdbNodeItem * AddNode(const boost::shared_ptr<pvdb::Node>& node);

  ///Remove all Qt and non-Qt items and add new ones
  void CleanMe();

  ///Create a subsection of the concept map, with item at the center
  ///Item is non-const, as it will be at index 0 in the concept map, that might be edited
  const boost::shared_ptr<pvdb::ConceptMap> CreateSubConceptMap(QtPvdbNodeItem * const item) const;

  ///Called whenever a concept is clicked or moved
  ///If item is nullptr, the last item might be deleted
  ///Use QGraphicsItem* due to QtKeyboardFriendlyGraphicsView working on QGraphicsItems
  ///Is implemented by OnItemUpdateRequestImpl
  void OnItemRequestUpdateImpl(const QGraphicsItem* const item);

  ///Called when a QtConceptItem requests to have its concept rated
  ///Note that this might be a ConceptItem on an Edge, but those signals are not connected
  ///item must be a Node, becaus only Nodes can be rated and because the Node is
  ///used for contruction of a sub concept map
  void OnNodeRequestsRateConcept(QtPvdbNodeItem * const item);

  ///Called when a QtConceptItem requests to have its examples rated
  ///Note that this might be a ConceptItem on an Edge, but those signals are not connected
  void OnNodeRequestsRateExamples(QtPvdbNodeItem * const item);

  ///Test this class
  static void Test();
};

bool operator==(const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& lhs, const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;
bool operator==(const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& lhs, boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;
bool operator==(const boost::shared_ptr<QtPvdbConceptMapRateWidget>& lhs, const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;
bool operator==(const boost::shared_ptr<QtPvdbConceptMapRateWidget>& lhs, boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;
bool operator!=(const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& lhs, const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;
bool operator!=(const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& lhs, boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;
bool operator!=(const boost::shared_ptr<QtPvdbConceptMapRateWidget>& lhs, const boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;
bool operator!=(const boost::shared_ptr<QtPvdbConceptMapRateWidget>& lhs, boost::shared_ptr<const QtPvdbConceptMapRateWidget>& rhs) = delete;

#endif // QTPVDBCONCEPTMAPRATEWIDGET_H

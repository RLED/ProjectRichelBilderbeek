#ifndef QTTESTPLANEMAINDIALOG_H
#define QTTESTPLANEMAINDIALOG_H

#include <iosfwd>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/geometry.hpp>

#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
  class QtTestPlaneMainDialog;
}

namespace ribi {

class QtTestPlaneMainDialog : public QtHideAndShowDialog
{
  Q_OBJECT
    
public:
  explicit QtTestPlaneMainDialog(QWidget *parent = 0);
  QtTestPlaneMainDialog(const QtTestPlaneMainDialog&) = delete;
  QtTestPlaneMainDialog& operator=(const QtTestPlaneMainDialog&) = delete;
  ~QtTestPlaneMainDialog() noexcept;
    
private slots:
  void OnAnyChange();

private:
  Ui::QtTestPlaneMainDialog *ui;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

//std::ostream& operator<<(std::ostream& os, const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p) noexcept;

} //~namespace ribi


#endif // QTTESTPLANEMAINDIALOG_H
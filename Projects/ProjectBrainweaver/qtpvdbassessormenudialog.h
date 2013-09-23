#ifndef QTPVDBASSESSORMENUDIALOG_H
#define QTPVDBASSESSORMENUDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"
#include "pvdbfwd.h"
#include "pvdbfile.h"
#pragma GCC diagnostic pop


namespace Ui { class QtPvdbAssessorMenuDialog; }

namespace ribi {
namespace pvdb {

class QtPvdbAssessorMenuDialog : public ribi::QtHideAndShowDialog
{
    Q_OBJECT
    
public:
  explicit QtPvdbAssessorMenuDialog(QWidget* parent = 0);
  QtPvdbAssessorMenuDialog(const QtPvdbAssessorMenuDialog&) = delete;
  QtPvdbAssessorMenuDialog& operator=(const QtPvdbAssessorMenuDialog&) = delete;
  ~QtPvdbAssessorMenuDialog() noexcept;

private slots:
  void keyPressEvent(QKeyEvent *);
  void on_button_create_assessment_clicked();

  void on_button_about_clicked();

  void on_button_quit_clicked();

  void on_button_assess_result_clicked();

private:
  Ui::QtPvdbAssessorMenuDialog *ui;
};

} //~namespace pvdb

} //~namespace ribi

#endif // QTPVDBASSESSORMENUDIALOG_H

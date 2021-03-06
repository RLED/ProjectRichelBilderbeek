#ifndef QTPVDBTESTCONCEPTMAPRATEWIDGETDIALOG_H
#define QTPVDBTESTCONCEPTMAPRATEWIDGETDIALOG_H

#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"

#ifdef PVDB_USE_FORWARD_DECLARATIONS_248738
#include "pvdbfwd.h"
#else
#include "pvdbnode.h"
#include "qtpvdbconceptitem.h"
#include "qtpvdbdisplayconceptitem.h"
#include "qtpvdbnodeitem.h"
#include "qtpvdbrateconceptitem.h"
#endif

namespace Ui { class QtPvdbTestConceptMapRateWidgetDialog; }

///Tests all QtPvdbConceptMapWidgets
///especially the connection between the pointer and its displayal items:
///If something via the pointer is changed, this must be displayed directly
class QtPvdbTestConceptMapRateWidgetDialog : public QtHideAndShowDialog
{
  Q_OBJECT
  
public:
  explicit QtPvdbTestConceptMapRateWidgetDialog(QWidget *parent = 0);
  ~QtPvdbTestConceptMapRateWidgetDialog();

protected:

  void keyPressEvent(QKeyEvent *);

private slots:

  void on_button_test_modify_clicked();

private:

  Ui::QtPvdbTestConceptMapRateWidgetDialog *ui;

  static void Test();
};

#endif // QTPVDBTESTCONCEPTMAPRATEWIDGETDIALOG_H

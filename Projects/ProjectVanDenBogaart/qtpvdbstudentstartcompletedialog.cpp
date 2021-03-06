#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qtpvdbstudentstartcompletedialog.h"

#include <fstream>
#include <QKeyEvent>
#include <QFileDialog>
#include "pvdbfile.h"
#include "pvdbclusterfactory.h"
#include "pvdbconceptmapfactory.h"
//#include "pvdbmenudialog.h"
#include "qtpvdbclusterdialog.h"
#include "qtpvdbconceptmapdialog.h"
#include "qtpvdbcreateassessmentmenudialog.h"
#include "trace.h"
#include "ui_qtpvdbstudentstartcompletedialog.h"

QtPvdbStudentStartCompleteDialog::QtPvdbStudentStartCompleteDialog(
  const boost::shared_ptr<pvdb::File> file,
  QWidget* parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtPvdbStudentStartCompleteDialog),
    m_back_to_menu(false),
    m_file(file)
{
  ui->setupUi(this);
  assert(m_file);
  assert(IsEqual(*file,*m_file));
}

QtPvdbStudentStartCompleteDialog::~QtPvdbStudentStartCompleteDialog()
{
  delete ui;
}

void QtPvdbStudentStartCompleteDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }
  if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_S) { Save(); return; }
  QDialog::keyPressEvent(e);
}

void QtPvdbStudentStartCompleteDialog::on_button_start_associate_clicked()
{
  assert(m_file);
  QtPvdbClusterDialog d(m_file);
  this->ShowChild(&d);
  if (d.GoBackToMenu())
  {
    m_back_to_menu = true;
    close();
  }
}

void QtPvdbStudentStartCompleteDialog::on_button_start_construct_clicked()
{
  QtPvdbConceptMapDialog d(m_file);
  this->ShowChild(&d);
  if (d.GoBackToMenu())
  {
    m_back_to_menu = true;
    close();
  }
}

void QtPvdbStudentStartCompleteDialog::Save()
{
  const std::string filter_str = std::string("*.") + pvdb::File::GetFilenameExtension();
  const std::string filename_raw = QFileDialog::getSaveFileName(0,"Sla de concept map op",
    QString(),
    filter_str.c_str()).toStdString();
  if (!filename_raw.empty())
  {
    const std::string filename
      =  (filename_raw.size() < pvdb::File::GetFilenameExtension().size()
        || filename_raw.substr( filename_raw.size() - 3, 3 ) != pvdb::File::GetFilenameExtension()
       ? filename_raw + std::string(".") + pvdb::File::GetFilenameExtension()
       : filename_raw);
    assert(filename.size() > 3
      && filename.substr( filename.size() - 3, 3 ) == pvdb::File::GetFilenameExtension()
      && "File must have correct file extension name");
    m_file->Save(filename);
    { const std::string debug_str = "File saved as " + filename; TRACE(debug_str); }
  }
}

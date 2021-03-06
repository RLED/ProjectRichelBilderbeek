#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qtpvdbcreateassessmentcompletedialog.h"

#include <QKeyEvent>
#include <QFileDialog>
#include "pvdbconceptmap.h"
#include "pvdbfile.h"
#include "pvdbconceptmapfactory.h"

#include "ui_qtpvdbcreateassessmentcompletedialog.h"


#include "trace.h"

QtPvdbCreateAssessmentCompleteDialog::QtPvdbCreateAssessmentCompleteDialog(QWidget* parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtPvdbCreateAssessmentCompleteDialog),
    m_back_to_menu(false)
{
  ui->setupUi(this);
}

QtPvdbCreateAssessmentCompleteDialog::~QtPvdbCreateAssessmentCompleteDialog()
{
  delete ui;
}

const std::string QtPvdbCreateAssessmentCompleteDialog::GetQuestion() const
{
  return ui->edit->text().toStdString();
}

void QtPvdbCreateAssessmentCompleteDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) close();
}

void QtPvdbCreateAssessmentCompleteDialog::on_button_save_clicked()
{
  const std::string filter_str = std::string("*.") + pvdb::File::GetFilenameExtension();
  const std::string filename_raw = QFileDialog::getSaveFileName(0,"Sla het assessment invoer-bestand op",
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
    Save(filename);

    m_back_to_menu = true;
    close();
  }
}

void QtPvdbCreateAssessmentCompleteDialog::Save(const std::string& filename) const
{
  assert(filename.size() > 3
    && filename.substr( filename.size() - 3, 3 ) == pvdb::File::GetFilenameExtension()
    && "File must have correct file extension name");
  const std::string question = ui->edit->text().toStdString();
  const boost::shared_ptr<pvdb::File> file(new pvdb::File);
  {
    boost::shared_ptr<pvdb::ConceptMap> concept_map(
      pvdb::ConceptMapFactory::Create(question));
    assert(concept_map);
    file->SetConceptMap(concept_map);
  }
  assert(file->GetQuestion() == question);
  file->Save(filename);
}

void QtPvdbCreateAssessmentCompleteDialog::SetQuestion(const std::string& question)
{
  ui->edit->setText(question.c_str());
}

void QtPvdbCreateAssessmentCompleteDialog::on_edit_textChanged(const QString &arg1)
{
  assert(ui->edit->text() == arg1);
  ui->button_save->setEnabled(!arg1.isEmpty());

}

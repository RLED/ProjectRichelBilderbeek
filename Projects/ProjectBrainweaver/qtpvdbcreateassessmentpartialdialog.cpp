#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtpvdbcreateassessmentpartialdialog.h"

#include <QKeyEvent>
#include <QFileDialog>
#include "pvdbfile.h"

#include "ui_qtpvdbcreateassessmentpartialdialog.h"
#pragma GCC diagnostic pop

ribi::pvdb::QtPvdbCreateAssessmentPartialDialog::QtPvdbCreateAssessmentPartialDialog(
  const boost::shared_ptr<pvdb::File> file, QWidget* parent)
  : QDialog(parent),
    ui(new Ui::QtPvdbCreateAssessmentPartialDialog),
    m_file(file)
{
  ui->setupUi(this);
  assert(!"TODO");
  /*
  {
    const std::vector<std::string> v = m_file->GetQuestion();
    if (v.size() == 4)
    {
      ui->edit_1->setText(v[0].c_str());
      ui->edit_2->setText(v[1].c_str());
      ui->edit_3->setText(v[2].c_str());
      ui->edit_complete->setText(v[3].c_str());
    }
    else
    {
      ui->edit_1->setText("");
      ui->edit_2->setText("");
      ui->edit_3->setText("");
      ui->edit_complete->setText("");
    }
  }
  */
  QObject::connect(ui->edit_1,SIGNAL(textChanged(QString)),this,SLOT(AnyEditChanged()));
  QObject::connect(ui->edit_2,SIGNAL(textChanged(QString)),this,SLOT(AnyEditChanged()));
  QObject::connect(ui->edit_3,SIGNAL(textChanged(QString)),this,SLOT(AnyEditChanged()));
  QObject::connect(ui->edit_complete,SIGNAL(textChanged(QString)),this,SLOT(AnyEditChanged()));
}

void ribi::pvdb::QtPvdbCreateAssessmentPartialDialog::AnyEditChanged()
{
  /*
  m_file.SetQuestions(
    {
      ui->edit_1->text().toStdString(),
      ui->edit_2->text().toStdString(),
      ui->edit_3->text().toStdString(),
      ui->edit_complete->text().toStdString()
    }
  );
  */
}

ribi::pvdb::QtPvdbCreateAssessmentPartialDialog::~QtPvdbCreateAssessmentPartialDialog()
{
  delete ui;
}

void ribi::pvdb::QtPvdbCreateAssessmentPartialDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) close();
}

void ribi::pvdb::QtPvdbCreateAssessmentPartialDialog::on_button_save_clicked()
{
}

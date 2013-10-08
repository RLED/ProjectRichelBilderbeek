#ifndef QTHOMETRAINERMAINDIALOG_H
#define QTHOMETRAINERMAINDIALOG_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
  class QtHometrainerMainDialog;
}

namespace ribi {

struct TestQuestionMainDialog;
struct QtQuestionDialog;
struct Question;
struct HometrainerMainDialog;

class QtHometrainerMainDialog : public QtHideAndShowDialog
{
  Q_OBJECT
  
public:
  explicit QtHometrainerMainDialog(
    const boost::shared_ptr<const HometrainerMainDialog>& dialog,
    QWidget *parent = 0) noexcept;
  QtHometrainerMainDialog(const QtHometrainerMainDialog&) = delete;
  QtHometrainerMainDialog& operator=(const QtHometrainerMainDialog&) = delete;
  ~QtHometrainerMainDialog() noexcept;
  
  const boost::shared_ptr<const QtQuestionDialog> GetDialog() const { return m_qtdialog; }
  void SetQuestion(const boost::shared_ptr<const Question> s);

protected:
  void keyPressEvent(QKeyEvent* event);

private:
  Ui::QtHometrainerMainDialog *ui;
  std::size_t m_current_question_index;


  int m_n_correct;
  int m_n_incorrect;
  boost::shared_ptr<QtQuestionDialog> m_qtdialog;
  std::vector<boost::shared_ptr<const Question> > m_questions;

  static boost::shared_ptr<QtQuestionDialog> CreateQtQuestionDialog(
    const boost::shared_ptr<const Question> s);
  void DisplayScore();
  void OnSubmitted(const bool is_correct);


  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

private slots:
  void NewQuestion();

};

} //~namespace ribi

#endif // QTHOMETRAINERMAINDIALOG_H

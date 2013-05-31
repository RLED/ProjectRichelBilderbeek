// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef SIMPLECHATWIDGET_H_
#define SIMPLECHATWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WJavaScript>
#include <Wt/WSound>

namespace Wt {
  class WApplication;
  class WPushButton;
  class WText;
  class WLineEdit;
  class WTextArea;
}

class SimpleChatServer;
class ChatEvent;

/**
 * \defgroup chatexample Chat example
 */
/*@{*/

/*! \brief A self-contained chat widget.
 */
class SimpleChatWidget : public Wt::WContainerWidget
{
public:
  /*! \brief Create a chat widget that will connect to the given server.
   */
  SimpleChatWidget(SimpleChatServer& server, Wt::WContainerWidget *parent = 0);

  /*! \brief Delete a chat widget.
   */
  ~SimpleChatWidget();

  /*! \brief Show a simple login screen.
   */
  void letLogin();

  /*! \brief Start a chat for the given user.
   *
   * Returns false if the user could not login.
   */
  bool startChat(const Wt::WString& user);

  void logout();

  SimpleChatServer& server() { return server_; }

  int userCount() { return users_.size(); }

protected:
  virtual void createLayout(Wt::WWidget *messages, Wt::WWidget *userList,
			    Wt::WWidget *messageEdit,
			    Wt::WWidget *sendButton, Wt::WWidget *logoutButton);

  virtual void updateUsers();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

private:
  typedef std::map<Wt::WString, bool> UserMap;
  UserMap users_;

  SimpleChatServer&     server_;

  Wt::JSlot             clearInput_;

  Wt::WString           user_;

  Wt::WLineEdit        *userNameEdit_;
  Wt::WText            *statusMsg_;

  Wt::WContainerWidget *messages_;
  Wt::WContainerWidget *messageEditArea_;
  Wt::WTextArea        *messageEdit_;
  Wt::WPushButton      *sendButton_;
  Wt::WContainerWidget *userList_;

  Wt::WSound* messageReceived_;

  void login();
  void send();
  void updateUser();
  bool loggedIn() const;

  /* called from another session */
  void processChatEvent(const ChatEvent& event);
};

/*@}*/

#endif // SIMPLECHATWIDGET

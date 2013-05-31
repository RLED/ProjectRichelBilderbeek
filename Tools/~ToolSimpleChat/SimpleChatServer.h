// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef SIMPLECHATSERVER_H_
#define SIMPLECHATSERVER_H_

#include <boost/noncopyable.hpp>

#include <Wt/WSignal>
#include <Wt/WString>

namespace Wt {
  class WServer;
}

#include <set>
#include <map>
#include <boost/thread.hpp>

/**
 * @addtogroup chatexample
 */
/*@{*/

/*! \brief Encapsulate a chat event.
 */
class ChatEvent
{
public:
  /*! \brief Enumeration for the event type.
   */
  enum Type { Login, Logout, Rename, Message };

  /*! \brief Get the event type.
   */
  Type type() const { return type_; }

  /*! \brief Get the user who caused the event.
   */
  const Wt::WString& user() const { return user_; }

  /*! \brief Get the message of the event.
   */
  const Wt::WString& message() const { return message_; }

  /*! \brief Get the extra data for this event.
   */
  const Wt::WString& data() const { return data_; }

  /*! \brief Get the message formatted as HTML, rendered for the given user.
   */
  const Wt::WString formattedHTML(const Wt::WString& user) const;

private:
  Type type_;
  Wt::WString user_;
  Wt::WString data_;
  Wt::WString message_;

  /*
   * Both user and html will be formatted as html
   */
  ChatEvent(const Wt::WString& user, const Wt::WString& message)
    : type_(Message), user_(user), message_(message)
  { }

  ChatEvent(Type type, const Wt::WString& user,
	    const Wt::WString& data = Wt::WString::Empty)
    : type_(type), user_(user), data_(data)
  { }

  friend class SimpleChatServer;
};

typedef boost::function<void (const ChatEvent&)> ChatEventCallback;

/*! \brief A simple chat server
 */
class SimpleChatServer : boost::noncopyable
{
public:
  /*! \brief Create a new chat server.
   */
  SimpleChatServer(Wt::WServer& server);

  /*! \brief Try to login with given user name.
   *
   * Returns false if the login was not successfull. The passed callback method
   * is posted to when a new chat event is received.
   */
  bool login(const Wt::WString& user, const ChatEventCallback& handleEvent);

  /*! \brief Logout from the server.
   */
  void logout(const Wt::WString& user);

  /*! \brief Changes the name.
   */
  bool changeName(const Wt::WString& user, const Wt::WString& newUser);

  /*! \brief Get a suggestion for a guest user name.
   */
  Wt::WString suggestGuest();

  /*! \brief Send a message on behalve of a user.
   */
  void sendMessage(const Wt::WString& user, const Wt::WString& message);

  /*! \brief Typedef for a collection of user names.
   */
  typedef std::set<Wt::WString> UserSet;

  /*! \brief Get the users currently logged in.
   */
  UserSet users();

private:
  struct UserInfo {
    std::string sessionId;
    ChatEventCallback eventCallback;
  };

  typedef std::map<Wt::WString, UserInfo> UserMap;

  Wt::WServer& server_;
  boost::recursive_mutex mutex_;
  UserMap users_;

  void postChatEvent(const ChatEvent& event);
};

/*@}*/

#endif // SIMPLECHATSERVER_H_

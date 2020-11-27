// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:nil -*- 
// vim: ts=8 sw=2 smarttab

#pragma once

#include <boost/intrusive/slist.hpp>

#include "crimson/net/Dispatcher.h"
#include "crimson/common/log.h"

using crimson::net::Dispatcher;

class ChainedDispatchers {
  boost::intrusive::slist<
    Dispatcher,
    boost::intrusive::linear<true>,
    boost::intrusive::cache_last<true>> dispatchers;
public:
  void push_front(Dispatcher& dispatcher) {
    dispatchers.push_front(dispatcher);
  }
  void push_back(Dispatcher& dispatcher) {
    dispatchers.push_back(dispatcher);
  }
  void erase(Dispatcher& dispatcher) {
    dispatchers.erase(dispatchers.iterator_to(dispatcher));
  }
  bool empty() const {
    return dispatchers.empty();
  }
  seastar::future<> ms_dispatch(crimson::net::Connection* conn, MessageRef m);
  void ms_handle_accept(crimson::net::ConnectionRef conn);
  void ms_handle_connect(crimson::net::ConnectionRef conn);
  void ms_handle_reset(crimson::net::ConnectionRef conn, bool is_replace);
  void ms_handle_remote_reset(crimson::net::ConnectionRef conn);
};

using ChainedDispatchersRef = seastar::lw_shared_ptr<ChainedDispatchers>;

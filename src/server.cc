// Copyright (C) 2016 by Anna Seppala.
//
// This file is part of the hpp-corbaserver.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include "hpp/corbaserver/affordance/server.hh"

#include <hpp/util/exception.hh>
#include <hpp/util/debug.hh>
#include <hpp/corbaserver/server.hh>
#include "affordance.impl.hh"

namespace hpp
{
  namespace affordanceCorba
  {
    Server::Server (corbaServer::Server* server)
      : corbaServer::ServerPlugin (server),
      impl_ (NULL)
    {}

    Server::~Server ()
    {
      if (impl_) delete impl_;
    }

    std::string Server::name () const
    {
      return "affordance";
    }

    /// Start corba server
    void Server::startCorbaServer(const std::string& contextId,
				  const std::string& contextKind)
    {
      bool mThd = parent()->multiThread();
      impl_ = new corba::Server <impl::Afford>   (0, NULL, mThd, "child");

      impl_->implementation ().setServer (this);

      if (impl_->startCorbaServer(contextId, contextKind,
                                  "affordanceCorba", "affordance") != 0) {
	HPP_THROW_EXCEPTION (hpp::Exception, "Failed to start affordance corba server.");
      }

      // TODO this a very fragile. It works because startCorbaServer is called
      // after setProblemSolverMap in hpp::corbaServer::Server::getContext
      // implementation (file hpp-corbaserver/src/server.cc)
      impl_->implementation().resetAffordanceConfig();
    }
  } // namespace affordanceCorba
} // namespace hpp

HPP_CORBASERVER_DEFINE_PLUGIN(hpp::affordanceCorba::Server)

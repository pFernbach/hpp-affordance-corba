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
      initializeTplServer (impl_, contextId, contextKind, name(), "affordance");
      impl_->implementation ().setServer (this);

      // TODO this a very fragile. It works because startCorbaServer is called
      // after setProblemSolverMap in hpp::corbaServer::Server::getContext
      // implementation (file hpp-corbaserver/src/server.cc)
      impl_->implementation().resetAffordanceConfig();
    }

    ::CORBA::Object_ptr Server::servant(const std::string& name) const
    {
      if (name == "affordance") return impl_->implementation()._this();
      throw std::invalid_argument ("No servant " + name);
    }
  } // namespace affordanceCorba
} // namespace hpp

HPP_CORBASERVER_DEFINE_PLUGIN(hpp::affordanceCorba::Server)

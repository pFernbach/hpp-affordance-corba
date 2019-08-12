// Copyright (C) 2016 by Anna Seppala
//
// This file is part of the hpp-corbaserver.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef HPP_AFFORDANCE_CORBA_SERVER_HH
# define HPP_AFFORDANCE_CORBA_SERVER_HH

# include <hpp/corba/template/server.hh>
# include <hpp/corbaserver/server-plugin.hh>
# include <hpp/corbaserver/affordance/fwd.hh>
# include <hpp/corbaserver/affordance/config.hh>
# include "hpp/corbaserver/problem-solver-map.hh"

namespace hpp
{
  namespace affordanceCorba
  {
    namespace impl
    {
      class Afford;
    } // namespace impl

    class HPP_AFFORDANCE_CORBA_DLLAPI Server : public corbaServer::ServerPlugin
    {
    public:
      Server (corbaServer::Server* parent);

      /// \brief Shutdown CORBA server
      ~Server ();

      void startCorbaServer(const std::string& contextId,
			    const std::string& contextKind);

      std::string name () const;

    private:
      corba::Server <impl::Afford>* impl_;
    };

  } // end of namespace affordanceCorba.
} // end of namespace hpp.
#endif // HPP_AFFORDANCE_CORBA_SERVER_HH

// Copyright (C) 2016 by Anna Seppala.
//
// This file is part of the hpp-corbaserver.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <iostream>
#include <hpp/util/debug.hh>
#include "hpp/corbaserver/server.hh"
#include <hpp/corbaserver/problem-solver-map.hh>
#include <hpp/core/problem-solver.hh>

typedef hpp::corbaServer::Server CorbaServer;
typedef hpp::core::ProblemSolverPtr_t ProblemSolverPtr_t;
typedef hpp::core::ProblemSolver ProblemSolver;

int
main (int argc, const char* argv[])
{
  ProblemSolverPtr_t problemSolver = ProblemSolver::create ();
  CorbaServer corbaServer (problemSolver, argc,
			   const_cast<const char**> (argv), true);

  corbaServer.startCorbaServer ();
  corbaServer.loadPlugin (corbaServer.mainContextId(), "affordance-corba.so");
  corbaServer.processRequest(true);
}

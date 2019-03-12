// Copyright (C) 2009, 2010 by Florent Lamiraux, Thomas Moulard, JRL.
//
// This file is part of the hpp-affordance-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef HPP_AFFORDANCE_CORBA_IMPL_HH
# define HPP_AFFORDANCE_CORBA_IMPL_HH
# include <omniORB4/CORBA.h>
# include <hpp/fcl/BVH/BVH_model.h>
# include "hpp/core/problem-solver.hh"
# include "hpp/corbaserver/problem-solver-map.hh"
# include "hpp/corbaserver/affordance/fwd.hh"

# include "affordance-idl.hh"

namespace hpp
{
  namespace affordanceCorba
  {
    namespace impl
    {
      class Afford : public virtual
      POA_hpp::corbaserver::affordance::Afford
      {
      public:

	      Afford ();

				Afford (const core::ProblemSolverPtr_t& problemSolver);

          void setProblemSolverMap (hpp::corbaServer::ProblemSolverMapPtr_t psMap);

				void resetAffordanceConfig() throw (hpp::Error);

				affordance::OperationBases_t createOperations () throw (hpp::Error);

				void setAffordanceConfig (const char* affType, const hpp::doubleSeq& conf)
					throw (hpp::Error);

				hpp::doubleSeq* getAffordanceConfig (const char* affType)
					throw (hpp::Error);

				void setMargin (const char* affType, CORBA::Double margin)
					throw (hpp::Error);

				void setNeighbouringTriangleMargin (const char* affType,
					CORBA::Double nbTriMargin) throw (hpp::Error);
					
				void setMinimumArea (const char* affType, CORBA::Double minArea)
					throw (hpp::Error);

        bool checkModel (const char* obstacleName) throw (hpp::Error);

				void affordanceAnalysis (const char* obstacleName, 
                    const affordance::OperationBases_t & operations,std::vector<double> reduceSizes=std::vector<double>()) throw (hpp::Error);

                void analyseObject (const char* obstacleName, const hpp::doubleSeq& reduceSizesCorba) throw (hpp::Error);
				
                void analyseAll (const hpp::doubleSeq& reduceSizesCorba) throw (hpp::Error);

				void deleteAffordancesByType (const char* affordance,
					const char* obstacleName) throw (hpp::Error);

					void deleteAffordances (const char* obstacleName)
						throw (hpp::Error);

				void addAffObjects (const affordance::OperationBases_t& ops,
					const std::vector<affordance::CollisionObjects_t>& affObjs,
					const char* obstacleName) 
					throw (hpp::Error);
				
				hpp::doubleSeqSeqSeqSeq* getAffordancePoints (const char* affordance)
					throw (hpp::Error);

				hpp::Names_t* getAffRefObstacles (const char* affordance)
					throw (hpp::Error);

				hpp::Names_t* getAffordanceTypes () throw (hpp::Error);

        hpp::Names_t* getAffordanceConfigTypes () throw (hpp::Error);

      private:
          corbaServer::ProblemSolverMapPtr_t psMap_;
          core::ProblemSolverPtr_t problemSolver()
          {
              return psMap_->selected();
          }
      }; // class Afford
    } // namespace impl
  } // namespace affordanceCorba
} // namespace hpp

#endif //HPP_AFFORDANCE_CORBA_IMPL_HH

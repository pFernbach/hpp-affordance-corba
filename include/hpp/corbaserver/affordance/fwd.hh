// Copyright (C) 2010 by Anna Seppala, CNRS.
//
// This file is part of the hpp-affordance-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef HPP_AFFORDANCE_CORBA_FWD_HH
# define HPP_AFFORDANCE_CORBA_FWD_HH

//FIXME: should be replaced by CORBA base types forward declarations.
# include <omniORB4/CORBA.h>
# include <hpp/fcl/math/vec_3f.h>
# include <hpp/core/fwd.hh>
# include <hpp/affordance/fwd.hh>

namespace hpp
{
namespace fcl {
  template <typename T> class BVHModel;
  class CollisionGeometry;
  class RSS;
  class ShapeBase;
  class Triangle;
}
  namespace affordanceCorba
  {
    typedef core::ProblemSolver ProblemSolver;
    typedef core::ProblemSolverPtr_t ProblemSolverPtr_t;

    typedef fcl::CollisionGeometry CollisionGeometry_t;
    typedef boost::shared_ptr <CollisionGeometry_t> CollisionGeometryPtr_t;
    typedef hpp::pinocchio::Transform3f Transform3f;
    typedef fcl::BVHModel < fcl::RSS > Polyhedron_t;
    typedef boost::shared_ptr <Polyhedron_t> PolyhedronPtr_t;
    typedef fcl::ShapeBase BasicShape_t;
    typedef boost::shared_ptr <BasicShape_t> BasicShapePtr_t;

    typedef std::map <std::string, BasicShapePtr_t> ShapeMap_t;
    typedef std::map <std::string, std::vector <fcl::Triangle> > TriangleMap_t;
    typedef std::map <std::string, std::vector <fcl::Vec3f> > VertexMap_t;

    typedef hpp::pinocchio::matrix_t matrix_t;
    typedef hpp::pinocchio::vector_t vector_t;
    typedef hpp::pinocchio::vector3_t vector3_t;
    typedef hpp::pinocchio::size_type size_type;
    namespace impl
    {
      using CORBA::Boolean;
      using CORBA::Double;
      using CORBA::Short;
      using CORBA::SystemException;
      using CORBA::ULong;
      using CORBA::UShort;

    }
  } // end of namespace affordanceCorba.
} // end of namespace hpp.

#endif //! HPP_AFFORDANCE_CORBA_FWD_HH

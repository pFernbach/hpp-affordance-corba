// Copyright (C) 2009, 2010 by Florent Lamiraux, Thomas Moulard, JRL.
//
// This file is part of the hpp-affordance-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <iostream>

#include <pinocchio/fwd.hpp>
#include <hpp/util/debug.hh>

#include "hpp/affordance/affordance-extraction.hh"
#include "hpp/affordance/operations.hh"
#include <hpp/pinocchio/collision-object.hh>
#include <hpp/core/problem-solver.hh>
#include <string>
#include "affordance.impl.hh"

namespace hpp
{
  typedef std::vector<pinocchio::CollisionObjectPtr_t> ObjectStdVector_t;
  typedef hpp::core::FclCollisionObject FclCollisionObject;
  typedef hpp::core::FclCollisionObject* FclCollisionObjectPtr_t;
  typedef hpp::core::FclCollisionObjectSharePtr_t FclCollisionObjectSharePtr_t;
  typedef hpp::core::AffordanceObjects_t AffordanceObjects_t;
  namespace affordanceCorba
  {
    namespace impl
    {

        const std::string affSuffix = "aff";

        Afford::Afford () {}

                Afford::Afford (const core::ProblemSolverPtr_t& /*problemSolver*/) {}

				void Afford::resetAffordanceConfig() throw (hpp::Error)
				{
                    problemSolver()->affordanceConfigs.add("Support", vector3_t (0.3,0.3,0.05));
                    problemSolver()->affordanceConfigs.add("Lean", vector3_t (0.1,0.3,0.05));
                    problemSolver()->affordanceConfigs.add("Support45", vector3_t (0.1,0.3,0.05));
				}

				affordance::OperationBases_t Afford::createOperations () throw (hpp::Error)
				{
                    if (!problemSolver()->affordanceConfigs.has("Support")) {
						throw hpp::Error ("No 'Support' affordance type found Afford::createOperations ()");
					}
                        const hpp::pinocchio::vector3_t & sconf = problemSolver()->affordanceConfigs.get("Support");

                    if (!problemSolver()->affordanceConfigs.has("Lean")) {
						throw hpp::Error ("No 'Lean' affordance type found in Afford::createOperations ()");
					}
                        const hpp::pinocchio::vector3_t & lconf = problemSolver()->affordanceConfigs.get("Lean");

                    if (!problemSolver()->affordanceConfigs.has("Support45")) {
                        throw hpp::Error ("No 'Support45' affordance type found in Afford::createOperations ()");
                    }
                        const hpp::pinocchio::vector3_t & s45conf = problemSolver()->affordanceConfigs.get("Support45");


            affordance::SupportOperationPtr_t support
						(new affordance::SupportOperation(sconf[0], sconf[1], sconf[2]));
      		affordance::LeanOperationPtr_t lean
						(new affordance::LeanOperation(lconf[0], lconf[1], lconf[2]));
            affordance::Support45OperationPtr_t support45
                        (new affordance::Support45Operation(s45conf[0], s45conf[1], s45conf[2]));

      		affordance::OperationBases_t operations;
      		operations.push_back(support);
      		operations.push_back(lean);
            operations.push_back(support45);

					return operations;
				}

				void Afford::setAffordanceConfig (const char* affType,
					const hpp::doubleSeq& conf) throw (hpp::Error)
				{
					if (conf.length () != 3) {
						throw hpp::Error ("Configuration vector has invalid size.");
					}
					const vector3_t config (conf[0], conf[1], conf[2]);
                    problemSolver()->affordanceConfigs.add(affType, config);

                    /*const std::map<std::string, core::AffordanceConfig_t> map = problemSolver()->map
                        <core::AffordanceConfig_t> ();*/
				}
				hpp::doubleSeq* Afford::getAffordanceConfig (const char* affType)
					throw (hpp::Error)
				{
                    if (!problemSolver()->affordanceConfigs.has(affType)) {
						throw hpp::Error ("No given affordance type found in Afford::getAffordanceConfig");
					}
                    const vector3_t& config = problemSolver()->affordanceConfigs.get(affType);
					hpp::doubleSeq* conf = new hpp::doubleSeq ();
					conf->length ((CORBA::ULong)config.size ());
					for (std::size_t idx = 0; idx < conf->length(); idx++) {
						(*conf)[(CORBA::ULong)idx] = config[idx];
					}
					return conf;
				}

				void Afford::setMargin (const char* affType, CORBA::Double margin)
					throw (hpp::Error)
				{
                                    if (!problemSolver()->affordanceConfigs.has(affType)) {
						throw hpp::Error ("No given affordance type found in Afford::setMargin");
					}
                    vector3_t config = problemSolver()->affordanceConfigs.get(affType);
					config[0] = margin;

                    problemSolver()->affordanceConfigs.add(affType, config);
				}

				void Afford::setNeighbouringTriangleMargin (const char* affType,
					CORBA::Double nbTriMargin) throw (hpp::Error)
				{
                    if (!problemSolver()->affordanceConfigs.has(affType)) {
						throw hpp::Error ("No given affordance type found in Afford::setNeighbouringTriangleMargin");
					}
                    vector3_t config = problemSolver()->affordanceConfigs.get(affType);
					config[1] = nbTriMargin;

                    problemSolver()->affordanceConfigs.add(affType, config);
				}

				void Afford::setMinimumArea (const char* affType, CORBA::Double minArea)
					throw (hpp::Error)
				{
                    if (!problemSolver()->affordanceConfigs.has(affType)) {
						throw hpp::Error ("No given affordance type found in Afford::setMinimunArea");
					}
                    vector3_t config = problemSolver()->affordanceConfigs.get(affType);
					config[2] = minArea;

                    problemSolver()->affordanceConfigs.add(affType, config);
				}

        bool isBVHModelTriangles (const hpp::pinocchio::FclCollisionObjectPtr_t& object)
        {
            if (object->collisionGeometry ()->getNodeType () == fcl::BV_OBBRSS) {
                const affordance::BVHModelOBConst_Ptr_t model = boost::static_pointer_cast<const affordance::BVHModelOB>
                                                (object->collisionGeometry ());
                if (model->getModelType () == fcl::BVH_MODEL_TRIANGLES) {
                    return true;
                }
            }
            return false;
        }

        bool Afford::checkModel (const char* obstacleName) throw (hpp::Error)
        {
          std::list<std::string> obstacles =
                        problemSolver()->obstacleNames(false, true);
					std::list<std::string>::iterator objIt = std::find
						(obstacles.begin (), obstacles.end (), obstacleName);
					if (objIt == obstacles.end ()) {
    	      throw hpp::Error ("No obstacle by given name found. Unable to analyse.");
      		}
          if (!isBVHModelTriangles ((problemSolver()->obstacle (obstacleName))->fcl ())){
              return false; // wrong model type -> return false
          }
          return true;
        }

          void Afford::affordanceAnalysis (const char* obstacleName,
                    const affordance::OperationBases_t & operations, std::vector<double> reduceSizes) throw (hpp::Error)
	      {
					std::list<std::string> obstacles =
                        problemSolver()->obstacleNames(true, true);
					std::list<std::string>::iterator objIt = std::find
						(obstacles.begin (), obstacles.end (), obstacleName);
                    while(reduceSizes.size()<operations.size())
                        reduceSizes.push_back(0.);

					if (objIt == obstacles.end ()) {
    	      throw hpp::Error ("No obstacle by given name found. Unable to analyse.");
      		}
					try {
            affordance::SemanticsDataPtr_t aff = affordance::affordanceAnalysis
                ((problemSolver()->obstacle (obstacleName)->fcl()) , operations);
 					  std::vector<std::vector<fcl::CollisionObjectPtr_t > > affObjs =
                      affordance::getReducedAffordanceObjects( aff,reduceSizes);
						// add fcl::CollisionObstacles to problemSolver
						addAffObjects (operations, affObjs, obstacleName);
						} catch (const std::exception& exc) {
	          throw Error (exc.what ());
	        }
	      }

          void Afford::analyseObject (const char* obstacleName,const hpp::doubleSeq& reduceSizesCorba) throw (hpp::Error)
	      {
                    std::vector<double> reduceSizes;    // copy corba list to vector
                    for(size_type i=0 ; i < (size_type)reduceSizesCorba.length() ; ++i){
                        reduceSizes.push_back(reduceSizesCorba[(CORBA::ULong)i]);
                    }
					// first erase affordance information for obstacleName
				  deleteAffordances(obstacleName);
					affordance::OperationBases_t operations = createOperations ();
                    affordanceAnalysis (obstacleName, operations,reduceSizes);
				}

                void Afford::analyseAll (const hpp::doubleSeq &reduceSizesCorba) throw (hpp::Error)
				{
                    std::vector<double> reduceSizes;    // copy corba list to vector
                    for(size_type i=0 ; i < (size_type)reduceSizesCorba.length() ; ++i){
                        reduceSizes.push_back(reduceSizesCorba[(CORBA::ULong)i]);
                    }
					// first clear all old affordances:
                    problemSolver()->affordanceObjects.clear();// clear <std::vector<boost::shared_ptr<hpp::pinocchio::CollisionObject> > > ();
					affordance::OperationBases_t operations = createOperations ();
                    for (hpp::ObjectStdVector_t::const_iterator objIt =
                        problemSolver()->collisionObstacles ().begin ();
                        objIt != problemSolver()->collisionObstacles ().end (); objIt++)
						{
							const char* obstacleName = (*objIt)->name ().c_str ();
                            affordanceAnalysis (obstacleName, operations,reduceSizes);
						}
				}

				// delete affordances by type for given object
				void Afford::deleteAffordancesByType (const char* affordance,
					const char* obstacleName) throw (hpp::Error)
				{
					const std::string noObject = "";
					if (obstacleName == noObject) {
                        problemSolver()->affordanceObjects.erase(affordance);
					} else {
              if (!problemSolver()->affordanceObjects.has(affordance)) {
                  std::cout << "Afford::deleteAffordanceByType: no affordance objects to delete" << std::endl;
                  return;
              }
                        AffordanceObjects_t affs =
                            problemSolver()->affordanceObjects.get(affordance);

						for (unsigned int objIdx = 0; objIdx < affs.size (); objIdx++)
						{
                            if (affs[objIdx].first == obstacleName) {
								affs.erase(affs.begin () + objIdx);
								objIdx--;
							}
						}
                        problemSolver()->affordanceObjects.add(affordance,affs);
					}
				}

				// delete all affordances for given object
                void Afford::deleteAffordances (const char* obstacleNameNonAff)
					throw (hpp::Error)
				{
                    std::string obstacleName (obstacleNameNonAff);
                    obstacleName += affSuffix;
					const std::string noObject = "";
					if (obstacleName == noObject) {
						// if no obstacleName given, delete all affs in problemSolver
                        //problemSolver()->clear <std::vector<boost::shared_ptr<hpp::pinocchio::CollisionObject> > > ();
                        problemSolver()->affordanceObjects.clear();
					} else {
                        std::list<std::string> keys = problemSolver()->obstacleNames(true, false); /*problemSolver()->getKeys
                            <std::vector<boost::shared_ptr<hpp::pinocchio::CollisionObject> >,
                            std::list<std::string> > ();*/
						std::list<std::string>::iterator affIt = keys.begin ();
                        for (; affIt != keys.end (); affIt++)
                        {
                            for (std::map<std::string, AffordanceObjects_t>::iterator kit =
                                 problemSolver()->affordanceObjects.map.begin(); kit != problemSolver()->affordanceObjects.map.end();
                                 ++kit)
                            {
                                if((kit->first).find((*affIt)) != std::string::npos)
                                {
                                    problemSolver()->removeObstacle(kit->first);
                                    AffordanceObjects_t& affs =kit->second;
                                    for (unsigned int objIdx = 0; objIdx < affs.size (); objIdx++)
                                    {
                                        if (affs[objIdx].first == obstacleName) {
                                            affs.erase(affs.begin () + objIdx);
                                            objIdx--;
                                        }
                                    }
                                    problemSolver()->affordanceObjects.add(*affIt,affs);
                                }
                            }
						}
					}
				}


				void Afford::addAffObjects (const affordance::OperationBases_t& ops,
					const std::vector<affordance::CollisionObjects_t>& affObjs,
                    const char* obstacleNameNonAff) throw (hpp::Error)
				{
                    std::string obstacleName (obstacleNameNonAff);
                    obstacleName += affSuffix;
					for (unsigned int opIdx = 0; opIdx < ops.size (); opIdx++)
					{
                        AffordanceObjects_t objs;
						affordance::CollisionObjects_t affs = affObjs[opIdx];
						for (unsigned int objIdx = 0; objIdx < affs.size (); objIdx++) {
                            /*FclCollisionObjectSharePtr_t obj =
                            FclCollisionObjectSharePtr_t (new FclCollisionObject(*(affs[objIdx])));*/
                            std::stringstream ss;  ss << opIdx << "_" << objIdx;
                            std::string ig = obstacleName + ss.str();
                            problemSolver()->addObstacle(ig,*(affs[objIdx]),false,false);
                            hpp::pinocchio::CollisionObjectPtr_t obj  = problemSolver()->obstacle (ig);
                            objs.push_back (std::make_pair(ig, obj));
						}
                        if (problemSolver()->affordanceObjects.has(ops[opIdx]->affordance_)) {
                            //std::vector<FclCollisionObjectSharePtr_t >
                            AffordanceObjects_t mapObjs = problemSolver()->affordanceObjects.get(ops[opIdx]->affordance_);
							objs.insert (objs.begin () + objs.size (), mapObjs.begin (), mapObjs.end ());
						}
                        problemSolver()->affordanceObjects.erase(ops[opIdx]->affordance_);
                        problemSolver()->affordanceObjects.add(ops[opIdx]->affordance_, objs);
					}
				}

				hpp::doubleSeqSeqSeqSeq* Afford::getAffordancePoints (char const* affordance)
					throw (hpp::Error)
					{
						hpp::doubleSeqSeqSeqSeq *affs;
                        if (!problemSolver()->affordanceObjects.has(std::string (affordance))) {
							throw hpp::Error ("No affordance type of given name found. Unable to get affordance points.");
							}
                       const AffordanceObjects_t & affObjs =
                            problemSolver()->affordanceObjects.get(affordance);
						std::size_t nbAffs = affObjs.size ();
						affs = new hpp::doubleSeqSeqSeqSeq ();
						affs->length ((CORBA::ULong)nbAffs);
						for (std::size_t affIdx = 0; affIdx < nbAffs; affIdx++)
						{
							affordance::BVHModelOBConst_Ptr_t model =
                                affordance::GetModel (affObjs[affIdx].second->fcl());
							std::size_t nbTris = model->num_tris;
							hpp::doubleSeqSeqSeq tris;
							tris.length ((CORBA::ULong)nbTris);
							  for (std::size_t triIdx = 0; triIdx < nbTris; triIdx++)
								{
									hpp::doubleSeqSeq triangle;
									const fcl::Triangle& refTri = model->tri_indices[triIdx];
									triangle.length (3);
									for (unsigned int vertIdx= 0; vertIdx < 3; vertIdx++) {
                                    fcl::Vec3f p (affObjs[affIdx].second->fcl()->getRotation () *
											model->vertices [refTri[vertIdx]] +
                                            affObjs[affIdx].second->fcl()->getTranslation ());
								  	hpp::doubleSeq point;
								  	// point always 3D
								  	point.length (3);
								  	for (std::size_t idx = 0; idx < 3; idx++) {
								  		point[(CORBA::ULong)idx] = p[idx];
								  	}
										triangle[(CORBA::ULong)vertIdx] = point;
									}
									tris[(CORBA::ULong)triIdx] = triangle;
								}
								(*affs)[(CORBA::ULong)affIdx] = tris;
							}
							return affs;
						}

        hpp::Names_t* fromStringVector(const std::vector<std::string>& input)
        {
            CORBA::ULong size = (CORBA::ULong)input.size ();
            char** nameList = hpp::Names_t::allocbuf(size);
            hpp::Names_t *jointNames = new hpp::Names_t (size, size, nameList);
            for (std::size_t i = 0; i < input.size (); ++i)
            {
                std::string name = input[i];
                nameList [i] =
                        (char*) malloc (sizeof(char)*(name.length ()+1));
                strcpy (nameList [i], name.c_str ());
            }
            return jointNames;
        }

				hpp::Names_t* Afford::getAffRefObstacles (const char* affordance)
					throw (hpp::Error)
				{
						std::vector<std::string> objList;
                        if (!problemSolver()->affordanceObjects.has(std::string (affordance))) {
							throw hpp::Error ("No affordance type of given name found. Unable to get reference collision object.");
							}
                        AffordanceObjects_t affObjs = problemSolver()->affordanceObjects.get(affordance);
						for (std::size_t affIdx = 0; affIdx < affObjs.size (); affIdx++)
						{
                            /*affordance::BVHModelOBConst_Ptr_t model =
                                affordance::GetModel (affObjs[affIdx]->fcl());*/
                        objList.push_back(affObjs[affIdx].first);
						}
						hpp::Names_t* objListPtr = fromStringVector (objList);
						return objListPtr;
				}

				hpp::Names_t* Afford::getAffordanceTypes () throw (hpp::Error)
				{
                    std::vector<std::string> affTypes = problemSolver()->affordanceObjects.getKeys<std::vector<std::string> >();
            if (affTypes.empty ()) {
        		throw hpp::Error ("No affordances found. Return empty list.");
            }
					hpp::Names_t* affTypeListPtr = fromStringVector (affTypes);
					return affTypeListPtr;
				}

        hpp::Names_t* Afford::getAffordanceConfigTypes () throw (hpp::Error)
                {
            std::vector<std::string> affTypes = problemSolver()->affordanceConfigs.getKeys< std::vector<std::string> >();
            if (affTypes.empty ()) {
        	 std::cout << "No affordance configs found. Return empty list." << std::endl;
           hpp::Names_t* empty = new hpp::Names_t ();
           empty->length (0);
           return empty;
          }
					hpp::Names_t* affTypeListPtr = fromStringVector (affTypes);
					return affTypeListPtr;
				}


    } // namespace impl
  } // namespace affordanceCorba
} // namespace hpp

# Import Gepetto viewer helpwer class
# Import robot. Needed to create a robot instance for the viewer application
from hpp.corbaserver.affordance import Robot
# Import the affordance helper class to extract useful surface
# objects from the environment, and create an instance of affordanceTool
from hpp.corbaserver.affordance.affordance import AffordanceTool
# Import Problem solver (holds most of the generated data)
from hpp.corbaserver.problem_solver import ProblemSolver
from hpp.gepetto import Viewer

# Create instance of the hyq robot, the problem solver and the viewer
robot = Robot('hyq')
ps = ProblemSolver(robot)
r = Viewer(ps)

# Setting initial configuration to show on the viewer
q_init = robot.getCurrentConfig()
q_init[0:3] = [-2, 0, 0.63]
robot.setCurrentConfig(q_init)
r(q_init)

# Set coulour variables for different affordance types
SupportColour = [0.0, 0.95, 0.80]
LeanColour = [0.9, 0.5, 0]

afftool = AffordanceTool()

# Set the affordance configuration. The configuration vector has size 3
# and comprises the error margin, the angle margin for neighbouring triangles
# and the minimum area, in that order.
# If no configuration is set, a default configuration is used.
afftool.setAffordanceConfig('Support', [0.3, 0.3, 0.05])
afftool.setAffordanceConfig('Lean', [0.1, 0.3, 0.05])

# Load obstacle models and visualise affordances. When loading an obstacle,
# the affordance analysis is done automatically.
afftool.loadObstacleModel("hpp-affordance-corba", "darpa", "planning", r)
afftool.visualiseAffordances('Support', r, SupportColour)

# If affordance configuration is changed, the affordance analysis must
# be relaunched.
# First, delete the already created affordance objects (this function also
# deletes them from viewer)
afftool.deleteAffordances(r)
# Next, change the configuration settings for affordance type 'Support'.
# Notice that only the minimum area is changed.
afftool.setAffordanceConfig('Support', [0.3, 0.3, 0.1])
# Now, reanalyse the scene with updated requirements
afftool.analyseAll()
# And visualise. Note that the below function may be used to visualise one
# object or all objects, depending on its parameters
afftool.visualiseAffordances('Support', r, SupportColour)
# Now fewer support surfaces were found due to the stricter requirement.
afftool.deleteAffordances(r)
# Go back to default values for affordance configuration for all affordance types
afftool.resetAffordanceConfig()
afftool.analyseAll()
afftool.visualiseAffordances('Support', r, SupportColour)
# This procedure may be repeated, and functions that take only one
# configuration parameter also exist. This makes changing configuration settings
# more intuitive.
afftool.deleteAffordances(r)
# Change angle margin (accepted deviation of the normal of an affrodance surface
# from that required by an affordance type)
afftool.setMargin('Support', 0.0681487)
afftool.analyseAll()
afftool.visualiseAffordances('Support', r, SupportColour)

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

afftool.setAffordanceConfig('Support', [0.3, 0.3, 0.05])
afftool.setAffordanceConfig('Lean', [0.1, 0.3, 0.05])

# Load obstacle models and visualise affordances. When loading an obstacle,
# the affordance analysis is done automatically.
afftool.loadObstacleModel("hpp-affordance-corba", "darpa", "planning", r)
afftool.loadObstacleModel("hpp-affordance-corba", "box", "box1", r)
afftool.visualiseAffordances('Support', r, SupportColour)

# If an object is translated or rotated, the affordance analysis must
# be relaunched.
# First, delete the already created affordance objects (this function also
# deletes them from viewer)
afftool.deleteAffordances(r, 'box1/base_link_0')
# Next, move the obstacle and recompute its position (updates viewer)
ps.moveObstacle('box1/base_link_0', [0, 1, 0.25, 1, 0, 0, 0])
r.computeObjectPosition()
# Now, reanalyse the object that was moved
afftool.analyseObject('box1/base_link_0')
# And visualise. Note that the below function may be used to visualise one
# object or all objects, depending on its parameters
afftool.visualiseAffordances('Support', r, SupportColour, 'box1/base_link_0')
afftool.visualiseAffordances('Lean', r, LeanColour, 'box1/base_link_0')

# Create more obstacles to further demonstrate the analysis process:

# Some Lean affordances are no more valid due to the orientation of
# the obstacle
afftool.loadObstacleModel("hpp-affordance-corba", "box", "box2", r)
afftool.deleteAffordances(r, 'box2/base_link_0')
ps.moveObstacle('box2/base_link_0', [0, 1.5, 0.5, 0.985, 0.174, 0, 0])
r.computeObjectPosition()
afftool.analyseObject('box2/base_link_0')
afftool.visualiseAffordances('Support', r, SupportColour, 'box2/base_link_0')
afftool.visualiseAffordances('Lean', r, LeanColour, 'box2/base_link_0')

# No support affordances are found due to the orientation of the obstacle
afftool.loadObstacleModel("hpp-affordance-corba", "box", "box3", r)
afftool.deleteAffordances(r, 'box3/base_link_0')
ps.moveObstacle('box3/base_link_0', [0, 2, 0.75, 0.940, 0.342, 0, 0])
r.computeObjectPosition()
afftool.analyseObject('box3/base_link_0')
afftool.visualiseAffordances('Support', r, SupportColour, 'box3/base_link_0')
afftool.visualiseAffordances('Lean', r, LeanColour, 'box3/base_link_0')

// include
#include <iostream>
#include <fstream>

#include <rw/math.hpp> // Pi, Deg2Rad
#include <rw/math/Q.hpp>
#include <rw/math/Transform3D.hpp>
#include <rw/math/RPY.hpp>
#include <rw/math/Vector3D.hpp>
#include <rw/math/EAA.hpp>
#include <rw/rw.hpp>

// include for the PRM planner
#include <rw/pathplanning/QConstraint.hpp>
#include <rwlibs/pathplanners/prm/PRMPlanner.hpp>
#include <rwlibs/pathplanners/prm/PartialIndexTable.hpp>
#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>


// Namespace's
using namespace std;
using namespace rw::math;
using namespace rw::common;
using namespace rw::kinematics;
using namespace rw::loaders;
using namespace rw::models;
using namespace rw::pathplanning;
using namespace rw::proximity;
using namespace rw::trajectory;
using namespace rwlibs::pathplanners;
using namespace rwlibs::proximitystrategies;

// definition
#define MAXTIME 10.

// Code

void tostring(std::string x)
{
	std::string os = x;
	os.replace(0,4,"setQ");
}

int main(int argc, char** argv) {

		Math::seed(); // making the solution probabilistic
		const string wcFile = "/home/student/Desktop/RobWorkStudio/Kr16WallWorkCell/Scene.wc.xml";
		const string deviceName = "KukaKr16";
		const string deviceGripper = "ToolMount";
		const string bottle = "Bottle";
		cout << "Trying to use work cell " << wcFile << " and device " << deviceName << endl;

		WorkCell::Ptr wc = WorkCellLoader::Factory::load(wcFile);
		Device::Ptr device = wc->findDevice(deviceName);
		Frame* grip = wc->findFrame(deviceGripper);
		Frame* Item = wc->findFrame(bottle);
		if (device == NULL) {
			cerr << "Device: " << deviceName << " not found!" << endl;
			return 0;
		}
		if (grip == NULL) {
			cerr << "gripper: " << grip << " not found!" << endl;
			return 0;
		}
		if (device == NULL) {
			cerr << "Item: " << Item << " not found!" << endl;
			return 0;
		}

		const State state = wc->getDefaultState();



		// for writing out to luaprojekt2.txt
		ofstream myfile;
		myfile.open("luaprojekt2.txt");
		if (myfile.is_open())
		{
			myfile << "wc = rws.getRobWorkStudio():getWorkCell()\n";
			myfile << "state = wc:getDefaultState()\n";
			myfile << "device = wc:findDevice(\"" << deviceName << "\")\n";
			myfile << "Tools = wc:findFrame(\"" << deviceGripper << "\")\n";
			myfile << "Item = wc:findFrame(\"" << bottle << "\")\n";
			myfile << "table = wc:findFrame(\"Table\")\n\n";

			myfile << "function setQ(q)\n";
			myfile << "qq = rw.Q(#q,q[1],q[2],q[3],q[4],q[5],q[6])\n";
			myfile << "device:setQ(qq,state)\n";
			myfile << "rws.getRobWorkStudio():setState(state)\n";
			myfile << "rw.sleep(0.5)\n";
			myfile << "end\n\n";

			myfile << "function attach(obj,tool)\n";
			myfile << "rw.gripFrame(obj, tool, state)\n";
			myfile << "rws.getRobWorkStudio():setState(state)\n";
			myfile << "rw.sleep(0.1)\n";
			myfile << "end\n\n";

		}

		Q q_pick(6,-3.142,-0.827,-3.002,-3.143,0.099,-1.573);// pick point
		Q q_place(6,1.571,0.006,0.030,0.153,0.762,4.490);// place point

		State newState;
		newState = state;
		device->setQ(q_pick,newState);
		Kinematics::gripFrame(Item,grip,newState);

		CollisionDetector detector(wc, ProximityStrategyFactory::makeDefaultCollisionStrategy());

		QConstraint::Ptr constraint = QConstraint::make(&detector, device,newState);
		QSampler::Ptr sampler = QSampler::makeConstrained(QSampler::makeUniform(device),constraint); // sampler

		double resolution = 0.5;

		PRMPlanner* prm = PRMPlanner(device, wc, state, detector, resolution);
		prm->setCollisionCheckingStrategy(PRMPlanner::LAZY);
		prm->setNeighSearchStrategy(PRMPlanner::BRUTE_FORCE);
		prm->setShortestPathSearchStrategy(PRMPlanner::A_STAR);
		prm->buildRoadmap(1000);



		Path<Q> path;

		Timer t;
		t.resetAndResume();

		prm->query(q_pick, q_place, path, MAXTIME);

		t.pause();

		if (t.getTime() >= MAXTIME) {
			cout << "Notice: max time of " << MAXTIME << " seconds reached." << endl;
		}

		for (QPath::iterator it = path.begin(); it < path.end(); it++) {
				cout << *it << endl;
				if (it == path.begin())
				{
					myfile << "setQ({";
					for (int i = 0; i < 6; i++)
					{
						myfile << it[0][i];
						if (i <=4 )
						{
							myfile << ",";
						}
					}
					myfile << "})\n";

					myfile << "attach(Item,Tools)\n";
				}else if (it != path.begin() && it != path.end()-1)
				{
					myfile << "setQ({";
								for (int i = 0; i < 6; i++)
								{
									myfile << it[0][i];
									if (i <=4 )
									{
										myfile << ",";
									}
								}
								myfile << "})\n";
				}else
				{
					myfile << "setQ({";
						for (int i = 0; i < 6; i++)
						{
							myfile << it[0][i];
							if (i <=4 )
							{
								myfile << ",";
							}
						}
					myfile << "})\n";
					myfile << "attach(Item,table)\n";
					myfile << "setQ(0,0,0,0,0,0)\n"; // show that the item is not attach anymore
				}

			}
		// program ending, and closing open things
			myfile.close();
			cout << "Program done." << endl;



	return 0; // end program.
}

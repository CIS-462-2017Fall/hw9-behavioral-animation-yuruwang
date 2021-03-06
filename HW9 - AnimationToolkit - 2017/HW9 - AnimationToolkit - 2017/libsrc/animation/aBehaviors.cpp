#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 dir = (targetPos - actorPos).Normalize();
	Vdesired = dir * actor->gMaxSpeed;

	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 dir = (actorPos - targetPos).Normalize();
	Vdesired = dir * actor->gMaxSpeed;

	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	vec3 e = targetPos - actorPos;
	Vdesired = actor->KArrival * e;
	

	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	vec3 e = actorPos - targetPos;
	Vdesired = actor->KDeparture * ( e / (e.Length() * e.Length()));

	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel( BehaviorController* actor)
{

	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

	//TODO: add your code here
	vec3 Varrival(0, 0, 0);
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 e = targetPos - m_actorPos;
	Varrival = actor->KArrival * e;
	Vdesired = Varrival;
	
	vec3 Vavoid(0, 0, 0);
	//TODO: add your code here to compute Vavoid 


	// Step 2. compute Lb
	//TODO: add your code here
	double LB = actor->TAvoid * Vdesired.Length();

	// Step 3. find closest obstacle 
	//TODO: add your code here
	double minDist = DBL_MAX;
	double dist = 0;
	Obstacle closestO;
	
	for (Obstacle o : *mObstacles) {
		vec3 obstaclePos = o.m_Center.getLocalTranslation();
		dist = (obstaclePos - m_actorPos).Length();
		if (dist < minDist) {
			minDist = dist;
			closestO = o;
			m_obstaclePos = o.m_Center.getLocalTranslation();
		}
	}
	
	m_obstaclePos = closestO.m_Center.getLocalTranslation();


	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TODO: add your code here
	
	mat3 Rmat = actor->getGuide().getLocalRotation();  // is rotattion matrix of lead agent

	vec3 obstacle_posB = Rmat.Inverse() * (m_obstaclePos - m_actorPos);
	double dx = obstacle_posB[2];
	double dy = obstacle_posB[0];
	double ro = closestO.m_Radius;
	double rb = actor->gAgentRadius;
	bool collision = false;

	// get rid of obstacle that behind agent
	if (obstacle_posB[2] < (-1.0) * rb) {
		collision = false;
	}
	else {
		if (abs(dx) > LB + rb + ro) {
			collision = false;
		}
		else {
			if (abs(dy) > ro + rb) {
				collision = false;
			}
			else {
				collision = true;
			}
		}
	}
	

	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TODO: add your code here
	vec3 VAvoid_b = vec3(0.0, 0.0, 0.0);
	if (collision) {
		vec3 n = (-1.0) * vec3(dy, 0.0, 0.0) / abs(dy);
		double magnitude = actor->KAvoid * ((ro + rb)- abs(dy)) / (rb + ro);
		VAvoid_b = n * magnitude;
	}
	else {
		VAvoid_b = vec3(0.0, 0.0, 0.0);
	}
	
	vec3 VAvoid_world = Rmat * VAvoid_b;
	vec3 actorVel_world = m_actorVel;
	Vdesired = VAvoid_world + Vdesired;
	
	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// compute Vdesired = Vwander

	// Step. 1 find a random direction
	//TODO: add your code here
	vec3 n = vec3(rand() % 100 - 50, 0.0, rand() % 100 - 50);
	if (n == vec3(0, 0, 0)) {
		n = vec3(1, 0, 0);
	}
	n = n / n.Length();
	
	// Step2. scale it with a noise factor
	//TODO: add your code here
	vec3 r_noise = actor->KNoise * n;


	// Step3. change the current Vwander  to point to a random direction
	//TODO: add your code here
	m_Wander = m_Wander + r_noise;


	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TODO: add your code here
	m_Wander = actor->KWander * m_Wander / m_Wander.Length();
	vec3 v0_world = actor->getVelocity();
	Vdesired = v0_world + m_Wander;

	if (Vdesired.Length() > 200) {
		Vdesired = Vdesired.Normalize() * 200;
	}

	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target
	 
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	//TODO: add your code here
	
	if (actor == leader) {
		vec3 leaderPos = leader->getPosition();
		vec3 e = targetPos - leaderPos;
		Vdesired = actor->KArrival * e;
	}


	// Step 2. if not first agent compute Valign as usual
	//TODO: add your code here
	
	vec3 Vsum = vec3(0.0, 0.0, 0.0);
	int count = 0;
	if (actor != leader) {
		for (int i = 0; i < agentList.size(); ++i) {
			BehaviorController* agent = agentList[i].getBehaviorController();
			if (agent == actor) {
				continue;
			}
			double dist = (agent->getPosition() - actorPos).Length();
			if (dist > actor->gKNeighborhood) {
				continue;
			}

			vec3 agentVel_0 = agent->getVelocity();
			Vsum = Vsum + agentVel_0;
			count++;
		}

		if (count > 0) {
			Vdesired = actor->KAlignment * Vsum / (double)count;
		}
		else {
			Vdesired = vec3(0.0, 0.0, 0.0);
		}
		
	}
		
	
	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TODO: add your code here to compute Vdesired 
	
	vec3 Vsum = vec3(0.0, 0.0, 0.0);
	int count = 0;
	for (int i = 0; i < agentList.size(); ++i) {
		BehaviorController* agent = agentList[i].getBehaviorController();

		if (agent == actor) {
			continue;
		}
		
		
		double dist = (agent->getPosition() - actorPos).Length();
		if (dist > actor->gKNeighborhood) {
			continue;
		}

		vec3 e = actorPos - agent->getPosition();
		vec3 VDepature_world = e / (e.Length() * e.Length());
		Vsum = Vsum + VDepature_world;
		count++;
	}

	if (count > 0) {
		Vdesired = actor->KSeparation * Vsum / (double)count;
	}
	else {
		Vdesired = vec3(0.0, 0.0, 0.0);
	}
	

	if (Vdesired.Length() < 50.0) {
		Vdesired = vec3(0.0, 0.0, 0.0);
	}
		
		
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TODO: add your code here 
	
	vec3 massCenter = vec3(0.0, 0.0, 0.0);
	int count = 0;
	for (int i = 0; i < agentList.size(); ++i) {
		BehaviorController* agent = agentList[i].getBehaviorController();

		massCenter= massCenter + agent->getPosition();
		count++;
	}

	if (count > 0) {
		massCenter = massCenter / (double)count;
	}
	else {
		massCenter = actorPos;
	}
	

	Vdesired = actor->KCohesion * (massCenter - actorPos);
	
	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	vec3 targetPos = m_pTarget->getLocalTranslation();

	// compute Vdesired = Vflocking
	// TODO: add your code here
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader

	vec3 V_seperate(0.0, 0.0, 0.0);
	vec3 V_align(0.0, 0.0, 0.0);
	vec3 V_cohesion(0.0, 0.0, 0.0);

	if (actor == leader) {
		vec3 e = targetPos - actorPos;
		Vdesired = actor->KArrival * e;

	}
	else {
		Vdesired = vec3(0.0, 0.0, 0.0);		
	}

	// for seperate--------------------------------------------------------------------------------
	vec3 Vsum = vec3(0.0, 0.0, 0.0);
	int count = 0;
	for (int i = 0; i < agentList.size(); ++i) {
		BehaviorController* agent = agentList[i].getBehaviorController();

		if (agent == actor) {
			continue;
		}

		double dist = (agent->getPosition() - actorPos).Length();
		if (dist > actor->gKNeighborhood) {
			continue;
		}

		vec3 e = actorPos - agent->getPosition();
		vec3 VDepature_world = e / (e.Length() * e.Length());
		Vsum = Vsum + VDepature_world;
		count++;
	}

	if (count > 0) {
		V_seperate = actor->KSeparation * Vsum / (double)count;
	}
	else {
		V_seperate = vec3(0.0, 0.0, 0.0);
	}


	if (V_seperate.Length() < 50.0) {
		V_seperate = vec3(0.0, 0.0, 0.0);
	}


	// for alignment----------------------------------------------------------------------------

	// Step 2. if not first agent compute Valign as usual
	//TODO: add your code here

	Vsum = vec3(0.0, 0.0, 0.0);
	count = 0;
	for (int i = 0; i < agentList.size(); ++i) {
		BehaviorController* agent = agentList[i].getBehaviorController();
		if (agent == actor) {
			continue;
		}
		double dist = (agent->getPosition() - actorPos).Length();
		if (dist > actor->gKNeighborhood) {
			continue;
		}

		vec3 agentVel_0 = agent->getVelocity();
		Vsum = Vsum + agentVel_0;
		count++;
	}

	if (count > 0) {
		V_align = actor->KAlignment * Vsum / (double)count;
	}
	else {
		V_align = vec3(0.0, 0.0, 0.0);
	}

	// for cohesion--------------------------------------------------------------------

	vec3 massCenter = vec3(0.0, 0.0, 0.0);
	count = 0;
	for (int i = 0; i < agentList.size(); ++i) {
		BehaviorController* agent = agentList[i].getBehaviorController();

		massCenter = massCenter + agent->getPosition();
		count++;
	}

	if (count > 0) {
		massCenter = massCenter / (double)count;
	}
	else {
		massCenter = actorPos;
	}


	V_cohesion = actor->KCohesion * (massCenter - actorPos);

	float c_seperate = 1.0;
	float c_alignment = 1.0;
	float c_cohesion = 1.0;
	Vdesired = Vdesired + c_seperate * V_seperate + c_alignment * V_align + c_cohesion * V_cohesion;

	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	vec3 targetPos = m_pTarget->getLocalTranslation();

	// TODO: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 4.0;  float CArrival = 2.0;

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	mat3 Rmat = leader->getGuide().getLocalRotation();  // is rotattion matrix of lead agent
	vec3 Tmat = leader->getGuide().getLocalTranslation();  // is rotattion matrix of lead agent

	vec3 V_seperate(0.0, 0.0, 0.0);
	vec3 V_arrival (0.0, 0.0, 0.0);

	if (actor == leader) {
		vec3 e = targetPos - actorPos;
		Vdesired = actor->KArrival * e;

	}
	else {
		Vdesired = vec3(0.0, 0.0, 0.0);
	}

	
	// for seperate--------------------------------------------------------------------------------
	
	if (actor != leader) {
		vec3 Vsum = vec3(0.0, 0.0, 0.0);
		int count = 0;
		for (int i = 0; i < agentList.size(); ++i) {
			BehaviorController* agent = agentList[i].getBehaviorController();

			if (agent == actor) {
				continue;
			}

			if (agent == leader) {
				continue;
			}

			double dist = (agent->getPosition() - actorPos).Length();
			if (dist > actor->gKNeighborhood) {
				continue;
			}

			vec3 e = actorPos - agent->getPosition();
			vec3 VDepature_world = e / (e.Length() * e.Length());
			Vsum = Vsum + VDepature_world;
			count++;
		}

		if (count > 0) {
			V_seperate = actor->KSeparation * Vsum / (double)count;
		}
		else {
			V_seperate = vec3(0.0, 0.0, 0.0);
		}


		if (V_seperate.Length() < 50.0) {
			V_seperate = vec3(0.0, 0.0, 0.0);
		}
	}
	
	
	
	// for arrival----------------------------------------------------------------------------

	vec3 followerTarget = vec3(0.0, 0.0, -500.0);
	followerTarget = leader->getPosition() + Rmat * followerTarget;
	if (actor != leader) {
		vec3 e = followerTarget - actorPos;
		V_arrival = actor->KArrival * e;
	}
	
	
	Vdesired = Vdesired + CSeparation * V_seperate + CArrival * V_arrival;

	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////


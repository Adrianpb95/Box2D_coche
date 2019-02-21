/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	07 / 02 / 2018
*/


#include "Elements.hpp"
#include "Scene.hpp"
#include <vector>

Vehicle::Vehicle(Scene & scene) : GameObject(scene){

	////COCHE
	{		
		//VALORES DE LAS RUEDAS
		b2BodyDef wheel_def;
		wheel_def.type = b2_dynamicBody;
		b2Body * wheel;
		b2CircleShape wheel_shape;
		wheel_shape.m_radius = 25.f;
		b2FixtureDef wheel_fixture;
		wheel_fixture.shape = &wheel_shape;
		wheel_fixture.density = 0.5f;
		wheel_fixture.restitution = 0.f;
		wheel_fixture.friction = 200.0f;

		//RUEDA IZQ	
		wheel_def.position.Set(60.f, 650.f);
		wheel = scene.phxWorld->CreateBody(&wheel_def);
		wheel->CreateFixture(&wheel_fixture);
		bodies["left_wheel"] = wheel;

		//RUEDA DER		
		wheel_def.position.Set(160.f, 650.f);
		wheel = scene.phxWorld->CreateBody(&wheel_def);
		wheel->CreateFixture(&wheel_fixture);
		bodies["right_wheel"] = wheel;		

		//VALORES DE LA CARROCERIA
		b2BodyDef car_def;
		car_def.type = b2_dynamicBody;
		b2Body * car;
		b2PolygonShape car_shape;
		b2FixtureDef car_fixture;
		car_fixture.shape = &car_shape;
		car_fixture.density = 1.0f;
		car_fixture.restitution = 0.75f;
		car_fixture.friction = 0.50f;

		//CARROCERIA
		car_shape.SetAsBox(100.f, 10.f);
		car_def.position.Set(110.f, 600.f);
		car = scene.phxWorld->CreateBody(&car_def);
		car->CreateFixture(&car_fixture);
		bodies["car"] = car;

		//JOINTS DE LAS RUEDAS 
		b2RevoluteJointDef joint_def;
		b2RevoluteJoint * joint;

		//RUEDA DERECHA
		joint_def.bodyA = bodies["right_wheel"];
		joint_def.bodyB = bodies["car"];	
		joint_def.localAnchorA.Set(0.0f, 0.0f);
		joint_def.localAnchorB.Set(50.0f, -20.0f);
		
		joint_def.collideConnected = false;
		joint = (b2RevoluteJoint*)scene.phxWorld->CreateJoint(&joint_def);
		joints["car_rightWheel"] = joint;

		//RUEDA IZQUIERDA --> MOTOR DEL COCHE
		joint_def.bodyA = bodies["left_wheel"];
		joint_def.bodyB = bodies["car"];	
		joint_def.localAnchorA.Set(0.0f, 0.0f);
		joint_def.localAnchorB.Set(-50.0f, -20.0f);
		
		joint_def.collideConnected = false;
		joint_def.enableMotor = true;
		joint_def.maxMotorTorque = 10000000.f;
		joint_def.motorSpeed = 0.f; 

		joint = (b2RevoluteJoint*)scene.phxWorld->CreateJoint(&joint_def);		
		r_joints["carMotor"] = joint;

		//GRUA PIEZA ABAJO		
		car_fixture.shape = &car_shape;
		car_fixture.density = 0.01f;
		car_fixture.restitution = 0.f;
		car_fixture.friction = 20.0f;

		car_shape.SetAsBox(100.f, 10.f);
		car_def.position.Set(110.f, 625.f);
		car = scene.phxWorld->CreateBody(&car_def);
		car->CreateFixture(&car_fixture);
		bodies["car_trunk_down"] = car;

		car_shape.SetAsBox(100.f, 10.f);
		car_def.position.Set(110.f, 645.f);
		car = scene.phxWorld->CreateBody(&car_def);
		car->CreateFixture(&car_fixture);
		bodies["car_trunk"] = car;

		//GRUA PIEZA IZQ
		car_shape.SetAsBox(5.f, 40.f, b2Vec2(-100,30),0);		
		car = scene.phxWorld->CreateBody(&car_def);
		bodies["car_trunk"]->CreateFixture(&car_fixture);		

		////GRUA PIEZA DER
		car_shape.SetAsBox(5.f, 40.f, b2Vec2(100,30), 0);
		car = scene.phxWorld->CreateBody(&car_def);
		bodies["car_trunk"]->CreateFixture(&car_fixture);
		
		//JOINT COCHE - GRUA (DESPLAZAMIENTO LATERAL)
		b2PrismaticJointDef joint_def_pris;
		b2PrismaticJoint * joint_pris;

		joint_def_pris.bodyA = bodies["car_trunk_down"];
		joint_def_pris.bodyB = bodies["car"];
		joint_def_pris.localAnchorA.Set(0, -25);
		joint_def_pris.localAnchorB.Set(0, 0);

		joint_def_pris.enableLimit = true;
		joint_def_pris.lowerTranslation = 0;
		joint_def_pris.upperTranslation = 0;

		joint_def_pris.enableMotor = true;
		joint_def_pris.maxMotorForce = 100000;
		joint_def_pris.motorSpeed = 0;

		joint_def_pris.collideConnected = false;
		joint_pris = (b2PrismaticJoint*)scene.phxWorld->CreateJoint(&joint_def_pris);
		p_joints["car_truckDown"] = joint_pris;		

		//JOINT COCHE - GRUA (ROTACION)
		joint_def.bodyA = bodies["car_trunk"];
		joint_def.bodyB = bodies["car_trunk_down"];
		joint_def.localAnchorA.Set(-100.0f, -30.0f);
		joint_def.localAnchorB.Set(-100.0f, 0.0f);

		joint_def.collideConnected = false;

		joint_def.enableLimit = true;
		joint_def.referenceAngle = 0;
		joint_def.lowerAngle = -2.5;
		joint_def.upperAngle = -0.035;

		joint_def.enableMotor = true;
		joint_def.maxMotorTorque = 10000000.f;
		joint_def.motorSpeed = 0.f;

		joint = (b2RevoluteJoint*)scene.phxWorld->CreateJoint(&joint_def);
		r_joints["truckMotor"] = joint;
	}
}

void Vehicle::move(float vel){	
	r_joints["carMotor"]->SetMotorSpeed(vel);
}

void Vehicle::truckTranslation(float vel) {
	if(vel > 0)
		p_joints["car_truckDown"]->SetLimits(0, 100);
	else
		p_joints["car_truckDown"]->SetLimits(0, 0); //devolvemos la grua a su posicion al soltar el boton

	p_joints["car_truckDown"]->SetMotorSpeed(vel);
}

void Vehicle::truckRotation(float vel) {	
	r_joints["truckMotor"]->SetMotorSpeed(vel);		
}

Turret::Turret(Scene & scene) : GameObject(scene) {	

	//PELOTA
	b2BodyDef ball_def;
	ball_def.type = b2_dynamicBody;
	b2Body * ball;
	b2CircleShape ball_shape;
	ball_shape.m_radius = 80.f;
	b2FixtureDef ball_fixture;
	ball_fixture.shape = &ball_shape;
	ball_fixture.density = 0.05f;
	ball_fixture.restitution = 0.0f;
	ball_fixture.friction = 100.0f;

	ball_def.position.Set(1750.f, 800.f);
	ball = scene.phxWorld->CreateBody(&ball_def);
	ball->CreateFixture(&ball_fixture);
	bodies["turretBall"] = ball;
	
	//EJE DE UNION	
	b2BodyDef pivot_def;
	pivot_def.type = b2_staticBody;
	b2Body * pivot;
	b2CircleShape pivot_shape;
	pivot_shape.m_radius = 15.f;
	b2FixtureDef pivot_fixture;
	pivot_fixture.shape = &pivot_shape;
	pivot_fixture.density = 1.0f;
	pivot_fixture.restitution = 0.75f;
	pivot_fixture.friction = 1.0f;

	pivot_def.position.Set(1700.f, 600.f);
	pivot = scene.phxWorld->CreateBody(&pivot_def);
	pivot->CreateFixture(&pivot_fixture);
	bodies["turretPivot"] = pivot;
	
	//VALORES TORRETA
	b2BodyDef turret_def;
	turret_def.type = b2_staticBody;
	b2Body * turret;
	b2PolygonShape turret_shape;
	b2FixtureDef turret_fixture;
	turret_fixture.shape = &turret_shape;
	turret_fixture.density = 1.f;

	//CUERPO TORRETA
	turret_shape.SetAsBox(20.f, 200.f);
	turret_def.position.Set(1700.f, 400.f);
	turret_fixture.isSensor = true;	

	turret = scene.phxWorld->CreateBody(&turret_def);
	turret->CreateFixture(&turret_fixture);
	bodies["turretBody"] = turret;
	
	
	//BRAZO DERECHO TORRETA
	turret_def.type = b2_dynamicBody;
	turret_shape.SetAsBox(20.f, 100.f);
	turret_def.position.Set(1770.f, 650);
	turret_fixture.isSensor = false;
	turret_def.angle = -70.f;
	
	turret = scene.phxWorld->CreateBody(&turret_def);
	turret->CreateFixture(&turret_fixture);
	bodies["turretRigth_Arm"] = turret;		

	////BRAZO IZQUIERDO TORRETA
	turret_shape.SetAsBox(20.f, 100.f);
	turret_def.position.Set(1630.f, 650);
	turret_fixture.isSensor = false;
	turret_def.angle = 70.f;

	turret = scene.phxWorld->CreateBody(&turret_def);
	turret->CreateFixture(&turret_fixture);
	bodies["turretLeft_Arm"] = turret;
	
	b2RevoluteJointDef joint_def;
	b2RevoluteJoint * joint;

	joint_def.bodyA = bodies["turretPivot"];
	joint_def.bodyB = bodies["turretRigth_Arm"];
	joint_def.localAnchorA.Set(0.0f, 0.0f);
	joint_def.localAnchorB.Set(0.0f, -70.0f);
	joint_def.collideConnected = false;
	joint_def.enableMotor = true;
	joint_def.maxMotorTorque = 1500000000.f;
	joint_def.motorSpeed = 0.f;
	
	joint = (b2RevoluteJoint*)scene.phxWorld->CreateJoint(&joint_def);
	r_joints["pivotR"] = joint;

	joint_def.bodyA = bodies["turretPivot"];
	joint_def.bodyB = bodies["turretLeft_Arm"];
	joint_def.localAnchorA.Set(0.0f, 0.0f);
	joint_def.localAnchorB.Set(0.0f, -70.0f);
	joint_def.collideConnected = false;
	joint_def.enableMotor = true;
	joint_def.maxMotorTorque = 1500000000.f;
	joint_def.motorSpeed = 0.f;
	joint = (b2RevoluteJoint*)scene.phxWorld->CreateJoint(&joint_def);
	r_joints["pivotL"] = joint;
}

void Turret::move(float vel) {	
	r_joints["pivotR"]->SetMotorSpeed(vel);
	r_joints["pivotL"]->SetMotorSpeed(vel);	
}

Trigger::Trigger(Scene & scene) : GameObject(scene) {

	activeBool = false;

	//TRIGGER TORRETA
	b2BodyDef triggerT_def;
	triggerT_def.type = b2_staticBody;
	b2Body * triggerT;
	b2PolygonShape triggerT_shape;
	b2FixtureDef triggerT_fixture;
	triggerT_fixture.shape = &triggerT_shape;

	triggerT_fixture.isSensor = true;
	
	triggerT_shape.SetAsBox(200.f, 10.f);
	triggerT_def.position.Set(1820.f, 200.f);
	triggerT = scene.phxWorld->CreateBody(&triggerT_def);
	triggerT->CreateFixture(&triggerT_fixture);
	bodies["triggerT"] = triggerT;	

	//TRIGGER PLATAFORMA CONTENEDORA
	triggerT_shape.SetAsBox(50.f, 10.f);
	triggerT_def.position.Set(500.f, 780.f);
	triggerT = scene.phxWorld->CreateBody(&triggerT_def);
	triggerT->CreateFixture(&triggerT_fixture);
	bodies["triggerP"] = triggerT;
}

Platform::Platform(Scene & scene) : GameObject(scene) {	

	// SUELO
	{
		b2BodyDef body_definition;

		body_definition.type = b2_staticBody;
		body_definition.position.Set(0.f, 600.f);                           // Posición inicial absoluta
		body_definition.angle = 0.f;

		b2Body * body = scene.phxWorld->CreateBody(&body_definition);

		//Vertices del suelo
		vector<b2Vec2> sueloShapes = {
			b2Vec2(0.f, 0.f),
			b2Vec2(0.f, scene.heightScene),
			b2Vec2(0.f, 0.f),
			b2Vec2(200.f, 0.f),
			b2Vec2(200.f, 0.f),
			b2Vec2(400.f, -100.f),
			b2Vec2(400.f, -100.f),
			b2Vec2(600.f, -150.f),
			b2Vec2(800.f, -120.f),
			b2Vec2(1000.f, -500.f),
			b2Vec2(1400, -400.f),
			b2Vec2(1400, -400.f),
			b2Vec2(1920, -400.f),
			b2Vec2(1920, -400.f),
			b2Vec2(1910, scene.heightScene)
		};
		b2EdgeShape body_shape;
		b2FixtureDef body_fixture;

		// Se añande una fixture al body:
		for (int i = 0; i < sueloShapes.size() - 1; i++) {
			if (sueloShapes[i].x != 800.f) {											//Hueco en medio
				body_shape.Set(sueloShapes[i], sueloShapes[i + 1]);						//Coordenadas locales respecto al centro del body		
				body_fixture.shape = &body_shape;
				body->CreateFixture(&body_fixture);
			}
		}
	}


	//PLATAFORMA MOVIL
	b2BodyDef platform_def;
	platform_def.type = b2_staticBody;
	b2Body * platform;
	b2PolygonShape platform_shape;
	b2FixtureDef platform_fixture;
	platform_fixture.shape = &platform_shape;
	platform_fixture.density = 1.0f;
	platform_fixture.restitution = 0.0f;
	platform_fixture.friction = 1.0f;

	platform_shape.SetAsBox(100.f, 10.f);
	platform_def.position.Set(900.f, -5.f);
	platform = scene.phxWorld->CreateBody(&platform_def);
	platform->CreateFixture(&platform_fixture);
	bodies["platform"] = platform;

	//PLATAFORMA AEREA
	platform_shape.SetAsBox(10.f, 60.f);
	platform_def.position.Set(600.f, 800.f);	
	platform = scene.phxWorld->CreateBody(&platform_def);
	platform->CreateFixture(&platform_fixture);
	bodies["skyPlatform"] = platform;

	platform_shape.SetAsBox(100.f, 10.f, b2Vec2(-100, -50), 0);
	platform = scene.phxWorld->CreateBody(&platform_def);
	bodies["skyPlatform"]->CreateFixture(&platform_fixture);
	
	platform_shape.SetAsBox(10.f, 40.f, b2Vec2(-200, -20), 0);
	platform = scene.phxWorld->CreateBody(&platform_def);
	bodies["skyPlatform"]->CreateFixture(&platform_fixture);

}

void Platform::move(float vel) {
	//Solo se puede mover entre los valores de y --> (-10,800)
	if(bodies["platform"]->GetTransform().p.y > (-10-vel) && bodies["platform"]->GetTransform().p.y < (800-vel)){
		b2Vec2 posAux(0, vel);
		bodies["platform"]->SetTransform(bodies["platform"]->GetTransform().p + posAux, 0);
	}
}
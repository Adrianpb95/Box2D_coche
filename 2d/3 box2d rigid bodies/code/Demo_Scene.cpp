/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	07 / 02 / 2018
*/


#include "Demo_Scene.hpp"

Demo_Scene::Demo_Scene(){	
	phxWorld = create_physics_world(widthScene	, heightScene);		
	add_game_object("vehicle", shared_ptr< GameObject >(car = new Vehicle(*this)));
	add_game_object("turret", shared_ptr< GameObject >(turret = new Turret(*this)));
	add_game_object("triggerT", shared_ptr< GameObject >(triggerTurret = new Trigger(*this)));
	add_game_object("platform_Movil", shared_ptr< GameObject >(platformMovil = new Platform(*this)));
	finish = false;
	
}

void Demo_Scene::update(float deltaTime){
	//Scene::update(deltaTime);		
	phxWorld->Step(deltaTime, 8, 4);

	//Detecta si el coche esta sobre el trigger de la torreta
	if (triggerTurret->bodies["triggerT"]->GetContactList() != nullptr )
		if(triggerTurret->bodies["triggerT"]->GetContactList()->other == car->bodies["left_wheel"]) {
			triggerTurret->activeBool = true;
			platformMovil->bodies["platform"]->SetTransform(b2Vec2(900,100), 0);			
		}

	//Detecta si la bola esta sobre el trigger del contenedor
	if (triggerTurret->bodies["triggerP"]->GetContactList() != nullptr)
		if (triggerTurret->bodies["triggerP"]->GetContactList()->other == turret->bodies["turretBall"]) {
			finishScene();
		}

	//Detecta si el coche se ha caido por el hueco
	if (car->bodies["left_wheel"]->GetTransform().p.y < 50) {		
		resetScene();
	}
}

void Demo_Scene::render(sf::RenderWindow & renderer){		
	for (auto game_object : game_objects) {
		game_object.second->render(*phxWorld,renderer);
	}
	//Pinta los sistemas de particulas
	if (finish) {
		sf::Time elapsed = timer.restart();
		particles1.update(elapsed);
		particles2.update(elapsed);
		renderer.draw(particles1);
		renderer.draw(particles2);
	}
}

shared_ptr< b2World > Demo_Scene::create_physics_world(float world_width, float world_height){	
	shared_ptr< b2World > physics_world(new b2World(b2Vec2(0, -100.f)));
	return physics_world;
}

void Demo_Scene::keyboard(Event event){
	
	//Entrada de teclado
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		car->move(0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		car->move(-50000000.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		car->move(50000000.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && triggerTurret->activeBool)
		turret->move(-50.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && triggerTurret->activeBool)
		turret->move(50.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && triggerTurret->activeBool)
		platformMovil->move(+1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && triggerTurret->activeBool)
		platformMovil->move(-1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		car->truckTranslation(50);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		car->truckRotation(-1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		car->truckRotation(1);
	
	//Salida de teclado
	if (event.type == sf::Event::EventType::KeyReleased) {

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			car->move(0.f);
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			car->move(0.f);
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			turret->move(0.f);
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			turret->move(0.f);
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			car->truckTranslation(0);
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			car->truckRotation(0);
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			car->truckRotation(0);


		
	}


}

void Demo_Scene::resetScene(){

	//Se eliman todos los elementos y se crean otros iguales
	for (auto body : car->bodies) {
		phxWorld->DestroyBody(body.second);
	}
	for (auto body : turret->bodies) {
		phxWorld->DestroyBody(body.second);
	}
	for (auto body : triggerTurret->bodies) {
		phxWorld->DestroyBody(body.second);
	}
	for (auto body : platformMovil->bodies) {
		phxWorld->DestroyBody(body.second);
	}
	add_game_object("vehicle", shared_ptr< GameObject >(car = new Vehicle(*this)));
	add_game_object("turret", shared_ptr< GameObject >(turret = new Turret(*this)));
	add_game_object("triggerT", shared_ptr< GameObject >(triggerTurret = new Trigger(*this)));
	add_game_object("platform_Movil", shared_ptr< GameObject >(platformMovil = new Platform(*this)));
	finish = false;
}

void Demo_Scene::finishScene() {	
	finish = true;
	particles1.setEmitter(Vector2f(400, 250));
	particles2.setEmitter(Vector2f(600, 215));
}
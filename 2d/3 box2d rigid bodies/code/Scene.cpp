/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	07 / 02 / 2018
*/

#include "Scene.hpp"
#include "GameObject.hpp"

Scene::Scene(){
	widthScene = 1920;
	heightScene = 1080;
}

void Scene::add_game_object(const string & name, shared_ptr< GameObject > & game_object){	
	game_objects[name] = game_object;	
}




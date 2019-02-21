/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	07 / 02 / 2018
*/


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML\Graphics.hpp>
#include <Box2D/Box2D.h>


////////////////////////////////////////////////////////////
// NameSpaces
////////////////////////////////////////////////////////////
using namespace sf;
using namespace std;


class Scene;
class GameObject{

	////////////////////////////////////////////////////////////
	// Declaracion de variables			
	////////////////////////////////////////////////////////////
public:
	Scene & scene;
	map< string, b2Body * > bodies;
	map< string, b2Joint * > joints;

	map< string, b2RevoluteJoint * > r_joints;
	map< string, b2PrismaticJoint * > p_joints;

	////////////////////////////////////////////////////////////
	// Declaracion de funciones	
	////////////////////////////////////////////////////////////
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor DEFAULT de GameObject
	/// @param scene	Escena sobre la que trabajar
	////////////////////////////////////////////////////////////
	GameObject(Scene & scene);

	////////////////////////////////////////////////////////////
	/// \brief Invierte el sistema de coordenadas 
	/// @param	box2d_position		Vector de posicion en box2D
	/// @param	window_height		Altura de la ventana
	////////////////////////////////////////////////////////////
	inline sf::Vector2f GameObject::box2d_position_to_sfml_position(const b2Vec2 & box2d_position, float window_height);

	////////////////////////////////////////////////////////////
	/// \brief Pinta el objeto
	/// @param physics_world	Ref al mundo fisico
	/// @param renderer			Ventana 2D de pintado de la escena
	////////////////////////////////////////////////////////////
	void render(b2World & physics_world, sf::RenderWindow & renderer);

};
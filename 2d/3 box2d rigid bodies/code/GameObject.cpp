/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	07 / 02 / 2018
*/

#include "GameObject.hpp"
#include "Scene.hpp"

GameObject::GameObject(Scene & scene) : scene(scene) {
}	

//En Box2D las coordenadas Y crecen hacia arriba y en SFML crecen hacia abajo desde el borde superior.
//Esta función se encarga de convertir el sistema de coordenadas para que la escena no se vea invertida.
inline Vector2f GameObject::box2d_position_to_sfml_position(const b2Vec2 & box2d_position, float window_height)
{
	return Vector2f(box2d_position.x, window_height - box2d_position.y);
}

void GameObject::render(b2World & physics_world, RenderWindow & renderer){
	// Se cachea el alto de la ventana en una variable local:
	float window_height = (float)renderer.getSize().y;

	// Se recorre toda la lista de bodies de physics_world:
	for (b2Body * body = physics_world.GetBodyList(); body != nullptr; body = body->GetNext())
	{
		// Se obtiene el transform del body:
		const b2Transform & body_transform = body->GetTransform();

		// Se recorre la lista de fixtures del body:
		for (b2Fixture * fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
		{
			// Se obtiene el tipo de forma de la fixture:
			b2Shape::Type shape_type = fixture->GetShape()->GetType();

			if (shape_type == b2Shape::e_circle)
			{
				// Se crea un CircleShape a partir de los atributos de la forma de la fixture y del body:
				// En SFML el centro de un círculo no está en su position. Su position es la esquina superior izquierda
				// del cuadrado en el que está inscrito. Por eso a position se le resta el radio tanto en X como en Y.

				b2CircleShape * circle = dynamic_cast< b2CircleShape * >(fixture->GetShape());

				float  radius = circle->m_radius;
				b2Vec2 center = circle->m_p;

				CircleShape shape;

				shape.setPosition(box2d_position_to_sfml_position(b2Mul(body_transform, center), window_height) - Vector2f(radius, radius));
				shape.setFillColor(Color::Blue);
				shape.setRadius(radius);

				renderer.draw(shape);
			}
			else
				if (shape_type == b2Shape::e_edge)
				{
					// Se toman los dos vértices del segmento y se ajusta su posición usando el transform del body.
					// Los vértices resultantes se convierten y se ponen en un array para finalmente dibujar el segmento
					// que los une usando la sobrecarga del método draw() que permite dibujar primitivas de OpenGL a
					// partir de datos de vértices.

					b2EdgeShape * edge = dynamic_cast< b2EdgeShape * >(fixture->GetShape());

					b2Vec2 start = b2Mul(body_transform, edge->m_vertex1);
					b2Vec2 end = b2Mul(body_transform, edge->m_vertex2);

					Vertex line[] =
					{
						Vertex(box2d_position_to_sfml_position(start, window_height), Color::Green),
						Vertex(box2d_position_to_sfml_position(end, window_height), Color::Green),
					};

					renderer.draw(line, 2, Lines);
				}
				else
					if (shape_type == b2Shape::e_polygon)
					{
						// Se toma la forma poligonal de Box2D (siempre es convexa) y se crea a partir de sus vértices un
						// ConvexShape de SFML. Cada vértice de Box2D hay que transformarlo usando el transform del body.

						b2PolygonShape * box2d_polygon = dynamic_cast< b2PolygonShape * >(fixture->GetShape());
						ConvexShape       sfml_polygon;

						int number_of_vertices = box2d_polygon->GetVertexCount();

						sfml_polygon.setPointCount(number_of_vertices);
						sfml_polygon.setFillColor(Color::Yellow);

						for (int index = 0; index < number_of_vertices; index++)
						{
							sfml_polygon.setPoint
							(
								index,
								box2d_position_to_sfml_position(b2Mul(body_transform, box2d_polygon->GetVertex(index)), window_height)
							);
						}

						renderer.draw(sfml_polygon);
					}
		}
	}
}





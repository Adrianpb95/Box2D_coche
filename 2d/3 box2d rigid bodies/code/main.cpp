
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on april of 2016                                          *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Demo_Scene.hpp"


using namespace sf;
using namespace std;


int main (){
	
    RenderWindow window(VideoMode(1920, 1080), "Animation Examples: Box2D Rigid Bodies", Style::Titlebar | Style::Close, ContextSettings(32));

    window.setVerticalSyncEnabled (true);	

	Demo_Scene demo_scene;    

    bool running = true;

    Clock timer;
    float delta_time = 0.017f;          // ~60 fps

    do
    {
        timer.restart ();

        // Process window events:

        Event event;

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed)
            {
                running = false;
            }
        }

		//event:
		demo_scene.keyboard(event);

        // Update:
		demo_scene.update(delta_time);	
		

        // Render:
        window.clear ();
		demo_scene.render(window);       
        window.display ();

        delta_time = (delta_time + timer.getElapsedTime ().asSeconds ()) * 0.5f;
    }
    while (running);



	return EXIT_SUCCESS;

}

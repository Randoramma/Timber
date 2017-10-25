
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
using namespace sf;

int main(int, char const**)
{
    // Create the main window
    RenderWindow window(VideoMode(1920, 1080), "Timber !!!");
    
    // Set the Icon
    Image icon;
//    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
//        return EXIT_FAILURE;
//    }
    //window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // Load a sprite to display
    Texture texture;
    if (!texture.loadFromFile(resourcePath("graphics") + "background.png")) {
        return EXIT_FAILURE;
    }
    Sprite sprite(texture);
    
    // Create a graphical text to display
    Font font;
    if (!font.loadFromFile(resourcePath("fonts") + "KOMIKAP_.ttf")) {
        return EXIT_FAILURE;
    }
    Text text("TIMBER !!!", font, 50);
    text.setFillColor(Color::Black);
    
    // Load a music to play
    Music music;
    if (!music.openFromFile(resourcePath("sound") + "nice_music.ogg")) {
        return EXIT_FAILURE;
    }
    
    // Play the music
    music.play();
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == Event::Closed) {
                window.close();
            }
            
            // Escape pressed: exit
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
        }
        
        // Clear screen
        window.clear();
        
        // Draw the sprite
        window.draw(sprite);
        
        // Draw the string
        //window.draw(text);
        
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}




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
#include <sstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main(int, char const**)
{
    RenderWindow window(VideoMode(1920, 1080), "Timber !!!");
    
    Image icon;
    if (!icon.loadFromFile(resourcePath("graphics") + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    Texture textureBackground;
    if (!textureBackground.loadFromFile(resourcePath("graphics") + "background.png")) {
        return EXIT_FAILURE;
    }
    
    View view(FloatRect(0,0,1920, 1080));
    window.setView(view);
    
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);
    
    Texture treeTexture;
    treeTexture.loadFromFile(resourcePath("graphics") + "tree.png");
    Sprite treeSprite;
    treeSprite.setTexture(treeTexture);
    treeSprite.setPosition(810, 0);
    
    
    Texture beeTexture;
    beeTexture.loadFromFile(resourcePath("graphics") + "bee.png");
    Sprite beeSprite;
    beeSprite.setTexture(beeTexture);
    beeSprite.setPosition(0, 800);
    bool beeActive = false; // is bee moving?
    float beeSpeed = 0.0f; // speed which the bee can fly.
    
    Texture cloudTexture;
    cloudTexture.loadFromFile(resourcePath("graphics") + "cloud.png");
    
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    
    spriteCloud1.setTexture(cloudTexture);
    spriteCloud2.setTexture(cloudTexture);
    spriteCloud3.setTexture(cloudTexture);
    
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 150);
    spriteCloud3.setPosition(0, 250);
    
    bool cloud1Active = false; // is the cloud visible
    bool cloud2Active = false; // is the cloud visible
    bool cloud3Active = false; // is the cloud visible
    
    float cloud1Speed = 0.0f; // current cloud1 speed
    float cloud2Speed = 0.0f; // current cloud2 speed
    float cloud3Speed = 0.0f; // current cloud3 speed
    // Score
    
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;
    
    Font font;
    if (!font.loadFromFile(resourcePath("fonts") + "KOMIKAP_.ttf")) {
        return EXIT_FAILURE;
    }
    
    messageText.setFont(font);
    scoreText.setFont(font);
    
    messageText.setString("Press enter to start");
    scoreText.setString("Score = 0");
    
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    
    //    Text text("TIMBER !!!", font, 50);
    //    text.setFillColor(Color::Black);
    
    // Place Text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width /2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920/2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);
    
    Music music;
    if (!music.openFromFile(resourcePath("sound") + "nice_music.ogg")) {
        return EXIT_FAILURE;
    }
    
    Clock clock;
    
    // Time bar functionality
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920/2)-timeBarStartWidth/2, 980);
    Time gameTimeGoal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
    
    bool paused = true;
    /***************************
     Prepare Branches
     ****************************/
    
    Texture textureBranch;
    textureBranch.loadFromFile(resourcePath("graphics") + "branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        // set the origin to center so we can spin it without changing its position.
        branches[i].setOrigin(220, 20);
    }
    
    /***************************
     Build the Player graphic
     ****************************/
    
    
    Texture texturePlayer;
    texturePlayer.loadFromFile(resourcePath("graphics") + "player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    
    side playerSide = side::LEFT;  // player starts on L side.
    
    Texture textureRIP;
    textureRIP.loadFromFile(resourcePath("graphics") + "rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);
    
    Texture textureAxe;
    textureAxe.loadFromFile(resourcePath("graphics") + "axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;
    
    Texture textureLog;
    textureLog.loadFromFile(resourcePath("graphics") + "log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);
    
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    
                               
//    updateBranches(1);
//    updateBranches(2);
//    updateBranches(3);
//    updateBranches(4);
//    updateBranches(5);
    
    /***************************
     Handle User Input
     ****************************/
    
    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window.close();
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            music.play();
            paused = false;
            score = 0;
            timeRemaining = 5;
        }
        
        /***************************
         
         Update the Scene
         
         ***************************/
        
        if (!paused) {
            // upadte TimeBar based on time
            Time dt = clock.restart(); // restart clock to measure each frames duration.
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            
            // monitor time remaining
            if (timeRemaining <= 0.0f)
            {
                paused = true;
                music.stop();
                messageText.setString("Out of Time!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080/2.0f);
            }
            
            // bee controls
            if (!beeActive) {
                // speed of the bee
                srand((int)time(0) * 10);
                beeSpeed = ((rand() %200) + 200);
                
                // height of the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                beeSprite.setPosition(2000, height);
                beeActive = true;
            } else {
                beeSprite.setPosition(beeSprite.getPosition().x - (beeSpeed * dt.asSeconds()), beeSprite.getPosition().y);
                if (beeSprite.getPosition().x < -100) {
                    beeActive = false;
                }
            }
            
            // cloud 1 controls
            if (!cloud1Active) {
                // speed of the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);
                
                // height of the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150) - 150;
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }  else {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
                if (spriteCloud1.getPosition().x > 1920) {
                    cloud1Active = false;
                }
            }
            
            // cloud 2 controls
            if (!cloud2Active) {
                // speed of the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);
                
                // height of the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }  else {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
                if (spriteCloud2.getPosition().x > 1920) {
                    cloud2Active = false;
                }
            }
            
            // cloud 3 controls
            if (!cloud3Active) {
                // speed of the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);
                
                // height of the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }else {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
                if (spriteCloud3.getPosition().x > 1920) {
                    cloud3Active = false;
                }
            }
            
            /***************************
             Update Score Text
             ****************************/
            
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());
            
            /***************************
             Update Banch sprites
             ****************************/
            
            for (int i = 0; i < NUM_BRANCHES; i ++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height); // move sprite to the left side.
                    branches[i].setRotation(180); // flip the sprite around.
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height); // move sprite to the right side.
                    branches[i].setRotation(0); // set rotation to normal.
                }
                else
                {
                    branches[i].setPosition(3000, height); // hide the branches.
                }
                    
                
            }
            
        } // paused
        
        /***************************
         
         Draw the Scene
         
         ***************************/
        
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }
        window.draw(treeSprite);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP); 
        window.draw(timeBar);
        window.draw(beeSprite);
        if (paused)
        {
            window.draw(messageText);
        }
        window.draw(scoreText);
        window.display();
    }
    return EXIT_SUCCESS;
}

void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES-1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }
    // spawn new branch
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r) {
        case 0:
            branchPositions[0] = side:: LEFT;
            break;
        case 1:
            branchPositions[0] = side:: RIGHT;
            break;
        default: //2-4
            branchPositions[0] = side:: NONE;
            break;
    }
}





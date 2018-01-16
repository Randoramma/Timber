
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
// FPS Rectangle Wrong Size
// Consider cloud etc. background objects
// Move to multiple files or functions
// Speed up code by putting sstream in a block, no need to update score 100s of times a second
// Configure FPS display, no need to update too often, once every 100 frames is fine
// Move trees, foreground, background
// Personalize black, transparent rectangle behind score
// Personalize FPS counter black rectangle, with some transparency
// Make clouds more efficient with arrays, more clouds
// Consider all if loads replaced with try and catch
// make sure order is same for graphics and drawing
// Add menu:
//     Play
//     High Scores
//     Help
//     Quit
// Flip axe for left side
// Create and add personal graphics
// Replace numbers with constants
// Chop sound after timeout

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include <sstream>

enum class Side {LEFT, RIGHT, NONE}; // player/branch location

void updateBranches(int NUMBER_OF_BRANCHES, sf::Sprite branches[], Side branchPositions[], int seed);

int main()
{
  sf::VideoMode vm(1920, 1080); // create a video mode object
  sf::RenderWindow window(vm, "Timber !!!"); // create and open a window for the game

  // Bee Variables (OBJECT?)
  bool bee_active = false;
  float bee_speed = 0.0f;

  // Clouds (OBJECTS?)
  const int NUMBER_OF_CLOUDS = 6;

  int cloudSpeeds[NUMBER_OF_CLOUDS];
  bool cloudsActive[NUMBER_OF_CLOUDS];

  // Branch Variables
  const int NUMBER_OF_BRANCHES = 6;
    
    sf::Music music;
    if (!music.openFromFile(resourcePath("sound") + "nice_music.ogg")) {
        return EXIT_FAILURE;
    }

  Side branchPositions[NUMBER_OF_BRANCHES];

  // Player Variables
  Side playerSide = Side::LEFT; // player starts on left, CONSIDER changing side enum
  const float PLAYER_HEIGHT = 720;
  const float PLAYER_POSITION_LEFT = 580;
  const float PLAYER_POSITION_RIGHT = 1200;

  // Line Axe Up to Tree
  const float AXE_HEIGHT = 830;
  const float AXE_POSITION_LEFT = 700;
  const float AXE_POSITION_RIGHT = 1075;

  // Log Variables
  bool log_active = false;
  float logSpeedX = 1000;
  float logSpeedY = -1500;

  // Declare General Game Variables
  int score = 0;
  bool paused = true; // games starts paused
  bool accept_input = false; // control player input
  int textLastDrawn = 0; // limit drawing/updating text

  // Declare Textures
  sf::Texture textureBackground;
  sf::Texture textureTree;
  sf::Texture textureTree2;
  sf::Texture textureBee;
  sf::Texture textureCloud;
  sf::Texture textureBranch;
  sf::Texture texturePlayer;
  sf::Texture textureGravestone;
  sf::Texture textureAxe;
  sf::Texture textureLog;

  // Declare Sprites
  sf::Sprite spriteBackground;
  sf::Sprite spriteTree;
  sf::Sprite spriteTree2;
  sf::Sprite spriteTree3;
  sf::Sprite spriteTree4;
  sf::Sprite spriteTree5;
  sf::Sprite spriteTree6;
  sf::Sprite spriteBee;
  sf::Sprite clouds[NUMBER_OF_CLOUDS];
  sf::Sprite branches[NUMBER_OF_BRANCHES];
  sf::Sprite spritePlayer;
  sf::Sprite spriteGravestone;
  sf::Sprite spriteAxe;
  sf::Sprite spriteLog;

  // Declare Fonts
  sf::Font font;

  // Declare Texts
  sf::Text messageText;
  sf::Text scoreText;
  sf::Text frameRateText;

  // Declare Rectangle Variables
  sf::FloatRect textRect;

  sf::RectangleShape scoreRectangle;
  sf::RectangleShape frameRateRectangle;

  // Declare Sound Variables
  sf::SoundBuffer chopBuffer;
  sf::SoundBuffer deathBuffer;
  sf::SoundBuffer ootBuffer;

  sf::Sound chop;
  sf::Sound death;
  sf::Sound outOfTime;

  // Declare Time Variables
  sf::Clock clock; // measure time

  // Time Bar (OBJECT?)
  sf::RectangleShape timeBar;
  float time_bar_start_width = 400;
  float time_bar_height = 80;

  float time_remaining = 6.0f;
  float time_bar_width_per_second = time_bar_start_width / time_remaining; // sets timeBar width based on time remaining

  // Load Graphics
  if (!textureBackground.loadFromFile(resourcePath("graphics") + "background.png")) // load a graphic into the texture
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!textureTree.loadFromFile(resourcePath("graphics") + "tree.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!textureTree2.loadFromFile(resourcePath("graphics") + "tree2.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!textureBee.loadFromFile(resourcePath("graphics") + "bee.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!textureCloud.loadFromFile(resourcePath("graphics") + "cloud.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!textureBranch.loadFromFile(resourcePath("graphics") + "branch.png")) 
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!texturePlayer.loadFromFile(resourcePath("graphics") + "player.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!textureGravestone.loadFromFile(resourcePath("graphics") + "rip.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!textureAxe.loadFromFile(resourcePath("graphics") + "axe.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }
  if (!textureLog.loadFromFile(resourcePath("graphics") + "log.png"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  // Load Fonts
  if (!font.loadFromFile(resourcePath("fonts") + "KOMIKAP_.ttf"))
    {
      std::cerr << "Failed to load font.\n";
      exit(1);
    }

  // Load Sounds
  if (!chopBuffer.loadFromFile(resourcePath("sound") +"chop.wav"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!deathBuffer.loadFromFile(resourcePath("sound") + "death.wav"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  if (!ootBuffer.loadFromFile(resourcePath("sound") + "out_of_time.wav"))
    {
      std::cerr << "Failed to load image.\n";
      exit(1);
    }

  // Set Textures and Positions
  spriteBackground.setTexture(textureBackground);
  spriteBackground.setPosition(0, 0);

  spriteTree.setTexture(textureTree);
  spriteTree.setPosition(810, 0);

  spriteTree2.setTexture(textureTree2);
  spriteTree2.setPosition(20, 0);

  spriteTree3.setTexture(textureTree2);
  spriteTree3.setPosition(375, -350);

  spriteTree4.setTexture(textureTree2);
  spriteTree4.setPosition(1275, -450);

  spriteTree5.setTexture(textureTree2);
  spriteTree5.setPosition(1375, -175);

  spriteTree6.setTexture(textureTree2);
  spriteTree6.setPosition(1770, 0);

  spriteBee.setTexture(textureBee);
  spriteBee.setPosition(0, 700);

  spritePlayer.setTexture(texturePlayer);
  spritePlayer.setPosition(PLAYER_POSITION_LEFT, PLAYER_HEIGHT);

  spriteGravestone.setTexture(textureGravestone);
  spriteGravestone.setPosition(600, 860);

  spriteAxe.setTexture(textureAxe);
  spriteAxe.setOrigin({spriteAxe.getLocalBounds().width, 0}); // set origin to center
  spriteAxe.setScale({-1, 1}); // flip axe, texture made for right position
  spriteAxe.setPosition(AXE_POSITION_LEFT, AXE_HEIGHT);

  spriteLog.setTexture(textureLog);
  spriteLog.setPosition(810, 720);

  timeBar.setSize(sf::Vector2f(time_bar_start_width, time_bar_height)); // sf::Vector2f hold 2 float variables
  timeBar.setFillColor(sf::Color::Red);
  timeBar.setPosition((1920 / 2) - (time_bar_start_width / 2), 980); // horizontally centers timeBar

  // Set Texture and Set Clouds
  for (int i = 0; i < NUMBER_OF_CLOUDS; i++)
    {
      clouds[i].setTexture(textureCloud);
      clouds[i].setPosition(-300, i * 150);
      cloudsActive[i] = false;
      cloudSpeeds[i] = 0;
    }

  // Set Texture and Set Branches
  for (int i = 0; i < NUMBER_OF_BRANCHES; i++)
    {
      branches[i].setTexture(textureBranch);
      branches[i].setPosition(-2000, -2000); // hide off-screen
      branches[i].setOrigin(220, 20); // set sprite origin to center, can spin without changing its position
    }

  // Set Texts
  messageText.setFont(font);
  messageText.setCharacterSize(75);
  messageText.setFillColor(sf::Color::White);
  messageText.setString("Press Enter to Start!");

  // Position Text in Boxes
  textRect = messageText.getLocalBounds(); // get coordinates of rectangle that wraps messageText
  messageText.setOrigin(textRect.left + (textRect.width / 2.0f),
                        textRect.top + (textRect.height / 2.0f)); // moves origin from top-left to center of rectangle
  messageText.setPosition(1920 / 2.0f, 1080 / 2.0f); // center of screen, given that size

  scoreText.setFont(font);
  scoreText.setCharacterSize(100);
  scoreText.setFillColor(sf::Color::White);
  scoreText.setPosition(20, 20);
  scoreText.setString("Score = 0");

  frameRateText.setFont(font);
  frameRateText.setFillColor(sf::Color::White);
  frameRateText.setCharacterSize(50);
  frameRateText.setPosition(1550, 20);

  // Backgrounds for the text
  scoreRectangle.setFillColor(sf::Color(0, 0, 0, 150));
  scoreRectangle.setSize(sf::Vector2f(600,105));
  scoreRectangle.setPosition(0,30);

  frameRateRectangle.setFillColor(sf::Color(0, 0, 0, 150));
  frameRateRectangle.setSize(sf::Vector2f(400, 50));
  frameRateRectangle.setPosition(1525, 30);

  // Set Sounds
  chop.setBuffer(chopBuffer);
  death.setBuffer(deathBuffer);
  outOfTime.setBuffer(ootBuffer);

  while (window.isOpen())
    {
      /* *********************************************************************
         *  Handles player input
         * *******************************************************************
      */
      sf::Event event; // for key release detection

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
          window.close();
        }

      // Start game
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && paused == true)
        {

          paused = false;

          // Reset time and score
          score = 0;
          time_remaining = 6.0f;

          for (int i = 1; i < NUMBER_OF_BRANCHES; i++)
            {
              branchPositions[i] = Side::NONE; // clear branches
            }

          spriteGravestone.setPosition(675, 2000); // hide gravestone
          spritePlayer.setPosition(580, 720); // position player on left

          accept_input = true;
        }

      if (accept_input)
        {

          // Left Cursor Key
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
              playerSide = Side::LEFT;
              score++;

              time_remaining += (3 / score) + 0.15f; // higher score gives less time

              spriteAxe.setOrigin(spriteAxe.getLocalBounds().width, 0); // set origin to center
              spriteAxe.setScale(-1, 1); // flip axe

              spritePlayer.setPosition(PLAYER_POSITION_LEFT, PLAYER_HEIGHT); // change player position left

              spriteAxe.setPosition(AXE_POSITION_LEFT, AXE_HEIGHT); // move axe to left

              updateBranches(NUMBER_OF_BRANCHES, branches, branchPositions, score); // update branches

              // set log flying to the left
              spriteLog.setPosition(810, 720);
              logSpeedX = 5000; // zooms to the right
              log_active = true;

              accept_input = false;

              chop.play(); // play chop sound
            }

          // Right Cursor Key
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
              playerSide = Side::RIGHT; // make sure player on right
              score++;

              time_remaining += (3 / score) + 0.15f; // add to remaining time, higher score gives less time

              spriteAxe.setOrigin(spriteAxe.getLocalBounds().width, 0); // set origin to center
              spriteAxe.setScale(1, 1); // flip axe
              spriteAxe.setOrigin(0, 0);

              spritePlayer.setPosition(PLAYER_POSITION_RIGHT, PLAYER_HEIGHT); // change player position right

              spriteAxe.setPosition(AXE_POSITION_RIGHT, AXE_HEIGHT); // move axe to right

              updateBranches(NUMBER_OF_BRANCHES, branches, branchPositions, score); // update branches

              // set log flying to the left
              spriteLog.setPosition(810, 720);
              logSpeedX = -5000; // log zooms to left
              log_active = true;

              accept_input = false;

              chop.play(); // play chop sound
            }
        }

      while (window.pollEvent(event))
        {
          if (event.type == sf::Event::KeyReleased && !paused)
            {
              accept_input = true; // listen for keys

              spriteAxe.setPosition(2000, spriteAxe.getPosition().y); // place axe off screen on key release
            }
        }

      /* ******************************************************************
         * Update the Scene
         * *******************************************************************
      */
      if (!paused)
        {
          sf::Time deltaTime = clock.restart(); // measure time, dt = time elapsed sinced last start of clock

          time_remaining -= deltaTime.asSeconds(); // subtract from the remaining time
          timeBar.setSize(sf::Vector2f(time_bar_width_per_second * time_remaining, time_bar_height)); // adjust size of timeBar

          // Time Runs Out
          if (time_remaining <= 0.0f)
            {
              paused = true;

              messageText.setString("Out of time!");

              // reposition text based on new size
              sf::FloatRect textRect = messageText.getLocalBounds();
              messageText.setOrigin(textRect.left + (textRect.width / 2.0f),
                                    textRect.top + (textRect.height / 2.0f));
              messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

              outOfTime.play(); // play out of time sound
            }

          // setup bee
          if (!bee_active)
            {
              // bee speed
              srand((int) time(0));
              bee_speed = (rand() % 200) + 200; // random number between 199 and 399 px/sec

              // bee height
              srand((int) time(0));
              float height = (rand() % 500) + 500; // random number between 499 and 999
              spriteBee.setPosition(2000, height);
              bee_active = true;
            }

          // move bee
          else
            {
              // moves from right to left
              // lower fps, faster bee, higher fps, slower bee (appears same speed),
              // .asSeconds returns a fraction of 1
              spriteBee.setPosition(spriteBee.getPosition().x - (bee_speed * deltaTime.asSeconds()),
                                    spriteBee.getPosition().y);

              // bee is off left edge of screen
              if (spriteBee.getPosition().x < -100)
                {
                  bee_active = false; // required for new bee next frame
                }
            }

          // Manage the clouds with arrays
          for (int i = 0; i < NUMBER_OF_CLOUDS; i++)
            {
              if (!cloudsActive[i])
                {
                  // How fast is the cloud
                  srand((int)time(0) * i);
                  cloudSpeeds[i] = (rand() % 200);

                  // How high is the cloud
                  srand((int)time(0) * i);
                  float height = (rand() % 150);
                  clouds[i].setPosition(-200, height);
                  cloudsActive[i] = true;

                }
              else
                {
                  // Set the new position
                  clouds[i].setPosition(
                        clouds[i].getPosition().x +
                        (cloudSpeeds[i] * deltaTime.asSeconds()),
                        clouds[i].getPosition().y);

                  // Has the cloud reached the right hand edge of the screen?
                  if (clouds[i].getPosition().x > 1920)
                    {
                      // Set it up ready to be a whole new cloud next frame
                      cloudsActive[i] = false;
                    }

                }

            }
          // End Enhanced

          /*
          // cloud 1
          if (!cloud1_active)
            {
              // cloud speed
              srand((int) time(0) * 10); // * 10 b/c need discrepancy
              cloud1_speed = (rand() % 200);

              srand((int) time(0) * 10);
              float height = (rand() % 150);
              spriteCloud1.setPosition(-200, height);
              cloud1_active = true;
            }

          else
            {
              // moves from left to right
              spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1_speed * deltaTime.asSeconds()),
                                       spriteCloud1.getPosition().y);

              // cloud is off screen
              if (spriteCloud1.getPosition().x > 1920)
                {
                  cloud1_active = false; // setup new cloud
                }
            }

          // cloud 2
          if (!cloud2_active)
            {
              // cloud speed
              srand((int) time(0) * 20); // * 10 b/c need discrepancy
              cloud2_speed = (rand() % 200);

              srand((int) time(0) * 20);
              float height = (rand() % 150);
              spriteCloud2.setPosition(-200, height);
              cloud2_active = true;
            }

          else
            {
              // moves from left to right
              spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2_speed * deltaTime.asSeconds()),
                                       spriteCloud2.getPosition().y);

              // cloud is off screen
              if (spriteCloud2.getPosition().x > 1920)
                {
                  cloud2_active = false; // setup new cloud
                }
            }

          // cloud 3
          if (!cloud3_active)
            {
              // cloud speed
              srand((int) time(0) * 30); // * 10 b/c need discrepancy
              cloud3_speed = (rand() % 200);

              srand((int) time(0) * 30);
              float height = (rand() % 150);
              spriteCloud3.setPosition(-200, height);
              cloud3_active = true;
            }

          else
            {
              // moves from left to right
              spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3_speed * deltaTime.asSeconds()),
                                       spriteCloud3.getPosition().y);

              // cloud is off screen
              if (spriteCloud3.getPosition().x > 1920)
                {
                  cloud3_active = false; // setup new cloud
                }
            }
           */

          /*
          //  score text
          std::stringstream sstream;
          sstream << "Score = " << score;
          scoreText.setString(sstream.str());
          */

          // Draw the score and the frame rate once every 100 frames
          textLastDrawn++;
          if (textLastDrawn == 100) {
              //  the score text
              std::stringstream ss;
              ss << "Score = " << score;
              scoreText.setString(ss.str());

              // Draw the fps
              std::stringstream ss2;
              ss2 << "FPS = " << 1/deltaTime.asSeconds();
              frameRateText.setString(ss2.str());
              textLastDrawn = 0;
            }
          // End Enhanced Code

          //  branch sprites
          for (int i = 0; i < NUMBER_OF_BRANCHES; i++)
            {
              float height = i * 150; // branches are 150px apart

              if (branchPositions[i] == Side::LEFT)
                {
                  branches[i].setPosition(610, height); // move to the left side
                  branches[i].setOrigin(220, 40);
                  branches[i].setRotation(180); // flip sprite
                }

              else if (branchPositions[i] == Side::RIGHT)
                {
                  branches[i].setPosition(1330, height); // move to the right side
                  branches[i].setOrigin(220, 40);
                  branches[i].setRotation(0); // flip sprite to normal
                }

              else
                {
                  branches[i].setPosition(3000, height); // hide branch
                }
            }

          // flying log
          if (log_active)
            {
              spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * deltaTime.asSeconds()),
                                    spriteLog.getPosition().y + (logSpeedY * deltaTime.asSeconds()));

              // log reaches off screen
              if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                  // setup new log
                  log_active = false;
                  spriteLog.setPosition(810, 720);
                }
            }

          // Player Squashed
          if (branchPositions[5] == playerSide)
            {
              // death
              paused = true;
              accept_input = false;

              spriteGravestone.setPosition(525, 760); // move gravestone
              spritePlayer.setPosition(2000, 660); // hide player

              messageText.setString("SQUISHED!!!"); // change message

              // center message on screen
              sf::FloatRect textRect = messageText.getLocalBounds();

              messageText.setOrigin(textRect.left + (textRect.width + 2.0f),
                                    textRect.top + (textRect.height / 2.0f));

              messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

              death.play();
            }
        } // End if (!paused)

      /* ********************************************************************
         * Draw the scene, Draws in Layered Order
         * ******************************************************************
        */

      window.clear(); // clear everything from the last frame

      window.draw(spriteBackground); // draw our game scene here

      // Enhanced version
      for (int i = 0; i < NUMBER_OF_CLOUDS; i++)
        {
          window.draw(clouds[i]);
        }

      window.draw(spriteTree2);
      window.draw(spriteTree3);
      window.draw(spriteTree4);
      window.draw(spriteTree5);
      window.draw(spriteTree6);
      // End Enhanced Code

      
      // draw clouds
        for (int i = 0; i < NUMBER_OF_CLOUDS; i++)
        {
            window.draw(clouds[i]);
        }

      

      for (int i = 0; i < NUMBER_OF_BRANCHES; i++)
        {
          window.draw(branches[i]); // draw branches
        }

      window.draw(spriteTree); // draw tree
      window.draw(spritePlayer); // draw player
      window.draw(spriteAxe); // draw axe
      window.draw(spriteLog); // draw log
      window.draw(spriteGravestone); // draw gravestone
      window.draw(scoreRectangle); // background for score // Enhanced
      window.draw(frameRateRectangle); // background for FPS // Enhanced
      window.draw(spriteBee); // draw bee
      window.draw(scoreText); // draw score
      window.draw(frameRateText); // draw FPS // Enhanced
      window.draw(timeBar); // after tree so it is visible

      if (paused)
        {
          window.draw(messageText); // draw message
        }

      window.display(); // show everything we just drew
    }

  return 0;
}

void updateBranches(int NUMBER_OF_BRANCHES, sf::Sprite branches[], Side branchPositions[], int seed)
{
  for (int j = NUMBER_OF_BRANCHES - 1; j > 0; j--)
    {
      branchPositions[j] = branchPositions[j - 1]; // move branches down by 1
    }

  // Spawn a new branch at position 0

  srand((int) time(0) + seed);
  int random = (rand() % 5);

  switch (random)
    {
    case 0:
      branchPositions[0] = Side::LEFT;
      break;
    case 1:
      branchPositions[0] = Side::RIGHT;
      break;
    default:
      branchPositions[0] = Side::NONE;
    }
}


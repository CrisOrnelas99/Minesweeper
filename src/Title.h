//
// Created by willw on 04/15/2025.

//
#ifndef TITLE_H
#define TITLE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Demolition.h"
#include "Difficulty.h"
#include "Effects.h"

inline void title()
{
    sf :: RenderWindow title;
    title.create(sf :: VideoMode({1920,1080}), "MINESWEEPER", sf :: State :: Fullscreen);
    title.setFramerateLimit(60);
    //initialize screen object
    bool mouseHoverPlay=false,mouseHoverDemolition=false,mouseHoverExit=false;
    while (title.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std :: optional event = title.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf :: Event :: Closed>())
                title.close();
            //closes the window when the ESC key is pressed
            else if (const auto* keyPressed = event->getIf<sf :: Event :: KeyPressed>())
            {
                if (keyPressed->scancode == sf :: Keyboard :: Scancode :: Escape)
                    title.close();
            }
            //sets up invisible buttons that the user can click for the desired action
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    if (sf::Mouse::getPosition(title).x >=751 && sf::Mouse::getPosition(title).x <=1175 && sf::Mouse::getPosition(title).y >=680 && sf::Mouse::getPosition(title).y <=780)
                    {
                        title.close();
                        //difficulty window opens
                        difficulty();
                    }
                    else if (sf::Mouse::getPosition(title).x >=751 && sf::Mouse::getPosition(title).x <=1175 && sf::Mouse::getPosition(title).y >=817 && sf::Mouse::getPosition(title).y <=910)
                    {
                        title.close();
                        //demolition window opens
                        Demolition();
                    }
                    else if (sf::Mouse::getPosition(title).x >=751 && sf::Mouse::getPosition(title).x <=1175 && sf::Mouse::getPosition(title).y >=953 && sf::Mouse::getPosition(title).y <=1044)
                    {
                        title.close();
                    }
                }
            }
            //finds position of mouse whenever it is moved in order to highlight buttons
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(title);
                if ((localPosition.x >=751 && localPosition.x <=1175) && (localPosition.y >= 680 && localPosition.y <=780))
                {
                    loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new_play.png");
                    title.display();
                    mouseHoverPlay = true;
                }
                else if ((localPosition.x >= 752 && localPosition.x <= 1178) && (localPosition.y >= 817 && localPosition.y <= 910))
                {
                    loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new_demolition.png");
                    title.display();
                    mouseHoverDemolition = true;
                }
                else if ((localPosition.x >= 753 && localPosition.x <= 1177) && (localPosition.y >= 953 && localPosition.y <= 1044))
                {
                    loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new_exit.png");
                    title.display();
                    mouseHoverExit = true;
                }
                else
                {
                    loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new.png");
                    title.display();
                    mouseHoverPlay=false,mouseHoverDemolition=false,mouseHoverExit=false;
                }
            }
        }
        //series of if statements that keep button highlighted until the mouse is moved off of the button
        if (mouseHoverPlay==true && mouseHoverDemolition==false && mouseHoverExit==false)
        {
            loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new_play.png");
            title.display();
        }


        else if (mouseHoverDemolition==true && mouseHoverExit==false && mouseHoverPlay==false)
        {
            loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new_demolition.png");
            title.display();
        }


        else if (mouseHoverExit==true && mouseHoverPlay==false && mouseHoverDemolition==false)
        {
            loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new_exit.png");
            title.display();
        }

        else
        {
            loadScreen(title, "../../src/imagesAudio/Minesweeper_title_screen_new.png");
            title.display();
        }
    }
}
#endif //TITLE_H
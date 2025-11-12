//
// Created by willw on 04/08/2025.
//
#ifndef DEMOLITION_H
#define DEMOLITION_H
#include "Effects.h"
void title();

inline void Demolition()
{
    int rows;
    int columns;
    //determine whether a square has a mine or not
    bool grid[20][20]={false};
    //determine whether a user has selected a square or not
    bool selected[20][20]={false};
    //determine whether a selected mine has been scored or not
    bool scored[20][20]={false};
    int gameOver=0;
    int mines=0;
    int score=0;
    int lives=5;
    int clicks=0;
    std::stringstream scoreStream;
    scoreStream << score;
    std::stringstream livesStream;
    livesStream << lives;
    //places 60 mines
    while (mines<60)
    {
        //randomly selects a spot on the grid and places a mine if there is not one already
        rows=rand()%20;
        columns=rand()%20;
        if (!grid[rows][columns])
        {
            grid[rows][columns]=true;
            mines++;
        }
    }
    sf :: RenderWindow demolition;
    sf::Font font("../../src/CascadiaCode.ttf");
    sf::Text Score(font);
    Score.setCharacterSize(55);
    Score.setPosition({849.f,110.f});
    Score.setFillColor(sf::Color::Black);
    Score.setString(scoreStream.str());
    sf::Text LivesLeft(font);
    LivesLeft.setCharacterSize(55);
    LivesLeft.setPosition({1040.f,110.f});
    LivesLeft.setFillColor(sf::Color::Black);
    LivesLeft.setString("Lives:");
    sf::Text Lives(font);
    Lives.setCharacterSize(50);
    Lives.setPosition({1250.f,115.f});
    Lives.setFillColor(sf::Color::Black);
    Lives.setString(livesStream.str());
    demolition.create(sf :: VideoMode({1920,1080}), "MINESWEEPER", sf :: State :: Fullscreen);
    demolition.setFramerateLimit(60);
    //create effects manager
    Effects effects;
    sf::Clock clk;  //sfml stopWatch
    bool didExplode=false;

    while (demolition.isOpen())
    {
        float secsSinceLastFrame = clk.restart().asSeconds();

        while (const std :: optional event = demolition.pollEvent())
        {
            //ends program if the user closes the window
            if (event->is<sf :: Event :: Closed>())
                demolition.close();
            //closes window if the ESC key is pressed
            else if (const auto* keyPressed = event->getIf<sf :: Event :: KeyPressed>())
            {
                if (keyPressed->scancode == sf :: Keyboard :: Scancode :: Escape)
                    demolition.close();
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                //checks if the user has left-clicked on the grid or on one of the buttons
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    if (sf::Mouse::getPosition(demolition).x >=17 && sf::Mouse::getPosition(demolition).x <=189 && sf::Mouse::getPosition(demolition).y >=14 && sf::Mouse::getPosition(demolition).y <=89)
                    {
                        demolition.close();
                        //title window is reopened
                        title();
                    }
                    else if (sf::Mouse::getPosition(demolition).x >=1731 && sf::Mouse::getPosition(demolition).x <=1901 && sf::Mouse::getPosition(demolition).y >=14 && sf::Mouse::getPosition(demolition).y <=89)
                    {
                        demolition.close();
                        //demolition window is reopened
                        Demolition();
                    }
                    for (rows=0; rows<20; rows++)
                    {
                        for (columns=0; columns<20; columns++)
                        {
                            if (gameOver==0 && clicks!=0 && sf::Mouse::getPosition(demolition).x >=610+(rows*35) && sf::Mouse::getPosition(demolition).x <=610+(1+rows)*35 && sf::Mouse::getPosition(demolition).y >=190+(columns*35) && sf::Mouse::getPosition(demolition).y <=190+(1+columns)*35)
                            {
                                //if the square has not already been selected and does not have a mine then take away a life
                                if (!grid[rows][columns] && !selected[rows][columns])
                                {
                                    lives-=1;
                                    livesStream.str(std::string());
                                    livesStream << lives;
                                    Lives.setString(livesStream.str());

                                    if (lives<=0 && !didExplode)
                                    {
                                        //explode in center
                                        const sf::Vector2f center = demolition.getView().getCenter();
                                        //trigger effects
                                        effects.spawn<ExplosionSoundEffect>("../../src/imagesAudio/explosion.wav");
                                        effects.spawn<RingWaveEffect>(center, 0.f, 600.f, 0.1f, sf::Color(255,80,30));
                                        effects.spawn<ScreenFlashEffect>(sf::Color(255,255,255,220), 0.05f);
                                        didExplode=true;
                                    }
                                }
                                selected[rows][columns]=true;
                            }
                            //makes first click of the game not take away a life
                            else if (gameOver==0 && clicks==0 && sf::Mouse::getPosition(demolition).x >=610+(rows*35) && sf::Mouse::getPosition(demolition).x <=610+(1+rows)*35 && sf::Mouse::getPosition(demolition).y >=190+(columns*35) && sf::Mouse::getPosition(demolition).y <=190+(1+columns)*35)
                            {
                                selected[rows][columns]=true;
                                clicks++;
                            }
                        }
                    }
                }
            }
        }
        //update effects
        effects.update(secsSinceLastFrame);

        loadScreen(demolition, "../../src/imagesAudio/Minesweeper_demolition.png");

        for (rows=0; rows<20; rows++)
        {
            for (columns=0; columns<20; columns++)
            {
                //if the square is not a mine
                if (grid[rows][columns] == false)
                {
                    int mineCount=countMines(grid,rows,columns);
                    //switch case for displaying number squares
                    switch (mineCount)
                    {
                        case 1:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum1Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        case 2:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum2Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        case 3:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum3Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        case 4:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum4Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        case 5:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum5Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        case 6:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum6Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        case 7:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum7Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        case 8:
                        {
                            drawTile(demolition, "../../src/imagesAudio/minNum8Medium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            break;
                        }
                        default:
                        {
                            drawTile(demolition, "../../src/imagesAudio/emptySelectedSquareMedium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                            //if an empty square is selected, the floodDemolition function is called to select other safe squares near it
                            if (selected[rows][columns])
                            {
                                floodDemolition(grid, selected, rows, columns);
                            }
                            break;
                        }
                    }
                }
                //if the square is a mine
                if (grid[rows][columns] == true)
                {
                    drawTile(demolition, "../../src/imagesAudio/mineMedium.png",     35, 35, 610.f + (35*rows), 190.f + (35*columns));
                    //if the square has been selected and has not been scored yet then add 500 points
                    if (selected[rows][columns] && !scored[rows][columns])
                    {
                        scored[rows][columns] = true;
                        score+=500;
                        scoreStream.str(std::string());
                        scoreStream << score;
                        Score.setString(scoreStream.str());
                        //floodDemolition is used to select safe squares adjacent to the mine to give player more info
                        floodDemolition(grid, selected, rows, columns);
                    }
                }
            }
        }

        for (rows=0; rows<20; rows++)
        {
            for (columns=0; columns<20; columns++)
            {
                //if the square is not selected then redraw it over the generated board
                if (selected[rows][columns] == false)
                {
                    drawTile(demolition, "../../src/imagesAudio/emptySquareMedium.png", 35, 35, 610.f + (35*rows), 190.f + (35*columns));
                }
            }
        }
        for (rows=0; rows<20; rows++)
        {
            for (columns=0; columns<20; columns++)
            {
                //if the user presses a safe square and is out of lives the game ends
                if (selected[rows][columns] == true && grid[rows][columns] == false && lives<=0)
                    gameOver=1;
            }
        }
        if (gameOver != 1)
        {
            for (rows=0; rows<20; rows++)
            {
                for (columns=0; columns<20; columns++)
                {
                    //if all mines are selected the game ends
                    if (selected[rows][columns] == true && grid[rows][columns] == true)
                    {
                        gameOver=2;
                    }
                    //if any mines are not selected then the game continues
                    else if (selected[rows][columns] == false && grid[rows][columns] == true)
                    {
                        gameOver=0;
                        break;
                    }
                }
                if (gameOver==0)
                    break;
            }
        }
        //switch case for both game over possibilities
        switch (gameOver)
        {
            //game over for loss
            case 1:
                for (rows=0; rows<20; rows++)
                {
                    for (columns=0; columns<20; columns++)
                    {
                        if (grid[rows][columns] == true)
                        {
                            drawTile(demolition, "../../src/imagesAudio/mineMedium.png",     35, 35, 610.f + (35*rows), 190.f + (35*columns));
                        }
                    }
                }
                break;
            //game over for win
            case 2:
                for (rows=0; rows<20; rows++)
                {
                    for (columns=0; columns<20; columns++)
                    {
                        if (grid[rows][columns] == true)
                        {
                            drawTile(demolition, "../../src/imagesAudio/mineWinMedium.png",  35, 35, 610.f + (35*rows), 190.f + (35*columns));
                        }
                    }
                }
        }
        demolition.draw(Score);
        demolition.draw(LivesLeft);
        demolition.draw(Lives);
        if (sf::Mouse::getPosition(demolition).x >=17 && sf::Mouse::getPosition(demolition).x <=189 && sf::Mouse::getPosition(demolition).y >=14 && sf::Mouse::getPosition(demolition).y <=89)
        {
            drawTile(demolition, "../../src/imagesAudio/backButtonHighlighted.png", 173, 77, 17.f, 14.f);
        }
        else
        {
            drawTile(demolition, "../../src/imagesAudio/backButton.png", 173, 77, 17.f, 14.f);
        }
        if (sf::Mouse::getPosition(demolition).x >=1731 && sf::Mouse::getPosition(demolition).x <=1901 && sf::Mouse::getPosition(demolition).y >=14 && sf::Mouse::getPosition(demolition).y <=89)
        {
            drawTile(demolition, "../../src/imagesAudio/resetButtonHighlighted.png", 173, 77, 1730.f, 14.f);
        }
        else
        {
            drawTile(demolition, "../../src/imagesAudio/resetButton.png", 173, 77, 1730.f, 14.f);
        }

        effects.draw(demolition);
        demolition.display();
    }
}
#endif //DEMOLITION_H

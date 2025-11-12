//
// Created by willw on 04/08/2025.
//
#ifndef MEDIUM_H
#define MEDIUM_H
#include "Effects.h"

void difficulty();
//display board screen with tiles
inline void Medium()
{
    int rows;
    int columns;
    //determine whether a square has a mine or not
    bool grid[20][20]={false};
    //determine whether a user has selected a square or not
    bool selected[20][20]={false};
    //determine whether a selected square triggered by floodMedium has already been scored or not
    bool scored[20][20]={false};
    //determine whether a user has placed a flag on a square or not
    bool flagged[20][20]={false};
    int gameOver=0;
    int mines=0;
    int score=0;
    std::stringstream scoreStream;
    scoreStream << score;
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
    sf :: RenderWindow medium;
    sf::Font font("../../src/CascadiaCode.ttf");
    sf::Text Score(font);
    Score.setCharacterSize(55);
    Score.setPosition({849.f,110.f});
    Score.setFillColor(sf::Color::Black);
    Score.setString(scoreStream.str());
    medium.create(sf :: VideoMode({1920,1080}), "MINESWEEPER", sf :: State :: Fullscreen);
    medium.setFramerateLimit(60);
    //create effect manager
    Effects effects;
    sf::Clock clk;  //sfml stopwatch

    while (medium.isOpen())
    {
        float secsSinceLastFrame = clk.restart().asSeconds();

        while (const std :: optional event = medium.pollEvent())
        {
            //ends program if the user closes the window
            if (event->is<sf :: Event :: Closed>())
                medium.close();
            //closes window if the ESC key is pressed
            else if (const auto* keyPressed = event->getIf<sf :: Event :: KeyPressed>())
            {
                if (keyPressed->scancode == sf :: Keyboard :: Scancode :: Escape)
                    medium.close();
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                //checks if the user has right-clicked on the grid
                if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {   //scan every cell in grid, check if mouse in cell
                    for (rows=0; rows<20; rows++)
                    {
                        for (columns=0; columns<20; columns++)
                        {
                            //if square is not currently flagged, place a flag
                            if (gameOver==0 && flagged[rows][columns]==false && sf::Mouse::getPosition(medium).x >=610+(rows*35) && sf::Mouse::getPosition(medium).x <=610+(1+rows)*35 && sf::Mouse::getPosition(medium).y >=190+(columns*35) && sf::Mouse::getPosition(medium).y <=190+(1+columns)*35)
                            {
                                flagged[rows][columns]=true;
                            }
                            //if square is currently flagged, then remove the flag
                            else if (gameOver==0 && flagged[rows][columns]==true && sf::Mouse::getPosition(medium).x >=610+(rows*35) && sf::Mouse::getPosition(medium).x <=610+(1+rows)*35 && sf::Mouse::getPosition(medium).y >=190+(columns*35) && sf::Mouse::getPosition(medium).y <=190+(1+columns)*35)
                            {
                                flagged[rows][columns]=false;
                            }
                        }
                    }
                }
                //checks if the user has left-clicked on the grid or on one of the buttons
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    if (sf::Mouse::getPosition(medium).x >=17 && sf::Mouse::getPosition(medium).x <=189 && sf::Mouse::getPosition(medium).y >=14 && sf::Mouse::getPosition(medium).y <=89)
                    {
                        medium.close();
                        //difficulty window is reopened
                        difficulty();
                    }
                    else if (sf::Mouse::getPosition(medium).x >=1731 && sf::Mouse::getPosition(medium).x <=1901 && sf::Mouse::getPosition(medium).y >=14 && sf::Mouse::getPosition(medium).y <=89)
                    {
                        medium.close();
                        //medium window is reopened
                        Medium();
                    }
                    for (rows=0; rows<20; rows++)
                    {
                        for (columns=0; columns<20; columns++)
                        {
                            if (gameOver==0 && flagged[rows][columns]==false && sf::Mouse::getPosition(medium).x >=610+(rows*35) && sf::Mouse::getPosition(medium).x <=610+(1+rows)*35 && sf::Mouse::getPosition(medium).y >=190+(columns*35) && sf::Mouse::getPosition(medium).y <=190+(1+columns)*35)
                            {
                                //if the square has not already been selected and there is no mine, then add 100 points
                                if (!grid[rows][columns] && !selected[rows][columns])
                                {
                                    score+=100;
                                    scoreStream.str(std::string());
                                    scoreStream << score;
                                    Score.setString(scoreStream.str());
                                }
                                selected[rows][columns]=true;
                                //if is mine
                                if (grid[rows][columns])
                                {   //location of mine
                                    float cellCenterX = 610.f + 35.f*rows + 17.5f;
                                    float cellCenterY = 190.f + 35.f*columns + 17.5f;
                                    //trigger effects
                                    effects.spawn<ExplosionSoundEffect>("../../src/imagesAudio/explosion.wav");
                                    effects.spawn<RingWaveEffect>(sf::Vector2f{cellCenterX,cellCenterY}, 0.f, 600.f, 0.1f, sf::Color(255,80,30));
                                    effects.spawn<ScreenFlashEffect>(sf::Color(255,255,255,220), 0.05f);
                                }
                            }
                        }
                    }
                }
            }
        }
        //update effect
        effects.update(secsSinceLastFrame);
        loadScreen(medium, "../../src/imagesAudio/Minesweeper_medium.png");
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
                            drawTile(medium, "../../src/imagesAudio/minNum1Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        case 2:
                        {
                            drawTile(medium, "../../src/imagesAudio/minNum2Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        case 3:
                        {
                            drawTile(medium, "../../src/imagesAudio/minNum3Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        case 4:
                        {
                            drawTile(medium, "../../src/imagesAudio/minNum4Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        case 5:
                        {
                            drawTile(medium, "../../src/imagesAudio/minNum5Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        case 6:
                        {
                            drawTile(medium, "../../src/imagesAudio/minNum6Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        case 7:
                        {
                            drawTile(medium, "../../src/imagesAudio/minNum7Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        case 8:
                        {
                            drawTile(medium, "../../src/imagesAudio/minNum8Medium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            break;
                        }
                        default:
                        {
                            drawTile(medium, "../../src/imagesAudio/emptySelectedSquareMedium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                            //if an empty square is selected, the floodMedium function is called to select other safe squares near it
                            if (selected[rows][columns])
                            {
                                score+=floodScore(grid, selected, scored, rows, columns);
                                scoreStream.str(std::string());
                                scoreStream << score;
                                Score.setString(scoreStream.str());
                            }
                            break;
                        }
                    }
                }
                //if the square is a mine
                if (grid[rows][columns] == true)
                    drawTile(medium, "../../src/imagesAudio/mineMedium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
            }
        }
        for (rows=0; rows<20; rows++)
        {
            for (columns=0; columns<20; columns++)
            {
                //if the square is not selected then redraw it over the generated board
                if (selected[rows][columns] == false)
                    drawTile(medium, "../../src/imagesAudio/emptySquareMedium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
            }
        }
        for (rows=0; rows<20; rows++)
        {
            for (columns=0; columns<20; columns++)
            {
                //if the user has right-clicked on an unselected square then draw the flag
                if (flagged[rows][columns] == true && selected[rows][columns] == false)
                    drawTile(medium, "../../src/imagesAudio/minFlagMedium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
            }
        }
        for (rows=0; rows<20; rows++)
        {
            for (columns=0; columns<20; columns++)
            {
                //if the user has selected a mine the game ends
                if (selected[rows][columns] == true && grid[rows][columns] == true)
                    gameOver=1;
            }
        }
        if (gameOver != 1)
        {
            for (rows=0; rows<20; rows++)
            {
                for (columns=0; columns<20; columns++)
                {
                    //if all safe squares are selected the game ends
                    if (selected[rows][columns] == true && grid[rows][columns] == false)
                    {
                        gameOver=2;
                    }
                    //if any safe squares have not been selected then the game continues
                    else if (selected[rows][columns] == false && grid[rows][columns] == false)
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
                            drawTile(medium, "../../src/imagesAudio/mineMedium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
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
                            drawTile(medium, "../../src/imagesAudio/mineWinMedium.png", 35, 35, 610.f+(35*rows), 190.f+(35*columns));
                    }
                }
        }
        medium.draw(Score);
        if (sf::Mouse::getPosition(medium).x >=17 && sf::Mouse::getPosition(medium).x <=189 && sf::Mouse::getPosition(medium).y >=14 && sf::Mouse::getPosition(medium).y <=89)
            drawTile(medium, "../../src/imagesAudio/backButtonHighlighted.png", 173, 77, 17.f, 14.f);

        else
            drawTile(medium, "../../src/imagesAudio/backButton.png", 173, 77, 17.f, 14.f);

        if (sf::Mouse::getPosition(medium).x >=1731 && sf::Mouse::getPosition(medium).x <=1901 && sf::Mouse::getPosition(medium).y >=14 && sf::Mouse::getPosition(medium).y <=89)
            drawTile(medium, "../../src/imagesAudio/resetButtonHighlighted.png", 173, 77, 1730.f, 14.f);

        else
            drawTile(medium, "../../src/imagesAudio/resetButton.png", 173, 77, 1730.f, 14.f);

        effects.draw(medium);
        medium.display();
    }
}

#endif //MEDIUM_H

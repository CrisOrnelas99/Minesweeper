//
// Created by willw on 04/08/2025.
//
#ifndef HARD_H
#define HARD_H
#include "Effects.h"

void difficulty();
//display board screen with tiles

inline void Hard()
{
    int rows;
    int columns;
    //determine whether a square has a mine or not
    bool grid[30][30]={false};
    //determine whether a user has selected a square or not
    bool selected[30][30]={false};
    //determine whether a selected square triggered by floodHard has already been scored or not
    bool scored[30][30]={false};
    //determine whether a user has placed a flag on a square or not
    bool flagged[30][30]={false};
    int gameOver=0;
    int mines=0;
    int score=0;
    std::stringstream scoreStream;
    scoreStream << score;
    //places 180 mines
    while (mines<180)
    {
        //randomly selects a spot on the grid and places a mine if there is not one already
        rows=rand()%30;
        columns=rand()%30;
        if (!grid[rows][columns])
        {
            grid[rows][columns]=true;
            mines++;
        }
    }
    sf :: RenderWindow hard;
    sf::Font font("../../src/CascadiaCode.ttf");
    sf::Text Score(font);
    Score.setCharacterSize(60);
    Score.setPosition({765.f,10.f});
    Score.setFillColor(sf::Color::Black);
    Score.setString(scoreStream.str());
    hard.create(sf :: VideoMode({1920,1080}), "MINESWEEPER", sf :: State :: Fullscreen);
    hard.setFramerateLimit(60);
    //create effect manager
    Effects effects;
    sf::Clock clk;  //sfml clock

    while (hard.isOpen())
    {
        float secsSinceLastFrame = clk.restart().asSeconds();

        while (const std :: optional event = hard.pollEvent())
        {
            //ends program if the user closes the window
            if (event->is<sf :: Event :: Closed>())
                hard.close();
            //closes window if the ESC key is pressed
            else if (const auto* keyPressed = event->getIf<sf :: Event :: KeyPressed>())
            {
                if (keyPressed->scancode == sf :: Keyboard :: Scancode :: Escape)
                    hard.close();
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                //checks if the user has right-clicked on the grid
                if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {
                    //scan every cell in grid, check if mouse in cell
                    for (rows=0; rows<30; rows++)
                    {
                        for (columns=0; columns<30; columns++)
                        {
                            //if square is not currently flagged, place a flag
                            if (gameOver==0 && flagged[rows][columns]==false && sf::Mouse::getPosition(hard).x >=511+(rows*30) && sf::Mouse::getPosition(hard).x <=511+(1+rows)*30 && sf::Mouse::getPosition(hard).y >=93+(columns*30) && sf::Mouse::getPosition(hard).y <=93+(1+columns)*30)
                            {
                                flagged[rows][columns]=true;
                            }
                            //if square is currently flagged, then remove the flag
                            else if (gameOver==0 && flagged[rows][columns]==true && sf::Mouse::getPosition(hard).x >=511+(rows*30) && sf::Mouse::getPosition(hard).x <=511+(1+rows)*30 && sf::Mouse::getPosition(hard).y >=93+(columns*30) && sf::Mouse::getPosition(hard).y <=93+(1+columns)*30)
                            {
                                flagged[rows][columns]=false;
                            }
                        }
                    }
                }
                //checks if the user has left-clicked on the grid or on one of the buttons
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    if (sf::Mouse::getPosition(hard).x >=17 && sf::Mouse::getPosition(hard).x <=189 && sf::Mouse::getPosition(hard).y >=14 && sf::Mouse::getPosition(hard).y <=89)
                    {
                        hard.close();
                        //difficulty window is reopened
                        difficulty();
                    }
                    else if (sf::Mouse::getPosition(hard).x >=1731 && sf::Mouse::getPosition(hard).x <=1901 && sf::Mouse::getPosition(hard).y >=14 && sf::Mouse::getPosition(hard).y <=89)
                    {
                        hard.close();
                        //hard window is reopened
                        Hard();
                    }
                    for (rows=0; rows<30; rows++)
                    {
                        for (columns=0; columns<30; columns++)
                        {
                            if (gameOver==0 && flagged[rows][columns]==false && sf::Mouse::getPosition(hard).x >=511+(rows*30) && sf::Mouse::getPosition(hard).x <=511+(1+rows)*30 && sf::Mouse::getPosition(hard).y >=93+(columns*30) && sf::Mouse::getPosition(hard).y <=93+(1+columns)*30)
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
                                    float cellCenterX = 511.f + 30.f*rows + 15.f;
                                    float cellCenterY =  93.f + 30.f*columns + 15.f;
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
        //update effects
        effects.update(secsSinceLastFrame);

        loadScreen(hard, "../../src/imagesAudio/Minesweeper_hard.png");
        for (rows=0; rows<30; rows++)
        {
            for (columns=0; columns<30; columns++)
            {
                //if the square is not a mine
                if (grid[rows][columns]==false)
                {
                    int mineCount=countMines(grid,rows,columns);
                    //switch case for displaying number squares
                    switch (mineCount)
                    {
                        case 1:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum1Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        case 2:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum2Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        case 3:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum3Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        case 4:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum4Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        case 5:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum5Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        case 6:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum6Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        case 7:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum7Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        case 8:
                        {
                            drawTile(hard, "../../src/imagesAudio/minNum8Hard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            break;
                        }
                        default:
                        {
                            drawTile(hard, "../../src/imagesAudio/emptySelectedSquareHard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                            //if an empty square is selected, the floodHard function is called to select other safe squares near it
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
                    drawTile(hard, "../../src/imagesAudio/mineHard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
            }
        }
        for (rows=0; rows<30; rows++)
        {
            for (columns=0; columns<30; columns++)
            {
                //if the square is not selected then redraw it over the generated board
                if (selected[rows][columns] == false)
                    drawTile(hard, "../../src/imagesAudio/emptySquareHard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
            }
        }
        for (rows=0; rows<30; rows++)
        {
            for (columns=0; columns<30; columns++)
            {
                //if the user has right-clicked on an unselected square then draw the flag
                if (flagged[rows][columns] == true && selected[rows][columns] == false)
                    drawTile(hard, "../../src/imagesAudio/minFlagHard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
            }
        }
        for (rows=0; rows<30; rows++)
        {
            for (columns=0; columns<30; columns++)
            {
                //if the user has selected a mine the game ends
                if (selected[rows][columns] == true && grid[rows][columns] == true)
                    gameOver=1;
            }
        }
        if (gameOver != 1)
        {
            for (rows=0; rows<30; rows++)
            {
                for (columns=0; columns<30; columns++)
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
                for (rows=0; rows<30; rows++)
                {
                    for (columns=0; columns<30; columns++)
                    {
                        if (grid[rows][columns] == true)
                            drawTile(hard, "../../src/imagesAudio/mineHard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                    }
                }
                break;
            //game over for win
            case 2:
                for (rows=0; rows<30; rows++)
                {
                    for (columns=0; columns<30; columns++)
                    {
                        if (grid[rows][columns] == true)
                            drawTile(hard, "../../src/imagesAudio/mineWinHard.png", 30, 30, 511.f+(30*rows), 93.f+(30*columns));
                    }
                }
        }
        hard.draw(Score);
        if (sf::Mouse::getPosition(hard).x >=17 && sf::Mouse::getPosition(hard).x <=189 && sf::Mouse::getPosition(hard).y >=14 && sf::Mouse::getPosition(hard).y <=89)
            drawTile(hard, "../../src/imagesAudio/backButtonHighlighted.png", 173, 77, 17.f, 14.f);

        else
            drawTile(hard, "../../src/imagesAudio/backButton.png", 173, 77, 17.f, 14.f);

        if (sf::Mouse::getPosition(hard).x >=1731 && sf::Mouse::getPosition(hard).x <=1901 && sf::Mouse::getPosition(hard).y >=14 && sf::Mouse::getPosition(hard).y <=89)
            drawTile(hard, "../../src/imagesAudio/resetButtonHighlighted.png", 173, 77, 1730.f, 14.f);

        else
            drawTile(hard, "../../src/imagesAudio/resetButton.png", 173, 77, 1730.f, 14.f);

        effects.draw(hard);
        hard.display();
    }
}
#endif //HARD_H

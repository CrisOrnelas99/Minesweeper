//
// Created by willw on 04/07/2025.
//
#ifndef EASY_H
#define EASY_H
#include "Effects.h"
void difficulty();
inline void Easy()
{
    int rows;
    int columns;
    //determine whether a square has a mine or not
    bool grid[10][10]={false};
    //determine whether a user has selected a square or not
    bool selected[10][10]={false};
    //determine whether a selected square triggered by floodEasy has already been scored or not
    bool scored[10][10]={false};
    //determine whether a user has placed a flag on a square or not
    bool flagged[10][10]={false};
    int gameOver=0;
    int mines=0;
    int score=0;
    std::stringstream scoreStream;
    scoreStream << score;
    while (mines<10)
    {
        rows=rand()%10;
        columns=rand()%10;
        if (!grid[rows][columns])
        {
            grid[rows][columns]=true;
            mines++;
        }
    }
    sf :: RenderWindow easy;
    sf::Font font("../../src/CascadiaCode.ttf");
    sf::Text Score(font);
    Score.setCharacterSize(50);
    Score.setPosition({911.f,225.f});
    Score.setFillColor(sf::Color::Black);
    Score.setString(scoreStream.str());
    easy.create(sf :: VideoMode({1920,1080}), "MINESWEEPER", sf :: State :: Fullscreen);
    easy.setFramerateLimit(60);
    //create effect manager
    Effects effects;
    sf::Clock clk;  //SFML stopwatch

    while (easy.isOpen())
    {
        float secsSinceLastFrame = clk.restart().asSeconds();

        while (const std :: optional event = easy.pollEvent())
        {
            //ends program if the user closes the window
            if (event->is<sf :: Event :: Closed>())
                easy.close();
            //closes window if the ESC key is pressed
            else if (const auto* keyPressed = event->getIf<sf :: Event :: KeyPressed>())
            {
                if (keyPressed->scancode == sf :: Keyboard :: Scancode :: Escape)
                    easy.close();
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                //checks if the user has right-clicked on the grid
                if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {   //scan every cell in grid, check if mouse in cell
                    for (rows=0; rows<10; rows++)
                    {
                        for (columns=0; columns<10; columns++)
                        {
                            //if square is not currently flagged, place a flag
                            if (gameOver==0 && flagged[rows][columns]==false && sf::Mouse::getPosition(easy).x >=712+(rows*50) && sf::Mouse::getPosition(easy).x <=712+(1+rows)*50 && sf::Mouse::getPosition(easy).y >=289+(columns*50) && sf::Mouse::getPosition(easy).y <=289+(1+columns)*50)
                            {
                                flagged[rows][columns]=true;
                            }
                            //if square is currently flagged, then remove the flag
                            else if (gameOver==0 && flagged[rows][columns]==true && sf::Mouse::getPosition(easy).x >=712+(rows*50) && sf::Mouse::getPosition(easy).x <=712+(1+rows)*50 && sf::Mouse::getPosition(easy).y >=289+(columns*50) && sf::Mouse::getPosition(easy).y <=289+(1+columns)*50)
                            {
                                flagged[rows][columns]=false;
                            }
                        }
                    }
                }
                //checks if the user has left-clicked on the grid or on one of the buttons
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    if (sf::Mouse::getPosition(easy).x >=17 && sf::Mouse::getPosition(easy).x <=189 && sf::Mouse::getPosition(easy).y >=14 && sf::Mouse::getPosition(easy).y <=89)
                    {
                        easy.close();
                        //difficulty window is reopened
                        difficulty();
                    }
                    else if (sf::Mouse::getPosition(easy).x >=1731 && sf::Mouse::getPosition(easy).x <=1901 && sf::Mouse::getPosition(easy).y >=14 && sf::Mouse::getPosition(easy).y <=89)
                    {
                        easy.close();
                        //easy window is reopened
                        Easy();
                    }
                    for (rows=0; rows<10; rows++)
                    {
                        for (columns=0; columns<10; columns++)
                        {
                            if (gameOver==0 && flagged[rows][columns]==false && sf::Mouse::getPosition(easy).x >=712+(rows*50) && sf::Mouse::getPosition(easy).x <=712+(1+rows)*50 && sf::Mouse::getPosition(easy).y >=289+(columns*50) && sf::Mouse::getPosition(easy).y <=289+(1+columns)*50)
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
                                {
                                    //location of mine
                                    float cellCenterX = 712.f + 50.f*rows + 25.f;
                                    float cellCenterY = 289.f + 50.f*columns + 25.f;
                                    //trigger effects
                                    effects.spawn<ExplosionSoundEffect>("../../src/imagesAudio/explosion.wav");
                                    effects.spawn<RingWaveEffect>(sf::Vector2f{cellCenterX,cellCenterY}, 0.f, 600.f, 0.15f, sf::Color(255,80,30));
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

        loadScreen(easy, "../../src/imagesAudio/Minesweeper_easy.png");
        for (rows=0; rows<10; rows++)
        {
            for (columns=0; columns<10; columns++)
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
                            drawTile(easy, "../../src/imagesAudio/minNum1Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        case 2:
                        {
                            drawTile(easy, "../../src/imagesAudio/minNum2Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        case 3:
                        {
                            drawTile(easy, "../../src/imagesAudio/minNum3Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        case 4:
                        {
                            drawTile(easy, "../../src/imagesAudio/minNum4Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        case 5:
                        {
                            drawTile(easy, "../../src/imagesAudio/minNum5Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        case 6:
                        {
                            drawTile(easy, "../../src/imagesAudio/minNum6Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        case 7:
                        {
                            drawTile(easy, "../../src/imagesAudio/minNum7Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        case 8:
                        {
                            drawTile(easy, "../../src/imagesAudio/minNum8Easy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            break;
                        }
                        default:
                        {
                            drawTile(easy, "../../src/imagesAudio/emptySelectedSquareEasy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                            //if an empty square is selected, the floodEasy function is called to select other safe squares near it
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
                    drawTile(easy, "../../src/imagesAudio/mineEasy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
            }
        }
        for (rows=0; rows<10; rows++)
        {
            for (columns=0; columns<10; columns++)
            {
                //if the square is not selected then redraw it over the generated board
                if (selected[rows][columns] == false)
                    drawTile(easy, "../../src/imagesAudio/emptySquareEasy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
            }
        }
        for (rows=0; rows<10; rows++)
        {
            for (columns=0; columns<10; columns++)
            {
                //if the user has right-clicked on an unselected square then draw the flag
                if (flagged[rows][columns] == true && selected[rows][columns] == false)
                    drawTile(easy, "../../src/imagesAudio/minFlagEasy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
            }
        }
        for (rows=0; rows<10; rows++)
        {
            for (columns=0; columns<10; columns++)
            {
                //if the user has selected a mine the game ends
                if (selected[rows][columns] == true && grid[rows][columns] == true)
                    gameOver=1;
            }
        }
        if (gameOver != 1)
        {
            for (rows=0; rows<10; rows++)
            {
                for (columns=0; columns<10; columns++)
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
                for (rows=0; rows<10; rows++)
                {
                    for (columns=0; columns<10; columns++)
                    {
                        if (grid[rows][columns] == true)
                            drawTile(easy, "../../src/imagesAudio/mineEasy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                    }
                }
                break;
            //game over for win
            case 2:
                for (rows=0; rows<10; rows++)
                {
                    for (columns=0; columns<10; columns++)
                    {
                        if (grid[rows][columns] == true)
                            drawTile(easy, "../../src/imagesAudio/mineWinEasy.png",50,50,712.f+(50*rows), 289.f+(50*columns));
                    }
                }
        }
        easy.draw(Score);
        if (sf::Mouse::getPosition(easy).x >=17 && sf::Mouse::getPosition(easy).x <=189 && sf::Mouse::getPosition(easy).y >=14 && sf::Mouse::getPosition(easy).y <=89)
            drawTile(easy, "../../src/imagesAudio/backButtonHighlighted.png",173, 77, 17.f, 14.f);

        else
            drawTile(easy, "../../src/imagesAudio/backButton.png",173, 77, 17.f, 14.f);

        if (sf::Mouse::getPosition(easy).x >=1731 && sf::Mouse::getPosition(easy).x <=1901 && sf::Mouse::getPosition(easy).y >=14 && sf::Mouse::getPosition(easy).y <=89)
            drawTile(easy, "../../src/imagesAudio/resetButtonHighlighted.png",173, 77, 1730.f, 14.f);

        else
            drawTile(easy, "../../src/imagesAudio/resetButton.png",173, 77, 1730.f, 14.f);

        effects.draw(easy);
        easy.display();
    }
}

#endif //EASY_H

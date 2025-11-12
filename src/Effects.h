#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

// draw large image into screen
static void loadScreen(sf::RenderWindow &screen, std::string path)
{
    screen.clear(sf::Color::Black); //clear image with black
    sf::Texture texture(path, false, sf::IntRect({0,0},{1920,1080}));   //load images with size
    sf::Sprite sprite(texture); //create sprite
    screen.draw(sprite);    //display sprite
}

// draw a tile at specific location
static void drawTile(sf::RenderWindow &screen, std::string path, int w, int h, float x, float y)
{
    sf::Texture texture(path, false, sf::IntRect({0,0}, {w,h}));    //load small image
    sf::Sprite sprite(texture); //create sprite
    sprite.setPosition({x,y});  //move image to location
    screen.draw(sprite);    //display sprite
}

//works for any grid size
template <std::size_t H, std::size_t W>
// Count mines near a square
int countMines(const bool (&grid)[H][W], int rows, int columns) //grid where mine is, and where it is
{
    int checkHorizontal;
    int checkVertical;
    int count = 0;
    //peek at the eight neighbors
    for (int horizontal = -1; horizontal <= 1; horizontal++)    //looks at horizontal neigbors
    {
        for (int vertical = -1; vertical <= 1; vertical++)  //looks at vertical neighbors
        {
            if (horizontal == 0 && vertical == 0)   //skip where you are
                continue;
            else
            {   //get neighbor coordinates
                checkHorizontal = rows + horizontal;
                checkVertical = columns + vertical;
            }
            // makes sure the array stays in bounds
            if ((checkHorizontal >= 0 && checkHorizontal < static_cast<int>(H)) &&  (checkVertical   >= 0 && checkVertical  < static_cast<int>(W)))
            {
                // if neighbor square is on board and has mine, add 1
                if (grid[checkHorizontal][checkVertical])
                    count++;
            }
        }
    }
    return count;
}

//works for any grid size
template <std::size_t H, std::size_t W>
// flood reveal and scoring
int floodScore(const bool (&grid)[H][W], bool (&selected)[H][W], bool (&scored)[H][W], int rows, int columns)
{
    int score = 0;
    int checkHorizontal;
    int checkVertical;
    //look at the 8 neighbors
    for (int horizontal = -1; horizontal <= 1; horizontal++)
    {
        for (int vertical = -1; vertical <= 1; vertical++)
        {   //skip where you are
            if (horizontal == 0 && vertical == 0)
            {
                continue;
            }
            else
            {   //get neighbor coordinates
                checkHorizontal = rows + horizontal;
                checkVertical = columns + vertical;
            }
            // makes sure the array stays in bounds
            if ((checkHorizontal >= 0 && checkHorizontal < static_cast<int>(H)) && (checkVertical   >= 0 && checkVertical   < static_cast<int>(W)))
            {
                // if the square is not a mine and has not already been scored, then that square is selected and then scored
                if (!grid[checkHorizontal][checkVertical] && !scored[checkHorizontal][checkVertical])
                {
                    selected[checkHorizontal][checkVertical] = true;
                    scored[checkHorizontal][checkVertical] = true;
                    score += 100;
                }
            }
        }
    }
    return score;
}

    //use any grid size
template <std::size_t H, std::size_t W>
// flood for demolition
void floodDemolition(const bool (&grid)[H][W], bool (&selected)[H][W], int rows, int columns)
{
    int checkHorizontal;
    int checkVertical;
    //look at the 8 neighbors
    for (int horizontal = -1; horizontal <= 1; horizontal++)
    {
        for (int vertical = -1; vertical <= 1; vertical++)
        {   //skip your picked mine
            if (horizontal == 0 && vertical == 0)
            {
                continue;
            }
            else
            {   //get neighbor coordinates
                checkHorizontal = rows + horizontal;
                checkVertical = columns + vertical;
            }
            // makes sure the array stays in bounds
            if ((checkHorizontal >= 0 && checkHorizontal < static_cast<int>(H)) && (checkVertical   >= 0 && checkVertical   < static_cast<int>(W)))
            {
                // if the square is not a mine then select it
                if (!grid[checkHorizontal][checkVertical])
                {
                    selected[checkHorizontal][checkVertical] = true;
                }
            }
        }
    }
}

// abstract class for visual effects
class Effect {
public:
    virtual bool update(float) = 0; // move the effect forward one step, return true when done
    virtual void draw(sf::RenderWindow& window) const = 0; // draw the effect
    virtual ~Effect() = default; // delete effect through base pointer
};
    //effects manager
class Effects {
    private:
    std::vector<Effect*> effectsList; // stores pointers to effects

    public:
    // create new effect of type T, with any arguments passed
    template <class T, class... Args>
    T& spawn(Args... args)  // create new effect with given arguments
    {
        T* created = new T(args...);  // allocate effect
        effectsList.push_back(created);  // store base-class pointer to update and draw for later
        return *created; // return reference to new effect
    }

    // add new effect to list
    void spawn(Effect* effectPtr)
    {
        effectsList.push_back(effectPtr);  // store it ti update and draw later
    }

    // advance and remove finished
    void update(float frameTimeSec)
    {
        int i = 0;
        //loop through effects
        while (i < (int)effectsList.size())
        { // loop over list of effects to do
            bool done = effectsList[i]->update(frameTimeSec);
            if (done)
            {
                delete effectsList[i]; // free memory
                effectsList.erase(effectsList.begin() + i); // remove from list
            }
            else
            {
                ++i; // move to next effect
            }
        }
    }

    // draw all active effects
    void draw(sf::RenderWindow& window) const
    {   //go through each effect and draw them
        for (int i = 0; i < (int)effectsList.size(); ++i)
            effectsList[i]->draw(window);
    }
    //clear all effects from list when new game
    void clearAll()
    {   //loop through effects and delete
        for (int i = 0; i < (int)effectsList.size(); ++i)
            delete effectsList[i];
        effectsList.clear();
    }
        //clean up all effects
    ~Effects()
    {
        clearAll();
    }
};

// circle that expands from a center point
class RingWaveEffect : public Effect {

private:
    float startRadius{0.f}; // start size
    float endRadius{0.f};  // end size
    int framesLived{0};  // frames elapsed
    int totalFrames{1}; // total frames to live
    sf::CircleShape ringShape;  // SFML circle shape

public:
    RingWaveEffect(sf::Vector2f center, float startRadiusIn, float endRadiusIn, float lifetimeSecondsIn, sf::Color outlineColor)
        : startRadius(startRadiusIn), endRadius(endRadiusIn),  framesLived(0)
    {
        // convert seconds to frames using 60 fps
        const int fps = 60;
        //if time 0 or less, use 1 to work just in case
        if (lifetimeSecondsIn <= 0.f)
            totalFrames = 1;
        else
        {    //convert seconds to frames and round to nearest integer
            int f = (int)(lifetimeSecondsIn * fps + 0.5f);
            totalFrames = (f < 1 ? 1 : f);  //ensure at least 1 frame
        }

        ringShape.setFillColor(sf::Color::Transparent); // no fill, only outline
        ringShape.setOutlineThickness(6.f);  // outline thickness
        ringShape.setOutlineColor(outlineColor); // outline color
        ringShape.setPosition(center); // fixed center position
    }
        //take one step in every frame
    bool update(float) override
    {
        // 0 is started, 1 is end of progress, figure out how far along we are
        float progress = (totalFrames > 0) ? (float)framesLived / (float)totalFrames : 1.f;
        if (progress < 0.f)
            progress = 0.f; // force low
        if (progress > 1.f)
            progress = 1.f;  // force high

        float radius = startRadius + (endRadius - startRadius) * progress; // grow radius
        ringShape.setRadius(radius); // apply radius
        ringShape.setOrigin(sf::Vector2f{ radius, radius }); // keep centered

        if (framesLived < totalFrames)
            ++framesLived; // advance one frame
        return framesLived >= totalFrames; // done when we reach total frames
    }

    void draw(sf::RenderWindow& window) const override
    {
        window.draw(ringShape); // show circle outline
    }
};

// solid color rectangle that covers view
class ScreenFlashEffect : public Effect
    {
    private:
        sf::Color fillColor; // color to fill the screen
        int framesLived{0}; // frames elapsed
        int totalFrames{1}; // total frames to live
    public:
        ScreenFlashEffect(sf::Color fillColorIn, float lifetimeSecondsIn) // set color and time
            : fillColor(fillColorIn), framesLived(0)
        {
            // convert seconds to frames using 60 fps
            const int fps = 60;
            //if time 0 or less, use 1 to work just in case
            if (lifetimeSecondsIn <= 0.f)
                totalFrames = 1;
            else
            {
                //convert seconds to frames and round to nearest integer
                int f = (int)(lifetimeSecondsIn * fps + 0.5f);
                totalFrames = (f < 1 ? 1 : f);  //ensure at least 1 frame
            }
        }
        //take one step in every frame
        bool update(float) override
        {
            if (framesLived < totalFrames)
                ++framesLived; // advance one frame
            return framesLived >= totalFrames; // finished when out of frames
        }

        void draw(sf::RenderWindow& window) const override
        {
            sf::RectangleShape rect; // simple rectangle
            sf::Vector2f viewSize   = window.getView().getSize(); // current view size
            sf::Vector2f viewCenter = window.getView().getCenter(); // current view center
            rect.setSize(viewSize); // fill the view
            rect.setOrigin(sf::Vector2f{ viewSize.x * 0.5f, viewSize.y * 0.5f }); // center origin
            rect.setPosition(viewCenter);  // place at center
            rect.setFillColor(fillColor); // solid color
            window.draw(rect); // draw it
        }
    };
//play explosion sound when mine found and game end
class ExplosionSoundEffect : public Effect
{
private:
    sf::SoundBuffer buffer; // holds audio samples
    sf::Sound sound; // plays the buffer
public:
    ExplosionSoundEffect(const std::string& file, float vol = 100.f)
        : buffer(), sound(buffer)
    {   // load audio data
        if (buffer.loadFromFile(file))
            { // load audio data
            sound.setVolume(vol);
            sound.play(); // play once
        }
    }
    //check if still playing each frame .if still playing return false
    bool update(float) override
    {
        return sound.getStatus() == sf::SoundSource::Status::Stopped; // done when sound stops
    }

    void draw(sf::RenderWindow&) const override { }
};

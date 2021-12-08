#include <iostream>
#include <memory>
#include <vector>
#include <cstdlib>

#include "textObserver.h"
#include "../utils/message.h"
#include "../utils/color.h"

using namespace std;

TextObserver::TextObserver(CC3K* subject, int width, int height) : subject{subject}, width{width}, height{height}
{

}

TextObserver::~TextObserver()
{
}

void TextObserver::notify()
{

    int playerX = subject->getPlayer()->getX();
    int playerY = subject->getPlayer()->getY();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Bonus: Add fog
            // If the position is outside of the circle centered at the player, output fog
            if ((playerX-j)*(playerX-j)+5*(playerY-i)*(playerY-i) > 48)
            {
                cout << "~";
            }
            else
            {
                pair<char, string> state = subject->getState(j, i);
                if (state.second == Color::RESET) 
                {
                    cout << state.first;
                }
                else 
                {
                    cout << state.second << state.first << Color::RESET;
                }
            }

        }
        cout << endl;
    }
    cout << subject->getGameStatus();
    cout << " ";

    vector<Message> theMessages = subject->getMessages();

    for (int i = 0; i < theMessages.size(); i++)
    {
        cout << " " << theMessages[i].color <<  theMessages[i].text  << Color::RESET;
    }
    cout << endl;

}

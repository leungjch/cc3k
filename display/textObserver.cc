#include <iostream>
#include <memory>
#include <vector>

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
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
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
        cout << endl;
    }
    cout << subject->getGameStatus();
    cout << " ";

    vector<Message> theMessages = subject->getMessages();

    for (int i = 0; i < theMessages.size(); i++)
    {
        cout << theMessages[i].color <<  theMessages[i].text  << Color::RESET << endl;
    }
}

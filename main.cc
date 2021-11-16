#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[]) 
{
    // Work with STL containers only
    vector<string> args(argv, argv+argc);

    cout << "Welcome to CC3K!" << endl;
}

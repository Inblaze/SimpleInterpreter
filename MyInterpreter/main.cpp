/*
    Simple painting code interpreter by:
    Yifei Chen, Wangwang Liu, Yuming Lin
*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "parser.h"
#include "my_semantics.h"

using namespace std;

int main()
{

    //cout << "Start to parse..." << endl;
    Parser();
    // close the scanner
    cout<<endl;
    cout << "[Interpreter]Done." << endl;
    closeScanner();
    close_drawer();
    system("python ./drawit.py");
}

#include <iostream>
#include "AA.h"
using namespace std;
int AA::data2 = 5;
AA::AA(int t)
{
    data = t;
    //ctor
}

void Disp(AA & a)
{
    cout << a.data << endl;
}
/*
void Disp2()
{
        cout <<AA::data2 << endl;
}
*/
AA::~AA()
{
    //dtor
}

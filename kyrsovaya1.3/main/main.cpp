#include <iostream>
#include "Calculator.h"

using namespace std;

int main()
{
    Calculator* test; //creating an object for the calculator class
    test = new Calculator(); //ininzialization of the object for the calculator class 
    test->ReadPhrase(); //reading an original phrase from the file
    test->PrefixOuput(); //printing the result 
}
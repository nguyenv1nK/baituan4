#pragma once
#include <iostream>
#include "Controller.h"

using namespace std;

class View
{	
public:
	int Loginscreen();
	int Menu();
	int Mess();
	int Friends();
	int Messrec();
	int Block();
private:
	Controller controller;
};


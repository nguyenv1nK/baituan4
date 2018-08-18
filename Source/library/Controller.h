
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "sqlite3.h"
#include "Model.h"
#include <conio.h>

using namespace std;



class Controller
{
public:
	int id;
	char *username;
	char *password;
	char *city;
	char *username1;
	char *password1;
	char *city1;
	int flag;
	Model model;

	void SignUp();
	void SignIn();
	void SignOut();
	void ShowMessSend(int);
	void ShowMessRec(int);
	void SendMess(int);
	void AddFriend(int);
	void ShowFriend(int);
	void Block(int);
	void ShowNewMessRec(int);
	void Conversation(int);
	void RBlock(int);
};


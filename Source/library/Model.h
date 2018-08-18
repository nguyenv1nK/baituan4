#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <sstream>
#include <list>

using namespace std;
class Model
{
public:
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char *test;
	sqlite3_stmt *stmt;
	list <string> answer;
	string sql1;

	static int callback(void *, int, char**, char**);
	void Open();
	void InsertSQL(sqlite3 *db, char *, char *, char *);
	int SelectSQL(sqlite3 *db, char *fn, char *ln);
	int SelectSQL1(sqlite3 *db, char *fn);
	int ConvertUsernameToId(char *fr1end);
	void SelectMess(sqlite3 *db, int id);
	void ShowMessSend(sqlite3 *db,int id1);
	void ShowMessRec(sqlite3 *db, int id1);
	bool CheckBlock(sqlite3 *db, int id1, int id2);
	void WriteMess(sqlite3 *db, int id1, int id2, string mess, char *time);
	bool CheckFriend(sqlite3 *db, int id1, int id2);
	void ShowFriend(sqlite3 *db, int id1);
	void BlockFriend(sqlite3 *db, int id1, int id2);
	void WriteToFriend(sqlite3 *db, int id1, int id2);
	void ShowMessDetail(sqlite3 *db, int id1, int id2);
	void CheckSeen(sqlite3 *db, int id1);
	void RemoveBlock(sqlite3 *db, int id1, int id2);
	void ListFriendByCity(sqlite3 *db, int id1);
	void InsertInterTime(sqlite3 *db, int id1, int id2);
	void ShowNewMessRec(sqlite3 *db, int id1);
	void Conversation(sqlite3 *db, int id1, int id2);
};


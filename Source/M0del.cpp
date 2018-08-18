#include "Model.h"
#include <iostream>
#include <cstring>
#include <ctime>


using namespace std;

int Model::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i=0; i<argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
		printf("\n");
		return 0;
}

void Model::Open()
{
	rc = sqlite3_open("tuan4.db", &db);
	if(rc)
	{
		fprintf(stderr, "Can't open database: %s\n",sqlite3_errmsg(db));
		exit(0);
	}
}
void Model::InsertSQL(sqlite3 *db, char *fn, char *ln, char *city)
{
	Open();
	if(!db)
		return;

	sql1 = "insert into user(username, password, idcity) values('"+string(fn)+"', '"+string(ln)+"', '"+string(city)+"')";
	char *sql = new char[sql1.length() + 1];
	std::strcpy(sql, sql1.c_str());
	int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_close(db);
}
int Model::SelectSQL(sqlite3 *db, char *user, char *pass)
{
	Open();
	int id = 0;
	if(!db)
		return 0;
	sql = "SELECT * FROM user WHERE username = ? and password = ?";
	rc = sqlite3_prepare(db, sql, strlen(sql), &stmt, &test);
	if(rc == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, user, strlen(user), 0);
		sqlite3_bind_text(stmt, 2, pass, strlen(pass), 0);
		while(sqlite3_step(stmt)==SQLITE_ROW)
		{
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> id ;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n",zErrMsg );
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return id;
}
int Model::SelectSQL1(sqlite3 *db, char *user)
{
	Open();
	int id = 0;
	if (!db)
		return 0;
	sql1 = "SELECT id FROM user WHERE username = '" +string(user)+"'";
	char *sql = new char[sql1.length() + 1];
	std::strcpy(sql, sql1.c_str());
	rc = sqlite3_prepare(db, sql, strlen(sql), &stmt, &test);
	if (rc == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> id;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return id;
}

int Model::ConvertUsernameToId(char *fd)
{	
	Open();
	int id2 = -1;
	sql="select id from user where username = ?";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if( rc == SQLITE_OK )
	{
		sqlite3_bind_text(stmt, 1, fd, strlen(fd), 0);
		if(sqlite3_step(stmt)==SQLITE_ROW)
		{
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> id2;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n",zErrMsg );
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return id2;
}


bool Model::CheckBlock(sqlite3 *db, int id1, int id2) 
{
	Open();
	int block = 0;
	if (!db)return 0;
	sql = "SELECT flagBlock FROM friend WHERE (id1 = ? AND id2 = ?) OR (id1 = ? AND id2 = ?)";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_int(stmt, 3, id2);
		sqlite3_bind_int(stmt, 4, id1);

		if (sqlite3_step(stmt) == SQLITE_ROW) 
		{
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> block;
		}
	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return 
		!block;
}

void Model::ShowMessSend(sqlite3 *db, int id1)
{

	Open();
	if(!db)
		return ;
	sql = "select * from (select * from messenger where idsend = ?) as m left join user on (user.id = m.idreceive)";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);

	if(rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id1);
		
		while(sqlite3_step(stmt) == SQLITE_ROW)
		{	

			cout <<"Da gui den:" <<string((const char *)sqlite3_column_text(stmt, 6)) << endl;
			cout <<"voi noi dung:" << string((const char *)sqlite3_column_text(stmt, 2)) << endl;
			cout << "vao luc:" << string((const char *)sqlite3_column_text(stmt, 3)) << endl;
			cout << string((const char *)sqlite3_column_text(stmt, 4)) << endl;
			cout << endl;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n",zErrMsg );
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
}
void Model::CheckSeen(sqlite3 *db, int id1)
{
	Open();
	if (!db)
		return;
	sql= "  UPDATE messenger set status = 'da xem' where  idreceive = ? and status = 'chua xem' ; ";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

void Model::ShowMessRec(sqlite3 *db, int id1)
{

	Open();
	if (!db)
		return;
	sql = "select * from (select * from messenger where idreceive = ? and status = 'da xem') as m left join user on (user.id = m.idsend)";
		
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);

	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id1);

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{

			cout << string((const char *)sqlite3_column_text(stmt, 6)) <<" da gui tin nhan"<< endl;
			cout << "voi noi dung:" << string((const char *)sqlite3_column_text(stmt, 2)) << endl;
			cout << "vao luc:" << string((const char *)sqlite3_column_text(stmt, 3)) << endl;
			cout << endl;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

void Model::ShowNewMessRec(sqlite3 *db, int id1)
{

	Open();
	if (!db)
		return;
	sql = "select * from (select * from messenger where idreceive = ? and status ='chua xem' ) as m left join user on (user.id = m.idsend)";

	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);

	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id1);

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{

			cout << string((const char *)sqlite3_column_text(stmt, 6)) << " da gui tin nhan" << endl;
			cout << "voi noi dung:" << string((const char *)sqlite3_column_text(stmt, 2)) << endl;
			cout << "vao luc:" << string((const char *)sqlite3_column_text(stmt, 3)) << endl;
			cout << endl;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

void Model::WriteMess(sqlite3 *db, int id1, int id2, string mess, char *time)
{	
	Open();
	sql1 = "INSERT INTO messenger(idsend, idreceive, mess, time, status) VALUES (?, ?, '" + mess + "', ?,'Chua xem');";
	char *sql = new char[sql1.length() + 1];
	std::strcpy(sql, sql1.c_str());
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if(rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_text(stmt, 3, time, strlen(time), 0);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
	fprintf(stderr, "SQL error: %s\n",zErrMsg );
	sqlite3_free(zErrMsg);
	}
	int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

bool Model::CheckFriend(sqlite3 *db, int id1,int id2)
{	
	Open();
	int flagFriend = 0;
	sql = "select * from friend where ( id1 = ? and id2 = ?) or ( id1 = ? and id2 = ?)";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if( rc == SQLITE_OK )
	{
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_int(stmt, 3, id2);
		sqlite3_bind_int(stmt, 4, id1);

		if(sqlite3_step(stmt) == SQLITE_ROW)
		{
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> flagFriend;
				if(flagFriend !=0)
					flagFriend = 1;
				sqlite3_finalize(stmt);
		}
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return flagFriend;
	
}

void Model::WriteToFriend(sqlite3 *db, int id1, int id2)
{	
	int block = 0;
	Open();
	if(!db)
		return;

		sql = "insert into friend(id1,id2,flagBlock) values (?, ?, ?)";
	
		rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
		if( rc == SQLITE_OK)
		{
			sqlite3_bind_int(stmt, 1, id1);
			sqlite3_bind_int(stmt, 2, id2);
			sqlite3_bind_int(stmt, 3, block);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
		else
		{
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
		}
		int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
		sqlite3_close(db);
}

void Model::ShowFriend(sqlite3 *db, int id1)
{	
	Open();
	cout<<"\n=====List Friend=====\n";
	if (!db)
		return;
	else
		sql = "SELECT DISTINCT user.username FROM ((SELECT * FROM friend where (id1 = ? OR id2 = ?) AND flagBlock = 0) as A LEFT JOIN user ON (A.id2 = user.id OR A.id1 = user.id)) ORDER BY intertime desc";
		
		rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
		if( rc == SQLITE_OK)
		{
			sqlite3_bind_int(stmt, 1, id1);
			sqlite3_bind_int(stmt, 1, id1);
			while(sqlite3_step(stmt) == SQLITE_ROW)
			{
				cout << string((const char *) sqlite3_column_text(stmt, 0)) <<endl;
			}
			sqlite3_finalize(stmt);
			cout << endl;
		}
		else
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		sqlite3_close(db);
}


void Model::BlockFriend(sqlite3 *db, int id1, int id2) 
{
	if (!db)return;
	Open();
	sql = "UPDATE friend SET flagBlock = 1 WHERE id1 = ? AND id2 = ?";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if (rc == SQLITE_OK) {
		
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_close(db);
}


void Model::RemoveBlock(sqlite3 *db, int id1, int id2) 
{
	if (!db)return;
	Open();
	sql = "UPDATE friend SET flagBlock = 0 WHERE id1 = ? AND id2 = ?";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if (rc == SQLITE_OK) 
	{

		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);

		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

	}
	else 
	{

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

void Model::ListFriendByCity(sqlite3 *db, int id1)
{
	if (!db) return;
	Open();
	sql = "select username,namecity from ((select * from friend where id1 = ?) as m left join user on (user.id = m.id2))as n left join city on (user.idcity = city.idcity)  ORDER BY user.idcity asc";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id1);

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			cout << string((const char *)sqlite3_column_text(stmt, 0)) << "  den tu  " << string((const char *)sqlite3_column_text(stmt, 1)) << endl;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

}

void Model::InsertInterTime(sqlite3 *db, int id1, int id2)
{
		if (!db) return;
		Open();
		sql = "UPDATE friend SET intertime = ? WHERE id1 = ? AND id2 = ?";
		time_t now = time(0);
		int i = now;
		rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
		if (rc == SQLITE_OK)
		{

			sqlite3_bind_int(stmt, 1, i);
			sqlite3_bind_int(stmt, 2, id1);
			sqlite3_bind_int(stmt, 3, id2);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);

		}
		else
		{

			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
		sqlite3_close(db);

}


void Model::Conversation(sqlite3 *db, int id1, int id2)
{
	if (!db) return;
	Open();
	sql = "select username,mess from (select * from messenger  as m left join user on user.id = m.idsend)  where (idreceive = ? and idsend = ?) or (idsend = ? and idreceive = ? ) order by time asc ";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &test);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{

			cout << string((const char *)sqlite3_column_text(stmt, 6)) << " : " ;
			cout << string((const char *)sqlite3_column_text(stmt, 2)) << endl;
			cout << endl;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
}







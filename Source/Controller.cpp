#include "Controller.h"
#include <cstring>
#include <string>
#include <ctime>




using namespace std;


void Controller::SignUp()
{
a:	cout << "Nhap cac thong tin dang ki" << endl;
	username1 = new char[20];
	city1 = new char [20];
	password1 = new char[20];
	cout << "Username:";
	cin >> username1;
	cout << "\nPassword:";
	cin >> password1;
	cout << "\nMa thanh pho :\n1.Ha Noi\n2.Hai Phong\n3.Hai Duong\n4.Thai Binh\n5.Nam Dinh\n6.Tuyen Quang" << endl;
	cout << "\nMa thanh pho:";
	cin >> city1;
	if ( model.SelectSQL1(model.db, username1) > 0)
	{
		cout << "\nTen tai khoan da ton tai, vui long chon ten khac !\n";
		goto a;
	}
	else
	{
		model.InsertSQL(model.db, username1, password1, city1);
		cout << "\nDang ki thanh cong !" << endl;
	}
}


void Controller::SignIn()
{
	username = new char[20];
	password = new char[20];

	cout << "\nUsername:";
	cin >> username;
	cout << "\nPassword:";
	cin >>	password;
	cout << endl;

	model.Open();

	if(model.SelectSQL(model.db, username, password) > 0)
	{
		cout << "Dang nhap thanh cong !\n";
		id = model.SelectSQL(model.db, username, password);
		flag = 1;
	}
	else
	{
		cout << "Dang nhap that bai, kiem tra lai mat khau va tai khoan !\n";
		flag = 0;
	}
}


void Controller::SignOut()
{
	flag = 0;
	cout << "Dang xuat thanh cong !\n";
}




void Controller::ShowMessSend(int id)
{
	model.Open();
	model.ShowMessSend(model.db, id);

}

void Controller::ShowMessRec(int id)
{	
	model.Open();
	model.ShowMessRec(model.db, id);
	model.CheckSeen(model.db, id);
}

void Controller::ShowNewMessRec(int id)
{
	model.Open();
	model.ShowNewMessRec(model.db, id);
	model.CheckSeen(model.db, id);
}


void Controller::SendMess(int id)
{
	int id2;
	char *fr;
	fr = new char[20];
	cout << "Nhap ten nguoi nhan :\n";
	cin >> fr;
	model.Open();
	id2 = model.ConvertUsernameToId(fr);
	if(id2 > 0)
	{
		char *dt;
		string mess;
		mess = new char[100];
		dt = new char;
		if(!model.CheckBlock(model.db, id, id2))
		{
			cout << "Ban da bi nguoi dung block !\n";
		}
		else
		{
			cout << "Nhap tin nhan:\n ";
			cin.ignore();
			getline(cin, mess);
			time_t now = time(0);
			dt = ctime(&now);
			model.WriteMess(model.db, id, id2, mess, dt);
			model.InsertInterTime(model.db, id, id2);
			cout << "Da gui tin nhan!\n";
			
		}
	}

}


void Controller::AddFriend(int id)
{
	cout << "Nhap ten nguoi ban muon them: \n";
	char *n;
	n = new char[20];
	cin >> n;
	model.Open();
	int id2 = model.ConvertUsernameToId(n);
	if (id == id2)
		cout << "Khong the ket ban voi ban than\n";
	else
	{
		if (id2 > 0)
		{
			if (!model.CheckFriend(model.db, id, id2))
			{
				model.WriteToFriend(model.db, id, id2);
				model.InsertInterTime(model.db, id, id2);
				cout << "Them ban thanh cong !" << endl;

			}
			else
			{
				cout << "Ban va tai khoan nay da la ban be !\n";
			}
		}
		else
			cout << "Tai khoan khong ton tai !\n";
	}
}


void Controller::ShowFriend(int id)
{

	model.ShowFriend(model.db, id);
	cout << "\nNhan ctl +C de hien danh sach ban be theo thanh pho";
	int key;
	key = _getch();
	if (key == 3)
	{
		system("cls");
		cout << "\n=====Danh sach ban be theo thanh pho=====\n\n\n";
		model.ListFriendByCity(model.db,id);
		
	}
	cin.get();
}


void Controller::Block(int id1)
{
	cout << "Nhap ten ban be muon block !\n";
	cout << "***Luu y: sau khi block, ban khong the nhan tin nhan tu tai khoan nay ! \n";
	char *userblock;
	userblock = new char[20];
	cin >> userblock;
	model.Open();
	int id2 = model.ConvertUsernameToId(userblock);
	if (id2 == id1)
		cout << "Khong the block ban than !\n";
	else
	{
		if (id2 > 0)
		{
			model.BlockFriend(model.db, id1, id2);
			cout << "Tai khoan da bi block thanh cong !\n";
		}
		else
			cout << "Tai khoan khong ton tai !\n";
	}
}
void Controller::RBlock(int id1)
{
	cout << "Nhap ten ban be muon bo block !\n";
	char *userblock;
	userblock = new char[20];
	cin >> userblock;
	model.Open();
	int id2 = model.ConvertUsernameToId(userblock);
		if (id2 > 0)
		{
			model.RemoveBlock(model.db, id1, id2);
			cout << "Da bo block !\n";
		}
		else
			cout << "Tai khoan khong ton tai !\n";
}
void Controller::Conversation(int id1)
{
	cout << "\nHien thi cuoc tro chuyen voi : ";
	char *n;
	n = new char[20];
	
	cin >> n;
	model.Open();
	int id2 = model.ConvertUsernameToId(n);
	if (id2 > 0)
	{
		model.Conversation(model.db, id1, id2);
		cout << "\nNhan ctr+r de rep tin nhan hoac gui tiep " << endl;
		int key;
		key = _getch();
		if (key == 18)
		{
			char *dt;
			string mess;
			mess = new char[100];
			dt = new char;
			cout << "Nhap tin nhan:\n ";
			cin.ignore();
			getline(cin, mess);
			time_t now = time(0);
			dt = ctime(&now);
			model.WriteMess(model.db, id1, id2, mess, dt);
			model.InsertInterTime(model.db, id1, id2);
		}
		cin.get();
	
	}
	else
		cout << "\n Tai khoan khong ton tai";
}


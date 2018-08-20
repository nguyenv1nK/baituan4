#include "View.h"
#include <stdio.h>


using namespace std;

int  View::Loginscreen()
{
	char chose[3];
	cout << "==========Login Screen==========\n\n\n";
	cout << "1.Dang nhap\n";
	cout << "2.Dang ki\n";
	cout << "Lua chon:\n";
	cin >> chose;
	switch (chose[0])
	{
	case '1':
		system("cls");
		controller.SignIn();

		if (controller.flag == 1)
			return Menu();
		else
			return Loginscreen();
			break;
	case '2':
		system("cls");
		controller.SignUp();
		break;
	default:
		system("cls");
		cout << "Khong co lua chon nay, yeu cau nhap lai !\n";
		return Loginscreen();
		
	}
}

int View::Menu()
{
		char chose[3];
		while (chose)
	{
		cout << "\n==========Menu==========\n\n";
		cout << "1.Tin nhan\n";
		cout << "2.Ban be\n";
		cout << "3.Dang xuat\n";
		cout << "Nhap lua chon :\n";
		cin >> chose;
		switch (chose[0])
		{
		case '1':
			system("cls");
			return Mess();
			break;
		case '2':
			system("cls");
			return Friends();
			break;
		case '3':
			system("cls");
			controller.SignOut();
			return Loginscreen();
			break;
		default:
			system("cls");
			cout << "Khong co lua chon nay !\n";
			break;
		}
	}
}

int View::Mess()
{
	char chose[3];
	cout << "\n==========Tin nhan==========\n\n" << endl;
	cout << "1.Gui tin nhan\n";
	cout << "2.Danh sach tin nhan da gui\n";
	cout << "3.Danh sach tin nhan da nhan\n";
	cout << "4.Cuoc tro chuyen\n";
	cout << "\n\nB de quay lai\n";
	cin >> chose;
	switch (chose[0])
	{
	case '1':
		system("cls");
		controller.SendMess(controller.id);
		break;
	case'2':
		system("cls");
		controller.ShowMessSend(controller.id);
		break;
	case '3':
		system("cls");
		controller.ShowMessRec(controller.id);
		break;
	case '4':
		system("cls");
		controller.Conversation(controller.id);
		break;
	case 'b':
		system("cls");
		return Menu();
		break;
	default:
		system("cls");
		cout << "Khong co lua chon nay, xin moi nhap lai !\n";
		break;
	}
	return Mess();
}
int View::Messrec()
{
	char chose[3];
	cout << "==========Tin nhan da nhan==========\n\n" << endl;
	cout << "1.Tin chua doc\n";
	cout << "2.Tin da doc\n";
	cout << "\n\nB de quay lai\n";
	cin >> chose;
	switch (chose[0])
	{
	case '1':
		system("cls");
		controller.ShowNewMessRec(controller.id);
		
		break;
	case'2':
		system("cls");
		controller.ShowMessRec(controller.id);
		break;
	case 'b':
		system("cls");
		return Mess();
		break;
	default:
		system("cls");
		cout << "Khong co lua chon nay, xin moi nhap lai !\n";
		
		break;
	}
	
	return Messrec();
}



int View::Friends()
{

		
		char chose[3];
		cout << "==========Ban be==========\n" << endl;
		cout << "1.Them ban\n";
		cout << "2.Danh sach ban\n";
		cout << "3.Block\n";
		cout << "\n\nB de quay lai \n";
		cin >> chose;
		switch (chose[0])
		{
		case '1':
			system("cls");
			controller.AddFriend(controller.id);
			break;
		case'2':
			system("cls");
			controller.ShowFriend(controller.id);
			break;
		case '3':
			system("cls");
			return Block();
			break;
		case'b':
			system("cls");
			return Menu();
			break;
		default:
			system("cls");
			cout << "Khong co lua chon nay, xin moi nhap lai !\n";
			break;
		}
		cin.get();
		return Friends();
}
int View::Block()
{	
	char chose[3];
	cout << "==========Block==========\n\n" << endl;
	cout << "1. Block 1 tai khoan\n";
	cout << "2. Bo block \n";
	cout << "\n\nB de quay lai\n";
	cin >> chose;
	switch (chose[0])
	{
	case '1':
		system("cls");
		controller.Block(controller.id);
		
		break;
	case'2':
		system("cls");
		controller.RBlock(controller.id);
		break;
	case 'b':
		system("cls");
		return Friends();
		break;
	default:
		system("cls");
		cout << "Khong co lua chon nay, xin moi nhap lai !\n";
		break;
	}
	 return Block();
}
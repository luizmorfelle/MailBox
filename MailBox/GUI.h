#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <windows.h>
#include <conio.h>
#include "Email.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

MailBox mail;
bool Exit = false;
bool inMenu = false;


void choose(int);
void Menu();
void controlCursor(int, int, int, int);

void hidecursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void cursor(int menuPosition, int posX, int posY, int maxPos, int jumpLines) {
	for (int i = 0; i <= maxPos; i++) {
		if (i == menuPosition) {
			gotoxy(posX, (i * jumpLines) + posY);
			putchar('>');
		}
		else {
			gotoxy(posX, (i * jumpLines) + posY);
			putchar(' ');
		}
	}
}
void drawMenu(int x, int y) {
	gotoxy(40, 1);
	cout << "Inlook Personal E-Mail";
	gotoxy(x, y);
	cout << "\tNew Folder";
	gotoxy(x, y + 2);
	cout << "\tDelete Folder";
	gotoxy(x, y + 4);
	cout << "\tNew E-Mail";
	gotoxy(x, y + 6);
	cout << "\tFolders";
	gotoxy(x, y + 8);
	cout << "\tExit";
}
void Menu() {
	inMenu = true;
	system("cls");
	system("color 0E");
	hidecursor();
	drawMenu(5, 2);
	controlCursor(5, 2, 4, 2);
}
void controlCursor(int posX, int posY, int maxPos,int jumpLines) {
	Exit = false;
	int menuPosition = 0;
	while (!Exit) {
		cursor(menuPosition, posX, posY, maxPos,jumpLines);
		char key = _getch();

		switch (key) {
		case 72 : case 'w': case 'W':
			menuPosition -= 1;
			if (menuPosition < 0) menuPosition = maxPos;
			break;

		case 80: case 's': case 'S':
			menuPosition += 1;
			if (menuPosition > maxPos) menuPosition = 0;
			break;

		case 13:
			choose(menuPosition);
			break;

		case 27:
			Menu();
			break;
		}
	}

}

void startGui(MailBox mailBox) {
	mail = mailBox;
	Menu();
}

void createFolder() {
	string name;
	gotoxy(40, 5);
	cout << "Enter folders name: ";
	cin >> name;

	createNewFolder(mail, name);
	Menu();
}

void deleteFolderG() {
	string name;
	gotoxy(40, 5);
	cout << "Enter folders name: ";
	cin >> name;

	deleteFolder(mail, name);
	Menu();
}
void newEmail() {
	auto* nav = new Email;
	cin.ignore();
	gotoxy(40, 5);
	cout << "Type the recipient: ";     // Digite o destinatário
	getline(cin, nav->receiver);

	gotoxy(40, 7);
	cout << "Type the subject: ";       // Digite o assunto
	getline(cin, nav->subject);

	gotoxy(40, 9);
	cout << "Type the content: ";       // Digite o conteudo.
	getline(cin, nav->content);

	struct tm newtime;

	time_t t = time(0);   // get time now
	errno_t err = localtime_s(&newtime,&t);

	nav->time.date.year     = newtime.tm_year + 1900;
	nav->time.date.month    = newtime.tm_mon + 1;
	nav->time.date.day      = newtime.tm_mday;
	nav->time.schedule.hour = newtime.tm_hour;
	nav->time.schedule.min  = newtime.tm_min;
	nav->time.schedule.sec  = newtime.tm_sec;

	//////

	char answer;
	gotoxy(40, 15);
	cout << "Do you want to send this email? (y/n) ";
	cin >> answer;
	int choose;
	if (answer == 'y' || answer == 'Y') {
		choose = 1;
	}
	else {
		choose = 3;
	}
	moveEmail(mail, nav, choose);
	Menu();
}

void showFoldersInMailBox() {
	for (int i = 0; i < mail.size; ++i) {
		gotoxy(40, 5 + (i * 2));
		cout << "  " << mail.folder[i].name << endl;
	}
	controlCursor(40, 5, mail.size, 2);
}

void showTimeStamp(MailBox mailBox, Email* email) {
	TimeStamp nav = email->time;

	cout << nav.schedule.hour << ":" << nav.schedule.min << ":" << nav.schedule.sec << "  " << nav.date.month << "/"
		<< nav.date.day << "/" << nav.date.year;
}

void showActualFolder(MailBox mail) {
	system("cls");
	drawMenu(5, 2);
	int y = 4;

	if (verifyEmptyFolder(mail, getActualFolder(mail))) {
		gotoxy(40, y);
		cout << "Empty Folder!" << endl;
	}
	else {
		Email* nav = mail.folder[getActualFolder(mail)].firstEmail;
		while (nav != nullptr) {
			gotoxy(40, y + 1);
			cout << nav->receiver + "  " + nav->subject << "                ";

			showTimeStamp(mail, nav);
			gotoxy(40, y + 3);
			cout << nav->content;

			nav = nav->next;
			y = y + 5;
		}
	}
	controlCursor(37, 5, numberOfMessagesUnread(mail, getActualFolder(mail)) - 1, 5);
	Menu();
}
void showOnlyUnread(MailBox mail) {
	system("cls");
	drawMenu(5, 2);
	int y = 4;

	if (verifyEmptyFolder(mail, getActualFolder(mail))) {
		gotoxy(40, y);
		cout << "Empty Folder!" << endl;
	}
	else {
		Email* nav = mail.folder[getActualFolder(mail)].firstEmail;
		while (nav != nullptr) {
			if (!nav->read) {

				gotoxy(40, y + 1);
				cout << nav->receiver + "  " + nav->subject << "                ";

				showTimeStamp(mail, nav);
				gotoxy(40, y + 3);
				cout << nav->content;

				nav = nav->next;
				y = y + 5;
			}
		}
	}
	controlCursor(37, 5, numberOfMessagesUnread(mail, getActualFolder(mail)) - 1, 5);
	Menu();
}
bool onlyRead() {
	char choose;
	system("cls");
	drawMenu(5, 2);
	gotoxy(40, 5);
	cout << "See only unread (y/n) ?";
	cin >> choose;

	if(choose == 'Y' or choose == 'y'){
		return true;
	}else{
		return false;
	}

	
}


void choose(int pos) {

	if (inMenu) {
		inMenu = false;
		switch (pos) {
		case 0:
			createFolder();
			break;
		case 1:
			deleteFolderG();
			break;
		case 2:
			newEmail();
			break;
		case 3:
			showFoldersInMailBox();
			break;
		case 4:
			Exit = true;
			break;
		}
	}
	else {
		system("cls");
		setActualFolder(mail, pos);

		if(onlyRead()){
			showOnlyUnread(mail);
			
		}else{
			showActualFolder(mail);
		}
		
		_getch();
	}

}

#include <iostream>
#include <stdio.h> // função auxiliar do GUI
#include <chrono> // Biblioteca para trabalhar com tempo
#include <string> // Função para trabalhar com string 
#include <windows.h> // libera algumas operações de console
#include <conio.h> // Função auxiliar do GUI
#include "Email.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

MailBox mail;				  // Caixa de email atual
bool Exit = false; 			  // Variavel de controle se quer sair
bool inMenu = false;		  // Variavel de controle se está no Menu
bool inFolder = false;		  // Variavel de controle se está no Pasta 
bool inMessageMenu = false;	  // Variavel de controle se está no Email
Email* navigator;			  // Ponteiro para ter acesso global 


void choose(int);
void Menu();
void controlCursor(int, int, int, int);

void hidecursor() {  //Função para esconder o cursor
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y) { //Função para o cursor se mover na tela
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void cursor(int menuPosition, int posX, int posY, int maxPos, int jumpLines) { // Função que desenha o cursor na tela
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
void drawMenu(int x, int y) { // Função que desenha o Menu
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
void Menu() {	  // Função que controla o Menu
	inMenu = true; 
	inFolder = false;
	inMessageMenu = false;
	system("cls");	           // Limpa a tela
	system("color 0E");        // Muda a cor 
	hidecursor();		       // Esconde o cusor
	drawMenu(5, 2);            // Desenha o Menu
	controlCursor(5, 2, 4, 2); // Mostra o cursor e controla ele
}
void controlCursor(int posX, int posY, int maxPos,int jumpLines) { // Controla o cursor 
	Exit = false;
	int menuPosition = 0; // Posição no Menu
	while (!Exit) {
		cursor(menuPosition, posX, posY, maxPos,jumpLines); // Controla cursor no Menu
		char key = _getch();

		switch (key) {
		case 72 : case 'w': case 'W': // Cursor pra cima
			menuPosition -= 1;
			if (menuPosition < 0) menuPosition = maxPos;
			break;

		case 80: case 's': case 'S': // Cursor pra baixo
			menuPosition += 1;
			if (menuPosition > maxPos) menuPosition = 0;
			break;

		case 13: // Ao apertar Enter envia a posição do cursor 
			choose(menuPosition);
			break;

		case 27: // Caso aperte ESC,volta ao menu
			Menu();
			break;
		}
	}

}

void startGui(MailBox mailBox) { // Inicia o GUI
	mail = mailBox;
	Menu();
}

void createFolder() { // Criar Pasta
	string name;
	gotoxy(40, 5);
	cout << "Enter folders name: ";
	cin >> name;

	createNewFolder(mail, name);
	Menu();
}

void deleteFolderG() { // Apaga pasta
	string name;
	gotoxy(40, 5);
	cout << "Enter folders name: ";
	cin >> name;

	deleteFolder(mail, name);
	Menu();
}
void newEmail() { // Cria e recebe novo email
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

	time_t t = time(0);					// recebe o tempo atual do computador 
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
	cout << "Do you want to send this email? (y/n) "; // Verifica se o usuário deseja enviar o email ou salvar como rascunho
	cin >> answer;
	int choose;
	if (answer == 'y' || answer == 'Y') {
		choose = 1;
	}
	else {
		choose = 3;
	}
	moveEmail(mail, nav, choose, choose);
	Menu();
}

void showFoldersInMailBox() { // Mostra todas as pastas da caixa de email
	for (int i = 0; i < mail.size; ++i) {
		gotoxy(40, 5 + (i * 2));
		cout << "  " << mail.folder[i].name << " | Number of Mails: " <<numberOfMessagesOfFolder(mail, i) <<"|" <<endl;
	}
	controlCursor(40, 5, mail.size - 1 , 2);
}

void showTimeStamp(MailBox mailBox, Email* email) { // Mostra o tempo 
	TimeStamp nav = email->time;

	cout << nav.schedule.hour << ":" << nav.schedule.min << ":" << nav.schedule.sec << "  " << nav.date.month << "/"
		<< nav.date.day << "/" << nav.date.year;
}

void showActualFolder(MailBox mail) { // Mostra todos os emails da pasta selecionada
	system("cls");
	drawMenu(5, 2);
	int y = 4;

	if (verifyEmptyFolder(mail, getActualFolder(mail))) { // Verifica se a pasta está vazia
		gotoxy(40, y);
		cout << "Empty Folder!" << endl;
		_getch();
		Menu();
	}
	else {
		Email* nav = mail.folder[getActualFolder(mail)].firstEmail;
		while (nav != nullptr) {
			
			gotoxy(40, y + 1);
			if (nav->read) cout << "Read - ";
			else cout << "NotRead - ";
			
			cout << nav->receiver + "  " + nav->subject << "                ";

			showTimeStamp(mail, nav);
			gotoxy(40, y + 3);
			cout << nav->content;

			nav = nav->next;
			y = y + 5;

		}
		controlCursor(37, 5, numberOfMessagesUnread(mail, getActualFolder(mail)) - 1, 5);
		
	}
	
}
void showOnlyUnread(MailBox mail) { // Mostra apenas os emails não lidos
	system("cls");
	drawMenu(5, 2);
	int y = 4;

	if (verifyEmptyFolder(mail, getActualFolder(mail))) {
		gotoxy(40, y);
		cout << "Empty Folder!" << endl;
		_getch();
		Menu();
	}
	else {
		Email* nav = mail.folder[getActualFolder(mail)].firstEmail;
		while (nav != nullptr) {
			if (!nav->read) {
			
				gotoxy(40, y + 1);
				if (nav->read) cout << "Read - ";
				else cout << "NotRead - ";
				cout << nav->receiver + "  " + nav->subject << "                ";

				showTimeStamp(mail, nav);
				gotoxy(40, y + 3);
				cout << nav->content;

				
				y = y + 5;
			}
			nav = nav->next;

		}
		controlCursor(37, 5, numberOfMessagesUnread(mail, getActualFolder(mail)) - 1, 5);
		
	}
	
}
bool onlyRead() { // Pergunta ao usuário se ele quer ver apenas os não lidos
	char choose;
	system("cls");
	drawMenu(5, 2);
	gotoxy(40, 5);
	cout << "See only unread (y/n) ?: ";
	cin >> choose;

	if(choose == 'Y' or choose == 'y'){
		return true;
	}else{
		return false;
	}

	
}

Email* navMail(int pos){ // Função que retorna o email desejado
	Email* nav = mail.folder[getActualFolder(mail)].firstEmail;
	int count = 0;
		while (count < pos) {
			nav = nav->next;
			count++;
		}
		return nav;
}

void messageMenu() { // Mostra as opções dos emails
	inMessageMenu = true;
	
	system("cls");
	gotoxy(5, 5);
	cout << "\tMark as Read/Unread";
	gotoxy(5, 7);
	cout << "\tDelete Email";
	gotoxy(5, 9);
	cout << "\tMove Email";
	gotoxy(5, 11);
	cout << "\tCancel";
	controlCursor(5, 5, 3, 2);

}
void choose(int pos) { // Função que controla todas as escolhas dos Menus

	if (inMenu) { //  Se está no menu
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
	else if (inFolder){ //  Se está na pasta
		inFolder = false;
		Email* nav = navMail(pos);
		navigator = nav;
		gotoxy(0, 0);
		messageMenu();


	} else if(inMessageMenu){ // Se está no EMail dentro da pasta
		inMessageMenu = false;
		string name;
		switch (pos) {
		case 0: // seta se está lido ou não lido
			setReaded(navigator);
			Menu();
			break;
		case 1:
			moveEmail(mail, navigator, 2, mail.actualFolder); // Move o email para a lixeira
			Menu();
			break;
		case 2: // move email para uma pasta especifica
			system("cls");
			gotoxy(7, 7);
			cin.ignore();
			cout << "Which folder do you want to move to? : ";
			getline(cin, name);
			moveEmail(mail, navigator, foundFolder(mail, name),mail.actualFolder);
			Menu();
			break;
		case 3: // cancela a operação e volta para o menu principal
			Menu();
			break;
		}
	}
	
	
	else{ // Mostra os emails 
		system("cls");
		setActualFolder(mail, pos);
		inFolder = true;
		if(onlyRead()){
			showOnlyUnread(mail);
		}else{
			showActualFolder(mail);
		}
		_getch();
		Menu();
	}

}

#pragma warning(disable : 4996)
#include <iostream>
#include <chrono>
#include "Email.h"
#include <windows.h>

using namespace std;

void startMailBox(MailBox& mailBox) {
	mailBox.folder[0].firstEmail = nullptr;
	mailBox.folder[1].firstEmail = nullptr;//
	mailBox.folder[2].firstEmail = nullptr;//
	mailBox.folder[3].firstEmail = nullptr;//
	mailBox.folder[4].firstEmail = nullptr;//

	mailBox.folder[0].name = "Inbox";    // Caixa de Entrada
	mailBox.folder[1].name = "Sent";     // Enviados
	mailBox.folder[2].name = "Trash";    // Lixeira
	mailBox.folder[3].name = "Drafts";   // Rascunhos
	mailBox.size = 4;                    // Define o numero de pastas inicial para 4
	mailBox.actualFolder = 0;            // Define a pasta atual como Inbox
}

/////// FOLDER FUNCTIONS ///////

void createNewFolder(MailBox& mailBox, const string& name) {
	if (foundFolder(mailBox, name) != -1) {
		cout << "Invalid name!" << endl;
		Sleep(250);
	}
	else if (mailBox.size == 10) {
		cout << "No more space!" << endl;
		Sleep(250);
	}
	else {
		mailBox.folder[mailBox.size].name = name;
		mailBox.folder[mailBox.size].firstEmail = nullptr;
		mailBox.size++;
	}
}

int foundFolder(const MailBox& mailBox, const string& name) {
	int control = 0;
	for (int i = 0; i < mailBox.size; ++i) {
		if (mailBox.folder[i].name == name) {
			return control;
		}
		else {
			control++;
		}
	}
	return -1;

}

void deleteFolder(MailBox& mailBox, const string& name) {
	if (foundFolder(mailBox, name) == -1 || name == "Inbox" || name == "Sent" || name == "Trash" || name=="Drafts") {
		cout << "Invalid name!" << endl;
		Sleep(250);
	}
	else {
		for (int i = foundFolder(mailBox, name); i <= mailBox.size; ++i) {
			mailBox.folder[i] = mailBox.folder[i + 1];
		}
		mailBox.size--;
	}

}

void moveBetweenFolders(MailBox& mailBox, const string& name) {
	if (foundFolder(mailBox, name) == -1) {
		cout << "Invalid name!" << endl;
		Sleep(250);
	}
	else {
		mailBox.actualFolder = foundFolder(mailBox, name);
	}
}

bool verifyEmptyFolder(MailBox mailBox, int num) {
	if (mailBox.folder[num].firstEmail == nullptr) return true;
	else return false;

}

int getActualFolder(MailBox mailBox) {
	return mailBox.actualFolder;
}

void setActualFolder(MailBox& mailBox, int num) {
	mailBox.actualFolder = num;
}



/////// EMAIL FUNCTIONS ///////

Email* getLastEmail(MailBox mailBox) {
	Email* nav = mailBox.folder[getActualFolder(mailBox)].firstEmail;

	while (nav->next != nullptr) {
		nav = nav->next;
	}
	return nav;
}

void receiveNewEmail(MailBox& mailBox) {
	setActualFolder(mailBox, 0);
	Email* lastEmail = receiveRandomData(mailBox);
	moveEmail(mailBox, lastEmail, 0);
}


void moveEmail(MailBox& mailBox, Email* email, int choose) {
	setActualFolder(mailBox, choose);
	if (verifyEmptyFolder(mailBox, choose)) {
		mailBox.folder[getActualFolder(mailBox)].firstEmail = email;
		email->next = nullptr;
	}
	else {
		email->next = mailBox.folder[getActualFolder(mailBox)].firstEmail;
		mailBox.folder[getActualFolder(mailBox)].firstEmail = email;
	}
}





string randomName() {
	string names[10] = { "Paul", "Steve", "Peter", "Mary", "Bob", "Sophie", "Joe", "Ellie", "Samuel", "James" };

	return names[rand() % 10];

}

string randomSubject() {
	string subjects[10] = { "Party", "Reunion", "CoffeeBreak", "Married", "Work", "Honey", "Lunch", "Business",
						   "Miss you", "Hello" };

	return subjects[rand() % 3];

}

string randomContent() {
	string content[10] = {
			"Praesent fermentum turpis non metus imperdiet, vitae tempus mauris maximus. ",
			"Donec a turpis nisi. Donec aliquam arcu lectus, in lobortis odio facilisis nec.",
			"Maecenas laoreet, neque a varius luctus, leo urna sollicitudin ligula,",
			"sit amet scelerisque ante elit at erat.",
			"5",
			"6",
			"7",
			"8",
			"9",
			"10" };

	return content[rand() % 3];

}

Email* receiveRandomData(MailBox& mailBox) {

	auto* nav = new Email;

	nav->receiver = randomName();

	nav->subject = randomSubject();

	nav->content = randomContent();

	struct tm newtime;

	time_t t = time(0);   // get time now
	errno_t err = localtime_s(&newtime, &t);

	nav->time.date.year = newtime.tm_year + 1900;
	nav->time.date.month = newtime.tm_mon + 1;
	nav->time.date.day = newtime.tm_mday;
	nav->time.schedule.hour = newtime.tm_hour;
	nav->time.schedule.min = newtime.tm_min;
	nav->time.schedule.sec = newtime.tm_sec;

	return nav;

}

void setReaded(Email* email) {
	email->read = email->read ? email->read = false : email->read = true;
	cout << email->read;
}


int numberOfMessagesOfMailBox(MailBox mailBox) {
	int cont = 0;
	for (int i = 0; i < mailBox.size; i++) {
		cont += numberOfMessagesOfFolder(mailBox, i);
	}
	return cont;
}

int numberOfMessagesOfFolder(MailBox& mailBox, int number) {

	int cont = 0;

	if (verifyEmptyFolder(mailBox, number)) {
		return cont;
	}
	else {

		Email* nav = mailBox.folder[number].firstEmail;

		while (nav != nullptr) {
			cont++;
			nav = nav->next;
		}
		return cont;

	}

}

int numberOfMessagesUnread(MailBox& mailBox, int number)
{
	int cont = 0;

	if (verifyEmptyFolder(mailBox, number)) {
		return cont;
	}
	else {

		Email* nav = mailBox.folder[number].firstEmail;

		while (nav != nullptr) {
			if (!nav->read) {
				cont++;
			}
			nav = nav->next;
		}
		return cont;

	}
}





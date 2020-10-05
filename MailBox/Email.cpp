#pragma warning(disable : 4996)
#include <iostream>
#include <chrono>
#include "Email.h"
#include <windows.h>

using namespace std;

void startMailBox(MailBox& mailBox) {
	mailBox.folder[0].firstEmail = nullptr; //Inicializa o inbox
	mailBox.folder[1].firstEmail = nullptr; //Inicializa o Sent
	mailBox.folder[2].firstEmail = nullptr; //Inicializa o Trash
	mailBox.folder[3].firstEmail = nullptr; //Inicializa o Draft


	mailBox.folder[0].name = "Inbox";       // Caixa de Entrada
	mailBox.folder[1].name = "Sent";        // Enviados
	mailBox.folder[2].name = "Trash";       // Lixeira
	mailBox.folder[3].name = "Drafts";      // Rascunhos
	mailBox.size = 4;                       // Define o numero de pastas inicial para 4
	mailBox.actualFolder = 0;               // Define a pasta atual como Inbox
}

/////// FOLDER FUNCTIONS ///////

void createNewFolder(MailBox& mailBox, const string& name) { // Função para criar nova Pasta
	if (foundFolder(mailBox, name) != -1) { //se a função não retornar -1 é não foi encontrado dentro da pasta o nome pelo qual procuramos
		cout << "Invalid name!" << endl;
		Sleep(250);
	}
	else if (mailBox.size == 10) { // tamanho maximo de pastas
		cout << "No more space!" << endl;
		Sleep(250);
	}
	else { // uma nova pasta e cria uma nova lista encadeada simples contida dentro dessa pasta
		mailBox.folder[mailBox.size].name = name;
		mailBox.folder[mailBox.size].firstEmail = nullptr;
		mailBox.size++;
	}
}

int foundFolder(const MailBox& mailBox, const string& name) { //Função que busca uma pasta pelo nome e retorn um número inteiro
	int control = 0; 
	for (int i = 0; i < mailBox.size; ++i) { // procura dentro do vetor de pastas
		if (mailBox.folder[i].name == name) { // quando encontrar uma pasta com o nome igual ao que procuramos
			return control; // vai retornar o indice da pasta
		}
		else {
			control++; 
		}
	}
	return -1; // se n encontrar nenhuma pasta com esse nome retorna -1

}

void deleteFolder(MailBox& mailBox, const string& name) { // Função que apaga uma pasta
	if (foundFolder(mailBox, name) == -1 || name == "Inbox" || name == "Sent" || name == "Trash" || name=="Drafts") { // ele vai procurar uma pasta com o nome que escrevemos e se não encontrar entra no if, e impede que apaguemos as pastas prinicpais
		cout << "Invalid name!" << endl; 
		Sleep(250); // espera 1/4 de segundo
	}
	else { // se a pasta que queremos apagar existir
		for (int i = foundFolder(mailBox, name); i <= mailBox.size; ++i) { // então faremos uma remoção na posição
			mailBox.folder[i] = mailBox.folder[i + 1];
		}
		mailBox.size--;
	}

}


bool verifyEmptyFolder(MailBox mailBox, int num) {  // Função que verifica se a pasta está vazia
	if (mailBox.folder[num].firstEmail == nullptr) return true; // se a referencia de primeiro for nula ele retorna que a pasta está vazia
	else return false;

}

int getActualFolder(MailBox mailBox) { // pega o indice da pasta que estamos navegando no momento
	return mailBox.actualFolder;
}

void setActualFolder(MailBox& mailBox, int num) { // muda a pasta que estamos navegando no momento
	mailBox.actualFolder = num;
}



/////// EMAIL FUNCTIONS ///////

Email* getLastEmail(MailBox mailBox) { //pega a referencia do ultimo email
	Email* nav = mailBox.folder[getActualFolder(mailBox)].firstEmail; // cria um navegador a partir da referencia de primeiro elemento

	while (nav->next != nullptr) { // navega até o ultimo elemento
		nav = nav->next;
	}
	return nav; // retorna a referencia de ultimo elemento
}

void receiveNewEmail(MailBox& mailBox) { //cria um email aleatório a partir dos dados pré estabelecidos
	setActualFolder(mailBox, 0);
	Email* lastEmail = receiveRandomData(mailBox);
	moveEmail(mailBox, lastEmail, 0, 0);
}


void moveEmail(MailBox& mailBox, Email* email, int choose, int origin) { // função de adicionar e mover emails de pasta
	Email* nav = mailBox.folder[origin].firstEmail;// cria um navegador a partir da referencia do primeiro elemento

	if(nav == email){ // se o elemento que queremos mover ou adicionar for o primeiro elemento
		mailBox.folder[origin].firstEmail = email->next; // atualiza a referencia de primeiro elemento na pasta de origem
	}else{
		if (choose != origin) { // verifica se nào queremos mover para a mesma pasta

			if (nav->next != nullptr) { // verifica se n é o ultimo elemento
				while (nav->next != email) { // navega até o elemento-1 que queremos mover
					nav = nav->next;
				}
				nav->next = email->next; // pegamos a referencia de proximo do elemento que queremos mover
			}

		}

	}
	setActualFolder(mailBox, choose); //muda a pasta atual para a pasta de destino

	if (verifyEmptyFolder(mailBox, choose)) { // se a pasta estiver vazia
		mailBox.folder[getActualFolder(mailBox)].firstEmail = email; // ele adiciona na primeira posição
		email->next = nullptr; // referencia de proximo vazia
	}
	else {
		email->next = mailBox.folder[getActualFolder(mailBox)].firstEmail; // referencia de proximo é o antigo primeiro elemento
		mailBox.folder[getActualFolder(mailBox)].firstEmail = email; // diz que o novo dado é o primeiro elemento
	}
}





string randomName() { // gera nomes aleatorios
	string names[10] = { "Paul", "Steve", "Peter", "Mary", "Bob", "Sophie", "Joe", "Ellie", "Samuel", "James" };

	return names[rand() % 10];

}

string randomSubject() { // Assuntos aleatorios
	string subjects[10] = { "Party", "Reunion", "CoffeeBreak", "Married", "Work", "Honey", "Lunch", "Business",
						   "Miss you", "Hello" };

	return subjects[rand() % 3];

}

string randomContent() { // conteudos dos emails aleatorios
	string content[10] = {
			"Praesent fermentum turpis non metus imperdiet, vitae tempus mauris maximus. ",
			"Donec a turpis nisi. Donec aliquam arcu lectus, in lobortis odio facilisis nec.",
			"Maecenas laoreet, neque a varius luctus, leo urna sollicitudin ligula,",
			"sit amet scelerisque ante elit at erat.",
			"sit amet scelerisque ante elit at erat.",
			"sit amet scelerisque ante elit at erat.",
			"sit amet scelerisque ante elit at erat.",
			"sit amet scelerisque ante elit at erat.",
			"sit amet scelerisque ante elit at erat.",
			"sit amet scelerisque ante elit at erat." };

	return content[rand() % 3];

}

Email* receiveRandomData(MailBox& mailBox) { // junta os dados para criar um email aleatorio de testes

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

void setReaded(Email* email) { // seta um email que foi lidos
	email->read = email->read ? email->read = false : email->read = true;
}


int numberOfMessagesOfMailBox(MailBox mailBox) { // conta quantos emails tem na caixa de emails
	int cont = 0;
	for (int i = 0; i < mailBox.size; i++) {
		cont += numberOfMessagesOfFolder(mailBox, i);
	}
	return cont;
}

int numberOfMessagesOfFolder(MailBox& mailBox, int number) { // quantos emails tem numa pasta especifica

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

int numberOfMessagesUnread(MailBox& mailBox, int number) // quantas mensagens não foram lidas
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





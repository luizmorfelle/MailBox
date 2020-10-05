#ifndef CAIXA_DE_E_MAIL_EMAIL_H
#define CAIXA_DE_E_MAIL_EMAIL_H

#include <iostream>
using namespace std;

struct Schedule { // Estrutura do horário
    int hour; // Hora
    int min;  // Minuto
    int sec;  // Segundo
};

struct Date { // Estrutura da data
    int day;    //Dia
    int month;  //Mês
    int year;   //Ano
};

struct  TimeStamp {     // Estrutura de tempo
    Date date;          // Data 
    Schedule schedule;  // Horário
};

struct Email {    //Estrutura do Email
    bool read = false;  //Variável para saber se o email foi lido
    string receiver;    //Variável para destinatário
    string subject;     //Variável para remetente
    TimeStamp time;     //Variável para guardar a data da mensagem 
    string content;     //Variável para guardar o conteudo do email
    Email* next;        //Ponteiro o próximo email
};

struct Folder {  // Estrutura das pasta
    string name; // Nome da pasta
    Email* firstEmail; // Ponteiro para o primeiro email

};

struct MailBox { // Estrutura da caixa de Email
    Folder folder[10]; // Array com 10 posições para as pastas 
    int actualFolder; // variável de controle para saber em qual pasta está
    int size = 0;     // variável de controle para o tamanho da lista
};


void startMailBox(MailBox&);

void createNewFolder(MailBox&, const string&); 

int foundFolder(const MailBox&, const string&);

void deleteFolder(MailBox&, const string&);

bool verifyEmptyFolder(MailBox, int);

int getActualFolder(MailBox);

void setActualFolder(MailBox&, int);

void showFolders(MailBox);

void showActualFolder(MailBox);

Email* getLastEmail(MailBox);

void createNewEmail(MailBox&);

Email* receiveEmailData(MailBox&);

void moveEmail(MailBox&, Email*, int, int);

void receiveNewEmail(MailBox&);

void showTimeStamp(MailBox, Email*);

Email* receiveRandomData(MailBox&);

void setReaded(Email*);

int numberOfMessagesOfMailBox(MailBox);

int numberOfMessagesOfFolder(MailBox&, int);

int numberOfMessagesUnread(MailBox&, int);



#endif //CAIXA_DE_E_MAIL_EMAIL_H

#ifndef CAIXA_DE_E_MAIL_EMAIL_H
#define CAIXA_DE_E_MAIL_EMAIL_H

#include <iostream>
using namespace std;

struct Schedule{
    int hour;
    int min;
    int sec;
};

struct Date {
    int day;
    int month;
    int year;
};

struct  TimeStamp {
    Date date;
    Schedule schedule;
};

struct Email{
    bool read = false;
    string receiver;
    string subject;
    TimeStamp time;
    string content;
    Email* next;
};

struct Folder {
    string name;
    Email* firstEmail;

};

struct MailBox {
    Folder folder[10];
    int actualFolder;
    int size = 0;
};


void startMailBox(MailBox&);

void createNewFolder(MailBox&,const string&);

int foundFolder(const MailBox&, const string&);

void deleteFolder(MailBox&,const string&);

void moveBetweenFolders(MailBox&,const string&);

bool verifyEmptyFolder(MailBox, int);

int getActualFolder(MailBox);

void setActualFolder(MailBox&, int);

void showFolders (MailBox);

void showActualFolder(MailBox);

Email * getLastEmail(MailBox);

void createNewEmail(MailBox&);

Email* receiveEmailData(MailBox&);

void moveEmail(MailBox&,Email*,int);

void receiveNewEmail(MailBox&);

void showTimeStamp(MailBox,Email*);

Email* receiveRandomData(MailBox&);

void setReaded(Email*);

int numberOfMessagesOfMailBox(MailBox);

int numberOfMessagesOfFolder(MailBox&, int);



#endif //CAIXA_DE_E_MAIL_EMAIL_H

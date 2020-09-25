#include <iostream>
#include "Email.h"
#include "GUI.h"

using namespace std;

void start(){
    MailBox mail;
    startMailBox(mail);
    receiveNewEmail(mail);
    receiveNewEmail(mail);
    receiveNewEmail(mail);
    startGui(mail);
}

int main() {

    start();
}

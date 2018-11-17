#include "weshare.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    WeShare w;
    w.show();

    return a.exec();
}

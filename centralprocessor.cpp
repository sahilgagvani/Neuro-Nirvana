#include "centralprocessor.h"
#include <iostream>
#include <fstream>
using namespace std;


centralprocessor::centralprocessor(QObject *parent):QObject(parent){
    treatements = 0;
}

//To save the current session log, todo: Ashneet
void centralprocessor::saveData(){
    fstream dataFile;
    dataFile.open("savedLogs.txt",fstream::app);

    if(!dataFile){
        dataFile.open("savedLogs.txt",fstream::out);
    }

    dataFile.close();

}

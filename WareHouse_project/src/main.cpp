#include "WareHouse.h"
#include <iostream>

using namespace std;

WareHouse* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: warehouse <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1]; //"/home/users/bsc/guyratz/Downloads/Skeleton/src/configFileExample.txt";
    WareHouse wareHouse(configurationFile);
    wareHouse.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}
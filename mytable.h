#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <cstring>

#define MAX_ROW 1024
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 1024
using namespace std;

class myrow{
    char *item[5];
    int sig; //when sig = 1, means that it is an introduction row
public:
    myrow(){sig = 0;}
    void setRow(int n,char * content){
        //cout<<"@"<<endl;
        item[n] = new char[MAX_VALUE_NAME];
        strcpy(item[n],content);
        //cout<<"@"<<endl;
    }
    void setIntro(char* content){sig = 1;;strcpy(item[0],content);}
    bool isIntro(){return sig==1?true:false;}
    char* getItem(int i){return item[i];}
};

class mytable{
    myrow tbl[MAX_ROW];
    int nrow;
public:
    mytable(){nrow = 0;}
    void setItem(int row,int col,char* ctt){
        if(!tbl[row].isIntro()){
            
            tbl[row].setRow(col,ctt);
        }
        else{cout<<"can not set item to an intro row"<<endl;}
    }
    void appendRow(char*a,char*b,char*c,char*d,char*e){
        setItem(nrow,0,a);
        setItem(nrow,1,b);
        setItem(nrow,2,c);
        setItem(nrow,3,d);
        setItem(nrow,4,e);
        this->nrow++;
        cout<<"the row num is"<<nrow<<endl;
    }
    void setIntro(int row,char * ctt){
        tbl[row].setIntro(ctt);
    }
    void printTable(){
        cout<<"the table is:"<<nrow<<endl;
        for(int i=0;i<nrow;i++){
            for(int j=0;j<5;j++){
                cout<<"\t"<<tbl[i].getItem(j);
            }
            cout<<endl;
        }
    }
};

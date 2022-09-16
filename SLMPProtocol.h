#ifndef SLMPPROTOCOL_H
#define SLMPPROTOCOL_H
#include <QObject>
#include <QAbstractSocket>
#include <QDebug>
#include <vector>
#include <QElapsedTimer>
#include <QThread>
#include <cmath>
#include "tcpClient.h"
#include <fstream>
#include <dirent.h>
#include <QDateTime>
#include "calarmlist.h"
#include <QFile>
#include <QDir>

#define REGISTER_AREA_SIZE 3072//3072 //No more than 512
#define FLAG_AREA_SIZE 3072//3072 //No more than 2048
#define LATCH_AREA_SIZE 256 // No more than 2048


#ifdef Q_OS_ANDROID
#define SEND_DELAY 30
#else
#define SEND_DELAY 5
#endif

//SLMP Protocol
using namespace std;

struct SAlarmEntry{
    QString description;
    unsigned short registerAddress;
    bool occured;
    int moduleNumber;
};

struct SAlarmOccurence{
    int alarmId;
    QDateTime occurenceDate;
    QDateTime restoreDate;
    bool occured;
};


class CSLMPProtocol : public QThread{
    Q_OBJECT
public:
    CSLMPProtocol(QThread *parent=0);
    ~CSLMPProtocol();

    Q_INVOKABLE void open();
    Q_INVOKABLE void reopen();
    Q_INVOKABLE void close();
    void waitForConnected();
    Q_INVOKABLE bool isOpen(){return socket.isOpen();}
    Q_INVOKABLE bool isConnected(){return connectedState;}
    bool isWaiting(){return waitingForResponse;}
    void refreshRegistersRequest();
    void refreshFlagsRequest();
    void refreshLatchRequest();
    void writeDRegister(unsigned short noOfRegister,short value);
    void writeMFlag(unsigned short noOfRegister, bool value);
    void doWriteRequestsFlags();
    void doWriteRequestsRegisters();
    void doWriteRequestsLatch();
    Q_INVOKABLE void startThread();
    void changeConnParameters(QString ip, unsigned short portNum);

    void setOffsetAddress(unsigned short firstD=0,unsigned short firstM=0);
    void readyRead(unsigned short registerWritingOffset=0);


    Q_INVOKABLE   void refreshAll();

    //Reference for writing to data register
   Q_INVOKABLE short D(unsigned short noOfRegister, short value){
        short p=-1;

        if(noOfRegister>=firstRegisterAddress && noOfRegister<REGISTER_AREA_SIZE+firstRegisterAddress){
            noOfRegister-=firstRegisterAddress;
            registerArea[noOfRegister]=value;
            writeRegisterArea[noOfRegister]=registerArea[noOfRegister];
            registerAreaWriteRequest[noOfRegister]=1;
            return writeRegisterArea[noOfRegister];}
        else return p;
    }

//    Q_INVOKABLE short D(unsigned short noOfRegister, short value){
//    D(noOfRegister)=value;
//    }



    //It is faster to use this one
    //simple return for reading from data register
  Q_INVOKABLE short RD(unsigned short noOfRegister){

        if(noOfRegister>=firstRegisterAddress && noOfRegister<REGISTER_AREA_SIZE+firstRegisterAddress){
            noOfRegister-=firstRegisterAddress;
            return registerArea[noOfRegister];}
        else return -1;
    }

    Q_INVOKABLE short M(unsigned short noOfRegister, bool value){
        short p=0;
        if(noOfRegister>=firstFlagAddress && noOfRegister<FLAG_AREA_SIZE+firstFlagAddress){
            noOfRegister-=firstFlagAddress;
            flagArea[noOfRegister]=value;
            writeFlagArea[noOfRegister]=flagArea[noOfRegister];
            flagAreaWriteRequest[noOfRegister]=1;

            return writeFlagArea[noOfRegister];}
        else return p;
    }

//    Q_INVOKABLE short M(unsigned short noOfRegister, bool value){
//        M(noOfRegister,value);
//    }

   Q_INVOKABLE  short RM(unsigned short noOfRegister){
        if(noOfRegister>=firstFlagAddress && noOfRegister<FLAG_AREA_SIZE+firstFlagAddress){
            noOfRegister-=firstFlagAddress;
            return flagArea[noOfRegister];}else return 0;
    }
    //Read double word register
    int RdoubleD(unsigned short noOfRegister){
        int p=-1;

        if(noOfRegister>=firstRegisterAddress && noOfRegister<REGISTER_AREA_SIZE-1+firstRegisterAddress )
           {noOfRegister-=firstRegisterAddress;
            p=(registerArea[noOfRegister]&0xffff) | ((registerArea[noOfRegister+1]<<16)&0xffff0000);
        }
        return p;
    }




   Q_INVOKABLE short L(unsigned short noOfRegister,bool value){
        short p=0;
        if(noOfRegister>=firstLatchAddress && noOfRegister<LATCH_AREA_SIZE+firstLatchAddress){
            noOfRegister-=firstLatchAddress;
            latchArea[noOfRegister]=value;
            writeLatchArea[noOfRegister]=latchArea[noOfRegister];
            latchAreaWriteRequest[noOfRegister]=1;
            return writeLatchArea[noOfRegister];}
        else return p;
    }

//Q_INVOKABLE short L(unsigned short noOfRegister,bool value){
//L(noOfRegister)=value;
//    }

Q_INVOKABLE    short RL(unsigned short noOfRegister){
        if(noOfRegister>=firstLatchAddress && noOfRegister<LATCH_AREA_SIZE+firstLatchAddress){
            noOfRegister-=firstLatchAddress;
            return latchArea[noOfRegister];}else return 0;
    }

    Q_INVOKABLE int DW(unsigned short noOfRegister, long value);
    Q_INVOKABLE int RDW(unsigned short noOfRegister);

    //functions are copy of RD,RDW,D,DW,M,L,RM,RL, implemented to satisfy QML requirement that functions should start from small letter
    Q_INVOKABLE int dW(unsigned short noOfRegister, long value){return DW(noOfRegister,value);}
    Q_INVOKABLE int rDW(unsigned short noOfRegister){return RDW(noOfRegister);}

    Q_INVOKABLE short l(unsigned short noOfRegister,bool value){return L(noOfRegister,value);}
    Q_INVOKABLE short rL(unsigned short noOfRegister){return RL(noOfRegister);}


    Q_INVOKABLE short m(unsigned short noOfRegister,bool value){return M(noOfRegister,value);}
    Q_INVOKABLE short rM(unsigned short noOfRegister){return RM(noOfRegister);}



    Q_INVOKABLE short d(unsigned short noOfRegister, short value){return D(noOfRegister,value);}
    Q_INVOKABLE short rD(unsigned short noOfRegister){return RD(noOfRegister);}

    //read single bit in word
    Q_INVOKABLE short rDB(unsigned short noOfRegister, short bitNumber);
    //set single bit in word
    Q_INVOKABLE short setDB(unsigned short noOfRegister, short bitNumber);
    //reset single bit in word
    Q_INVOKABLE short resetDB(unsigned short noOfRegister, short bitNumber);

    //read single bit in double word
    Q_INVOKABLE long rDWB(unsigned short noOfRegister, long bitNumber);
    //set single bit in word
    Q_INVOKABLE long setDWB(unsigned short noOfRegister, long bitNumber);
    //reset single bit in word
    Q_INVOKABLE long resetDWB(unsigned short noOfRegister, long bitNumber);



    Q_INVOKABLE short saveRecipe(QString recipeName);
    Q_INVOKABLE short saveRecipeSchema(QString recipeName);
    Q_INVOKABLE short loadRecipe(QString recipeName);
    Q_INVOKABLE QString getRecipeList(int num, QString pattern="");
    Q_INVOKABLE void removeRecipe(QString recipeName);
    Q_INVOKABLE void addDtoRecipe(short address,QString comment="UNKNOWN");
    Q_INVOKABLE void addDWtoRecipe(short address,QString comment="UNKNOWN");
    Q_INVOKABLE void addMtoRecipe(short address,QString comment="UNKNOWN");
    Q_INVOKABLE void addLtoRecipe(short address,QString comment="UNKNOWN");
    Q_INVOKABLE int getRecipeListSize(QString pattern);
    Q_INVOKABLE bool isRecipeExist(QString recipeName);
    Q_INVOKABLE bool isRecipeLoaded();
    Q_INVOKABLE QString getLastRecipeName();
    Q_INVOKABLE QString getBarcodeText();



    Q_INVOKABLE void stopThread();

    void takeRecipeSchemaFromFile(const char *filename);

    bool recipeLoaded;
    QString lastRecipeName;

    //Alarm handling

    Q_INVOKABLE void addAlarmEntry(QString description,unsigned short registerAddress,int moduleNumber=-1);
    void processAlarms();
    int findAlarmEntry(unsigned short registerAddress);
    Q_INVOKABLE bool isModuleAtAlarm(int moduleNumber);

    CAlarmList alarmList;
    vector<SAlarmEntry> alarms;
    vector<SAlarmOccurence> alarmHistory;
    bool alarmAtModule[20];

    vector<short> flagArea;
    vector<short> writeFlagArea;

    vector<short> registerArea;
    vector<short> writeRegisterArea;

    vector<short> latchArea;
    vector<short> writeLatchArea;

    vector<bool> registerAreaWriteRequest;
    vector<bool> flagAreaWriteRequest;
    vector<bool> latchAreaWriteRequest;

    vector<short>doubleWordRegisterArea;
    vector<short>wordRegisterArea;
    vector<short>flagRegisterArea;
    vector<short>latchRegisterArea;

    vector<QString>commentDoubleWordRecipe;
    vector<QString>commentWordRecipe;
    vector<QString>commentFlagRecipe;
    vector<QString>commentLatchRecipe;

    QString ipAddress;
    unsigned short port;
    tcp_client socket;
signals:

Q_INVOKABLE void valuesUpdated();
Q_INVOKABLE void alarmListUpdated();

void error(QString err);

private:

   // QByteArray readCacheBytes;
    QByteArray writeCacheBytes;


    bool socketLocked;
    bool waitingForResponse;
    bool allowWrite;
    bool connectedState;
    bool threadStarted;
    unsigned short firstRegisterAddress;
    unsigned short firstFlagAddress;
    unsigned short firstLatchAddress;
    bool badFrame;
    unsigned char commSequence;
    char recipeSeparatingCharacter;

protected:
    void run();
};

bool bitAt(unsigned char bitByte,unsigned short pos);

#endif

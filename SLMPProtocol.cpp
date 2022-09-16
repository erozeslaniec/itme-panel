#include "SLMPProtocol.h"

CSLMPProtocol::CSLMPProtocol(QThread *parent)
    :QThread(parent)
{
    threadStarted=false;
    socketLocked=false;
    allowWrite=false;
    flagArea.resize(FLAG_AREA_SIZE);
    writeFlagArea.resize(FLAG_AREA_SIZE);

    latchArea.resize(LATCH_AREA_SIZE);
    writeLatchArea.resize(LATCH_AREA_SIZE);

    registerArea.resize(REGISTER_AREA_SIZE);
    writeRegisterArea.resize(REGISTER_AREA_SIZE);

    flagAreaWriteRequest.resize(FLAG_AREA_SIZE);
    registerAreaWriteRequest.resize(REGISTER_AREA_SIZE);
    latchAreaWriteRequest.resize(LATCH_AREA_SIZE);

    ipAddress="192.168.3.39";
    port=4014;

    firstRegisterAddress=1700;
    firstFlagAddress=2000;
    firstLatchAddress=2000;

    for(unsigned long i=0;i<registerAreaWriteRequest.size();i++)
        registerAreaWriteRequest[i]=0;
    for(unsigned long i=0;i<flagAreaWriteRequest.size();i++)
        flagAreaWriteRequest[i]=0;
    for(unsigned long i=0;i<latchAreaWriteRequest.size();i++)
        latchAreaWriteRequest[i]=0;

    recipeLoaded=0;
    lastRecipeName="";

    recipeSeparatingCharacter=',';

}

CSLMPProtocol::~CSLMPProtocol()
{

}


void CSLMPProtocol::open(){
    socket.conn(ipAddress.toStdString(),port);

}

void CSLMPProtocol::reopen()
{
 socket.reconnect();
}

void CSLMPProtocol::close(){
    //if(isOpen());
    socket.disconn();
}



void CSLMPProtocol::readyRead(unsigned short registerWritingOffset)
{
    vector<unsigned char> readCacheBytes;
    readCacheBytes=socket.receive(2048);
    unsigned char header=commSequence;
    int noOfRegisters=readCacheBytes.size();

    switch(header){
    case 0x80:allowWrite=true;
        if(readCacheBytes.size()>=FLAG_AREA_SIZE/8+10){
            badFrame=false;
            for(int i=0;i<FLAG_AREA_SIZE;i++){
                flagArea[i]=bitAt(readCacheBytes[i/8+11],i%8);

            }}
        else badFrame=true;
        break;
    case 0x81:if(readCacheBytes.size()>100){ //512
            connectedState=true;
            badFrame=true;
            allowWrite=true;
            if((unsigned char)readCacheBytes[9]==0x00 && (unsigned char)readCacheBytes[10]==0x00){
                badFrame=false;
                int p=readCacheBytes[8]*256;
                noOfRegisters=(int(((unsigned short)readCacheBytes[7])&0xff-2)+p)/2;
                //noOfRegisters=960;
            }

            if(!badFrame)
                for(int i=0;i<noOfRegisters;i++)
                    registerArea[i+registerWritingOffset]=(readCacheBytes[i*2+11]&0xff)|((readCacheBytes[i*2+12]<<8)&0xff00);}

        break;
    case 0x82:allowWrite=true;
        if(readCacheBytes.size()>=LATCH_AREA_SIZE/8+10){
            badFrame=false;
            for(int i=0;i<LATCH_AREA_SIZE;i++){
                latchArea[i]=bitAt(readCacheBytes[i/8+11],i%8);

            }}
        else badFrame=true;
        break;

    case 0x85:;//Random write acknowledgement

    default:;
    }
    socketLocked=false;

}


void CSLMPProtocol::refreshRegistersRequest(){
    char buf[21]={0x50,0x00,0x00,static_cast<char>(0xff),static_cast<char>(0xff),0x03,0x00,0x0c,0x00,0x10,0x00,0x01,0x04,0x00,0x00,static_cast<char>(firstRegisterAddress&0xff),static_cast<char>(((short)firstRegisterAddress&0xff00)>>8),
                0x00,static_cast<char>(0xa8),static_cast<char>(0xff),0x00};
    int packageSize=255;
    //960 is maximum package

    #ifdef Q_OS_ANDROID
        packageSize=800;//800
    #endif


    buf[19]=(packageSize&0xff);
    buf[20]=(short(packageSize)&0xff00)>>8;
    //  0x00,0xa8,char(REGISTER_AREA_SIZE&0xff),((short)REGISTER_AREA_SIZE&0xff00)>>8};

    for(int i=0;i<REGISTER_AREA_SIZE;i+=packageSize+1){

    buf[15]=(i+firstRegisterAddress)&0xff;
    buf[16]=(short((i+firstRegisterAddress))&0xff00)>>8;
    commSequence=0x81;

    socket.send_data(buf,21);
    msleep(SEND_DELAY);

    readyRead(i);
    }

}

void CSLMPProtocol::refreshFlagsRequest(){

    char buf[21]={0x50,0x00,0x00,static_cast<char>(0xff),static_cast<char>(0xff),0x03,0x00,0x0c,0x00,0x10,0x00,0x01,0x04,0x00,0x00,static_cast<char>(firstFlagAddress&0xff),static_cast<char>(((short)firstFlagAddress&0xff00)>>8),
                  0x00,static_cast<char>(0x90),char((FLAG_AREA_SIZE/16)&0xff),0x00};
    commSequence=0x80;

    socket.send_data(buf,21);
    msleep(SEND_DELAY);


    readyRead();


}


void CSLMPProtocol::refreshLatchRequest(){

    char buf[21]={0x50,0x00,0x00,static_cast<char>(0xff),static_cast<char>(0xff),0x03,0x00,0x0c,0x00,0x10,0x00,0x01,0x04,0x00,0x00,static_cast<char>(firstLatchAddress&0xff),static_cast<char>(((short)firstLatchAddress&0xff00)>>8),
                  0x00,static_cast<char>(0x92),char((LATCH_AREA_SIZE/16)&0xff),0x00};
    commSequence=0x82;

    socket.send_data(buf,21);
    msleep(SEND_DELAY);


    readyRead();


}


void CSLMPProtocol::waitForConnected(){
    //socket->waitForConnected();
    socket.isOpen();
}



void CSLMPProtocol::doWriteRequestsRegisters(){
    char buf[520]={0x50,0x00,0x00,static_cast<char>(0xff),static_cast<char>(0xff),0x03,0x00,0x0c,0x00,0x10,0x00,0x02,0x14,0x00,0x00,0x00,0x00};
    //aa - word access points
    //0x0c - frame size

    int iterator=0;



    bool requestOccured=true;
    while(requestOccured){
        iterator=0;
        requestOccured=false;

        for(unsigned short i=0;i<REGISTER_AREA_SIZE;i++)
        {
            if(registerAreaWriteRequest[i] && iterator<10){
                requestOccured=true;
                buf[17+iterator*6+0]=(firstRegisterAddress+i)&0xff;
                buf[17+iterator*6+1]=((firstRegisterAddress+i)&0xff00)>>8;
                buf[17+iterator*6+2]=0x0;
                buf[17+iterator*6+3]=0xa8;
                buf[17+iterator*6+4]=writeRegisterArea[i]&0xff;
                buf[17+iterator*6+5]=(writeRegisterArea[i]&0xff00)>>8;
                registerAreaWriteRequest[i]=false;
                iterator++;
            }
        }
        buf[15]=iterator&0xff;

        unsigned short frameSize=iterator*6+8;
        buf[7]=frameSize&0xff;
        buf[8]=(frameSize&0xff00)>>8;
        if(requestOccured){
            commSequence=0x91;
            socket.send_data(buf,17+6*iterator);
            msleep(SEND_DELAY);
            readyRead();

        }
    }
}


void CSLMPProtocol::doWriteRequestsFlags(){
    char buf[566]={0x50,0x00,0x00,static_cast<char>(0xff),static_cast<char>(0xff),0x03,0x00,0x0c,0x00,0x10,0x00,0x02,0x14,0x01,0x00,static_cast<char>(0xaa)};
    int iterator=0;
    bool requestOccured=true;



    while(requestOccured){
        requestOccured=false;
        iterator=0;
        for(unsigned short i=0;i<FLAG_AREA_SIZE;i++)
        {
            if(flagAreaWriteRequest[i] && iterator<10){
                requestOccured=true;
                buf[16+iterator*5+0]=(firstFlagAddress+i)&0xff;
                buf[16+iterator*5+1]=((firstFlagAddress+i)&0xff00)>>8;
                buf[16+iterator*5+2]=0x0;
                buf[16+iterator*5+3]=0x90;
                buf[16+iterator*5+4]=writeFlagArea[i];
                flagAreaWriteRequest[i]=false;
                iterator++;
            }
        }
        buf[15]=iterator&0xff;
        unsigned short frameSize=iterator*5+7;
        buf[7]=frameSize&0xff;
        buf[8]=(frameSize&0xff00)>>8;


        if(requestOccured){
            commSequence=0x90;
            socket.send_data(buf,16+5*iterator);

            msleep(SEND_DELAY);
            readyRead();

        }
    }
}


void CSLMPProtocol::doWriteRequestsLatch(){
    char buf[566]={0x50,0x00,0x00,static_cast<char>(0xff),static_cast<char>(0xff),0x03,0x00,0x0c,0x00,0x10,0x00,0x02,0x14,0x01,0x00,static_cast<char>(0xaa)};
    int iterator=0;
    bool requestOccured=true;
    while(requestOccured){
        requestOccured=false;
        iterator=0;
        for(unsigned short i=0;i<LATCH_AREA_SIZE;i++)
        {
            if(latchAreaWriteRequest[i] && iterator<10){
                requestOccured=true;
                buf[16+iterator*5+0]=(firstLatchAddress+i)&0xff;
                buf[16+iterator*5+1]=((firstLatchAddress+i)&0xff00)>>8;
                buf[16+iterator*5+2]=0x0;
                buf[16+iterator*5+3]=0x92;
                buf[16+iterator*5+4]=writeLatchArea[i];
                latchAreaWriteRequest[i]=false;
                iterator++;
            }
        }
        buf[15]=iterator&0xff;
        unsigned short frameSize=iterator*5+7;
        buf[7]=frameSize&0xff;
        buf[8]=(frameSize&0xff00)>>8;


        if(requestOccured){
            commSequence=0x92;


            socket.send_data(buf,16+5*iterator);

            msleep(SEND_DELAY);
            readyRead();

        }
    }
}



void CSLMPProtocol::writeDRegister(unsigned short noOfRegister, short value){
    if(noOfRegister>=firstRegisterAddress && noOfRegister<firstRegisterAddress+REGISTER_AREA_SIZE)
    { writeRegisterArea[noOfRegister-firstRegisterAddress]=value;
        registerAreaWriteRequest[noOfRegister-firstRegisterAddress]=true;}
}



void CSLMPProtocol::writeMFlag(unsigned short noOfRegister, bool value){
    if(noOfRegister>=firstFlagAddress && noOfRegister<firstFlagAddress+FLAG_AREA_SIZE)
    { writeFlagArea[noOfRegister-firstFlagAddress]=value;
        flagAreaWriteRequest[noOfRegister-firstFlagAddress]=true;}
}




void CSLMPProtocol::refreshAll(){
    if(!isOpen())
   {    close();
        msleep(1000);
        open();
        msleep(1000);
    }

    else{

        doWriteRequestsFlags();

        doWriteRequestsRegisters();

        doWriteRequestsLatch();


        refreshFlagsRequest();

        refreshRegistersRequest();

        refreshLatchRequest();

        processAlarms();
   }


    valuesUpdated();

}

void CSLMPProtocol::setOffsetAddress(unsigned short firstD, unsigned short firstM){
    firstFlagAddress=firstM;
    firstRegisterAddress=firstD;
}


bool bitAt(unsigned char bitByte,unsigned short pos){
    return (bitByte & (1<<pos))>>pos;
}


int CSLMPProtocol::DW(unsigned short noOfRegister, long value){
    int p=-1;



    if(noOfRegister>=firstRegisterAddress && noOfRegister<firstRegisterAddress-1+REGISTER_AREA_SIZE)
    { writeRegisterArea[noOfRegister-firstRegisterAddress]=short(value&0xffff);
        writeRegisterArea[noOfRegister-firstRegisterAddress+1]=short((value&0xffff0000)>>16);
        registerAreaWriteRequest[noOfRegister-firstRegisterAddress]=true;
        registerAreaWriteRequest[noOfRegister-firstRegisterAddress+1]=true;
    }



    return p;
}

int CSLMPProtocol::RDW(unsigned short noOfRegister){
    int p=-1;

    if(noOfRegister>=firstRegisterAddress && noOfRegister<REGISTER_AREA_SIZE-1+firstRegisterAddress )
    {noOfRegister-=firstRegisterAddress;
        p=(registerArea[noOfRegister]&0xffff) | ((registerArea[noOfRegister+1]<<16)&0xffff0000);
    }
    return p;
}

short CSLMPProtocol::rDB(unsigned short noOfRegister, short bitNumber)
{
    short value=rD(noOfRegister);
    return value&(1<<bitNumber);
}

short CSLMPProtocol::setDB(unsigned short noOfRegister, short bitNumber)
{
    short value=rD(noOfRegister);
    value|=(1<<bitNumber);
    D(noOfRegister,value);
    return value;

}

short CSLMPProtocol::resetDB(unsigned short noOfRegister, short bitNumber)
{
    short value=rD(noOfRegister);
    short mask=~(1<<bitNumber);
    value&=mask;
    D(noOfRegister,value);
    return value;
}

long CSLMPProtocol::rDWB(unsigned short noOfRegister, long bitNumber)
{
    long value=rDW(noOfRegister);
    return value&(1<<bitNumber);
}

long CSLMPProtocol::setDWB(unsigned short noOfRegister, long bitNumber)
{
    long value=rDW(noOfRegister);
    value|=(1<<bitNumber);
    DW(noOfRegister,value);
    return value;
}

long CSLMPProtocol::resetDWB(unsigned short noOfRegister, long bitNumber)
{
    long value=rDW(noOfRegister);
    long mask=~(1<<bitNumber);
    value&=mask;
    DW(noOfRegister,value);
    return value;
}

void CSLMPProtocol::run(){
    open();

    while(threadStarted){
        refreshAll();
        msleep(SEND_DELAY);
    }


    close();
    quit();
}


void CSLMPProtocol::changeConnParameters(QString ip, unsigned short portNum){
    ipAddress=ip;
    port=portNum;

}

void CSLMPProtocol::startThread(){
    threadStarted=true;


    start();
}

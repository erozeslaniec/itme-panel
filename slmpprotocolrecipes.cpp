#include "SLMPProtocol.h"

short CSLMPProtocol::saveRecipe(QString recipeName){

    char filename[50]="recipes/";
    strcat(filename,recipeName.toStdString().c_str());
    QFile fileHandler(filename);
    if(!fileHandler.open(QIODevice::WriteOnly | QIODevice::Text))
        return 0;

    QTextStream file(&fileHandler);
    for(unsigned long i=0;i<doubleWordRegisterArea.size();i++)
        file<<"1"<<recipeSeparatingCharacter<<doubleWordRegisterArea[i]<<recipeSeparatingCharacter<<RDW(static_cast<unsigned short>(doubleWordRegisterArea[i]))<<recipeSeparatingCharacter<<commentDoubleWordRecipe.at(i)<<endl;
    for(unsigned long i=0;i<wordRegisterArea.size();i++)
        file<<"2"<<recipeSeparatingCharacter<<wordRegisterArea[i]<<recipeSeparatingCharacter<<RD(static_cast<unsigned short>(wordRegisterArea[i]))<<recipeSeparatingCharacter<<commentWordRecipe.at(i)<<endl;
    for(unsigned long i=0;i<flagRegisterArea.size();i++)
        file<<"3"<<recipeSeparatingCharacter<<flagRegisterArea[i]<<recipeSeparatingCharacter<<RM(static_cast<unsigned short>(flagRegisterArea[i]))<<recipeSeparatingCharacter<<commentFlagRecipe.at(i)<<endl;
    for(unsigned long i=0;i<latchRegisterArea.size();i++)
        file<<"4"<<recipeSeparatingCharacter<<latchRegisterArea[i]<<recipeSeparatingCharacter<<RL(static_cast<unsigned short>(latchRegisterArea[i]))<<recipeSeparatingCharacter<<commentLatchRecipe.at(i)<<endl;

    fileHandler.close();
    return 1;

}




short CSLMPProtocol::saveRecipeSchema(QString recipeName){
    QFile fileHandler(recipeName.toStdString().c_str());
    if(!fileHandler.open(QIODevice::WriteOnly | QIODevice::Text))
        return 0;
    QTextStream file(&fileHandler);

    for(unsigned long i=0;i<doubleWordRegisterArea.size();i++)
        file<<"1"<<recipeSeparatingCharacter<<doubleWordRegisterArea[i]<<recipeSeparatingCharacter<<RDW(static_cast<unsigned short>(doubleWordRegisterArea[i]))<<recipeSeparatingCharacter<<commentDoubleWordRecipe.at(i)<<endl;;
    for(unsigned long i=0;i<wordRegisterArea.size();i++)
        file<<"2"<<recipeSeparatingCharacter<<wordRegisterArea[i]<<recipeSeparatingCharacter<<RD(static_cast<unsigned short>(wordRegisterArea[i]))<<recipeSeparatingCharacter<<commentWordRecipe.at(i)<<endl;
    for(unsigned long i=0;i<flagRegisterArea.size();i++)
        file<<"3"<<recipeSeparatingCharacter<<flagRegisterArea[i]<<recipeSeparatingCharacter<<RM(static_cast<unsigned short>(flagRegisterArea[i]))<<recipeSeparatingCharacter<<commentFlagRecipe.at(i)<<endl;
    for(unsigned long i=0;i<latchRegisterArea.size();i++)
        file<<"4"<<recipeSeparatingCharacter<<latchRegisterArea[i]<<recipeSeparatingCharacter<<RL(static_cast<unsigned short>(latchRegisterArea[i]))<<recipeSeparatingCharacter<<commentLatchRecipe.at(i)<<endl;

    fileHandler.close();

    return 1;
}



short CSLMPProtocol::loadRecipe(QString recipeName){

    QString filename;
    filename ="recipes/"+recipeName;


    QFile fileHandler(filename);
    if(fileHandler.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream file(&fileHandler);
        recipeLoaded=1;
        lastRecipeName=recipeName;
        short id;
        unsigned   short address;
        long value;
        while(!file.atEnd()){
            QByteArray line=fileHandler.readLine();

            if(line.split(recipeSeparatingCharacter).size()>=3){
                id=line.split(recipeSeparatingCharacter).at(0).toShort();
                address=line.split(recipeSeparatingCharacter).at(1).toUShort();
                value=line.split(recipeSeparatingCharacter).at(2).toInt();

                switch(id){
                case 1:
                    DW(address,value);
                    break;
                case 2:
                    D(address,static_cast<short>(value));
                    break;
                case 3:
                    M(address,value);
                    break;
                case 4:
                    L(address,value);
                    break;

                }
            }
        }
        fileHandler.close();
        return 1;
    }
    return 0;
}




void CSLMPProtocol::takeRecipeSchemaFromFile(const char *filename){

    QFile fileHandler(filename);
    if(fileHandler.open(QIODevice::ReadOnly | QIODevice::Text)){

        while(!fileHandler.atEnd()){
            QByteArray line=fileHandler.readLine();


            if(line.split(recipeSeparatingCharacter).size()>=3){
                short id=line.split(recipeSeparatingCharacter)[0].toShort();
                short address=line.split(recipeSeparatingCharacter)[1].toShort();

                QString comment;
                if(line.split(recipeSeparatingCharacter).size()>=4)
                    comment=line.split(recipeSeparatingCharacter)[3].simplified();


                switch(id){
                case 1:
                    addDWtoRecipe(address,comment);
                    break;
                case 2:
                    addDtoRecipe(address,comment);
                    break;
                case 3:
                    addMtoRecipe(address,comment);
                    break;
                case 4:
                    addLtoRecipe(address,comment);
                    break;

                }
            }


        }
        fileHandler.close();
    }


}



void CSLMPProtocol::addAlarmEntry(QString description, unsigned short registerAddress, int moduleNumber)
{
    if(findAlarmEntry(registerAddress)==-1){
        SAlarmEntry temp;
        temp.description=description.simplified();
        temp.registerAddress=registerAddress;
        temp.occured=false;
        temp.moduleNumber=moduleNumber;
        alarms.push_back(temp);


    }


}

void CSLMPProtocol::processAlarms()
{
    for(int i=0;i<20;i++)
        alarmAtModule[i]=0;


    for(unsigned long i=0;i<alarms.size();i++)
    {
        if(!alarms[i].occured && RM(alarms[i].registerAddress)==1){
            //pushing alarm in front of alarm history list
            alarms[i].occured=true;
            Alarm temp(alarms[i].description, QDateTime::currentDateTime().toString(),static_cast<int>(i));
            alarmList.addAlarm(temp);
            alarmListUpdated();
        }
        if(alarms[i].occured){
            alarmAtModule[alarms[i].moduleNumber]=1;
        }

    }

    for(int i=0;i<alarmList.rowCount();i++){
        if(alarmList.Active(i) && RM(static_cast<unsigned short>(alarms[static_cast<unsigned long>(alarmList.AlarmID(i))].registerAddress))==0)
        {
            alarmList.setInactive(i);
            alarmList.setRestore(i,QDateTime::currentDateTime().toString());
            alarms[static_cast<unsigned long>(alarmList.AlarmID(i))].occured=false;
            alarmListUpdated();
        }

    }


}

int CSLMPProtocol::findAlarmEntry(unsigned short registerAddress)
{
    for(unsigned long i=0;i<alarms.size();i++)
        if(alarms[i].registerAddress==registerAddress)
            return static_cast<int>(i);
    return -1;
}

bool CSLMPProtocol::isModuleAtAlarm(int moduleNumber)
{
    if(moduleNumber<20 && moduleNumber>=0)
        return alarmAtModule[moduleNumber];
    return false;
}


int getdir (string dir, vector<string> &files, char *pattern)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == nullptr) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != nullptr) {
        if(strcmp(dirp->d_name,".")!=0 && strcmp(dirp->d_name,"..")!=0)
            if(strstr(dirp->d_name,pattern))
                files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

QString CSLMPProtocol::getRecipeList(int num, QString pattern){
    QDir directory;
    if(!directory.exists("recipes"))directory.mkdir("recipes");


    string dir="recipes/";
    QString ret;
    char tmp[50]="";
    vector<string> files=vector<string>();

    strcpy(tmp,pattern.toStdString().c_str());
    getdir(dir,files,tmp);

    sort(files.begin(),files.end());

    if(num<static_cast<int>(files.size())){
        ret=QString::fromStdString(files[static_cast<unsigned long>(num)]);
    }

    return ret;

}

int CSLMPProtocol::getRecipeListSize(QString pattern){



    string dir="recipes/";
    QString ret;
    char tmp[50]="";
    vector<string> files=vector<string>();

    strcpy(tmp,pattern.toStdString().c_str());
    getdir(dir,files,tmp);

    sort(files.begin(),files.end());



    return static_cast<int>(files.size());

}

bool CSLMPProtocol::isRecipeExist(QString recipeName)
{   ifstream file;
    char filename[50]="recipes/";

    strcat(filename,recipeName.toStdString().c_str());
    file.open(filename);
    bool ret=file.good();
    file.close();
    return ret;

}

bool CSLMPProtocol::isRecipeLoaded()
{
    return recipeLoaded;

}

QString CSLMPProtocol::getLastRecipeName()
{
    return lastRecipeName;
}

QString CSLMPProtocol::getBarcodeText()
{
    const size_t SIZE_OF_BARCODE=100;
    char c_array[SIZE_OF_BARCODE];
    short s_array[SIZE_OF_BARCODE/2];
    QString retString;
    for(unsigned short i=0;i<SIZE_OF_BARCODE/2;i++)
        s_array[i]=RD(1700+i);

    memcpy(c_array,reinterpret_cast<const char*>(s_array),sizeof(char)*SIZE_OF_BARCODE);
    bool clearBit=0;
    for(unsigned long i=0;i<SIZE_OF_BARCODE;i++){
        if(c_array[i]==0xd)clearBit=1;
        if(clearBit)c_array[i]=0;
    }

    retString=c_array;
    return retString;
}

void CSLMPProtocol::stopThread()
{
    threadStarted=false;
    msleep(SEND_DELAY*20);


}


void CSLMPProtocol::removeRecipe(QString recipeName){
    QString file="recipes/"+recipeName;
    if(recipeName!="")
        remove(file.toStdString().c_str());

}

void CSLMPProtocol::addDtoRecipe(short address,QString comment){
    wordRegisterArea.push_back(address);
    commentWordRecipe.push_back(comment.simplified());
}

void CSLMPProtocol::addDWtoRecipe(short address,QString comment){
    doubleWordRegisterArea.push_back(address);
    commentDoubleWordRecipe.push_back(comment.simplified());
}

void CSLMPProtocol::addMtoRecipe(short address,QString comment){
    flagRegisterArea.push_back(address);
    commentFlagRecipe.push_back(comment.simplified());
}


void CSLMPProtocol::addLtoRecipe(short address,QString comment){
    latchRegisterArea.push_back(address);
    commentLatchRecipe.push_back(comment.simplified());
}



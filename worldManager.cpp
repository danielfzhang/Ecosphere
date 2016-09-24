#include "main.h"
#include <iostream>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

extern sem_t terminateFlag;
extern sem_t endReading;
extern sem_t dayBEGINs;
extern sem_t endReadingCheck;
extern sem_t dayENDs;

extern pthread_mutex_t protectPrint;
extern struct dayInfoStruct todayInfo;

int creature()
{
  return 0;
}

void creature_info_print()
{
  cout<<"TBD"<<endl;
}
void update_today_info()
{
  //update new day information
  //rainy
  if( (double)rand() / RAND_MAX <= rainProb)
  todayInfo.todayIsRainy = true;
  else todayInfo.todayIsRainy = false;
  //windy
  if( (double)rand() / RAND_MAX <= windProb)
  todayInfo.todayIsWindy = true;
  else todayInfo.todayIsWindy = false;
  //year,month,day
  switch(todayInfo.globalDay)
  {
    case 1:
    todayInfo.globalMonth=1; break;
    case 32:
    todayInfo.globalMonth=2;break;
    case 60:
    todayInfo.globalMonth=3;break;
    case 91:
    todayInfo.globalMonth=4;break;
    case 121:
    todayInfo.globalMonth=5;break;
    case 152:
    todayInfo.globalMonth=6;break;
    case 182:
    todayInfo.globalMonth=7;break;
    case 213:
    todayInfo.globalMonth=8;break;
    case 244:
    todayInfo.globalMonth=9;break;
    case 274:
    todayInfo.globalMonth=10;break;
    case 305:
    todayInfo.globalMonth=11;break;
    case 335:
    todayInfo.globalMonth=12;break;
    case 366:
    todayInfo.globalMonth=1;
    todayInfo.globalDay=1;
    todayInfo.globalYear++;
    break;
  }
}

void *worldmanager(void* args)
{
  srand(time(0)-pthread_self());

  todayInfo.globalMonth=1;
  todayInfo.globalYear=1;
  todayInfo.globalDay=1;

  while(1)
  {
    //terminate the world
    if(todayInfo.globalDay>100)
    {
      sem_post(&terminateFlag);
      return NULL;
    }

    update_today_info();

    //print information of the day
    pthread_mutex_lock(&protectPrint);
    cout<<todayInfo.globalYear<<"Y/"<<todayInfo.globalMonth<<"M/"<<todayInfo.globalDay<<"D ";
    cout<<"  rainy:"<<todayInfo.todayIsRainy<<"  windy:"<<todayInfo.todayIsWindy<<endl;
    creature_info_print();
    pthread_mutex_unlock(&protectPrint);

    //notice creatures a day begins
    for(int i=0;i<creature();i++)//the last treeID is not been used
    {
      sem_post(&dayBEGINs);
    }
    //wait creatures to finish reading day info
    for(int i=0;i<creature();i++)
    {
      sem_wait(&endReading);
    }
    //creatures all done, program continues
    for(int i=0;i<creature();i++)
    {
      sem_post(&endReadingCheck);
    }
    //wait to end today
    for(int i=0;i<creature();i++)
    {
      sem_wait(&dayENDs);
    }

    //one day has gone
    usleep(usecOf1day);
    todayInfo.globalDay++;
    cout<<"---------------------------------------------------------------"<<endl;
  }
}
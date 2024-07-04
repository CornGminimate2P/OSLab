#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>


HANDLE rmutex,wmutex,readTry,resource;
int readcount,writecount;
char data[1024];


DWORD sem_wait(HANDLE sem);
DWORD sem_signal(HANDLE sem);

void randomDelay(void);
DWORD WINAPI writer(LPVOID who);
DWORD WINAPI reader(LPVOID who);

int main(void){
	int i;
	DWORD tid[5];				// Thread ID
	HANDLE th[5];				// Thread Handle
	int param[5]={0,1,2,3,4};
	readcount=writecount=0;
	rmutex = CreateSemaphore(
		NULL,   // default security attributes
		1,   // initial count
		1,   // maximum count
		NULL);  // unnamed semaphore
	wmutex = CreateSemaphore(
		NULL,   // default security attributes
		1,   // initial count
		1,   // maximum count
		NULL);  // unnamed semaphore
    readTry = CreateSemaphore(
		NULL,   // default security attributes
		1,   // initial count
		1,   // maximum count
		NULL);  // unnamed semaphore
	resource = CreateSemaphore(
		NULL,   // default security attributes
		1,   // initial count
		1,   // maximum count
		NULL);  // unnamed semaphore

	// Create 5 threads
	for(i=0;i<4;i++)
	th[i] = CreateThread(
		NULL,				// Default security attributes
		0,					// Default stack size
		reader,			// Thread function
		(void *)&param[i],	// Thread function parameter
		0,					// Default creation flag
		&tid[i]);			// Thread ID returned.

	th[4] = CreateThread(
		NULL,				// Default security attributes
		0,					// Default stack size
		writer,			// Thread function
		(void *)&param[4],	// Thread function parameter
		0,					// Default creation flag
		&tid[4]);				// Thread ID returned.

       // Wait until all threads finish
	for(i=0;i<5;i++)
		if(th[i]!=NULL)
			WaitForSingleObject(th[i],INFINITE);
	CloseHandle(rmutex);
	CloseHandle(wmutex);
	CloseHandle(readTry);
	CloseHandle(resource);
	return 0;
}

void randomDelay(void){
    int stime = ((rand()%2000)+100);
    Sleep(stime);
}


DWORD WINAPI reader(LPVOID who){
    int readerNo,i=0;

    readerNo = (int)*((int *)who);

    for(int ii=0;ii<readerNo;ii++) randomDelay();
	while(1){
        sem_wait(readTry);
		sem_wait(rmutex);
		readcount++;
		if(readcount==1)
			sem_wait(resource);
		sem_signal(rmutex);
        sem_signal(readTry);

			randomDelay();
       		printf("        (Consumer %d) Data is:%s\n",readerNo,data);fflush(stdout);
            for(int ii=0;ii<5;ii++)
                randomDelay();

		sem_wait(rmutex);
		readcount--;
		if(readcount==0)
			sem_signal(resource);
		sem_signal(rmutex);

// Remaining Section
		printf("       (Consumer %d) After exiting critical section\n",readerNo);fflush(stdout);
		if(data[0]=='0') break;
		randomDelay();
	}
    return 0;
}

DWORD WINAPI writer(LPVOID who){
    int i=0;
    char temp[1024];

while(1){
	printf("(Producer) Please enter a text (starting with 0 to stop) :");fflush(stdout);
	fgets(temp,1000,stdin);

	sem_wait(wmutex);
	writecount++;
	if(writecount==1)
        sem_wait(readTry);
    sem_signal(wmutex);
    sem_wait(resource);
    //Critical ssection
		printf("(Producer) Add %s into buffer\n",temp);fflush(stdout);
		for(i=0;i<1023;i++){
			if(temp[i]==0)break;
			data[i]=temp[i];
		}
		data[i]=0;
    //End of critical section
    sem_signal(resource);
    sem_wait(wmutex);
    writecount--;
    if(writecount==0)
        sem_signal(readTry);
	sem_signal(wmutex);

// Remaining Section
	printf("(Producer) After exiting critical section\n");fflush(stdout);
	if(temp[0]=='0')break;
    }
    return 0;
}


DWORD sem_wait(HANDLE sem){
    DWORD result = WaitForSingleObject(sem,INFINITE);

	switch(result){
		case WAIT_OBJECT_0:return 1;
		case WAIT_TIMEOUT: return 0;
	}
	return 0;
}

DWORD sem_signal(HANDLE sem){
    return ReleaseSemaphore(sem,1,NULL); // Increase by one
}

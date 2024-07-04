#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SIZE 4

struct shmareatype{
   int rp;
   int wp;
   int data[BUF_SIZE];
   int counter;
   // Following section is for Peterson's solution
   int flag[2];
   int turn;
};

void randomDelay(void);
void consumer(struct shmareatype *shmarea);
void producer(struct shmareatype *shmarea);
int  increment(int *counter);
int  decrement(int *counter);


int main(int argc, char *argv[]){
	TCHAR shmName[] = _T("lab5_2");
	struct shmareatype *shmarea; // Pointer to the shared segment
    int shmsize = sizeof(struct shmareatype); // share segment size
	HANDLE hMapFile;

	if(argc>1){ // If this is the child process it will run this instead
       hMapFile = OpenFileMapping(
				   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   shmName);               // name of mapping object

     if (hMapFile == NULL) {
      printf("Could not open file mapping object (%d).\n",
             GetLastError());
      return 0;
     }
 shmarea = (struct shmareatype *) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        shmsize);

    if (shmarea == NULL)
    {
      printf("Could not map view of file (%d).\n",
             GetLastError());
      return -1;
    }

        consumer(shmarea);

	    UnmapViewOfFile(shmarea);
       CloseHandle(hMapFile);
		return 0;
	}
    // Create Memory Mapping File
    hMapFile = CreateFileMapping(
				 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // max. object size
                 shmsize,                // buffer size
                 shmName);                 // name of mapping object

	if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE){
      printf("Could not create file mapping object...\n");
      return -1;
    }

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	TCHAR name[] = _T("lab5_4.exe 1"); // Using the Same code with parameter
	ZeroMemory(&si,sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi,sizeof(pi));

    printf("Before fork a child process...\n");

    if(!CreateProcess(NULL,name,
		NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)){
			fprintf(stderr,"Create process failed.\n");
			return -1;
	}

	//Parent Process
	// Map share memory
    shmarea = (struct shmareatype *) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        shmsize);

    if (shmarea == NULL)
    {
      printf("Could not map view of file (%d).\n",
             GetLastError());
      return -1;
    }
	shmarea->rp = shmarea->wp = shmarea->counter = 0;

	//Peterson's solution
	shmarea->flag[0] = shmarea->flag[1] = 0;
	shmarea->turn = 0;

	producer(shmarea);
	printf("Before going into the wait state...\n");
	WaitForSingleObject(pi.hProcess,INFINITE);
	printf("Child process has terminated\n");
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	UnmapViewOfFile(shmarea);
    CloseHandle(hMapFile);
    return 0;
}

void consumer(struct shmareatype *shmarea){
    int i=0;
	int remaintime;

    while(1) // consume data
    {
		printf("       (Consumer) Before entering critical section\n");
// Peterson's  ENTRY section
		shmarea->flag[0]=true;
		shmarea->turn = 1;
		while(shmarea->flag[1] && shmarea->turn==1) Sleep(1);
/////////////////////////////////
// Critical Section
	   remaintime = 1000; // Wait a few seconds before quitting
	   while((shmarea->counter==0)&& (remaintime>0)){
		   Sleep(1);remaintime--;
		   if (!(remaintime % 1000))
			   printf("        Waiting for %d second(s).\n", remaintime / 1000);
	   }
	   if(remaintime==0){
		   printf("        The consumer process waiting too long, stopping...\n");
		   shmarea->flag[0] = false; //EXIT here in case of waiting too long.
		   return;
	   }else{
           printf("         (Consumer) Data number:%d = %d\n",i++,shmarea->data[shmarea->rp]);
           shmarea->rp = ((shmarea->rp+1)%BUF_SIZE);
	       decrement(&(shmarea->counter));
	   }
// Peterson's EXIT section
		shmarea->flag[0] = false;
// Remaining Section
	    printf("       (Consumer) After exiting critical section\n");
	}
}

void producer(struct shmareatype *shmarea){
    int i;

    for(i=0;i<32;i++) // produce data
    {
		printf("(Producer) Before entering critical section\n");
// Peterson's ENTRY section
		shmarea->flag[1] = true;
		shmarea->turn = 0;
		while(shmarea->flag[0] && shmarea->turn==0) Sleep(1);
// Critical Section
		while(shmarea->counter==BUF_SIZE) Sleep(1);
        printf("(Producer)Enter data %d into share memory...\n",i);
        shmarea->data[shmarea->wp]=i;
       // move the write pointer so that the consumer know when to read.
        shmarea->wp = ((shmarea->wp+1)%BUF_SIZE);
		increment(&(shmarea->counter));
// Peterson's EXIT section
		shmarea->flag[1] = false;
// Remaining Section
	printf("(Producer) After exiting critical section\n");
	}
}

void randomDelay(void){
	// This function provides a delay which slows the process down so we can see what happens
   srand(time(NULL));
   int stime = (rand()%1000)+100;
   Sleep(stime);
}

int increment(int *counter){
// This function show how non-atomic instruction can cause problem in consumer-producer case
    int temp = *counter;printf("INC: read\n");
    randomDelay();
    temp = temp +1;printf("INC: ++\n");
    randomDelay();
    *counter = temp;printf("INC: write\n");
    return temp;
}

int decrement(int *counter){
// This function show how non-atomic instruction can cause problem in consumer-producer case
    int temp = *counter;printf("      DEC: read\n");
    randomDelay();
    temp = temp -1;printf("      DEC: --\n");
    randomDelay();
    *counter = temp;printf("      DEC: write\n");
    return temp;
}

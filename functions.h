void getTextLine()
{
        initCommand();
        while ((userInput != '\n') && (bufferChars < BUFFER_MAX_LENGTH)) 
        {
                buffer[bufferChars++] = userInput;
                userInput = getchar();
        }
        buffer[bufferChars] = 0x00;
        populateCommand();
}
void populateCommand()
{
        char* bufferPointer;
        	bufferPointer = strtok(buffer, " ");
        while (bufferPointer != NULL) {
                commandArgv[commandArgc] = bufferPointer;
                bufferPointer = strtok(NULL, " ");
                commandArgc++;
        }
}

void initCommand()
{
        while (commandArgc != 0) {
                commandArgv[commandArgc] = NULL;
                commandArgc--;
        }
        bufferChars = 0;
}


t_job* insertJob(pid_t pid, pid_t pgid, char* name, char* descriptor,
                 int status)
{
        usleep(10000);
        t_job *newJob = malloc(sizeof(t_job));

        newJob->name = (char*) malloc(sizeof(name));
        newJob->name = strcpy(newJob->name, name);
        newJob->pid = pid;
        newJob->pgid = pgid;
        newJob->status = status;
        newJob->descriptor = (char*) malloc(sizeof(descriptor));
        newJob->descriptor = strcpy(newJob->descriptor, descriptor);
        newJob->next = NULL;

        if (jobsList == NULL) {
                numActiveJobs++;
                newJob->id = numActiveJobs;
                return newJob;
        } else {
                t_job *auxNode = jobsList;
                while (auxNode->next != NULL) {
                        auxNode = auxNode->next;
                }
                newJob->id = auxNode->id + 1;
                auxNode->next = newJob;
                numActiveJobs++;
                return jobsList;
        }
}


int changeJobStatus(int pid, int status)
{
        usleep(10000);
        t_job *job = jobsList;
        if (job == NULL) {
                return 0;
        } else {
                int counter = 0;
                while (job != NULL) {
                        if (job->pid == pid) {
                                job->status = status;
                                return TRUE;
                        }
                        counter++;
                        job = job->next;
                }
                return FALSE;
        }
}

t_job* delJob(t_job* job)
{
        usleep(10000);
        if (jobsList == NULL)
                return NULL;
        t_job* currentJob;
        t_job* beforeCurrentJob;

        currentJob = jobsList->next;
        beforeCurrentJob = jobsList;

        if (beforeCurrentJob->pid == job->pid) {

                beforeCurrentJob = beforeCurrentJob->next;
                numActiveJobs--;
                return currentJob;
        }

        while (currentJob != NULL) {
                if (currentJob->pid == job->pid) {
                        numActiveJobs--;
                        beforeCurrentJob->next = currentJob->next;
                }
                beforeCurrentJob = currentJob;
                currentJob = currentJob->next;
        }
        return jobsList;
}

t_job* getJob(int searchValue, int searchParameter)
{
        usleep(10000);
        t_job* job = jobsList;
        switch (searchParameter) {
        case BY_PROCESS_ID:
                while (job != NULL) {
                        if (job->pid == searchValue)
                                return job;
                        else
                                job = job->next;
                }
                break;
        case BY_JOB_ID:
                while (job != NULL) {
                        if (job->id == searchValue)
                                return job;
                        else
                                job = job->next;
                }
                break;
        case BY_JOB_STATUS:
                while (job != NULL) {
                        if (job->status == searchValue)
                                return job;
                        else
                                job = job->next;
                }
                break;
        default:
                return NULL;
                break;
        }
        return NULL;
}

void printJobs()
{
        printf("\nActive jobs:\n");
        printf(
                "---------------------------------------------------------------------------\n");
        printf("| %7s  | %30s | %5s | %10s | %6s |\n", "job no.", "name", "pid",
               "descriptor", "status");
        printf(
                "---------------------------------------------------------------------------\n");
        t_job* job = jobsList;
        if (job == NULL) {
                printf("| %s %62s |\n", "No Jobs.", "");
        } else {
                while (job != NULL) {
                        printf("|  %7d | %30s | %5d | %10s | %6c |\n", job->id, job->name,
                               job->pid, job->descriptor, job->status);
                        job = job->next;
                }
        }
        printf(
                "---------------------------------------------------------------------------\n");
}

void welcomeScreen()

{
        printf("\n-------------------------------------------------\n");
        printf("\t SH42 v.1  \n");
        printf("\tAuthors: Jack Thor & John Wang\n");
        printf("-------------------------------------------------\n");
        printf("\n\n");
}

void shellPrompt()
{
        printf("sh142 :> ",getcwd(currentDirectory, 1024));
}

exit_status *setStatus(int status, char * name){
		if(exitStatus == NULL){
		printf("Me again 555 \n");
			exit_status *newStat = malloc(sizeof(exit_status));
			newStat->name = (char*) malloc(sizeof(name));
			newStat->name = strcpy(newStat->name, name);
			newStat->status = status;
			newStat->cmmdID = 1;
			return newStat;
		}
		exit_status *tmpExit = exitStatus ->next;
		while(tmpExit != NULL){
			tmpExit = tmpExit->next;
		}
		exit_status *newStat = malloc(sizeof(exit_status));
		newStat->name = (char*) malloc(sizeof(name));
		newStat->name = strcpy(newStat->name, name);
		newStat->status = status;
		
		if(tmpExit->cmmdID == 5){
			tmpExit = exitStatus;
			exitStatus = exitStatus->next;
			exitStatus->cmmdID = exitStatus->cmmdID - 1;
			while(tmpExit->next != NULL){
				tmpExit = tmpExit->next;
				tmpExit->cmmdID = tmpExit->cmmdID - 1;
			}
			
		}
		newStat->cmmdID = tmpExit->cmmdID + 1;
		tmpExit->next = newStat;
		printf("Me again\n");
		return exitStatus;
	}	

void getStatus(int aInt){
	if(exitStatus == NULL){
		printf(" No command has been executed \n");
	}else{
		exit_status *temp = exitStatus;
		while(temp != NULL){
			if(temp->cmmdID == aInt){
			printf(" &s exit status %d \n", temp->name, temp->status);
			break;
			}
			temp = temp->next;
		}
	}

}

int conditioneval(int boo, char * operators, char * operand){
	char first[1];
	char* oper;
	int op;
	strcpy(first, operand);
	if(first[0] == '!'){
		oper = strtok(operand,"!");
		op = !(system(oper));
	}else{
		op = system(operand);
		}
	if((strcmp("&&", operators))){
		return boo && op;
	}
	if(strcmp("||", operators)){
			return boo && op;
	}
	
}







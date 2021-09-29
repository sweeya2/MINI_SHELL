#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#define TIME 2

using namespace std;

void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << cwd << " ";
}

bool checkinput(string input,string valuetobechecked){
    if(input==valuetobechecked)
        return(true);
    return(false);
}

int main()
{
    cout << "SWEEYA\n200101079\nWelcome to Sweeya shell\n";
    sleep(4);
    FILE *fptr;
    fptr = fopen("/tmp/history.txt", "w");

    while (1) {
        int toCheck=0;
        fptr = fopen("/tmp/history.txt", "a");
        char *user = getenv("USER");
        printDir();
        char *input_str;
        input_str = (char *) malloc(1000*sizeof(char));
        scanf(" %[^\n]s", input_str);
        fprintf(fptr, "%s\n", input_str);
        fclose(fptr);


        //vector implementation for seperating space seperated
        vector<string> v;
        string temp = "";
        for(int i=0; i< strlen(input_str);++i){

            if(input_str[i]==' '){
                v.push_back(temp);
                temp = "";
            }
            else{
                temp.push_back(input_str[i]);
            }
	    }
	    v.push_back(temp);

        char *listener[v.size()+1];
        for(int i=0; i<v.size(); i++){
            char *cstr = new char[v[i].length() + 1];
            strcpy(cstr, v[i].c_str());
            listener[i]= cstr;
        }
        listener[v.size()]= NULL;

        //piping
        vector<string> vpiping;
        string temp2 = "";
        for(int i=0; i< strlen(input_str);++i){

            if(input_str[i]=='|'){
                vpiping.push_back(temp2);
                temp2 = "";
            }
            else{
                temp2.push_back(input_str[i]);
            }
	    }
	    vpiping.push_back(temp2);

        //redirection

        // > write
        vector<string> vWrite;
        string temp5 = "";
        for(int i=0; i< strlen(input_str);++i){

            if(input_str[i]=='>'){
                vWrite.push_back(temp5);
                temp5 = "";
            }
            else{
                temp5.push_back(input_str[i]);
            }
	    }
	    vWrite.push_back(temp5);

        // < read
        vector<string> vRead;
        string temp6 = "";
        for(int i=0; i< strlen(input_str);++i){

            if(input_str[i]=='<'){
                vRead.push_back(temp6);
                temp6 = "";
            }
            else{
                temp6.push_back(input_str[i]);
            }
	    }
	    vRead.push_back(temp6);

        // >> append
        vector<string> vAppend;
        string temp7 = "";
        for(int i=0; i< strlen(input_str);++i){

            if(input_str[i]=='>' && input_str[i+1]=='>'){
                vAppend.push_back(temp7);
                temp7 = "";
                i++;
            }
            else{
                temp7.push_back(input_str[i]);
            }
	    }
	    vAppend.push_back(temp7);

        //internal commands
        if(v.size()<3){
            char *out = getenv((v[0]).c_str());
            if (out != NULL){

                toCheck=1;
                cout << getenv((v[0]).c_str()) << endl;
                free(input_str);
                continue;
            }
            else if(checkinput(v[0], "cd")){
                toCheck=1;
                if(v.size()==1){
                    char *variable1;
                    variable1 = (char *)malloc(1000 * sizeof(char));
                    sprintf(variable1, "/home/%s", getenv("USER"));
                    string all = "/home/";
                    string another = getenv("USER");
                    chdir((all+another).c_str());
                    free(input_str);
                    continue;
                }
                else{
                    chdir((v[1]).c_str());
                    free(input_str);
                    continue;
                }
            }
            else if(checkinput(v[0], "history")){
                toCheck=1;
                int waste = 0;
                char eachOne[1000];
                fptr = fopen("/tmp/history.txt", "r");
                while ((waste = fscanf(fptr, "%s\n", eachOne)) != -1)
                {
                    cout << eachOne << "\n";
                }
                fclose(fptr);
                if (v.size()==2)
                {
                    if(checkinput(v[1], "exit") || checkinput(v[1], "quit") || checkinput(v[1], "x")){
                        remove("/tmp/history.txt");
                        break;
                    }
                    else{
                        cout << "wrong input"<< endl;
                    }
                }
                free(input_str);
                continue;
            }
            else if(checkinput(v[0], "exit")|| checkinput(v[0], "quit") || checkinput(v[0], "x")){
                toCheck=1;
                remove("/tmp/history.txt");
                break;
            }
            else if(checkinput(v[0], "help")){
                toCheck=1;
                cout << "1. USER, PATH, HOME, SHELL, TERM : environment variables\n2. cd : change directory to /home/user\n3. cd (some directory) : changes directory to the given directory\n4. history : to get all the input you've given before exit\n5. history exit : gets all the input you've given before exit and then exits from the shell\n6. exit (or) quit (or) x : exits from the shell\n7. (path functions) : e.g. ls\n8. (command1)|(command2) : piping and executing both commands\n9. (command)>(output.txt) : executes the command and outputs it to output.txt (write mode)\n10. (command)>>(output.txt) : executes the command and adds the output to output.txt (append mode)\n11. (command)<(input.txt) : executes the command using the input form input.txt (read mode)\n12. setenv (environment variable) = (new value) : sets new value for the given environment variable\n13. help : to open this again\nNOTE: Check spaces in piping, redirection and setenv as given here\n";
                free(input_str);
                continue;
            }
        }

        //PATH FUNCTIONS
        if(vpiping.size()<2 && vWrite.size()<2 && vRead.size()<2 && vAppend.size()<2){

            //setenv
            if(checkinput(v[0], "setenv")){
                toCheck=1;
                char *ifEnvVar = getenv(listener[1]);
                if (ifEnvVar != NULL)
                {
                    setenv(listener[1], listener[3], 1);
                }
                else{
                    cout << "wrong input"<<endl;
                }
                free(input_str);
                continue;
            }
            else{
                    pid_t pid = fork();
                    toCheck=1;
                    int status;
                    if ( pid==0) {

                        status = execvp(listener[0], listener);
                        if(status<0) toCheck=0;
                    }
                    else if(pid>0){
                        wait(NULL);
                        kill(pid, SIGTERM);
                    }

                    if(toCheck==0){
                        cout <<"wrong input"<< endl;
                        break;
                    }

                    free(input_str);
                    continue;
            }
        }

        //piping
        if( vpiping.size()==2){

            vector<string> firstPipe;
            string temp3 = "";
            int j=0;
            char *listener1[v.size()+1];
            for(int i=0; i< strlen((vpiping[0]).c_str());++i){

                if(vpiping[0][i]==' '){
                    firstPipe.push_back(temp3);
                    char *cstr = new char[temp3.length()];
                    strcpy(cstr, temp3.c_str());
                    listener1[j]= cstr;
                    j++;
                    temp3 = "";
                }
                else{
                    temp3.push_back(vpiping[0][i]);
                }
            }
            firstPipe.push_back(temp3);
            char *cstr = new char[temp3.length()];
            strcpy(cstr, temp3.c_str());
            listener1[j]= cstr;
            listener1[j+1]= NULL;

            vector<string> secondPipe;
            string temp4 = "";
            int k=0;
            char *listener2[v.size()+1];
            for(int i=0; i< strlen((vpiping[1]).c_str());++i){

                if(vpiping[1][i]==' '){
                    secondPipe.push_back(temp4);
                    char *cstr2 = new char[temp4.length()];
                    strcpy(cstr2, temp4.c_str());
                    listener2[k]= cstr2;
                    k++;
                    temp4 = "";
                }
                else{
                    temp4.push_back(vpiping[1][i]);

                }
            }
            secondPipe.push_back(temp4);
            char *cstr2 = new char[temp4.length()];
            strcpy(cstr2, temp4.c_str());
            listener2[k]= cstr2;
            listener2[k+1]= NULL;

            int relatedToPiping[2];
            pid_t p1, p2;

            if(pipe(relatedToPiping) >= 0){
                p1 = fork();
                if(p1==0){
                    dup2(relatedToPiping[1], STDOUT_FILENO);
                    close(relatedToPiping[0]);
                    close(relatedToPiping[1]);

                    if (execvp(listener1[0], listener1) < 0) {
                        cout<< "wrong input"<< endl;
                        exit(0);
                    }
                }
                else if(p1>0){
                    p2 = fork();

                    if (p2 < 0) {
                        cout<< "wrong input"<< endl;
                    }

                    else if (p2 == 0) {
                        dup2(relatedToPiping[0], STDIN_FILENO);
                        close(relatedToPiping[1]);


                        if (execvp(listener2[0], listener2) < 0) {
                            cout<< "wrong input"<< endl;
                            exit(0);
                        }
                    }
                    else {
                        close(relatedToPiping[1]);
                        waitpid(p1,NULL,0);
                        waitpid(p2,NULL,0);
                    }
                }
                else{
                    cout<< "wrong input"<< endl;
                }
            }
            else{
                cout<< "wrong input"<< endl;
            }

            kill(p1, SIGTERM);
            kill(p2, SIGTERM);
            free(input_str);
            continue;
        }

        //redirection

        // > write
        else if(vWrite.size() == 2){
            vector<string> firstPipe;
            string temp3 = "";
            int j=0;
            char *listener1[v.size()+1];
            for(int i=0; i< strlen((vWrite[0]).c_str());++i){

                if(input_str[i]==' '){
                    firstPipe.push_back(temp3);
                    char *cstr = new char[temp3.length() + 1];
                    strcpy(cstr, temp3.c_str());
                    listener1[j]= cstr;
                    j++;
                    temp3 = "";
                }
                else{
                    temp3.push_back(input_str[i]);
                }
            }
            firstPipe.push_back(temp3);
            char *cstr = new char[temp3.length() + 1];
            strcpy(cstr, temp3.c_str());
            listener1[j]= cstr;
            listener1[j+1]= NULL;
            string also= listener1[0];
                    if(access(vWrite[1].c_str(),F_OK)==0){
                        remove(vWrite[1].c_str());
                    }
                    pid_t writeHere = fork();
                    if ( writeHere ==0) {
                            int writemode;
                            writemode = open(vWrite[1].c_str(), O_WRONLY | O_CREAT,  0666);
                            dup2(writemode, 1);
                            execvp(listener1[0], listener1);
                            perror("wrong input");
                            close(writemode);
                            exit(0);
                    }
                    wait(NULL);
                    kill(writeHere, SIGTERM);
                    free(input_str);
                    continue;
        }

        // >> append
        else if(vAppend.size()==2){
            vector<string> firstPipe;
            string temp3 = "";
            int j=0;
            char *listener1[v.size()+1];
            for(int i=0; i< strlen((vAppend[0]).c_str());++i){

                if(input_str[i]==' '){
                    firstPipe.push_back(temp3);
                    char *cstr = new char[temp3.length() + 1];
                    strcpy(cstr, temp3.c_str());
                    listener1[j]= cstr;
                    j++;
                    temp3 = "";
                }
                else{
                    temp3.push_back(input_str[i]);
                }
            }
            firstPipe.push_back(temp3);
            char *cstr = new char[temp3.length() + 1];
            strcpy(cstr, temp3.c_str());
            listener1[j]= cstr;
            listener1[j+1]= NULL;

            pid_t appendHere= fork();

            if (appendHere == 0)
            {

                    int appendmode;
                    appendmode = open(vAppend[1].c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
                    dup2(appendmode, 1);
                    execvp(listener1[0], listener1);
                    perror("wrong input");
                    close(appendmode);
                    exit(0);

            }
            wait(NULL);
            kill(appendHere, SIGTERM);
            free(input_str);
            continue;
        }

        // < read
        else if(vRead.size()==2){
            vector<string> firstPipe;
            string temp3 = "";
            int j=0;
            char *listener1[v.size()+1];
            for(int i=0; i< strlen((vRead[0]).c_str());++i){

                if(input_str[i]=='<'){
                    firstPipe.push_back(temp3);
                    char *cstr = new char[temp3.length() + 1];
                    strcpy(cstr, temp3.c_str());
                    listener1[j]= cstr;
                    j++;
                    temp3 = "";
                }
                else{
                    temp3.push_back(input_str[i]);
                }
            }
            firstPipe.push_back(temp3);
            char *cstr = new char[temp3.length() + 1];
            strcpy(cstr, temp3.c_str());
            listener1[j]= cstr;
            listener1[j+1]= NULL;

            pid_t readHere= fork();
            if (readHere == 0)
            {
                int readMode;
                readMode = open(vRead[1].c_str(), O_RDONLY | O_CREAT, 0666);
                dup2(readMode, 0);
                execvp(listener1[0], listener1);
                perror("wrong input");
                close(readMode);
                exit(0);
            }
            wait(NULL);
            kill(readHere, SIGTERM);
            free(input_str);
            continue;
        }

    }
    return 0;
}

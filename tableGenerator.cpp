#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header


int main(int argc, char* argv[]) {
    SAConnection con;
    SACommand cmd;
    
    try {
        con.Connect("wmrdb", "root", "agu109", SA_MySQL_Client);
        printf("We are connected!\n");
        cmd.setConnection(&con);

        cmd.setCommandText(
            "CREATE TABLE tasks(taskID int NOT NULL AUTO_INCREMENT PRIMARY KEY, locationStartX int, locationStartY int, locationEndX int, locationEndY int, weight int)");
        cmd.Execute();

        cmd.setCommandText(
            "CREATE TABLE workers(workerID int NOT NULL AUTO_INCREMENT PRIMARY KEY, locationX int, locationY int, capacity int, battery int)");
        cmd.Execute();

        cmd.setCommandText(
            "CREATE TABLE assignments(assignmentID int NOT NULL AUTO_INCREMENT PRIMARY KEY, taskID int, robotID int, status int, startPercent int, endPercent int)");
        cmd.Execute();
    
        con.Commit();

        con.Disconnect();
        printf("We are disconnected!\n");
    }
    catch(SAException &x) {
        con.Rollback();
        printf("%s\n", (const char*)x.ErrText());
    }
    
    return 0;
}
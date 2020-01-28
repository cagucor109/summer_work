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
            "CREATE TABLE tasks(taskID int, locationStartX int, locationEndX int, weight int, availability int)");
        cmd.Execute();

        cmd.setCommandText(
            "CREATE TABLE robots(robotID int, locationX int, capacity int)");
        cmd.Execute();

        cmd.setCommandText(
            "CREATE TABLE assignments(taskID int, robotID int, status int)");
        cmd.Execute();
    
        con.Commit();

        // con.Disconnect();
        printf("We are disconnected!\n");
    }
    catch(SAException &x) {
        con.Rollback();
        printf("%s\n", (const char*)x.ErrText());
    }
    
    return 0;
}
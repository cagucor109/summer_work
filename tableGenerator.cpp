#include <iostream>  // for cout
#include <SQLAPI.h> // main SQLAPI++ header


int main(int argc, char* argv[]) {
    SAConnection con;
    SACommand cmd;
    
    try {
        con.Connect("wmrdb", "root", "agu109", SA_MySQL_Client);
        std::cout << "We are connected!\n" << std::endl;
        cmd.setConnection(&con);

        cmd.setCommandText(
            "CREATE TABLE tasks(taskID int NOT NULL AUTO_INCREMENT PRIMARY KEY, locationStartX int, locationStartY int, locationEndX int, locationEndY int, weight int, timeStamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)");
        cmd.Execute();

        cmd.setCommandText(
            "CREATE TABLE workers(workerID int NOT NULL AUTO_INCREMENT PRIMARY KEY, locationX int, locationY int, capacity int, battery int, maxSpeed int, status varchar(255), timeStamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)");
        cmd.Execute();

        cmd.setCommandText(
            "CREATE TABLE assignments(assignmentID int NOT NULL AUTO_INCREMENT PRIMARY KEY, taskID int, robotID int, status varchar(255), timeTaken int, distance int, timeStamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)");
        cmd.Execute();
    
        con.Commit();

        std::cout << "Tables generated..." << std::endl;

        con.Disconnect();
    }
    catch(SAException &x) {
        con.Rollback();
        printf("%s\n", (const char*)x.ErrText());
    }
    
    return 0;
}
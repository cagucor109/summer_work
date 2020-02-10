#include "SQLAPIcoms.hpp"
#include <iostream>

int main(int argc, char *argv[]){

    SAConnection con;

    SACommand cmd;
    
    try {
        con.Connect("wmrdb", "root", "agu109", SA_MySQL_Client);
        std::cout << "We are connected!\n" << std::endl;
        cmd.setConnection(&con);

        cmd.setCommandText(
            "DELETE FROM tasks");
        cmd.Execute();

        cmd.setCommandText(
            "ALTER TABLE tasks AUTO_INCREMENT = 1");
        cmd.Execute();

        cmd.setCommandText(
            "DELETE FROM workers");
        cmd.Execute();

         cmd.setCommandText(
            "ALTER TABLE workers AUTO_INCREMENT = 1");
        cmd.Execute();

        cmd.setCommandText(
            "DELETE FROM assignments");
        cmd.Execute();

         cmd.setCommandText(
            "ALTER TABLE assignments AUTO_INCREMENT = 1");
        cmd.Execute();
    
        con.Commit();
        std::cout << "All table data deleted..." << std::endl;

        con.Disconnect();
    }
    catch(SAException &x) {
        con.Rollback();
        printf("%s\n", (const char*)x.ErrText());
    }
    
    return 0;
}
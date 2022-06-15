#include "Table.h"


const std::string COMMANDS[8]={"open","close","saveAs","save","help","exit","edit","print"};

class Controller
{
private:
    Table table;
    
    bool isSaved;
    bool isOpen;
    std::string fileName;

    Controller();
    
    int match(std::string& test);

    void open(std::vector<std::string>& params);
    void close();
    void saveAs(std::vector<std::string>& params);
    void save();
    void help() const;
    void exit();
    void edit(std::vector<std::string>& params);
    void print();

    void trim(std::string& input);
    void split(std::string input,std::vector<std::string>& cmdArray);

    static Controller* controller;
public:
    
    ///Signleton patern
    static Controller* getController();
    ///Main function for program
    void Run();
};

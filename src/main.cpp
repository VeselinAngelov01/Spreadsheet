#include "../h/Controller.h"

/// Main function to start program
int main()
{
    
    /// Github: https://github.com/VeselinAngelov01/Spreadsheet
    Controller* controller=Controller::getController();
    controller->Run();
    delete controller;
    return 0;
}
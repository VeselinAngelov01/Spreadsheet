#include "../h/Controller.h"

Controller *Controller::controller = nullptr;

Controller *Controller::getController()
{
    if(!controller)
    {
        controller = new Controller();
    }
    return controller;
}

Controller::Controller()
{
    isSaved = false;
    isOpen = false;
}

void Controller::trim(std::string &input)
{
    int size = input.length();
    if (size == 0)
        throw std::invalid_argument("Empty string!");
    if (input[0] == ' ')
        input = input.erase(0, 1);
    if (size == 0)
        throw std::invalid_argument("Empty string!");
    for (int i = size - 1; i >= 0; --i)
    {
        if (input[i] == ' ')
        {
            input = input.erase(i, 1);
        }
        else
            break;
    }

    bool lastCharWS = false;
    for (int i = 0; i < size; ++i)
    {
        if (input[i] == ' ' && lastCharWS)
        {
            input.erase(i, 1);
            --i;
        }
        if (input[i] == ' ')
            lastCharWS = true;
        else
            lastCharWS = false;
    }
}

void Controller::split(std::string input, std::vector<std::string> &cmdArray)
{
    trim(input);
    int size = input.length();
    std::string temp;
    for (int i = 0; i < size; ++i)
    {
        if (input[i] == ' ')
        {
            cmdArray.push_back(temp);
            temp.clear();
        }
        else
        {
            temp += input[i];
        }
    }
    cmdArray.push_back(temp);
}

void Controller::Run()
{
    std::string input;
    std::vector<std::string> splited;
    help();
    while (true)
    {
        try
        {
            std::getline(std::cin, input);
            splited.clear();
            split(input, splited);
            if (splited[0] == COMMANDS[0])
            {
                if (splited.size() != 2)
                    throw std::invalid_argument("Invalid parameters count!");
                open(splited);
            }
            else if (splited[0] == COMMANDS[1])
            {
                if (splited.size() != 1)
                    throw std::invalid_argument("Invalid parameters count!");
                close();
            }
            else if (splited[0] == COMMANDS[2])
            {
                if (splited.size() != 2)
                    throw std::invalid_argument("Invalid parameters count!");
                saveAs(splited);
            }
            else if (splited[0] == COMMANDS[3])
            {
                if (splited.size() != 1)
                    throw std::invalid_argument("Invalid parameters count!");
                save();
            }
            else if (splited[0] == COMMANDS[4])
            {
                if (splited.size() != 1)
                    throw std::invalid_argument("Invalid parameters count!");
                help();
            }
            else if (splited[0] == COMMANDS[5])
            {
                if (splited.size() != 1)
                    throw std::invalid_argument("Invalid parameters count!");
                exit();
                break;
            }
            else if (splited[0] == COMMANDS[6])
            {
                if (splited.size() != 3)
                    throw std::invalid_argument("Invalid parameters count!");
                edit(splited);
            }
            else if (splited[0] == COMMANDS[7])
            {
                if (splited.size() != 1)
                    throw std::invalid_argument("Invalid parameters count!");
                print();
            }
            else
            {
                throw std::invalid_argument("No existing command!");
            }
            // std::cin.ignore(10000, '\n');
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << '\n';
        }
    }
}

int Controller::match(std::string &test)
{
    // 0 - int 1 - float 2 - string 3- formula 4- empty
    std::regex testInt("[+,-]?[1-9]{1}[0-9]*");
    std::regex testDouble("[+,-]?[1-9]{1}[0-9]*\\.{1}[0-9]+");
    std::regex testString("\"{1}([a-zA-Z0-9. -]*|\\{1}\"{1}|\\{2})*\"{1}");
    std::regex testFormula("={1}([-]?([1-9]{1}[0-9]*|R[1-9]{1}[0-9]*C[1-9]{1}[0-9]*|[1-9]{1}[0-9]*\\.{1}[0-9]+)[+\\-*/^]{1})+([-]?([1-9]{1}[0-9]*|R[1-9]{1}[0-9]*C[1-9]{1}[0-9]*|[1-9]{1}[0-9]*\\.{1}[0-9]+))");
    if (test.length() == 0)
        return 4;
    else if (std::regex_match(test, testInt))
        return 0;
    else if (std::regex_match(test, testDouble))
        return 1;
    else if (std::regex_match(test, testString))
        return 2;
    else if (std::regex_match(test, testFormula))
        return 3;
    else
        return -1;
}

void Controller::open(std::vector<std::string> &params)
{
    if (isOpen)
    {
        std::cout << "Do you want to save current file?(Y/N) ";
        std::string answ;
        std::cin >> answ;
        while (true)
        {
            if (answ == "Y")
            {
                if (fileName.length() != 0)
                {
                    save();
                }
                else
                {
                    saveAs(params);
                }
                isOpen = false;
                isSaved = false;
                break;
            }
            else if (answ == "N")
            {
                std::cout << "File closed!\n";
                break;
            }
            else
                std::cout << "Please enter Y/N: ";
        }
    }
    std::ifstream reader;
    try
    {
        reader.open(params[1], std::ios::in);
    }
    catch (const std::exception &e)
    {
        reader.open(params[1], std::fstream::in | std::fstream::trunc);
    }
    this->fileName = params[1];

    int currentColumnSize = 0;
    int lastColumnSize = -1;
    int type = -1;
    unsigned int currentRow = 1;
    unsigned int currCol = 0;
    bool empty = false;
    if (reader.is_open())
    {
        std::string line;

        while (std::getline(reader, line))
        {
            unsigned int size = line.length();
            std::string temp;
            currCol = 0;
            for (unsigned int i = 0; i < size; ++i)
            {
                if (line[i] == ',')
                {
                    
                    type = match(temp);
                    if (type == -1)
                    {
                        table.clear();
                        reader.close();
                        std::string error = {"Invalid data at row: " + std::to_string(currentRow) + " column: " + std::to_string(i + 1)};
                        throw std::invalid_argument(error);
                    }
                    else
                    {
                        table.edit(currentRow - 1, currCol, temp, type);
                        currentColumnSize++;
                    }
                    temp.clear();

                    empty = true;
                    currCol++;
                }
                else
                {
                    temp += line[i];
                }
            }
            type = match(temp);
            if (type == -1)
            {
                table.clear();
                reader.close();
                std::string error = {"Invalid data at row: " + std::to_string(currentRow) + " column: " + std::to_string(size)};
                throw std::invalid_argument(error);
            }
            else
            {
                table.edit(currentRow - 1, currCol, temp, type);
                currentColumnSize++;
            }

            if (currentColumnSize != lastColumnSize && lastColumnSize != -1)
            {
                std::string error = {"Invalid data at row: " + std::to_string(currentRow) + " column: " + std::to_string(size)};
                throw std::invalid_argument(error);
            }
            lastColumnSize = currentColumnSize;
            currentColumnSize = 0;
            currentRow++;
        }
        reader.close();
        isOpen = true;
        isSaved = true;
        std::cout << "Table loaded!\n";
    }
    else
        throw std::invalid_argument("Unable to open file");
}

void Controller::close()
{
    if (!isOpen)
        throw std::invalid_argument("No open spreadsheet!");
    if (!isSaved)
    {
        std::cout << "Do you want to save current file?(Y/N) ";
        std::string answ;
        std::cin >> answ;
        while (true)
        {
            if (answ == "Y")
            {
                if (fileName.length() != 0)
                {
                    save();
                }
                else
                {
                    std::cout << "Enter name of file: ";
                    std::string name;
                    std::cin >> name;
                    std::vector<std::string> params = {"NULL", name};
                    saveAs(params);
                }
                break;
            }
            else if (answ == "N")
            {
                std::cout << "File closed!\n";
                break;
            }
            else
                std::cout << "Please enter Y/N: ";
        }
    }
    else
        std::cout << "Successfully closed!\n";

    isSaved = false;
    isOpen = false;
}

void Controller::saveAs(std::vector<std::string> &params)
{
    if (!isOpen)
        throw std::invalid_argument("No open spreadsheet!");
    std::ofstream writer;
    writer.open(params[1], std::fstream::out | std::fstream::trunc);
    // TO DO -> write in file
    if (writer.is_open())
    {
        std::string toWrite;
        unsigned int rows = table.getMaxRow();
        unsigned int cols = table.getMaxColumn();
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                std::string val = table.getValueAt(i, j);
                if (val.length() > 0)
                {
                    writer << val;
                }
                if (j != cols - 1)
                    writer << ",";
            }
            writer << "\n";
        }
        writer.close();
        std::cout << "File saved!\n";
        isSaved = true;
    }
    else
        throw std::invalid_argument("Can not save file!");
}

void Controller::save()
{
    if (!isOpen)
        throw std::invalid_argument("No open spreadsheet!");
    if (fileName.length() == 0)
    {
        std::cout << "Enter name of file: ";
        std::string name;
        std::cin >> name;
        name+=".csv";
        std::vector<std::string> params = {"NULL", name};
        saveAs(params);
    }
    else
    {
        std::vector<std::string> params = {"NULL", fileName};
        saveAs(params);
    }
}

void Controller::help() const
{
    std::cout << "List of commands:\nopen <fileName> - open and load table\nclose - close current table\nsaveAs <fileName> - save table in specified file\nsave - save table in current file\nhelp - commands menu\nexit - terminate program\nedit <Adress> <Value> - change value of specified cell\nprint - print table\n";
}

void Controller::exit()
{
    if (isOpen && !isSaved)
    {
        std::cout << "Do you want to save current file?(Y/N) ";
        std::string answ;
        std::cin >> answ;
        while (true)
        {
            if (answ == "Y")
            {
                if (fileName.length() != 0)
                {
                    save();
                }
                else
                {
                    std::cout << "Enter name of file: ";
                    std::string name;
                    std::cin >> name;
                    std::vector<std::string> params = {"NULL", name};
                    saveAs(params);
                }
                break;
            }
            else if (answ == "N")
            {
                std::cout << "File closed!\n";
                break;
            }
            else
                std::cout << "Please enter Y/N: ";
        }
    }
    std::cout << "Exiting program...";
}

void Controller::edit(std::vector<std::string> &params)
{
    isOpen = true;
    // check if address is correct
    std::regex testAddress("R[1-9]{1}[0-9]*C[1-9]{1}[0-9]*");
    if (!(std::regex_match(params[1], testAddress)))
        throw std::invalid_argument("Invalid address!");
    int type = match(params[2]);
    if (type == -1)
        throw std::invalid_argument("Invalid value!");
    std::string tmp;
    int i = 1;
    for (i;; ++i)
    {
        if (params[1][i] == 'C')
            break;
        else
            tmp += params[1][i];
    }
    unsigned int row = std::stoi(tmp);
    ++i;
    int sz = params[1].length();
    tmp.clear();
    for (i; i < sz; ++i)
    {
        tmp += params[1][i];
    }
    unsigned int column = std::stoi(tmp);
    table.edit(row - 1, column - 1, params[2], type);
    isSaved = false;
    std::cout << "Cell updated!\n";
}

void Controller::print()
{
    if (!isOpen)
        throw std::invalid_argument("No open file!");
    table.print();
}

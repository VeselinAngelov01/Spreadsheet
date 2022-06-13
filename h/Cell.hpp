#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <queue>
#include <stack>
#include <cmath>
class Cell
{
private:
    unsigned int row;
    unsigned int col;
    std::string value;
    int type;// 0 - int 1 - float 2 - string 3- formula, 4- empty
public:
    Cell(unsigned int row, unsigned int col,std::string value,int type);

    std::string getValue() const;
    int getType() const;
    void change(std::string value,int type);

    unsigned int getRow() const;
    unsigned int getColumn() const;
};
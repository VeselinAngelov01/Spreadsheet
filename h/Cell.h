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
    int type;
public:
    ///Constructor
    Cell(unsigned int row, unsigned int col,std::string value,int type);
    //Return value of the cell
    std::string getValue() const;
    ///Return type of the cell\n
    /// 0 - int\n
    /// 1 - float\n
    /// 2 - string\n 
    /// 3 - formula\n 
    /// 4 - empty
    int getType() const;
    ///Chnage current cell value
    void change(std::string value,int type);
    ///Return cell's row
    unsigned int getRow() const;
    ///Return cell's column
    unsigned int getColumn() const;
};
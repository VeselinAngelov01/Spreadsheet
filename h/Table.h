#include "Cell.h"


class Table
{
private:
    std::vector<Cell> table;

    // 2 variables for easy print
    unsigned int max_row;
    unsigned int max_column;
    unsigned int max_size;
    std::string calculate(std::string value);
    std::string SYN(std::queue<std::string>& expr);
    int getTypeAt(unsigned int row,unsigned int column);
public:
    ///Constructor
    Table();   
    ///Return value at specific cell's value at (row,column)
    std::string getValueAt(unsigned int row,unsigned int column);
    ///Edit specific cell at (row,column)
    void edit(unsigned int row,unsigned int column,std::string value,int type);
    ///Print table
    void print();
    ///Clear table
    void clear();
    ///Return total rows
    unsigned int getRowCount() const;
    ///Return max max row
    unsigned int getMaxRow() const;
    ///Return max column number
    unsigned int getMaxColumn() const;
};
#include "Cell.hpp"


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
    Table();

    std::string getValueAt(unsigned int row,unsigned int column);
    void edit(unsigned int row,unsigned int column,std::string value,int type);
    void print();

    void clear();

    unsigned int getRowCount() const;
    unsigned int getMaxRow() const;
    unsigned int getMaxColumn() const;
};
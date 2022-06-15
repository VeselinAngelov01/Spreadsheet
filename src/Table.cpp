#include "../h/Table.h"

Table::Table()
{
    max_row = 0;
    max_column = 0;
    max_size = 10;
}

std::string Table::getValueAt(unsigned int row, unsigned int column)
{
    int sz = table.size();
    for (int i = 0; i < sz; ++i)
    {
        if (table.at(i).getRow() == row && table.at(i).getColumn() == column)
        {
            return this->table.at(i).getValue();
        }
    }
    return "";
}

void Table::edit(unsigned int row, unsigned int column, std::string value, int type)
{
    bool res = false;
    int sz = table.size();
    for (int i = 0; i < sz; ++i)
    {
        if (table.at(i).getRow() == row && table.at(i).getColumn() == column)
        {
            table.at(i).change(value, type);
            res = true;
        }
    }
    if (!res)
    {
        Cell temp(row, column, value, type);
        this->table.push_back(temp);
    }

    if (row + 1 > max_row)
        max_row = row + 1;
    if (column + 1 > max_column)
        max_column = column + 1;
    if (value.length() > max_size)
    {
        max_size = value.length();
    }
}

std::string Table::SYN(std::queue<std::string> &expr)
{
    std::stack<std::string> result;
    double first, second;
    while (!expr.empty())
    {
        std::string top = expr.front();
        expr.pop();
        if (top == "+")
        {
            first = std::stod(result.top());
            result.pop();
            second = std::stod(result.top());
            result.pop();
            result.push(std::to_string(first + second));
        }
        else if (top == "-")
        {
            first = std::stod(result.top());
            result.pop();
            second = std::stod(result.top());
            result.pop();
            result.push(std::to_string(second - first));
        }
        else if (top == "*")
        {
            first = std::stod(result.top());
            result.pop();
            second = std::stod(result.top());
            result.pop();
            result.push(std::to_string(first * second));
        }
        else if (top == "/")
        {
            first = std::stod(result.top());
            result.pop();
            second = std::stod(result.top());
            result.pop();
            if (first == 0)
                throw std::invalid_argument("Division by zero!");
            result.push(std::to_string(second / first));
        }
        else if (top == "^")
        {
            first = std::stod(result.top());
            result.pop();
            second = std::stod(result.top());
            result.pop();
            result.push(std::to_string(std::pow(second, first)));
        }
        else
        {
            result.push(top);
        }
    }
    if (result.size() != 1)
        throw std::invalid_argument("Invalid formula!");
    return result.top();
}

std::string Table::calculate(std::string value)
{
    int size = value.length();
    std::string temp;
    std::queue<std::string> numbers;
    std::stack<std::string> operators;
    for (int i = 1; i < size; ++i)
    {
        if (value[i] == 'R')
        {
            int ind = value.find('C', i + 1);
            unsigned int row = std::stoi(value.substr(i + 1, ind - i - 1));
            int old = ind;
            std::string col;
            ind++;
            while (true)
            {
                if ((value[ind] >= '0' && value[ind] <= '9') || value[ind] == '.')
                    col += value[ind++];
                else
                    break;
            }
            unsigned int column = std::stoi(col);
            if (row > max_row || column > max_column)
            {
                numbers.push("0");
            }
            else
            {
                std::string check = getValueAt(row-1,column-1);
                if(getTypeAt(row,column)==2)
                {
                    check = check.erase(0,1);
                    check = check.erase(check.length()-1,1);
                }
                try
                {
                    double d = std::stod(check);
                    numbers.push(check);
                }
                catch (const std::exception &e)
                {
                    numbers.push("0");
                }
            }

            i = ind - 1;
        }
        else if ((value[i] >= '0' && value[i] <= '9') || value[i] == '.')
        {
            temp += value[i];
        }
        else
        {
            if (temp.length() > 0)
                numbers.push(temp);
            temp.clear();
            if (value[i] == '^')
            {
                temp += value[i];
                operators.push(temp);
            }
            else if (value[i] == '*' || value[i] == '/')
            {
                while (!operators.empty() && operators.top() != "+" && operators.top() != "-")
                {
                    numbers.push(operators.top());
                    operators.pop();
                }
                temp += value[i];
                operators.push(temp);
            }
            else if (value[i] == '+' || value[i] == '-')
            {
                while (!operators.empty())
                {
                    numbers.push(operators.top());
                    operators.pop();
                }
                temp += value[i];
                operators.push(temp);
            }
            else
                throw std::logic_error("Impossible state!");
            temp.clear();
        }
    }
    if (temp.length() != 0)
        numbers.push(temp);
    while (!operators.empty())
    {
        numbers.push(operators.top());
        operators.pop();
    }
    return SYN(numbers);
}

int Table::getTypeAt(unsigned int row, unsigned int column)
{
    int sz = table.size();
    for (int i = 0; i < sz; ++i)
    {
        if (table.at(i).getRow() == row && table.at(i).getColumn() == column)
        {
            return this->table.at(i).getType();
        }
    }
    return 4;
}

void Table::print()
{
    int rows = table.size();
    int type;
    std::string temp;
    for (unsigned int i = 0; i < max_row; ++i)
    {
        for (unsigned int j = 0; j < max_column; ++j)
        {
            temp = getValueAt(i, j);

            if (temp.length() == 0)
            {
                if (j == max_column - 1)
                    std::cout << std::setfill(' ') << std::setw(max_size) << " "
                              << "\n";
                else
                    std::cout << std::setfill(' ') << std::setw(max_size) << " "
                              << "|";
            }
            else
            {
                type = getTypeAt(i, j);
                if (type == 4)
                {
                    std::cout << std::setfill(' ') << std::setw(max_size) << " ";
                }
                else if (type == 3)
                {
                    std::string res = calculate(temp);
                    std::cout << std::setfill(' ') << std::setw(max_size) << res;
                }
                else
                    std::cout << std::setw(max_size) << temp;
                if (j == max_column - 1)
                    std::cout << "\n";
                else
                    std::cout << "|";
            }
        }
    }
}

void Table::clear()
{
    table.clear();
}

unsigned int Table::getRowCount() const
{
    return this->table.size();
}

unsigned int Table::getMaxRow() const
{
    return this->max_row;
}
unsigned int Table::getMaxColumn() const
{
    return this->max_column;
}
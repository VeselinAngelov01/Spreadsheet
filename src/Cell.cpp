#include "../h/Cell.h"

Cell::Cell(unsigned int row, unsigned int col, std::string value, int type)
{
    this->value = value;
    this->type = type;
    this->row = row;
    this->col = col;
}

std::string Cell::getValue() const
{
    return this->value;
}

int Cell::getType() const
{
    return this->type;
}

void Cell::change(std::string value, int type)
{
    this->value = value;
    this->type = type;
}

unsigned int Cell::getRow() const
{
    return this->row;
}
unsigned int Cell::getColumn() const
{
    return this->col;
}
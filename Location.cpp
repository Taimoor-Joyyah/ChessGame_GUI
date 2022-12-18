//
// Created by J-Bros on 12/17/2022.
//

#include "Location.h"

void Location::set(int rank, int file) {
    this->rank = rank;
    this->file = file;
}

bool Location::equal(const Location &cell) const  {
    return cell.rank == rank && cell.file == file;
}
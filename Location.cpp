//
// Created by J-Bros on 12/17/2022.
//

#include "Location.h"

void Location::set(int rank, int file) {
    this->rank = rank;
    this->file = file;
}

bool Location::operator==(const Location &other) const {
    return other.rank == rank && other.file == file;
}

bool Location::equals(int rank, int file) {
    return this->rank == rank && this->file == file;
}

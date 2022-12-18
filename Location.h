//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_LOCATION_H
#define CHESSGAME_LOCATION_H


class Location {
public:
    int rank;
    int file;

    void set(int rank, int file);
    bool equal(const Location &cell) const;
};


#endif //CHESSGAME_LOCATION_H

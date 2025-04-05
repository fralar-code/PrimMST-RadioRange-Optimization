#ifndef COORDS_H
#define COORDS_H

// Class to store coordinates
class Coords {
public:
    Coords() : x(0), y(0) {} 
    Coords(int x_coord, int y_coord) : x(x_coord), y(y_coord) {}

    int getX() const { 
        return this->x;
    }
    int getY() const { 
        return this->y;
    }
private:
    int x;
    int y;
};

#endif // COORDS_H
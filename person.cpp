#include "person.h"

std::ostream &operator<<(std::ostream &os, const person &p) {
    os << "|" << p.surname << " " << p.name << "|";
    return os;
}
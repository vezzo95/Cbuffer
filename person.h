#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>

/**
 * @brief Tipo custom: person
 * 
 * Tipo custom che rappresenta una persona, dotata di nome e cognome.
**/
struct person {
    std::string name;
    std::string surname;

    /**
     * Costruttore di default, necessario per poter creare un array di person.
    **/
    person() {}

    /**
     * Costruttore secondario, permette di creare una person con nome e cognome definiti dall'utente.
     * @param n nome della persona
     * @param s cognome della persona
    **/
    person(const std::string n, const std::string s) : name(n), surname(s) {}
};

/**
 * @brief Operatore di stream in output
 * 
 * Overloading dell'operatore <<, per permettere la stampa su standard output di una variabile di tipo person
**/
std::ostream &operator<<(std::ostream &os, const person &p);

#endif
#include <iostream>
#include "cbuffer.h"
#include "person.h"


/**
 * @brief Test metodi fondamentali
 * 
 * Funzione senza parametri che instanzia alcuni cbuffer di tipo diverso
 * per testarne alcuni metodi.
**/
void test_metodi_fondamentali() {
    //buffer tipo custom
    cbuffer<person> c1(5);
    person p("Amuro", "Ray");
    std::cout << "persona p: " << p << std::endl;
    c1.insert(p);
    c1.insert({"Char", "Aznable"});
    c1.insert({"Noa", "Bright"});
    std::cout << "c1 con operatore <<: ";
    std::cout << c1 << std::endl;
    cbuffer<person>::iterator i, ie;
    std::cout << "c1 con iteratori: ";
    for(i = c1.begin(), ie = c1.end(); i != ie; i++)
        std::cout << *i << " ";
    std::cout << std::endl << "Accesso a c1[2] = " << c1[2] << std::endl;
    
    //buffer tipo int
    cbuffer<int> c2(5);
    c2.insert(1);
    c2.insert(4);
    c2.insert(-3);
    c2.insert(-2);
    c2.insert(123);
    c2.insert(1);
    c2.remove();
    std::cout << c2 << std::endl;
    cbuffer<int> c3(c2);
    cbuffer<int> c4 = c3;
    std::cout << "c4 con operatore <<: ";
    std::cout << c4 << std::endl;

    //buffer tipo unsigned int
    cbuffer<int>::iterator i1, ie1;
    i1 = c4.begin();
    ie1 = c4.end();
    std::cout << "c5 con operatore <<: ";
    cbuffer<unsigned int> c5(i1, ie1, c4.capacity());
    std::cout << c5 << std::endl;
}

/**
 * @brief Predicato intero positivo
 * 
 * Torna true se l'intero passato come parametro è >= 0, false altrimenti
 * @param value Il valore da verificare
 * @return Valore booleano risultante dal confronto
**/
struct positive_int{
    inline bool operator()(const int& value) {return (value >= 0);}
};

/**
 * @brief Predicato intero negativo
 * 
 * Torna true se l'intero passato come parametro è < 0, false altrimenti
 * @param value Il valore da verificare
 * @return Valore booleano risultante dal confronto
**/
struct negative_int {
    inline bool operator()(const int &value) {return (value < 0);}
};

/**
 * @brief Predicato cognome = "Ray"
 * 
 * Torna true se il cognome della persona è "Ray", false altrimenti
 * @param p La persona a cui verificare il cognome
 * @return Valore booleano risultante dal confronto
**/
struct srn_ray {
    inline bool operator()(const person &p) {return (p.surname == "Ray");} 
};

/**
 * @brief Valuta elementi del buffer
 * 
 * Verifica se gli elementi di un buffer soddisfino un predicato unario,
 * che riceve in input l'elemento del buffer da verificare
 * 
 * @param cb buffer circolare di elementi generici
 * @param pred funtore unario 
**/
template <typename T, typename P>
void evaluate_if(cbuffer<T> cb, P pred) {
    typename cbuffer<T>::size_type i;
    typename cbuffer<T>::size_type size = cb.size();

    for(i = 0; i < size; i++) {
        std::cout << "[" << i << "]: ";
        if(pred(cb[i]))
            std::cout << "true" << std::endl;
        else
            std::cout << "false" << std::endl;
    }
}


int main() {
    test_metodi_fondamentali();
    std::cout << "===========================\n";
    cbuffer<int> cb(5);
    cb.insert(1);
    cb.insert(2);
    cb.insert(-4);
    cb.insert(0);
    cb.insert(-1);

    cbuffer<person> cbp(5);
    cbp.insert({"Amuro", "Ray"});
    cbp.insert({"Char", "Aznable"});
    cbp.insert({"Noa", "Bright"});

    positive_int p;
    negative_int q;
    srn_ray r;
    
    std::cout << "Test evaluate_if con cbuffer di interi e funtore positive_int:" << std::endl;
    evaluate_if(cb, p);
    std::cout << "Test evaluate_if con cbuffer di interi e funtore negative_int:" << std::endl;
    evaluate_if(cb, q);
    std::cout << "Test evaluate_if con cbuffer di person e funtore srn_ray" << std::endl;
    evaluate_if(cbp, r);
}





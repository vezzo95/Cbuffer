#ifndef CBUFFER_H
#define CBUFFER_H

#include <ostream> // std::ostream
#include <iostream>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
 * @file cbuffer.h
 * @brief Dichiarazione della classe cbuffer
 * 
 * Buffer circolare di elementi generici T. La dimensione viene decisa in fase di costruzione.
**/

template <class T>
class cbuffer {
    public:
        typedef unsigned int size_type;
        class const_iterator;
        class iterator;

        friend class iterator;
        friend class const_iterator;

        /**
         * @bried Costruttore di default
         * 
         * Costruttore di default che instanzia un cbuffer vuoto.
        **/
        cbuffer() : _buffer(0), _capacity(0), _head(0), _size(0) {
            #ifndef NDEBUG
            std::cout << "cbuffer::cbuffer()" << std::endl;
            #endif    
        }

        /**
         * @brief Costruttore secondario
         * 
         * Costruttore secondario che prende in input la dimensione del cbuffer
        **/
        explicit cbuffer(size_type capacity) : _buffer(0), _capacity(0), _head(0), _size(0) {
            _buffer = new T[capacity];
            _capacity = capacity;
        
            #ifndef NDEBUG
            std::cout << "cbuffer::cbuffer(size_type)" << std::endl;
            #endif    
        }

        /**
         * @brief Costruttore di copia
         * 
         * Costruttore fondamentale, instanzia un cbuffer a partire dal reference di un altro cbuffer
        **/
        cbuffer(const cbuffer& other) : _buffer(0), _capacity(0), _head(0), _size(0) {
            _buffer = new T[other._capacity];
            _capacity = other._capacity;
            _head = other._head;
            _size = other._size;

            try{
                for(int i = 0; i < _capacity; i++)
                    _buffer[i] = other._buffer[i];
            }
            catch(...){
                delete[] _buffer;
                _buffer = 0;
                _capacity = 0;
                _head = 0;
                _size = 0;
                throw;
            }

            #ifndef NDEBUG
            std::cout << "cbuffer::cbuffer(const cbuffer&)" << std::endl;
            #endif     
        }

        /**
         * @brief Operatore di assegnamento
         * 
         * Overloading dell'operatore di assegnamento tra due cbuffer
        **/
        cbuffer &operator=(const cbuffer &other) {
            if(this != other) {
                cbuffer tmp(other);
                this->swap(tmp);
            }
            #ifndef NDEBUG
		    std::cout << "cbuffer::operator=(const cbuffer&)" << std::endl;
		    #endif

		    return *this;
        }

        /**
         * @brief Costruttore secondario
         * 
         * Costruttore secondario, prende in input l'iteratore di inizio e fine di un cbuffer
         * di tipo diverso da T. La conversione da Q a T è lasciata al compilatore.
         * @param begin Iteratore di inizio sequenza
         * @param end Iteratore di fine sequenza
         * @param dim Dimensione del buffer
         * @throw Eccezione di allocazione memoria
        **/
        template <typename IterT>
        cbuffer(IterT begin, IterT end, unsigned int dim) : _buffer(0), _head(0), _size(0), _capacity(0) {
            try {
                _buffer = new T[dim];
            }
            catch(...) {
                throw;
            }
            _capacity = dim;
            size_type index = 0;
            while(begin != end) {
                this->insert(*begin);
                begin++;
            }
        }

        /**
         * @brief Distruttore
         * 
         * Distruttore della classe cbuffer
        **/
        ~cbuffer() {
            delete[] _buffer;
            _buffer = 0;
            _capacity = 0;
            _head = 0;
            _size = 0;

            #ifndef NDEBUG
            std::cout << "cbuffer::~cbuffer()" << std::endl;
            #endif
        }

        /**
         * @brief Operatore di accesso all'elemento index-esimo
         * 
         * Permette di accedere all'elemento index-esimo del cbuffer in lettura e scrittura
         * @param index l'indice della posizione del buffer a cui si vuole accedere
         * @throw std::out_of_range se index >= _size
        **/
        T &operator[](size_type index) {
            try {
                return _buffer[to_phisycal(index)];
            }
            catch(...) {
                throw;
            }
        }

        /**
         * @brief Operatore di accesso all'elemento index-esimo
         * 
         * Permette di accedere all'elemento index-esimo del cbuffer in sola lettura
         * @param index l'indice della posizione del buffer a cui si vuole accedere
         * @throw std::out_of_range se index >= _size
        **/
        const T &operator[](size_type index) const {
		    try {
                return _buffer[to_phisycal(index)];
            }
            catch(...) {
                throw;
            }
	    }

        /**
         * @brief Operatore di uguaglianza
         * 
         * Overloading dell'operatore di uguaglianza tra un cbuffer e un cbuffer costante
         * Viene confrontato il reference
        **/
        bool operator==(const cbuffer& other) const {
            return *this == other;
        }

        /**
         * @brief Operatore di uguaglianza
         * 
         * Overloading dell'operatore di uguaglianza tra due cbuffer
         * Viene confrontato il reference
        **/
        bool operator==(cbuffer& other) const {
            return *this == other;
        }

        /**
         * @brief Operatore di disuguaglianza
         * 
         * Overloading dell'operatore di disuguaglianza tra un cbuffer e un cbuffer costante
         * Viene confrontato il reference
        **/
        bool operator!=(const cbuffer& other) const {
            return *this != other;
        }

        /**
         * @brief Operatore di disuguaglianza
         * 
         * Overloading dell'operatore di disuguaglianza tra due cbuffer
         * Viene confrontato il reference
        **/
        bool operator!=(cbuffer& other) const {
            return *this != other;
        }

        /**
         * @brief Capacità del buffer
         * 
         * Ritorna la dimensione dell'array dinamico.
         * @return La dimensione dell'array dinamico.
        **/
        size_type capacity() const {
            return _capacity;
        }

        /**
         * @brief Dimensione del buffer
         * 
         * Ritorna la dimensione attuale del buffer circolare.
         * @return La dimensione attuale del buffer circolare.
        **/
        size_type size() const {
            return _size;
        }

        /**
         * @brief Indice della testa
         * 
         * Ritorna l'indice in cui è memorizzata la testa del buffer'
         * @return L'indice in cui è memorizzata la testa del buffer
        **/
        size_type head() const {
            return _head;
        }

        /**
         * @brief Indice della coda
         * 
         * Ritorna l'indice in cui è memorizzata la coda del buffer'
         * @return L'indice in cui è memorizzata la coda del buffer
        **/
        size_type tail() const {
            return (_head + _size) % capacity;
        }

        /**
         * @brief Accesso in lettura/scrittura
         * 
         * Metodo getter per l'accesso all'index-esimo elemento del buffer
         * @pre E' necessario che index sia minore di _size
         * @param index Un indice del buffer
         * @return L'index-esimo elemento del buffer
        **/
        T &value(size_type index) const {
            try {
                return _buffer[to_phisycal(index)];
            }
            catch(...) {
                throw;
            }
        }

        /**
         * @brief Inserimento di un nuovo elemento
         * 
         * Metodo per inserire un nuovo elemento in coda al buffer.
         * @param value Un elemento da inserire
        **/
        void insert(const T &value) {
            _buffer[(_head + _size) % _capacity] = value;
            if(_size == _capacity)
                _head = (_head + 1) % _capacity;
            else
                _size++;
        }

        /** Rimozione di un elemento
         * 
         * Rimuove un elemento dalla testa del buffer
        **/
        void remove() {
            if(_size != 0) {
                _head = (_head + 1) % _capacity;
                _size--;
            }  
        }

        /**
         * @brief scambio tra cbuffer
         * 
         * Funzione che effettua lo scambio di due cbuffer
         * @param Un reference ad un cbuffer
        **/
        void swap(cbuffer &other) {
            std::swap(this->_buffer, other._buffer);
            std::swap(this->_capacity, other._capacity);
            std::swap(this->_head, other._head);
            std::swap(this->_size, other._size);
        }

        class iterator {
            private:
                const cbuffer *cb;
                unsigned int offset;

                friend class cbuffer;
                friend class const_iterator;

                //Costruttore utilizzato da begin e end
                iterator(cbuffer* c, unsigned int o) : cb(c), offset(o) {
                }
            
            public:
                typedef std::forward_iterator_tag iterator_category;
		        typedef T                         value_type;
		        typedef ptrdiff_t                 difference_type;
		        typedef T*                        pointer;
		        typedef T&                        reference;
                
                //Costruttore di default
                iterator() : cb(NULL), offset(0) {}

                //Costruttore di copia (iteratore di lettura e scrittura)
                iterator(const iterator& other) : 
                        cb(other.cb), offset(other.offset) {
                }

                //Costruttore di copia (Iteratore costante di sola lettura)
                iterator(const const_iterator& other) : 
                        cb(other.cb), offset(other.offset) {
                }

                //Operatore di assegnamento tra due iteratori
                iterator& operator=(const iterator &other) {
			        cb = other.cb;
                    offset = other.offset;
			        return *this;
		        }

                //Distruttore
                ~iterator() {}

                //Ritorna il dato riferito dall'iteratore (dereferenziamento)
                reference operator*() const {
                    return *(cb->_buffer + cb->to_phisycal(offset));
                }

                //Ritorna il puntatore al dato riferito dall'iteratore
                pointer operator->() const {
                    return cb._buffer + to_phisycal(offset);
                }

                //Operatore di pre-incremento
                iterator& operator++() {
                    offset++;
                    return *this;
                }

                //Operatore di post-incremento
                iterator operator++(int) {
                    iterator tmp(*this);
                    offset++;
                    return tmp;
                }

                //Operatore di uguaglianza (due iteratori)
		        bool operator==(const iterator &other) const {
			        return (cb == other.cb && offset == other.offset);
		        }

                //Operatore di uguaglianza (un iteratore e un iteratore costante)
                bool operator==(const const_iterator &other) const {
			        return (cb == other.cb && offset == other.offset);
		        }

		        //Operatore di disuguaglianza (due iteratori)
		        bool operator!=(const iterator &other) const {
			        return (cb != other.cb || offset != other.offset);
		        }
                
                //Operatore di disuguaglianza (un iteratore e un iteratore costante)
                bool operator!=(const const_iterator &other) const {
			        return (cb != other.cb || offset != other.offset);
		        }
        };

        /**
         * @brief Iteratore di inizio sequenza
         * 
         * Ritorna l'iteratore di inizio sequenza del cbuffer
        **/
        iterator begin() {
            return iterator(this, 0);
        }

        /**
         * @brief Iteratore di fine sequenza
         * 
         * Ritorna l'iteratore di fine sequenza del cbuffer
        **/
        iterator end() {
            return iterator(this, _size);
        }

        class const_iterator {
            private:
                const cbuffer * const cb;
                unsigned int offset;

                friend class cbuffer;
                friend class iterator;

                //Costruttore utilizzato da begin e end
                const_iterator(cbuffer* c, unsigned int o) : cb(c), offset(o) {
                }
            
            public:
                typedef std::forward_iterator_tag iterator_category;
		        typedef T                         value_type;
		        typedef ptrdiff_t                 difference_type;
		        typedef T*                        pointer;
		        typedef T&                        reference;
                
                //Costruttore di default
                const_iterator() : cb(NULL), offset(0) {}

                //Costruttore di copia 
                const_iterator(const const_iterator& other) : cb(other.cb), offset(other.offset) {
                }
                
                //Costruttore di copia (iteratore di lettura e scrittura)
                const_iterator(const iterator& other) : cb(other.cb), offset(other.offset) {
                }

                //Operatore di assegnamento (iteratore costante di sola lettura)
                const_iterator& operator=(const const_iterator &other) {
			        cb = other.cb;
                    offset = other.offset;
			        return *this;
		        }

                //Operatore di assegnamento (iteratore di lettura e scrittura)
                const_iterator& operator=(const iterator &other) {
			        cb = other.cb;
                    offset = other.offset;
			        return *this;
		        }

                //Distruttore
                ~const_iterator() {}

                //Ritorna il dato riferito dall'iteratore (dereferenziamento)
                reference operator*() const {
                    return *(cb->_buffer + cb->to_phisycal(offset));
                }
                
                //Ritorna il puntatore al dato riferito dall'iteratore
                pointer operator->() const {
                    return cb._buffer + to_phisycal(offset);
                }

                //Operatore di pre-incremento
                const_iterator& operator++() {
                    offset++;
                    return *this;
                }

                //Operatore di post incremento
                const_iterator operator++(int) {
                    const_iterator tmp(*this);
                    offset++;
                    return tmp;
                }

                //Operatore di uguaglianza (due iteratori costanti)
                bool operator==(const const_iterator &other) const {
			        return (cb == other.cb && offset == other.offset);
		        }

                //Operatore di uguaglianza (un iteratore e un iteratore costante)
		        bool operator==(const iterator &other) const {
			        return (cb == other.cb && offset == other.offset);
		        }
		        
                //Operatore di disuguaglianza (due iteratori costanti)
		        bool operator!=(const const_iterator &other) const {
			        return (cb != other.cb || offset != other.offset);
		        }
                
                //Operatore di disuguaglianza (un iteratore e un iteratore costante)
                bool operator!=(const iterator &other) const {
			        return (cb != other.cb || offset != other.offset);
		        }
        };

        /**
         * @brief Iteratore costante di inizio sequenza
         * 
         * Ritorna l'iteratore costante di inizio sequenza del cbuffer
        **/
        const_iterator begin() const {
            return const_iterator(this, 0);
        }

        /**
         * @brief Iteratore costante di fine sequenza
         * 
         * Ritorna l'iteratore costante di fine sequenza del cbuffer
        **/
        const_iterator end() const {
            return const_iterator(this, _size);
        }


    private:
        T* _buffer;
        size_type _capacity;
        size_type _head;
        size_type _size;

        /**
         * @brief Conversione dell'indice logico in fisico
         * 
         * Converte l'indice della locazione del buffer cui si vuole accedere nell'indice dell'array
         * cui si trova la posizione richiesta del buffer.
         * @param i indice del buffer
         * @return indice dell'array
        **/
        size_type to_phisycal(size_type i) const {
            if(i < _size)
                return (i + _head) % _capacity;
            else
                throw std::out_of_range("Index out of range");
        }   
};


        
template <typename T>
std::ostream& operator<<(std::ostream &os, const cbuffer<T> & cb) {
	for (typename cbuffer<T>::size_type i = 0; i < cb.size(); ++i)
		os << cb[i] << " ";
	return os;
}
#endif

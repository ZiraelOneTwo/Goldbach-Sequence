#include <stdio.h>
#include <time.h>

/* 
    Dato un N, controllare se la congettura di Golbach è verificata per ogni n<=N.
*/

/*
    PRE: n>2 e pari
    POST: restituisce 0 se la congettura di Goldbach vale per ogni i. 2<i<=n
                      k se la congettura di Goldbach non è verificata per k
*/
unsigned int golbach_sequenza(unsigned int n);

/*
    PRE: n>2 e pari
    POST: Restituisce   1 se esiste (x,y).x+y=n, x e y sono primi;
                        0 altrimenti
*/
unsigned int goldbach_singolo(unsigned int n); // COMPLETATE VOI LA LISTA DEI PARAMETRI

/*
    PRE:
    POST restituisce 1 se n > 2 e pari
                     0 altrimenti 
*/
unsigned char verifica_input(unsigned int n);

/*
    LE MIE FUNZIONI
*/
unsigned char is_prime(unsigned int n);
void init_prime_bits(void);

/*
    VARIABILI GLOBALI
*/
static unsigned long long prime_bits = 0ULL;

int main(void) 
{
    int N, res;

    scanf("%d", &N);
    if ( !verifica_input(N) )
        printf("Errore: la congettura di Goldbach è definita per interi pari e maggiori di 2, ricevuto %d\n", N);

    clock_t inizio = clock();
    res = golbach_sequenza(N);
    if (res==0) 
        printf("Congettura di Goldbach verificata fino a %d\n", N);
    else 
        printf("Congettura di Goldbach non verificata per %d\n", res);
    clock_t fine = clock();
    double tempo_trascorso = ((double)(fine - inizio)) / CLOCKS_PER_SEC;
    printf("Tempo di esecuzione: %f secondi\n", tempo_trascorso);
}

/*
    Per inizializzare una stringa di 64 bit, con fino a 129 valori dispari!
    1 in posizione n-esima, equivale a primo, 0 a non primo, algoritmo: crivello di eristotene.
*/
void init_prime_bits(void) 
{
    prime_bits = ~0ULL;     //Tutta la stringa ad 0xF.
    for (unsigned short i = 3; i <= __builtin_sqrt(129); i+=2)  // Basta fino alla radice quadrata per i divisori di n.
        if (prime_bits & (1ULL << ((i-3)>>1)))      // Controllo posizione per posizione.
            for (unsigned short j = i*i; j <= 129; j += (i<<1))     // Comincio da i*i ed incremento di 2i.
                prime_bits &= ~(1ULL << ((j-3)>>1));    // Setto ad ogni multiplo di i, bit = 0, il match avviene a complemento a 1. 
}

__inline__ unsigned char verifica_input(unsigned int n)
{
    return !(n&1) && (n>>2);    // Se n pari e >2(3 o "11" escluso perchè dispari in principio).
}

__inline__ unsigned char is_prime(unsigned int n)
{
    if (n == 2) return 1;
    if (n < 2 || !(n & 1)) return 0;     // NB: 1 non è primo, e se n è divisibile per 2, ho finito.
    if (n < 130) return (prime_bits & (1ULL << ((n-3)>>1))) != 0;   // Se n rientra nel range dei 129 valori dispari, allora con un op bitwise ho finito.
    
    // Algoritmo: https://en.wikipedia.org/wiki/Primality_test
    unsigned int l = __builtin_sqrt(n);     // Direttiva del compilatore per calcolare ottimamente la radice quadrata.
    for (unsigned int i = 5; i<=l; i+=6)    
        if (!(n%i) || !(n%(i+2)))
            return 0;
    return 1;
}

__inline__ unsigned int goldbach_singolo(unsigned int n) 
{
    if (!verifica_input(n)) return 0;
    if (n == 4 || is_prime(n-2)) {  // Casi estremi, se n=4=2+2 oppure se n-2 è primo, ho risolto.
        //printf("%d=%d+%d\n", n, 2, n-2);
        return 1;
    }
    
    unsigned int upper_start = n-3;     // N è sempre pari, a e b sono primi, quindi dispari, allora inizio dall'alto (upper_start) dispari.
    while (__builtin_expect(upper_start >= (n>>1), 1)) {    // Statisticamente più probabile che sia sempre minore uguale che maggiore.
        if (is_prime(upper_start) & is_prime(n-upper_start)) {  // Basta che agli estremi siano primi.
            //printf("%d=%d+%d\n", n, n-upper_start, upper_start);
            return 1;
        }
        upper_start -= 2;
    }
    return 0;
}

unsigned int golbach_sequenza(unsigned int n) 
{
    if (!verifica_input(n)) return n;
    init_prime_bits();  // Inizializzazione della stringa di bit codificata per i primi <130.
    unsigned int a = 4;
    while ((a <= n) && goldbach_singolo(a)) a+=2;   // Solo 2a=b+c soddisfa il criterio, dove b e c sono primi.
    return (a < n) ? a : 0;
}
// Capitale struct
typedef struct capit {
  char *nome;
  char *nazione;
  struct capit *left, *right;
} capitale;

// Prototypes
void capitale_stampa(const capitale*, FILE*);
capitale* capitale_crea(char*, char*);
void capitale_distruggi(capitale*);
capitale* capitale_leggi(FILE*);
void abr_capitale_stampa(const capitale*, FILE*);
void abr_capitale_stampa_preorder(const capitale*, FILE*, int);
void abr_capitale_distruggi(capitale*);
capitale* abr_inserisci(capitale*, capitale*);
capitale* crea_abr(FILE*);
capitale* abr_ricerca(capitale*, char*);
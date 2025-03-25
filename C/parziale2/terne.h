

typedef struct terna {
  int x,y,z;
  struct terna *left, *right;
} terna;


terna *terna_crea(int a, int b, int c);
void terna_distruggi(terna *t);
void terna_stampa(const terna *t, FILE *f);
int terna_confronta(terna *t, terna *v);
void termina(const char *messaggio);

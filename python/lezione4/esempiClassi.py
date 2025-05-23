# Pagine per approfondire:
# https://pytutorial-it.readthedocs.io/it/python3.11/classes.html
# https://docs.python.org/3/tutorial/classes.html
# https://docs.python.org/3/reference/datamodel.html

import math, functools

# quello che segue è un decorator che definisce gli operatori
# di confronto in funzione di quelli forniti nelel classi
# per approfondire:
#   https://docs.python.org/3.11/library/functools.html#functools.total_ordering
#   https://stackoverflow.com/questions/16238322/python-total-ordering-why-lt-and-eq-instead-of-le
@functools.total_ordering 
class Articolo:
    articoli_totali = 0  # attributo/variabile di classe, condiviso tra tutte le istanze

    # invocato scrivendo ad esempio:
    #   libro = Articolo("Lo Hobbit",10.50)
    def __init__(self, n, prezzo, commenti=None): # commenti è un parametro opzionale
        """
        Metodo che viene chiamato durante la costruzione di una istanza di
        Articolo per impostarne lo stato iniziale.
        """
        self.nome = n
        self.prezzo = prezzo
        if commenti is None:
            self.commenti = []
        else:
            self.commenti = commenti
        # ^^^ self.nome, .prezzo, .commenti sono attributi di istanza
        Articolo.articoli_totali += 1  # modifica dell'attributo di classe

    # invocato scrivendo 
    #  libro.valuta("Mario",4)   
    def valuta(self, utente, stelline):
        assert 1 <= stelline <= 5, "Stelline non valide"
        # ^^^ a <= x <= b è una sintassi ammessa in Python (in Java no; in C compila, ma non ha la semantica che ci si aspetta)
        self.commenti.append((utente, stelline))

    # invocato scrivendo 
    #  libro.valutazione_media()   
    def valutazione_media(self):
        if len(self.commenti) == 0:
            return 0

        tot = 0
        for (_, stelline) in self.commenti: # _ si usa per convenzione per indicare che un valore (in questo caso l'utente) non ci interessa
            tot += stelline

        # oppure le tre righe qui sopra si possono semplificare con la seguente espressione
        # tot = sum(  s for _,s in self.commenti  )
        return tot / len(self.commenti)

    # si invoca scrivendo libro.sconta(30)
    def sconta(self, percentuale):
        assert 0 <= percentuale <= 100, "Percentuale non valida"
        self.prezzo = self.prezzo - (self.prezzo * percentuale / 100)

    # metodo statico (chiamati anche di classe in Java) 
    @staticmethod
    def prezzo_99_cent(prezzo):
        """
        Esempio di metodo statico. Nota che non c'è self come argomento.
        """
        nuovo_prezzo = math.floor(prezzo)
        nuovo_prezzo += 0.99
        return nuovo_prezzo

    def __str__(self):
        """
        Metodo chiamato da str(oggetto) e da print(oggetto).
        Solitamente si implementa restituendo una stringa con una 
        rappresentazione informale/leggibile dell'oggetto.
        Se non è definito usa __repr__ (vedi sotto)
        """
        stelline_medie = self.valutazione_media()
        return f"{self.nome} - {self.prezzo}€ ({stelline_medie}/5)"

    def __repr__(self):
         """
         Metodo chiamato da repr(oggetto), e dalla console REPL quando si
         scrive oggetto e si preme invio.
         Se possibile, l'implementazione deve restituire una stringa che,
         quando passata a eval(), ricostruisce l'oggetto.
         Se non è definito usa il metodo di Object che stampa solo l'indirizzo
         """
         return f"Articolo('{self.nome}', {self.prezzo}, {self.commenti})"


    # per i metodi che seguono non c'è una definizione "giusta"
    # decidere se due oggetti sono uguali dipende dall'applicazione
    # non dalle proprietà del linguaggio python 

    # def __eq__(self, altro):
    #    """Metodo chiamato dall'espressione oggetto == altro.
    #       Se non è definito usa la definizione di Object basata sull'indirizzo in memoria"""
    #    return self.nome == altro.nome and self.prezzo == altro.prezzo and self.commenti == altro.commenti

    # def __hash__(self):
    #     """
    #     Metodo chiamato da hash(oggetto). È necessario implementarlo,
    #     insieme a __eq__, se si vogliono inserire istanze di Articolo in
    #     collezioni basate su funzioni hash, come i dizionari e gli insiemi
    #     Python.
    #     L'implementazione deve garantire: a == b implica hash(a) == hash(b).
    #     """
    #     tupla_commenti = tuple(self.commenti) # trasformo la lista dei commenti in una tupla per poterla usare con hash()
    #     return hash((self.nome, self.prezzo, tupla_commenti))

    def __lt__(self, altro):
        """
        Metodo chiamato dall'espressione oggetto < altro. 
        Viene usato ad esempio da sorted(lista_di_oggetti_Articolo).
        """

        assert type(altro) == Articolo, "Erorre: parametro altro deve essere di tipo Articolo"

        if self.nome < altro.nome:
          return True
        if self.nome > altro.nome:
          return False
        return self.prezzo < altro.prezzo

# --------------------------------------
# esempi creazione e modifica articoli
# --------------------------------------    
latte = Articolo("Latte 1LT", 1.20)
print(f"prezzo di {latte.nome}: {latte.prezzo}") # accedere a un attributo di istanza
latte.valuta("Mario", 5)                         # chiamare un metodo di istanza
latte.valuta("Roberta", 2)
Articolo.prezzo_99_cent(6.55)   # chiamare un metodo statico
print("Totale articoli creati:", Articolo.articoli_totali) # accedere a un attributo di classe
latteUHT = Articolo("Latte 1LT", 1.10)
latteUHT2 = Articolo("Latte 1LT", 1.10)
print("Totale articoli creati:", Articolo.articoli_totali) # accedere a un attributo di classe


# ---------------------
# ESEMPIO DI ALIASING 1
# ---------------------
latte2 = latte 
# latte e latte2 ora fanno riferimento allo stesso oggetto, ossia id(latte2) == id(latte)
latte2.prezzo = 1.50 
# ... quindi qui anche latte.prezzo == 1.50


# ---------------------
# ESEMPIO DI ALIASING 2
# ---------------------
def black_friday(articoli):
    for articolo in articoli:
        articolo.sconta(30)

cuffie = Articolo("Cuffie Bluetooth", 50)
pc = Articolo("PC portatile", 1200)
da_scontare = [cuffie, pc]
black_friday(da_scontare)
# ora cuffie e pc sono scontati del 30%


# ------------
# EREDITARIETÀ
# ------------
class Film(Articolo):
    def __init__(self, nome, prezzo, durata, commenti=None):
        super().__init__(nome, prezzo, commenti)
        self.durata = durata

    # Per esercitarsi: sovrascrivere __str__, __eq__, __hash__ per includere anche la durata


class Videogioco(Articolo):
    def __init__(self, nome, prezzo, piattaforma, commenti=None):
        super().__init__(nome, prezzo, commenti)
        self.piattaforma = piattaforma
        
    # Per esercitarsi: sovrascrivere __str__, __eq__, __hash__ per includere anche la piattaforma


cod = Videogioco("Call of Duty", 59.99, "PC")
avatar = Film("Avatar 2", 15.99, 120)
black_friday([cod, avatar])


# --------------------
# OPERATORI ARITMETICI
# --------------------

class Razionale:
    """
    Esempio di classe che definisce operatori aritmetici.
    https://docs.python.org/3/reference/datamodel.html#emulating-numeric-types
    """

    def __init__(self, numeratore, denominatore):
        if denominatore==0:
            raise RuntimeError("Il denominatore non può essere nullo")
        # forziamo il numeratore ad essere positivoo
        if denominatore<0:
          denominatore = -denominatore
          numeratore = - numeratore    
        self.numeratore = numeratore
        self.denominatore = denominatore

    def __add__(self, altro):
        """Metodo chiamato dall'espressione oggetto + altro."""
        d = self.denominatore * altro.denominatore
        n = self.numeratore * altro.denominatore + altro.numeratore * self.denominatore
        return Razionale(n, d)

    def __mul__(self, altro):
        """Metodo chiamato dall'espressione oggetto * altro."""
        n = self.numeratore * altro.numeratore
        d = self.denominatore * altro.denominatore
        return Razionale(n, d)
    
    def __eq__(self, altro):
        return self.numeratore*altro.denominatore == altro.numeratore*self.denominatore

    def __str__(self):
        return f"{self.numeratore}/{self.denominatore}"
    
     # Per esercitarsi: implementare __sub__ e __lt__

print("Definisco due numeri razionali e calcolo la loro somma e prodotto")
r1 = Razionale(2, 3)
r2 = Razionale(1, 5)
print("r1=", r1)
print("r2=", r2)
print(f"somma={r1+r2}, prodotto={r1 * r2}")
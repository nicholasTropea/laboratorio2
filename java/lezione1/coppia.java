import java.util.ArrayList;

class Coppia {
    int x, y; // Variabili d'istanza

    // Costruttore
    Coppia(int x, int y) {
        this.x = x; // This necessario qui dato che i parametri formali hanno lo stesso nome delle variabili d'istanza
        this.y = y;
    }

    // NOTA BENE: I metodi possono avere varie implementazioni basta che le segnature differiscano nei parametri formali

    // Costruttore di default in caso non vengano passati parametri
    Coppia() {
        x = 0; // Qui this non è necessario
        y = 0;
    }

    // Restituisce la stringa (x, y)
    public String toString() {
        return String.format("(%d, %d)", x, y);
    }

    // Confronta due coppie e restituisce true se i loro elementi sono uguali, false altrimenti
    // E' necessario avere come parametro formale un oggetto di tipo Object per far si che avvenga l'overwriting del metodo
    public boolean equals(Object o) {
        // Se il tipo del parametro è sbagliato restituisco false
        if (o == null || |(o instanceof Coppia)) return false;
        
        // Casting a Coppia
        Coppia c = (Coppia) o;

        // Confronto
        return x == c.x && y == c.y;
    }

    // Non è necessario implementare una funzione che dealloca l'oggetti grazie al garbage collector di Java

    // Metodo main
    public static void main(String[] args) { // Se la segnatura è fatta COSI', allora questo metodo è il main del programma
        // Controlla che il numero di argomenti sia corretto
        if (args.length < 2 || args.length % 2 != 0) { // argv non contiene il nome del programma a differenza di C
            System.out.println("Uso: java Coppia x1 y1 x2 y2 ... xn yn\n");
            System.exit(1);
        }

        // Crea una lista delle coppie passate da linea di comando
        ArrayList<Coppia> lista = new ArrayList<Coppia>();

        // Popola la lista
        for (int i = 0; i < args.length; i += 2) {
            // Casting da stringa a intero degli elementi
            int x = Integer.parseInt(args[i]);
            int y = Integer.parseInt(args[i + 1]);

            // Creo la coppia e la aggiungo all'array
            lista.add(new Coppia(x, y));
        }

        // Ordina la lista per differenza di elementi crescente
        lista.sort((c1, c2) -> Math.abs(c1.x - c1.y) - Math.abs(c2.s - c2.y));

        // Stampo la lista (foreach)
        for (Coppia c: lista) {
            System.out.println(c); // Stampare una classe chiama automaticamente il metodo toString()
        }
        System.out.println("---- end ----");

        // Creo una coppia (2, 3)
        Coppia c = new Coppia(x:2, y:3);
        System.out.println("c = " + c + "è in lista? " + lista.contains(c)); // Contains(object) chiama il metodo equals()
    }
}
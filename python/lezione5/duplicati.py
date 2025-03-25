#!/usr/bin/env python3
"""
Esempio di script per la navigazione del filesystem
Legge sulla riga di comando il nome di una directory
e restituisce il file più grande in essa contenuto
incluse tutte le sottodirectory

Funzioni di libreria per la gestione di file e directory

os.getcwd()       # restituisce directory corrente
os.chdir(path)    # cambia directory
os.listdir(path)  # elenca file (restituisce lista di stringhe)
os.access(path)   # verifica i permessi 
 
os.path.getsize(path) # dimensione file
os.path.exists(path)  # vero se il file/directory esiste  
os.path.isfile(path)  # vero se regular file
os.path.isdir(path)   # vero se directory
os.path.islink(path)  # vero se symbolic link
os.path.join(nome_dir,nome_file) # combina nome dir e file
os.path.abspath(path)  # restituisce path assoluto
os.path.realpath(path) # restituisce nome canonico eliminando link simbolici

Lista completa dei comandi su:
  https://docs.python.org/3/library/os.html
  https://docs.python.org/3/library/os.path.html

Per informazioni sui permessi dei file o come cambiarli
  man chmod

Per informazioni sui link simbolici e il loro uso:
   https://linuxize.com/post/how-to-create-symbolic-links-in-linux-using-the-ln-command/
"""

import os, os.path, sys, subprocess

def main(nomedir):
  # Lancia ricerca ricorsiva su nomedir
  if not os.path.exists(nomedir):
    print(f"Error: {nomedir} doesn't exist.")
    sys.exit(1)

  if not os.path.isdir(nomedir):
    print(f"Error: {nomedir} is not a directory.")
    sys.exit(1)

  # Invoca la funzione ricorsiva per creare l'elenco  
  nomeabs = os.path.abspath(nomedir)
  elenco = elenco_file(nomedir,set())

  print(elenco)
  
  ################# Nuova logica, trasformo l'elenco dei file in un dizionario per poi fare i confronti

  lunghezze = {}

  for f in elenco:
    # Controllo se esiste già la lunghezza f[1] nel dizionario
    if f[1] in lunghezze:
      lunghezze[f[1]].append(f[0])
    else:
      lunghezze[f[1]] = [f[0]]

  print(lunghezze)

  for lun in lunghezze:
    if len(lunghezze[lun]) > 1:
      visualizza_duplicati(lunghezze[lun])
  
  return
    

def visualizza_duplicati(lista_file):
  """
  Data una lista di nomi di file, restituisce quelli che hanno lo stesso contenuto
  """

  for i in range(len(lista_file)):
    for j in range(i + 1, len(lista_file)):
      # Uso il confronto della shell invece che implementarlo (cmp: confronto tra file byte x byte)
      # (Exit code: 0 -> same, 1 -> different, 2 -> Error)
      # $? contiene l'exit code dell'ultimo comando eseguito 

      file1 = lista_file[i]
      file2 = lista_file[j]

      if cmpshell(file1, file2) == 0:
        print("\n" + 120 * "-" + "\n\nWARNING!")
        print(f"\n{file1}\n\nAND\n\n{file2}\n\nARE THE SAME.")
        print("\n" + 120 * "-")
      
  return
        

def cmpshell(a, b):
  """
  Confronta i file a e b usando cmp della shell
  """
  #                stdout e stderr di cmp vengono messe dentro ris come informazioni   
  ris = subprocess.run(["cmp", a, b], stdout = subprocess.PIPE, stderr = subprocess.PIPE)

  return ris.returncode


# Funzione ricorsiva per cercare il file più grande
# nella directory corrente e in tutte le sue sottodirectory
def elenco_file(nome,dir_esplorate):
  """
    Restituisce una coppia (nome,file) per ogni 
    file dentro la directory nome e le sue
    sottodirectory.
  """

  assert os.path.isdir(nome), "Argomento deve essere una directory"

  print(f"Begin: {nome}", file = sys.stderr)

  # Inizializzo la lista di file trovati
  lista = []

  # Ottiene il contenuto della directory 
  listafile = os.listdir(nome)

  for f in listafile:
    nomecompleto = os.path.join(nome, f)

    # Verifica se il file è accessibile
    if not os.access(nomecompleto, os.F_OK):
      print("Error: Broken link -> ", nomecompleto, file = sys.stderr)
      continue

    # Distinguo tra file normali e directory
    if not os.path.isdir(nomecompleto):
      nuovadim = os.path.getsize(nomecompleto)
      nuovonome = nomecompleto

      # Aggiungo alla lista risultato il file appena incontrato
      # Il file è rappresentato dalla tupla (nome,dimensione)
      lista.append((nuovonome,nuovadim))
    else:
      # nomecompleto è una directory: possibile chiamata ricorsiva

      # Verifico che la directory sia esplorabile 
      if not os.access(nomecompleto, os.R_OK | os.X_OK):
        print(f"Error: Directory {nomecompleto} not accessable.", file = sys.stderr)

        continue

      # Verifica che la directory non sia già stata esplorata
      # va fatto con il realpath perchè la stessa dir può avere più nomi  
      nomereal = os.path.realpath(nomecompleto)

      if nomereal in dir_esplorate:
        print(f"Error: Directory {nomecompleto} already explored ", file = sys.stderr)
        print(f"with path {nomereal}", file = sys.stderr)
        
        continue

      dir_esplorate.add(nomereal)

      # Directory nuova e accessibile: esegui ricorsione
      lista_dir = elenco_file(nomecompleto, dir_esplorate)
      lista += lista_dir

  # Fine ciclo for su i file di questa directory     
  print(f"End: {nome}", file = sys.stderr)

  return lista
  
  
# Verifico argomenti sulla linea di comando e faccio partire la ricerca
if len(sys.argv) == 2:
    main(sys.argv[1])
else:
    print(f"Error: Incorrect usage.\nUsage: {sys.argv[0]} [dirName]")
#!/usr/bin/env python3

"""
esegue una serie di comandi in ogni sottodirectory 
della directory passata sulla linea di comando
L'output viene salvato su report.txt in ogni directory


os.getcwd()       # restituisce directory corrente
os.chdir(path)    # cambia directory
os.listdir(path)  # elenca file (restituisce lista di stringhe)
os.path.getsize(path) # dimensione file
os.path.exists(path)  # vero se il file/directory esiste  
os.path.isfile(path)  # vero se regular file
os.path.isdir(path)   # vero se directory
os.path.islink(path)  # vero se symbolic link

"""
import os, os.path, glob, signal, argparse
import sys, subprocess, time

Description = """
Esegue il test del primo esonero 24/25"""


# position of the makefile with respect to the source files
Makefile = "../makefile"
Report = "report.txt"
Valgrind = "valgrind --leak-check=full --show-leak-kinds=all"
Timewait = 10              # attesa tra esecuzione e successiva
Timelimit = 10             # attesa che finisca un comando C
DelayedTimelimit = 60


# cancella il file da /dev/shm
SHMDEL = "find /dev/shm -type f -not -name ecryptfs-giovanni-Private -delete"

# espande un insieme di nomi di file
def espandi(pattern):
  esp = glob.glob(pattern)
  return " ".join(esp)


# file dove viene salvato l'output
TMPout = "tmp.out"

def execute_command(command,logfile):
    # print(command,file=logfile)
    try:
      ris = subprocess.run(command.split(),encoding='UTF-8',
                           capture_output = True,timeout=Timelimit)
      #print(f"== Return code: {ris.returncode}", file=logfile);
      #if len(ris.stdout)>0:
      #  print("== stdout:  ", ris.stdout ,file=logfile);
      # if len(ris.stderr)>0:
      #  print("== stderr:  ", ris.stderr ,file=logfile);
      with open(TMPout,"w") as g:
        print(ris.stdout,file=g)
    except subprocess.TimeoutExpired as ex:
      # caso time out
      print("## Test failed: no result after %d seconds" % Timelimit, file=logfile)
      print("## stdout:  ", ex.stdout.decode() if ex.stdout else None,file=logfile);
      print("## stderr:  ", ex.stderr.decode() if ex.stderr else None,file=logfile);
      return False
    except Exception as ex:
      print("## Test failed:", str(ex), file=logfile)
      return False
    return ris.returncode==0


# esegue i test, da chiamare da dentro la directory
# output su outfile
def test_dir(outfile):
  print("--- Eseguo: make pitagora", file=outfile)
  if execute_command(f"make pitagora",outfile):
    print("--- compilazione ok ---")
    for comando in ["small 0 12 14 16", "t5 11 16 3", "t20 7 15 21 29 37 45 53"]:
      nome = comando.split()[0]
      print(f"--- Eseguo: pitagora {comando}")
      if execute_command(f"pitagora {comando}",outfile):
        if execute_command(f"diff -bB {TMPout} {nome}.out",outfile):
          print("stdout OK")
        else:
          print(f"stdout non corretto, confronta il tuo output con {nome}.out")
          return 
        if execute_command(f"diff -bB {nome}.pit {nome}.ris",outfile):
          print(f"File {nome}.pit OK")
        else:
          print(f"File {nome}.pit non corretto, confrontalo con {nome}.ris")
          return
      else:
        print("Errore esecuzione pitagora")
        return
  else:
    print("Errore in compilazione")


if __name__ == '__main__':
  test_dir(sys.stdout)
      

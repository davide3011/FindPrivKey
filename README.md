# **FindPrivKey**

FindPrivKey è un programma che utilizza la libreria [secp256k1](https://github.com/bitcoin-core/secp256k1) per eseguire una ricerca tramite forza bruta di una chiave privata associata a una o più chiavi pubbliche specificate in un file.

Il progetto sfrutta il multithreading per massimizzare l'efficienza e include un report periodico sullo stato della ricerca.

---

## **Caratteristiche**
- Generazione di chiavi private in stile 2009 (pseudo-casuale con seed basato sull'orario).
- Creazione delle chiavi pubbliche corrispondenti utilizzando la libreria `secp256k1`.
- Ricerca tramite forza bruta parallela su più thread.
- Report periodici ogni 10 secondi con:
  - Numero totale di tentativi.
  - Tempo trascorso.
  - Velocità dei tentativi (tentativi/s).

---

## **Prerequisiti**
Assicurati di avere installati i seguenti strumenti e librerie sul tuo sistema:
- **Compilatore GCC** per C++.
- Librerie di supporto:
  - `make`, `autoconf`, `automake`, `libtool`, `pkg-config`.

Su un sistema Debian/Ubuntu, puoi installarli con:
```bash
sudo apt-get update
sudo apt-get install build-essential autoconf automake libtool pkg-config

## **Installazione**
Clona il repository: Clona il progetto e assicurati di includere il submodule secp256k1:

```bash
git clone --recurse-submodules git@github.com:IlTuoNomeUtente/FindPrivKey.git
cd FindPrivKey

Configura e compila la libreria secp256k1: Esegui i seguenti comandi per configurare e compilare la libreria:

```bash
cd secp256k1
./autogen.sh
./configure --enable-module-recovery
make
cd ..

Compila il programma: Usa il seguente comando per compilare main.cpp:

```bash
g++ -o FindPrivKey main.cpp -I./secp256k1/include -L./secp256k1/.libs -lsecp256k1 -pthread
Prepara il file goal.txt: Crea un file goal.txt contenente le chiavi pubbliche in formato esadecimale non compresso (130 caratteri per riga, ognuna rappresenta una chiave pubblica).

Esempio di file goal.txt:

```bash
04c9e66f0f4b6f5a78d21e8b8e8e35c3216c9f2d76b3c8a5ab91c812f5e3d9e3a07dd5d35bf4efea82b923c5a3f1cf3e69e23bfc65fb98747c0c0e927b4dc0e6f8
04d61a74cbf0c9b2d23e6ff92e8e1e4b76c2d7c3d8b2f1a8d8a21b1e8d9a3f2c7f0b5d3f9e6d4c5a82e8b5c7e9f4c3b7d1e6f5b3a7d9e8f2d1e8b9a7c6f5d4c3b2
Esecuzione
Esegui il programma con il comando:

bash
Copia
Modifica
./FindPrivKey
Il programma:

Leggerà le chiavi pubbliche da goal.txt.
Genererà chiavi private in modo pseudo-casuale e creerà le corrispondenti chiavi pubbliche.
Confronterà ogni chiave pubblica generata con quelle presenti in goal.txt.
Se una corrispondenza viene trovata, il programma mostrerà:

La chiave privata corrispondente (in formato esadecimale).
La chiave pubblica trovata.
Funzionamento Interno
Generazione delle chiavi private:

Usa un PRNG (std::rand()) basato sull'orario corrente.
Ogni chiave privata è un array di 32 byte pseudo-casuali.
Creazione delle chiavi pubbliche:

La libreria secp256k1 viene utilizzata per generare le chiavi pubbliche a partire dalle chiavi private.
Parallelizzazione:

Il lavoro è suddiviso su più thread, uno per ogni core disponibile.
Un report periodico è generato ogni 10 secondi con dettagli sullo stato della ricerca.
Output del Programma
Durante l'esecuzione: Il programma mostrerà lo stato della ricerca ogni 10 secondi:
yaml
Copia
Modifica
[STATUS] Tentativi totali: 1000000, Tempo trascorso: 10 secondi, Rate: 100000 tentativi/s, Thread attivi: 8
Se una chiave viene trovata:
java
Copia
Modifica
Chiave privata trovata (hex): f1c2d3e4f5a6b7c8d9e0f1c2d3e4f5a6b7c8d9e0f1c2d3e4f5a6b7c8d9e0f1c2
Chiave pubblica corrispondente trovata!
Se non viene trovata alcuna chiave:
Copia
Modifica
Nessuna chiave privata trovata nel numero massimo di tentativi specificato.
Limitazioni
Questo programma è puramente dimostrativo e non è progettato per applicazioni reali di forza bruta su chiavi private.
L'algoritmo di generazione delle chiavi private non è crittograficamente sicuro.
Le prestazioni dipendono dalla potenza di calcolo della macchina.
Contributi
Se vuoi contribuire al progetto, sentiti libero di inviare una pull request o aprire una segnalazione.

Licenza
Questo progetto è distribuito sotto la licenza MIT. Consulta il file LICENSE per maggiori dettagli.

go
Copia
Modifica

Copia e incolla questo contenuto nel tuo `README.md` sul repository GitHub e aggiorna il file! 😊

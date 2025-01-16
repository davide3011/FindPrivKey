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

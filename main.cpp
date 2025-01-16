#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <secp256k1.h>
#include <random>
#include <chrono>
#include <unordered_set>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>
#include <mutex>

// Funzione per generare una chiave privata "2009-style"
std::vector<uint8_t> generate_2009_style_private_key() {
    std::vector<uint8_t> private_key(32);

    // Inizializza il PRNG con un seed basato sull'orario corrente
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Genera 32 byte pseudo-casuali
    for (auto &byte : private_key) {
        byte = std::rand() % 256; // Genera un valore tra 0 e 255
    }

    return private_key;
}

// Funzione per ottenere la chiave pubblica
std::vector<uint8_t> get_public_key(const std::vector<uint8_t>& private_key, secp256k1_context* ctx) {
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, private_key.data())) {
        throw std::runtime_error("Errore nella generazione della chiave pubblica");
    }

    std::vector<uint8_t> public_key(65);
    size_t len = public_key.size();
    secp256k1_ec_pubkey_serialize(ctx, public_key.data(), &len, &pubkey, SECP256K1_EC_UNCOMPRESSED);

    return public_key;
}

// Funzione per la ricerca tramite forza bruta (per thread)
void brute_force_thread(const std::unordered_set<std::string>& public_key_set, int64_t attempts_per_thread, std::atomic<bool>& found_flag, std::atomic<int64_t>& total_attempts, std::mutex& print_mutex) {
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

    int64_t attempts = 0;
    while (attempts < attempts_per_thread && !found_flag.load()) {
        attempts++;
        total_attempts++;

        auto private_key = generate_2009_style_private_key();
        try {
            auto generated_public_key = get_public_key(private_key, ctx);
            std::string generated_pubkey_str(generated_public_key.begin(), generated_public_key.end());

            if (public_key_set.find(generated_pubkey_str) != public_key_set.end()) {
                std::lock_guard<std::mutex> lock(print_mutex);
                std::cout << "Chiave privata trovata (hex): ";
                for (const auto& byte : private_key) {
                    std::cout << std::hex << std::setw(2) << std::setfill('0') << int(byte);
                }
                std::cout << "\nChiave pubblica corrispondente trovata!" << std::endl;

                found_flag.store(true); // Segnala che la chiave è stata trovata
                break;
            }
        } catch (const std::exception& e) {
            // Ignora errori nella generazione delle chiavi
        }
    }

    secp256k1_context_destroy(ctx);
}

void periodic_status_report(std::atomic<int64_t>& total_attempts, std::atomic<bool>& found_flag, unsigned int num_threads) {
    auto start_time = std::chrono::steady_clock::now();

    while (!found_flag.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        auto now = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();

        int64_t attempts = total_attempts.load();
        double rate = (elapsed_time > 0) ? (static_cast<double>(attempts) / elapsed_time) : 0;

        std::cout << "[STATUS] Tentativi totali: " << attempts 
                  << ", Tempo trascorso: " << elapsed_time << " secondi"
                  << ", Rate: " << rate << " tentativi/s"
                  << ", Thread attivi: " << num_threads << std::endl;
    }
}

int main() {
    std::ifstream file("goal.txt");
    if (!file.is_open()) {
        std::cerr << "Errore: Il file goal.txt non esiste." << std::endl;
        return 1;
    }

    std::unordered_set<std::string> public_key_set;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.size() == 130) { // 65 bytes in formato hex
            std::vector<uint8_t> pubkey(65);
            for (size_t i = 0; i < 65; ++i) {
                pubkey[i] = std::stoi(line.substr(2 * i, 2), nullptr, 16);
            }
            public_key_set.insert(std::string(pubkey.begin(), pubkey.end()));
        }
    }

    std::cout << "Trovate " << public_key_set.size() << " chiavi pubbliche valide nel file." << std::endl;

    // Configura i parametri per la parallelizzazione
    int64_t max_attempts = 1000000000000;
    unsigned int num_threads = std::thread::hardware_concurrency();
    int64_t attempts_per_thread = max_attempts / num_threads;
    std::atomic<bool> found_flag(false);
    std::atomic<int64_t> total_attempts(0);
    std::mutex print_mutex;

    std::cout << "\nInizio ricerca della chiave privata tramite forza bruta casuale su " << num_threads << " thread..." << std::endl;

    // Avvia il thread per il report periodico dello stato
    std::thread status_thread(periodic_status_report, std::ref(total_attempts), std::ref(found_flag), num_threads);

    // Crea e avvia i thread
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < num_threads; ++i) {
        threads.emplace_back(brute_force_thread, std::ref(public_key_set), attempts_per_thread, std::ref(found_flag), std::ref(total_attempts), std::ref(print_mutex));
    }

    // Attendi il completamento di tutti i thread
    for (auto& t : threads) {
        t.join();
    }

    // Termina il thread di status
    found_flag.store(true);
    status_thread.join();

    if (!found_flag.load()) {
        std::cout << "\nNessuna chiave privata trovata nel numero massimo di tentativi specificato." << std::endl;
    }

    return 0;
}



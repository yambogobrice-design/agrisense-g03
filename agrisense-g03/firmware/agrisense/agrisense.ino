#include <Arduino.h>              // Charge les fonctions de base d'Arduino.

const int BOUTON = 2;             // Donne le nom BOUTON à la broche D2, où le bouton est branché.
uint32_t nbAppuis = 0;            // Crée un compteur d'appuis qui commence à 0.
bool etatPrecedent = HIGH;        // Mémorise l'état du bouton au tour d'avant.

void setup() {                    // Fonction exécutée une seule fois, au démarrage de la carte.
  Serial.begin(115200);           // Démarre la communication avec l'ordinateur à 115200 bauds.
  while (!Serial && millis() < 3000) {}  // Attend que le moniteur série soit ouvert, au maximum 3 secondes.
  pinMode(BOUTON, INPUT_PULLUP);  // Met D2 en entrée avec une résistance interne qui la tire à 3,3 V (HIGH).
  Serial.println("=== AgriSense TP1 ===");  // Affiche un message de départ dans le moniteur série.
}

void loop() {                     // Fonction qui se répète en boucle, à l'infini.
  bool etat = digitalRead(BOUTON);  // Lit l'état actuel du bouton : HIGH (relâché) ou LOW (appuyé).
  if (etatPrecedent == HIGH && etat == LOW) {  // Si le bouton était relâché et qu'il est maintenant appuyé, c'est un nouvel appui.
    nbAppuis++;                   // Ajoute 1 au compteur d'appuis.
    Serial.print("appuis : ");    // Affiche le texte « appuis : » sans retour à la ligne.
    Serial.println(nbAppuis);     // Affiche le nombre d'appuis puis passe à la ligne.
  }                               // Fin du test.
  etatPrecedent = etat;           // Garde l'état actuel pour le comparer au prochain tour.
  delay(200);                     // Attend 200 ms sans rien faire, pour simuler un programme occupé.
}


#include <Arduino.h>              // Charge les fonctions de base d'Arduino.

const int BOUTON = 2;             // Le bouton est branché sur la broche D2.
volatile uint32_t nbAppuis = 0;   // Compteur d'appuis ; "volatile" car il est modifié par l'interruption.

void surAppui() { nbAppuis++; }   // Fonction appelée automatiquement à chaque appui : elle ajoute 1 au compteur (elle doit être très courte).

void setup() {                    // Exécuté une seule fois au démarrage.
  Serial.begin(115200);           // Démarre la communication avec l'ordinateur.
  while (!Serial && millis() < 3000) {}  // Attend le moniteur série, 3 secondes au maximum.
  pinMode(BOUTON, INPUT_PULLUP);  // D2 en entrée avec la résistance interne vers 3,3 V.
  attachInterrupt(digitalPinToInterrupt(BOUTON), surAppui, FALLING);  // Demande d'appeler surAppui() dès que D2 passe de HIGH à LOW .
}

void loop() {                     // Se répète à l'infini.
  static uint32_t affiche = 0;    // Mémorise la dernière valeur affichée .
  if (nbAppuis != affiche) {      // Si le compteur a changé depuis le dernier affichage...
    affiche = nbAppuis;           // ...on retient la nouvelle valeur...
    Serial.print("appuis : ");    // ...on affiche le texte...
    Serial.println(affiche);      // ...puis le nombre d'appuis.
  }
  delay(200);                     // Attend 200 ms (le programme est "occupé", mais l'interruption marche quand même).
}
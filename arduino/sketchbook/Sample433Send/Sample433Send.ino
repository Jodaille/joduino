#include <VirtualWire.h> // inclusion de la librairie VirtualWire
 
const char *msg = "c bo la vie"; // Tableau qui contient notre message
 
void setup() // Fonction setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Tuto VirtualWire"); // Petit message de bienvenue
 
    vw_setup(2000);     // initialisation de la librairie VirtualWire à 2000 bauds (note: je n'utilise pas la broche PTT)
}
 
void loop() // Fonction loop()
{
    Serial.print("TX ... "); // On signale le début de l'envoi
    vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message 
    vw_wait_tx(); // On attend la fin de l'envoi
    Serial.println("Done !"); // On signal la fin de l'envoi
    delay(1000); // Et on attend 1s pour pas flooder
}

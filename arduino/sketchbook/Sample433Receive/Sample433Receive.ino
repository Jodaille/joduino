#include <VirtualWire.h> // inclusion de la librairie VirtualWire
 
void setup() // Fonction setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Tuto VirtualWire"); // Petit message de bienvenue
 
    vw_setup(2000); // initialisation de la librairie VirtualWire à 2000 bauds (note: je n'utilise pas la broche PTT)
    vw_rx_start();  // Activation de la partie réception de la librairie VirtualWire
}
 
void loop() // Fonction loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
    uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau
 

 
    // REMARQUE ULTRA IMPORTANTE :
    //   Par le passé ces deux lignes se trouvaient en "haut de programme", c'était donc des variables globales.
    //   Elles ne doivent PAS être globales, c'est très important !
    //
    //    Pourquoi ?
    //    C'est très simple. Posez vous la question suivante : que vaut "buflen" si la réception d'un message échoue, 
    //    où qu'on reçoit un message de moins de VW_MAX_MESSAGE_LEN octets ?
    //    Au début "buflen" vaut VW_MAX_MESSAGE_LEN, mais après "buflen" vaudra une valeur < VW_MAX_MESSAGE_LEN.
    //    Et paf, la taille de votre buffer vient de diminuer pour la réception suivante, et ainsi de suite.
 
    if (vw_wait_rx_max(200)) // Si un message est reçu dans les 200ms qui viennent
    {
        if (vw_get_message(buf, &buflen)) // On copie le message, qu'il soit corrompu ou non
        {
            Serial.print("RX : ");
            for (byte i = 0; i < buflen; i++) // Si il n'est pas corrompu on l'affiche via Serial
                Serial.print((char)buf[i]);
            Serial.println("");
        }
    }
}

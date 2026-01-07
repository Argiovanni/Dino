# Aquisition des donnés sur le bus can

## Rapport avancement  (19/12)

### 1. Contexte 

Dans le cadre du projet D.I.N.O., une des composantes principales consiste à développer une ** interface d'acquisition pour un bus CAN** éxécuté sur une **Raspberry Pi connectée au véhicule**.
Ce serveur fournira une interface pour permetre au serveur web de récuperer les données de conduite.

### 2. état actuel de l'implémentation

Nous avons déjà mis en place une architecture physique pour pouvoir aquérir les data depuis le bus can vers la raspi. Actuellement, le module mcp2515 semble fonctionner, mais l'interface spi qui permet a la raspi de lire les donnés ne se synchronise pas.

### 3. Objectifs suivants:

Dans les prochains jours, nous avons définis plusieurs objectifs:
- Arriver a lire cette fameuse liaison spi
- Definir une interface en collaboration avec l'équipe web.

### 4. Conclusion
Si notre projet est actuellement bloqué par les difficultés à la lecture du bus CAN, nous avons toutde même des perspectives d'évolution pour pouvoir assez rapidement mettre en place une première phase du poc.

### Rapport d'avancement 07/01

La communication CAN entre Arduino et Raspberry pi marche enfin, le problème était lié au Mac qui ne délivrait pas 5V à l'arduino.

Le but ici est de simuler la communication dans une voiture de manière assez simplifié en passant par un controller CAN (MCP2515). Nous utilisons la librairie mcp2515 fourni par autowp (https://github.com/autowp/arduino-mcp2515).

La connection entre l’arduino et le MCP2515 est faite en spi et le MCP2515 permet d’envoyer les données sur le bus. 

L'arduino envoie des données comme la vitesse, le régime moteur ou encore la température de l'huile. La vitesse est représentée par un potentiomètre sur le breadboard. C'est aussi le cas du régime moteur. Le frein est représenté par le bouton poussoir.

## Câblage

![Connection Arduino-MCP2515](Dino/images/cablage_can.png "Connection en SPI")


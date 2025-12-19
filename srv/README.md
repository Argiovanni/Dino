# Code for web server running on raspi 3B

* [doc python-can](https://python-can.readthedocs.io/en/stable/index.html)


## Rapport d’avancement (19/12)

### 1. Contexte

Dans le cadre du projet D.I.N.O., une des composantes principales consiste à développer un **serveur web embarqué** exécuté sur une **Raspberry Pi connectée au véhicule**.
Ce serveur a pour objectif de fournir une interface utilisateur accessible depuis un écran embarqué ou un terminal externe (smartphone, tablette) connecté en Wi-Fi à la carte.

---

### 2. État actuel de l’implémentation

À ce stade du projet, les fonctionnalités suivantes ont été mises en place :

#### 2.1 Serveur web

* Le serveur web est développé en **Python** à l’aide du framework **Flask**.
* L’architecture du projet est modulaire (routes, templates, modèles, formulaires), facilitant l’évolutivité et la maintenance.
* Le serveur est conçu pour être exécuté directement sur la **Raspberry Pi 3B** embarquée dans le véhicule, mais n'a pour l'instant été testé que sur nos ordinateurs.

#### 2.2 Gestion des utilisateurs et authentification

* Une **page de connexion fonctionnelle** a été implémentée.
* Le système d’authentification repose sur **Flask-Login**.
* Les utilisateurs sont stockés dans une **base de données SQLite3**, adaptée à un système embarqué léger.
* La sélection d’un utilisateur se fait via une interface visuel et simple.
* Une fois authentifié, l’utilisateur est maintenu en session.

#### 2.3 Accès au tableau de bord

* Une **page de dashboard** est accessible uniquement après authentification.
* Cette page constitue le point d’entrée principal de l’interface utilisateur.
* Elle est destinée à afficher les informations liées à la conduite et à l’état du véhicule.

---

### 3. Travaux restant à réaliser

Plusieurs éléments restent à implémenter pour compléter cette partie du projet :

* **Conception graphique du tableau de bord**
  Le design du dashboard doit être défini afin d’afficher les informations de manière claire, ergonomique et adaptée à une utilisation embarquée (écran ou mobile).

* **Récupération des données du bus CAN**
  Les données du véhicule (vitesse, régime moteur, consommation, etc.) devront être récupérées via le **bus CAN**, en lien avec une autre partie du projet dédiée à l’acquisition des données OBD2.

* **Stockage et exploitation des données**
  Il est prévu de :

  * sauvegarder les données de conduite en base de données,
  * permettre l’affichage de **courbes d’évolution** (ex. vitesse, régime moteur, consommation),
  * offrir à l’utilisateur une visualisation de son comportement de conduite dans le temps.

---

### 4. Conclusion

La partie serveur web embarqué est aujourd’hui **fonctionnelle dans ses fondations** : serveur Flask opérationnel, authentification utilisateur en place et accès sécurisé à un dashboard.
Les prochaines étapes consisteront à enrichir cette base avec l’acquisition des données véhicule, leur visualisation en temps réel et leur exploitation sur le long terme.

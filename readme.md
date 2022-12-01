# Protocole applicatif TCP/IP

## A propos du projet

Le projet repose sur l'ajout d'un protocole applicatif ce rajoutant au dessus du protocole TCP/IP.
Cette couche se trouve au dessus de celle-ci et ne recréer par ce protocole mais un simplement une couche permettant de vérifier la validité de n'importe quel message.

Cela permettant de rajouter une sécurité sur le protocole TCP/IP. Et permettant de vérifier la longueur d'un message, le type de celui-ci et l'intégrité des messages.

Pour plus d'informations voir : **overlay network**

## Support du programme

L'application côté serveur peut supporter n'importe quel type de client, il n'est pas _obligatoire_ de passer par le client fourni.


1. Telnet adressIP port
2. ./client adressIP
3. Autres

## Créée avec 

L'entiereté du projet à été réaliser **sans** passer par de framework ni de librairie non inclus de base. Toutes les références vers l'api donnée des sockets et autres sont données dans le pdf.

* C

## Installation

L'installation est obligatoire pour pouvoir avoir accès à toutes les fonctionnalités de l'application.

**Cependant**, si vous décidez de ne pas executer le fichier ./Demo cela pourrait provoquer le disfonctionnement de l'application Server et Client qui n'arriveront pas à communiquer.

1. Création du rappport
    1. Veuillez bien à vérifier que si un **warning** ce produit, entrez la lettre q
    ```
        cd rapport
        ./RapportMake
    ```

**AVANT TOUTES CHOSES, executer le fichier ** _Demo_ dans le **dossier protocole**

2. Création et lancement du fichier serveur

```
    -> vous êtes dans le dossier protocole
    cd server
    ./DemoServeur
```

3. Création du fichier client et lancement
```
    -> vous êtes dans le dossier protocole
    cd client
    make
    ./client ipAdress
```



### Automatisation du client

Lancer le fichier client pour permettre l'envoie automatique de message sans devoir faire tout soit même.

```
    -> vous êtes dans le dossier client
    ./DemoClient ipAdress
```
###Erreur script Demo

Si une erreur arrive : executer cette commande dans la console qui enlevera les caractères supperflue (CRLF) :

1. sed -i 's/\r//' Demo


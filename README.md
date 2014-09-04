===================================
Ctoollhu - Some tools by 0x0ff.info
===================================

0x0ff.info
----------------------------
**Ce dossier contients les exemples présentés dans les différents articles du blog 0x0ff.info :**
  - http://www.0x0ff.info/2014/prng-et-generateur-de-cle/ => Examples/PRNG-et-generateur-de-cle/
  - http://www.0x0ff.info/2014/heap-based-buffer-overflow/ => Examples/PRNG-et-generateur-de-cle/

SNMPunK
----------------------------
**Cet outil permet trois choses :**
  - Générer un liste de clés en fonction de l'engineID renseignée et de la fonction de hashage choisie,
  - Brute-Forcer l'authentification avec la liste précédemment générée,
  - Brute-Forcer le chiffrement avec la liste précédemment générée.
  
**Visitez les deux liens ci-dessous pour comprendre comment utiliser cet outils :**
  - http://www.0x0ff.info/2013/snmpv3-authentification/
  - http://www.0x0ff.info/2013/snmpv3-des-cryptool/


cmatrix-1.2a-0x0ff.info-edit
----------------------------
  cmatrix est le célèbre programme de Chris Allegretta.
  Ajout d'une nouvelle fonctionnalité : la gestion d’un fichier de log. Lorsque le fichier est modifié, les brins deviennent rouge...
**Visitez le lien ci-dessous pour plus d'infos :**
  - http://www.0x0ff.info/2013/0x-cmatrix/

lbf5_backender.pl
-----------------
  Petit programme permettant d'identifier les backends derrière un Load Balancer F5.
  La découverte de l'adresse IP/Port des backends se fait grâce à un cookie spécifique.
**Visitez le lien ci-dessous pour plus d'infos :**
  - http://www.0x0ff.info/2013/load-balancer-f5-tiny-disclosure/

lsb_reverse.pl
-----------------
  Petit programme permettant d'extraire un message planqué dans le Least Significant Bit d'une image au format .bmp.
  /!\ A modifier selon l'ordre des bits cachés : Les bits extraits successivement peuvent être interpretés de plusieurs façons (exemple : 1000 0000 ou 0000 0001)...
  
nyan-cat
-----------------
  La commande cat mais version nyan... :)
  
ActiveStandbyChecker
-----------------
  Pour utiliser ce script il faut que l’IP de la machine sur laquelle vous le lancez soit autorisée à faire des requêtes SNMP vers les équipements. Vous n’avez normalement à toucher au script que pour changer la communauté.
  La configuration se fait dans un fichier « ips.txt ». La syntaxe de se fichier est, une ligne par équipement, chaque ligne doit être formatée comme ceci : [constructeur]:[ip]
  - Les IP des F5 à utiliser sont celles des vCMP
  - Les IP des Radware à utiliser sont celles des châssis
  - Les IP des Juniper (SRX) sont celles des membres du cluster
  - Les IP des Fortinet à utiliser sont celles des membres du cluster 


Utilisation de l'outil
======================
Introduction
-------------
Avant d'utiliser les differents scripts decrits ci-dessous, il faut prealablement taper dans un terminal la commande ./runSession dans le repertoire Salome où se situe le runSession. On peut trouver les differents scripts au niveau du repertoire : ./Salome-n°version/modules/SMESH_n°version/share/salome/plugins/smesh/Verima

Creation de la base
--------------------

La base de donnees ne doit, a priori, n'etre creee qu'une unique fois. Cependant, relancer le script de creation ne changera pas les informations contenues dans la base et n'a pas d'incidence sur la base ni sur son contenu.

  * createDatabase.py
        - Cree la structure de la base et le fichier myMeshDB.db
        - Si le fichier myMeshDB.db (c'est a dire si la base) existe deja, le script ne modifie pas la structure des tables et n'affecte pas les enregistrements qu'elles contiennent deja.
        - Le script a un unique parametre optionnel : le nom de la database. 

     .. code-block:: python

          python createDatabase.py -d maBaseAMoi.db

Initialisation d'un jeu de tests par defaut 
-------------------------------------------
Si on le souhaite, on peut initialiser un jeu de tests par defaut. Ce jeu de tests s'applique aux versions, aux mailleurs testes, aux scripts de maillage et enfin aux groupes de reference associes a ces maillages.
Les quatres fichiers python se situent dans le repertoire Base.

  * Pour les versions, il s'agit du fichier tableVersions. Dans la methode "remplit", on renseigne :
        - Le nom de la version
	- Un commentaire

  * Pour les mailleurs, il s'agit du fichier tableMailleurs. Dans la methode remplit, on renseigne :
        - Le nom du mailleur

  * Pour les scripts, il s'agit du fichier tableMaillages. Dans la methode remplit, on renseigne :
        - Le nom du cas test
        - Le nom du script python
	- Le nom du fichier med
        - Le numero de l'identifiant correspondant au mailleur teste
        - La dimension maximale du maillage
        - Les quatres seuils correspondant aux quatres criteres a savoir le temps CPU, l'aspect ratio, la longueur et enfin le nombre d'entites
	- Un commentaire

  * Pour les groupes de reference, il s'agit du fichier tableGroupesRef. Dans la methode remplit, on renseigne :
        - Le nom du groupe
	- Le numero d'identifiant correspondant au maillage

Remarque : actuellement, le jeu par defaut porte : 
  * sur les versions
	- n°id=1,'Salome7.2.0'
	- n°id=2,'Salome7.3.0'
	- n°id=3,'Salome7.4.0'
  * sur les mailleurs
	- n°id=1,'BLSURF'
	- n°id=2,'NETGEN1D2D'
	- n°id=3,'GHS3D+BLSURF'
	- n°id=4,'GHS3D+NETGEN1D2D'
	- n°id=5,'NETGEN1D2D3D'
  
Ajouter un enregistrement a une table 
-------------------------------------

Lors d'une nouvelle version de Salome, de l'arrivee d'un nouveau cas test ou d'un nouveau mailleur, il sera necessaire d'enrichir la base de donnees. Aucun contrle sur la coherence des valeurs n'est effectue. 

  * l'autoincrement

   Les identifiants ("id") des tables Maillages, Mailleurs et Versions sont crees de façon automatique lors de l'insertion d'un enregistrement dans la table. Il n'est ni possible ni necessaire de les preciser lors de la creation d'un enregistrement.
   Ainsi, pour ajouter un mailleur, il suffit de specifier son nom.

 
  * ajoutEnreg.py

        - Le script a deux parametres : le nom de la database (optionnel) et le nom de la table qu'il faut enrichir.
          les valeurs des colonnes doivent etre fournies dans l'ordre.

         .. code-block:: python

            python ajoutEnreg -d maBaseAMoi.db -t TableMailleurs  "monMailleur"


        - Les contrles effectues sont minimaux : nombre de valeurs de l'enregistrement et identifiant. En revanche, la coherence entre tables n'est pas verifiee. Ainsi, par exemple, pour entrer un nouveau cas test, il faut ajouter un enregistrement a la table des maillages. Si l'identifiant du mailleur n'existe pas, aucune erreur ne sera detectee.

         .. code-block:: python

          python ajoutEnreg -d maBaseAMoi.db -t TableMailleurs  "monMailleur"
          python ajoutEnreg -d maBaseAMoi.db -t TableMaillages  "monMaillage" "mesScripts/lanceMonMaillage" "/tmp/monFichierMed" 4 3 5 5 5 5 "essai pour mon Mailleur"

       
Changement de la version de reference
-------------------------------------
 
A priori, cette fonction ne devrait pas etre utilisee. mais ... Elle permet de changer la version de reference.

  * changeVersion.py

         .. code-block:: python

            python changeVersion.py Salome7.3 ou
            python changeVersion.py 3

Consultation des tables
-----------------------

  * le script Gui/visualiseDatabase.py (qui peut egalement etre lance de l'outil generique lance.py) permet de visualiser les tables contenues dans la base. (Attention au path de la database)

         .. code-block:: python

            python visualiseDatabase.py -d ../myMesh.db

.. image:: images/visualisation.png


Lancer un job de maillage particulier ou l'ensemble des tests
----------------------------------------------------------------

   * le script passejob.py permet de passer l'ensemble des tests ou un cas particulier. il admet les options suivantes :

       - '-a' pour passer l ensemble des Tests (non activee par defaut)
       - '-s' pour preciser le path du runAppli (par exemple ~/Appli). Permet au job de trouver le runAppli
       - '-d' pour preciser le fichier dataBase
       - '-v' pour specifier la version de Salome
       - si l'option -a n'est pas activee, il faut preciser l'identifiant du job a passer 


         .. code-block:: python

            python passeJobs.py -d ../myMesh.db 1

Modifier les scripts pour les integrer dans le mecanisme de test 
-------------------------------------------------------------------

    * ajout des lignes suivantes a la fin du script :

         .. code-block:: python

            from Stats.getStats import getStatsMaillage, getStatsGroupes
            from Stats.getCritere import getStatsCritere
            # 
            fichierMedResult = 'fichierMed.med'
            getStatsMaillage(monMaillage,fichierMedResult)
            getStatsGroupes(monMaillage,fichierMedResult)
            getStatsCritere(dimMaillage,monMaillage,fichierMedResult)

Lancement du script de comparaison
-----------------------------------
  * compareVersions.py
       - '-s' pour preciser le path du runAppli (par exemple ~/Appli). permet au job de trouver le runAppli
       - '-r' pour specifier les numeros de versions de reference pour chacun des scripts
       - '-d' pour preciser le fichier dataBase
       - '-f' pour specifier le nom du rapport html produit (/tmp/toto.html par defaut -) )

         .. code-block:: python

           python compareVersions.py -s ./runAppli -r 1,2,2 -d ./myMesh.db -f ./rapport.html
 

   Ici, pour les scripts n°1, 2 et 3, les versions de reference sont, respectivement "Salome7.2.0", "Salome7.3.0" et "Salome 7.3.0".


export/import de la base
-------------------------
  * exportDatabaseToCSV.py 

    - admet l option  -p (pour partiel) qui ne sauvegarde pas les tables a priori communes a tous : 
            * la table des mailleurs
            * la table des maillages
            * la table des versions
            * la table des machines
            * la table des groupes references

   - les fichiers sont ranges dans la directory ExportDB+date. la premiere ligne de chaque fichier contient le nom des colonnes, puis les valeurs par ligne

   - pour faire une vraie sauvegarde de la base (structure et donnees) il faut lancer sqlite3 et executer .dump


  * importDatabaseFromCSV.py

    - parametre d 'entree obligatoire : la directory qui contient les fichiers a importer 
    - admet l option  -p (pour partiel) qui n importe pas les tables a priori communes a tous  
    - admet l option  -f (pour force) qui pour les enregistrements qui existent deja dans la base remplace 
      par les valeurs donnees dans le fichier

Criteres de verification
========================
Principe
--------
Le principe est simple.
Pour chaque maillage, on definit des valeurs de reference associees au maillage. A chaque nouvelle version de Salome, on compare les resultats obtenus avec ces valeurs de reference pour le script etudie. On emet un warning a chaque fois que les ecarts relatifs depassent un certain seuil. 

Criteres
--------
Les criteres de verification portent sur :

  * Le temps CPU

  * Le nombre d'entites du maillage classe par type
        - Le nombre de noeuds
        - Le nombre de segments (maille 1D)
        - Le nombre de triangles (maille 2D)
        - Le nombre de quadrangles (maille 2D)
        - Le nombre de tetraedres (maille 2D)

  * Le rapport de tailles de chaque element du maillage (fonction GetAspectRatio)
        - Pour un maillage 3D, on calcul le ratio des mailles 3D
        - Pour un maillage 2D, on calcul le ratio des mailles 2D

  * La longueur de chaque element du maillage (fonction GetMaxElementLength)  
        - Pour un maillage 3D, on calcul la longueur des mailles 3D
        - Pour un maillage 2D, on calcul la longueur des mailles 2D

Ces criteres sont calcules sur tout le maillage et eventuellement sur des groupes de mailles de reference associes au maillage.

Pour chaque maillage, les valeurs de reference sont calculees sur la base d'une version specifique de Salome (qui peut etre differente d'un maillage a l'autre).

Pour le rapport de tailles et la longueur des mailles, on calcule systematiquement le maximun, le minimum, la moyenne, le 1er et 3eme quartile et enfin la mediane.

Pour ces quatres criteres, on definit un seuil a ne pas depasser (qui peut etre different d'un critere a l'autre). Actuellement, au sein d'un meme critere, les seuils sont identiques.


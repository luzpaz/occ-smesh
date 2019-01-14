la base
========

Schema de la base
-----------------

.. image:: images/bd.png
   :align: center


Organisation des sources
------------------------

  Les sources sont organisees ainsi :

  * La directory MaquetteMailleur contient les pythons necessaires au passage des tests : 

      - createDatabase.py    
      - ajoutEnreg.py  
      - changeVersion.py  
      - passeJobs.py.
      - compareVersions.py  
      - lance.py  
  
   a priori, seuls les scripts passeJobs.py et compareVersions ne devront etre passes a chaque version

  * Elle contient aussi la base de donnees (lorsque celle-ci a ete creee)

  * Sous MaquetteMailleur, la directory Doc contient les fichiers necessaires a l'elaboration de la doc. 
  * Sous MaquetteMailleur, la directory Scripts contient les fichiers necessaires a l'eleboration des maillages et references dans la database.
  * Sous MaquetteMailleur, la directory Gui contient les fichiers necessaires a la partie graphique, y compris les .ui a partir desquels il faut generer les .py
  * Sous MaquetteMailleur, la directory CreeDocuments contient les fichiers necessaires a la creation du rapport html. les patrons sont contenus dans TemplatesHtm 


La directory Base plus en detail 
--------------------------------

  * dataBase.py : definition de la class Base
    La methode Structure (jamais appelee) rappelle la commande sqlite3 pour voir de façon interactive la structure de la base (sqlite3 madabase.db)

  * les tables
    - une classe generique contenue dans tableDeBase
    - un fichier .py par table : tableGroupesRef.py, tableMaillages.py, tableMailleurs.py, tableRatios.py, tableVersions.py, tableGroupes.py, tableMachines.py, tableMailles.py, tablePerfs.py, tableTailles.py.  toutes les tables heritent de tableDeBase
    - la methode "remplit" remplit le jeu de test et doit etre modifiee (en particulier pour la definition de la version de reference)


  * creation eventuelle d'une nouvelle table il faut :
	- creer un fichier tableNouvelle.py
	- dans l'init appeler l init de TableDeBase avec le nom de la table en parametre
          et initialiser le nom des colonnes avec la methode setFields
        - creer une methode createSqlTable pour creer la table

     .. code-block:: python

        class TableMachines (TableDeBase):
           def __init__(self):
               TableDeBase.__init__(self,"Machines")
               self.setField(("id","nomMachine","Os"))

           def createSqlTable(self):
               query=QtSql.QSqlQuery()
               print "creation de TableMachine"
               print query.exec_("create table Machines(id int primary key, nomMachine varchar(10), os varchar(10));")


 
    - pour pouvoir editer la nouvelle table il faut :
        - ajouter un bouton par designer dans desFenetreChoix.ui
        - creer un signal dans maFenetreChoix.py qui appelle une methode qui
          ressemble a :

     .. code-block:: python

      def Push(self):
           editor=TableEditor(self.db.maTable,self)
           editor.exec_()



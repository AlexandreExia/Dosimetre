CREATE TABLE `activity` (
  `aID` int(11) NOT NULL auto_increment,
  `aName` text collate latin1_general_ci,
  `aClasse` text collate latin1_general_ci,
  `aDate` datetime default NULL,
  PRIMARY KEY  (`aID`)
) ENGINE=MyISAM AUTO_INCREMENT=59 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;


CREATE TABLE `activity_config` (
  `aID` int(11) NOT NULL,
  `aUserID` int(11) NOT NULL,
  `aIZ` int(11) default NULL,
  `aDED` double default NULL,
  `aDose` double default NULL,
  `aCoeff` double default NULL,
  `aUserRank` int(11) NOT NULL,
  PRIMARY KEY  (`aID`,`aUserID`),
  UNIQUE KEY `aIZ` (`aIZ`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


CREATE TABLE `batterie` (
  `ID_DOSI` int(11) NOT NULL auto_increment,
  `NBR_CYCLE` int(11) default NULL,
  `DATE_CONNECTION` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `DATE_DECONNEC` datetime default NULL,
  `TEST_DECHARGE` tinyint(4) default NULL,
  `TEST_CHARGE` tinyint(4) default NULL,
  `CHARGE_RESTANTE` int(11) default NULL,
  `FLAG` int(11) default NULL,
  `SONDE` varchar(30) collate latin1_general_ci NOT NULL default '',
  PRIMARY KEY  (`ID_DOSI`,`SONDE`),
  UNIQUE KEY `ID_DOSI` (`ID_DOSI`)
) ENGINE=MyISAM AUTO_INCREMENT=133 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

INSERT INTO `batterie` VALUES ('95','','0000-00-00 00:00:00','0000-00-00 00:00:00','0','0','0','0','123456');
INSERT INTO `batterie` VALUES ('121','40','2016-05-04 16:50:29','2016-05-11 15:57:55','0','0','0','0','285N5290070000P7');
INSERT INTO `batterie` VALUES ('124','40','2016-05-18 14:44:15','2016-05-18 14:40:09','0','0','0','0','12345678');
INSERT INTO `batterie` VALUES ('127','40','2016-05-17 21:58:41','2016-05-17 14:34:57','0','0','0','0','0123456789&#65533;');
INSERT INTO `batterie` VALUES ('132','0','2016-05-18 20:29:09','2016-05-18 20:25:03','0','0','0','0','0123456789');

CREATE TABLE `users` (
  `ID_Personne` int(11) NOT NULL auto_increment,
  `ID_Badge` varchar(32) default NULL,
  `Nom` varchar(32) NOT NULL,
  `Prenom` varchar(32) NOT NULL,
  `Date_Creation` datetime default NULL,
  PRIMARY KEY  (`ID_Personne`),
  UNIQUE KEY `ID_Badge` (`ID_Badge`)
) ENGINE=MyISAM AUTO_INCREMENT=40 DEFAULT CHARSET=latin1;

INSERT INTO `users` VALUES ('34','02006D7223','Serieux','Pas supprimé','2016-05-10 15:56:47');

CREATE TABLE `users_historic` (
  `ID_Personne` int(11) NOT NULL,
  `ID_Activity` int(11) NOT NULL,
  `Dose` double default NULL,
  `Ded_max` double default NULL,
  `Nombre_Alarme` int(11) NOT NULL,
  `Type_Alarme` tinytext collate latin1_general_ci,
  `Date_Creation` datetime default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

INSERT INTO `users_historic` VALUES ('29','24','0.052','1.203','2','dose','2016-05-10 15:36:48');
INSERT INTO `users_historic` VALUES ('29','0','0.053','0.256','2','dose','2016-05-10 15:38:13');
INSERT INTO `users_historic` VALUES ('29','0','0.053','0.256','2','dose','2016-05-10 15:38:14');
INSERT INTO `users_historic` VALUES ('32','0','0.053','0.256','2','dose','2016-05-10 16:00:20');
INSERT INTO `users_historic` VALUES ('32','0','0.053','0.256','2','dose','2016-05-10 16:00:21');
INSERT INTO `users_historic` VALUES ('32','0','0.053','0.256','2','dose','2016-05-10 16:00:22');
INSERT INTO `users_historic` VALUES ('34','0','0.053','0.256','2','dose','2016-05-10 16:01:12');
INSERT INTO `users_historic` VALUES ('34','0','0.053','0.256','2','dose','2016-05-10 16:01:13');
INSERT INTO `users_historic` VALUES ('34','0','0.053','0.256','2','dose','2016-05-10 16:01:14');
INSERT INTO `users_historic` VALUES ('34','0','0.038','0.12','0','DED DO','2016-05-17 13:36:15');
INSERT INTO `users_historic` VALUES ('0','2','2.196875','15','0','DED DOSE','2016-05-17 15:21:55');
INSERT INTO `users_historic` VALUES ('34','0','0.1125','15','0','DED DOSE','2016-05-17 15:23:47');


CREATE DATABASE SiteDirectory;

CREATE TABLE SiteUsers(
	id INT NOT NULL KEY AUTO_INCREMENT,
	username VARCHAR(255) NOT NULL,
	password VARCHAR(255) NOT NULL,
	email VARCHAR(255) NOT NULL,
	first_name VARCHAR(255),
	last_name VARCHAR(255),
	date_joined DATETIME
);

CREATE TABLE StockSymbols(
	stock_sym varchar(20) NOT NULL KEY,
	stock_name varchar(255),
);

CREATE TABLE PORTFOLIO(
PORTFOLIO VARCHAR(41) NOT NULL PRIMARY KEY,
BALANCE MONEY NOT NULL,

USERNAME VARCHAR(41),
CONSTRAINT C_FK_1 FOREIGN KEY (USERNAME) REFERENCES USERS (USERNAME)

);

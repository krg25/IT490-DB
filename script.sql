CREATE DATABASE StockApp;
\! echo "\nStockApp DB Created"

USE StockApp;
\! echo "\nCreating Table: SiteUsers"
/*DONT CHANGE SiteUsers, DON'T CHANGE StockData, DON'T CHANGE StockInfo, other files are now dependent on this layout; if you must, let ken know*/
CREATE TABLE SiteUsers(
	user_id INT NOT NULL KEY AUTO_INCREMENT,
	username VARCHAR(255) NOT NULL UNIQUE,
	password VARCHAR(255) NOT NULL,
	email VARCHAR(255) NOT NULL,
	first_name VARCHAR(255),
	last_name VARCHAR(255),
	date_joined TIMESTAMP NOT NULL
);
/*Unnessecary plus I don't get stock name from API
\! echo "\nCreating Table: StockInfo"
CREATE TABLE StockInfo(
	symbol varchar(20) NOT NULL KEY,
	stock_name varchar(255)
);
*/
\! echo "\nCreating Table: StockData"
CREATE TABLE StockData(
	symbol VARCHAR(20) NOT NULL,
/*	price_time ENUM('O', 'C') NOT NULL, */
	open DOUBLE(8,2) NOT NULL,
	high DOUBLE(8,2) NOT NULL,
	low DOUBLE(8,2) NOT NULL,
	price DOUBLE(8,2) NOT NULL,
	prvclose DOUBLE(8,2) NOT NULL,
	volume INT NOT NULL,
	timestamp TIMESTAMP NOT NULL,
	PRIMARY KEY(symbol, timestamp)
/*	FOREIGN KEY (symbol) REFERENCES StockInfo(symbol) */
);
\! echo "\nCreating Table: UserPortfolios"
CREATE TABLE UserPortfolios(
	portfolio_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	user_id INT NOT NULL,
	symbol VARCHAR(20) NULL,
	stock_owned INT NULL,
	stock_initial DOUBLE(8,2) NULL,
/*	stock_current 
	{SELECT price FROM StockData 
	WHERE (StockData.symbol=symbol AND timestamp>DATE_SUB(NOW(), INTERVAL 1 DAY)) 		ORDER BY timestamp DESC LIMIT 1;}
	(or something like that, can be added retroactively I believe)
*/
	FOREIGN KEY (user_id) REFERENCES SiteUsers(user_id),
	FOREIGN KEY (symbol) REFERENCES StockData(symbol)
);
\! echo "\nCreating Table: Transactions"
CREATE TABLE Transactions(
	trans_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	user_id INT NOT NULL,
/*	price_type ENUM('S', 'B') NOT NULL, */
	symbol VARCHAR(20) NOT NULL,
	stock_value DOUBLE(8,2) NOT NULL,
	trans_volume INT NOT NULL, 
	timestamp TIMESTAMP NOT NULL,
	FOREIGN KEY (user_id) REFERENCES SiteUsers(user_id),
	FOREIGN KEY (symbol) REFERENCES StockData(symbol)
);
\! echo "\nCreating Table: UserAccounts"
CREATE TABLE UserAccounts(
	user_id INT NOT NULL,
	portfolio_id INT NULL,
	user_wallet DOUBLE(8,2) NOT NULL DEFAULT 100000,
/*  	portfolio_value = (where userPortfolios.user_id = user_id, sum(stock_value))
	or something like that, I don't know 
*/
	FOREIGN KEY (portfolio_id) REFERENCES UserPortfolios(portfolio_id)
);

\! echo "\nCreating DB Connection User"
CREATE USER 'connectionuser'@'localhost' IDENTIFIED BY 'conn3ctpass';
GRANT ALL ON *.* TO 'connectionuser'@'localhost' WITH GRANT OPTION;

\! echo "\n\nDatabase Added. Adding Test User to SiteUsers."

INSERT INTO SiteUsers (username,password,email,first_name,last_name)
VALUES('test','test123','test@test.com','Test','User');

SELECT * FROM SiteUsers WHERE username='test';

SHOW TABLES;

\! echo "Script Complete"


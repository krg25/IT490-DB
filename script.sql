CREATE DATABASE StockApp;
\! echo "\nStockApp DB Created"

USE StockApp;
\! echo "\nCreating Table: SiteUsers"
/*DONT CHANGE SiteUsers, DON'T CHANGE StockData, DON'T CHANGE StockInfo, other files are now dependent on this layout; if you must, let ken know*/
CREATE TABLE SiteUsers(
	user_id INT NOT NULL KEY AUTO_INCREMENT,
	username VARCHAR(32) NOT NULL UNIQUE,
	password VARCHAR(32) NOT NULL,
	email VARCHAR(64) NOT NULL,
	first_name VARCHAR(32),
	last_name VARCHAR(32),
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
	stock_current DOUBLE(8,2) NULL,

/*	SELECT price FROM StockData WHERE StockData.symbol = $UserPortfolios.symbol,	
	stock_current DOUBLE(8,2) GENERATED ALWAYS AS (StockData.price * UserPortfolios.stock_owned),

unsure
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
	user_id INT NOT NULL PRIMARY KEY,
	user_wallet DOUBLE(8,2) NOT NULL DEFAULT 100000,
/*  	portfolio_value = (where userPortfolios.user_id = user_id, sum(stock_value))
	or something like that, I don't know 
*/
	FOREIGN KEY (user_id) REFERENCES SiteUsers(user_id)
);


\! echo "\nCreating Portfolio History Table"
CREATE TABLE PortfolioHistory(
	user_id INT NOT NULL PRIMARY KEY,
	timestamp TIMESTAMP NOT NULL,
	portfolio_value DOUBLE(8,2),
	FOREIGN KEY (user_id) REFERENCES SiteUsers(user_id)
);

\! echo "\nCreating Chat Log Table"
CREATE TABLE chat_log(
chat_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
username VARCHAR(32) NOT NULL,
content VARCHAR(128) NOT NULL,
timestamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

\! echo "\nCreating DB Connection User"
CREATE USER 'connectionuser'@'localhost' IDENTIFIED BY 'conn3ctpass';
GRANT ALL ON *.* TO 'connectionuser'@'localhost' WITH GRANT OPTION;

\! echo "\n\nDatabase Added. Adding Test User to SiteUsers."

INSERT INTO SiteUsers (user_id, username,password,email,first_name,last_name)
VALUES('2', 'test','test123','test@test.com','Test','User');

INSERT INTO UserAccounts (user_id) VALUES ('2');

SELECT * FROM SiteUsers WHERE username='test';
SELECT * FROM UserAccounts;

SHOW TABLES;

\! echo "Script Complete"


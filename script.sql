CREATE DATABASE StockApp;

USE StockApp;

CREATE TABLE SiteUsers(
	user_id INT NOT NULL KEY AUTO_INCREMENT,
	username VARCHAR(255) NOT NULL,
	password VARCHAR(255) NOT NULL,
	email VARCHAR(255) NOT NULL,
	first_name VARCHAR(255),
	last_name VARCHAR(255),
	date_joined TIMESTAMP NOT NULL
);

CREATE TABLE StockInfo(
	stock_sym varchar(20) NOT NULL KEY,
	stock_name varchar(255)
);

CREATE TABLE StockData(
	stock_sym VARCHAR(20) NOT NULL,
	price_time ENUM('O', 'C') NOT NULL,
	price FLOAT NOT NULL,
	volume INT NOT NULL,
	timestamp TIMESTAMP NOT NULL,
	PRIMARY KEY(stock_sym, timestamp),
	FOREIGN KEY (stock_sym) REFERENCES StockInfo(stock_sym)
);

CREATE TABLE UserPortfolio(
	portfolio_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	user_id INT NOT NULL,
	stock_sym VARCHAR(20) NULL,
	stock_volume INT NULL,
	stock_value FLOAT NULL,
	FOREIGN KEY (user_id) REFERENCES SiteUsers(user_id),
	FOREIGN KEY (stock_sym) REFERENCES StockData(stock_sym)
);

CREATE TABLE transactions(
	trans_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	user_id INT NOT NULL,
	price_type ENUM('S', 'B') NOT NULL,
	stock_sym VARCHAR(20) NOT NULL,
	value FLOAT NOT NULL,
	trans_volume INT NOT NULL, 
	FOREIGN KEY (user_id) REFERENCES SiteUsers(user_id),
	FOREIGN KEY (stock_sym) REFERENCES StockData(stock_sym)
);

CREATE TABLE userAccount(
	user_id INT NOT NULL,
	portfolio_id INT NULL,
	portfolio_value FLOAT NOT NULL DEFAULT 100000,
	FOREIGN KEY (portfolio_id) REFERENCES UserPortfolio(portfolio_id)
);


CREATE USER 'connectionuser'@'localhost' IDENTIFIED BY 'conn3ctpass';
GRANT ALL ON *.* TO 'connectionuser'@'localhost' WITH GRANT OPTION;




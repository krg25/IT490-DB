<?php

class stockDataLink
{
	private $sqlConnection;
	private $stock_symbol;
	private $stock_array;


	function __construct($sqlConn)
	{
		$this->sqlConnection = $sqlConn;
	}
	
	function storeStock($res) //need to store in StockInfo AND StockData
	{
		require_once("Logger.php.inc");
		$logger = new logger('StockData.php.inc->storeStock','rabbit/rabbit.ini');

	$q=("INSERT INTO StockData (symbol, open, high, low, price, prvclose, volume) 
VALUES ('".$res['symbol']."','".$res['open']."','".$res['high']."','".$res['low']."','".$res['price']."','".$res['prvclose']."','".$res['volume']."')");
		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);

		if(empty(mysqli_error($this->sqlConnection)))
		{
		echo ($num);
		return;
		}
		else
		{
			$logger->logError("Error inserting new stock into table","ERROR",mysqli_error($this->sqlConnection));
			echo "Error inserting new stock to table" . mysqli_error($this->sqlConnection);
		}
		
	}
	function apiLookup($symb)
	{
		require_once("Logger.php.inc");
		$logger = new logger("StockData.php.inc->apiLookup","rabbit.ini");

		
		$rabbit = new rabbitMQClient("rabbit/rabbit.ini","dmz");
		$response=$rabbit->send_request($symb);
		//echo ("from apiLookup");
		//var_dump($response);
		if($response['returnCode'] == 1){ 
		$this->storeStock($response);
		}else{
		//don't want to store bogus data, right? Do nothing.
		}
		return $response;

	}


	function getStock($symb)
	{
		require_once("Logger.php.inc");
		$logger = new logger("StockData.php.inc->getStock",'rabbit/rabbit.ini');

		$this->stock_symbol = $symb;

		$q="SELECT * FROM StockData WHERE (symbol='$symb' AND timestamp>DATE_SUB(NOW(), INTERVAL 1 DAY)) ORDER BY timestamp DESC"; //this returns the # of rows that match our stock symbol with a datetime less than 24 hours before current, should only return one line but may not, top result is most recent

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);

		if(empty(mysqli_error($this->sqlConnection)))
		{
			if($num >= 1) //should only be one result but may not
			{
				echo("Found " . $num. " entries".PHP_EOL);
				$this->stock_array = @mysqli_fetch_assoc($sqlResult); //pop the top, should be most recent
				return $this->stock_array;
				mysqli_close();
			}
			else
			{
				$logger->logDebug('No results in DB',PHP_EOL);

				echo("No results in DB".PHP_EOL);
				$this->stock_array = $this->apiLookup($this->stock_symbol);
				return $this->stock_array;
				mysqli_close();
			}
		}
		else
		{
			$logger->logError("SQL ERROR","ERROR",mysqli_error($this->sqlConnection));
			echo("SQL Error" . mysqli_error($this->sqlConnection));
		}
}
	



}

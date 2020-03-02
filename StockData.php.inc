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

	function getStock($symb)
	{
		$this->stock_symbol = $symb;

		$q="SELECT * FROM StockData WHERE (stock_sym='$symb' AND timestamp<DATE_SUB(NOW(), INTERVAL 24 HOUR)) ORDER BY timestamp DESC"; //this returns the # of rows that match our stock symbol with a datetime less than 24 hours before current, should only return one line but may not, top result is most recent

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);

		if(empty(mysqli_error($this->sqlConnection)))
		{
			if(num >= 1) //should only be one result but may not
			{

				$this->stock_array = @mysqli_fetch_assoc($sqlResult); //pop the top, should be most recent
				return $this->stock_array;
				mysqli_close();
			}
			else
			{
				$this->stock_array = apiLookup($this->stock_symbol);
				return $this->stock_array;
				mysqli_close();
			}
		}
		else
		{
//error in the sql
		}
}
	
	function apiLookup($symb)
	{
		
		$rabbit = new rabbitMQClient("rabbit/rabbit.ini","dmz");
		$response=$rabbit->send_request($symb);

		storeStock($response);
		return $response;

	}

	function storeStock($res) //need to store in StockInfo AND StockData
	{
	$q=("INSERT INTO StockData (symbol, open, high, low, price, prvclose, volume) VALUES (".$res['symbol'].",".$res['open'].",".$res['high'].",".$res['low'].",".$res['price'].",".$res['prvclose'].",".$res['volume'].")");
		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);

		if(empty(mysqli_error($this->sqlConnection)))
		{
		return;
		}
		else
		{
		echo "Error inserting new stock to table";
		}
		
	}


}

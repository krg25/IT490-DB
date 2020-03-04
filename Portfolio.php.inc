<?php

class portfolioLink
{
	private $sqlConnection;
	private $user_id;
	//private $stock_symbol;
	//private $stock_array;

	function __construct($sqlConn)
	{
		$this->sqlConnection = $sqlConn;
	}
	/*
	function storeStock($res) //need to store in StockInfo AND StockData
	{
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
		echo "Error inserting new stock to table" . mysqli_error($this->sqlConnection);
		}
		
	}

	function apiLookup($symb)
	{
		
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

	*/
	function getPortfolio($id)
	{
		$this->user_id = $id;

		$q="SELECT * FROM UserPortfolios WHERE (user_id='$id') ORDER BY symbol DESC"; 

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);

		if(empty(mysqli_error($this->sqlConnection)))
		{
			if($num >= 1) //should be one result or more
			{
				echo("Found " . $num. " entries".PHP_EOL);
				//I think returning the entire SQL response would be best?
				while ($row = @mysqli_fetch_assoc($sqlResult)){ //pop the top, should be most recent
				
				}
				return $this->stock_array;
				mysqli_close();
			}
			else
			{
				echo("No results in DB".PHP_EOL);
				$out['returnCode']=2;
				return $out;
				mysqli_close();
			}
		}
		else
		{
		echo("SQL Error" . mysqli_error($this->sqlConnection));
		}
}
	



}

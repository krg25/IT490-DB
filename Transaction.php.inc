<?php

class transactionLink
{
	private $sqlConnection;
	private $user_id;
	//private $stock_symbol;
	//private $stock_array;

	function __construct($sqlConn)
	{
		$this->sqlConnection = $sqlConn;
	}

	function storeTransaction($res) //need to store transaction
	{
	$q=("INSERT INTO Transactions (user_id, symbol, stock_value, trans_volume) 
VALUES ('".$res['user_id']."','".$res['symbol']."','".$res['value']."','".$res['volume']."')");
		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);

		if(empty(mysqli_error($this->sqlConnection)))
		{
		echo ($num);
		return;
		}
		else
		{
		echo "Error inserting new transaction to table" . mysqli_error($this->sqlConnection);
		}
		
	}

	function getTransactionsByUser($id)
	{
		$this->user_id = $id;

		$q="SELECT * FROM UserPortfolios WHERE (user_id='$id') ORDER BY symbol DESC"; 

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);
		$out = array();
		if(empty(mysqli_error($this->sqlConnection)))
		{
			if($num >= 1) //should be one result or more
			{
				echo("Found " . $num. " entries".PHP_EOL);
				while ($row = @mysqli_fetch_assoc($sqlResult)){
					$out[$row['trans_id']][$row['symbol']];
					$out[$row['trans_id']][$row['stock_value']];
					$out[$row['trans_id']][$row['trans_volume']];
					$out[$row['trans_id']][$row['timestamp']];
				}
				return $this->stock_array;
				mysqli_close();
			}
			else
			{
				echo("No results in DB".PHP_EOL);
				$out['meta']['returnCode']=2;
				return $out;
				mysqli_close();
			}
		}
		else
		{
		echo("SQL Error" . mysqli_error($this->sqlConnection));
		}
}
	function getTransactionsByTrans($id)
	{

		$q="SELECT * FROM Transactions WHERE (transaction_id='$id') ORDER BY symbol DESC"; 

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);
		$out = array();
		if(empty(mysqli_error($this->sqlConnection)))
		{
			if($num >= 1) //should be one result or more
			{
				echo("Found " . $num. " entries".PHP_EOL);
				while ($row = @mysqli_fetch_assoc($sqlResult)){
					$out[$row['user_id']][$row['symbol']];
					$out[$row['user_id']][$row['stock_value']];
					$out[$row['user_id']][$row['trans_volume']];
					$out[$row['user_id']][$row['timestamp']];
				}
				return $this->stock_array;
				mysqli_close();
			}
			else
			{
				echo("No results in DB".PHP_EOL);
				$out['meta']['returnCode']=2;
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

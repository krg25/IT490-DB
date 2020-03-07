<?php

class transactionLink
{
	private $sqlConnection;
	private $user_id;
	private $wallet;
	private $cost;
	//private $stock_symbol;
	//private $stock_array;

	function __construct($sqlConn)
	{
		$this->sqlConnection = $sqlConn;
	}

	function storeTransaction($res) //need to store transaction
	{
		require_once('Logger.php.inc');
		$logger = new logger('Transactions.php.inc->storeTransaction','rabbit/rabbit.ini');

	$q=("SELECT * FROM UserAccounts WHERE (user_id = ".$res['user_id'].")");
	$sqlResult = @mysqli_query($this->sqlConnection,$q);
	$num = @mysqli_num_rows($sqlResult);
	$user = @mysqli_fetch_assoc($sqlResult);

	//
	if(empty(mysqli_error($this->sqlConnection)))
	{
		if($user['user_wallet'] >= ($res['qty']*$res['price'])){
			$this->wallet = $user['user_wallet'];
			$this->cost = $res['qty']*$res['price'];
			$q=("INSERT INTO Transactions (user_id, symbol, stock_value, trans_volume) 
			VALUES ('".$res['user_id']."','".$res['symbol']."','".$res['price']."','".$res['qty']."')");
			$sqlResult = @mysqli_query($this->sqlConnection,$q);
			$num = @mysqli_num_rows($sqlResult);

			if(empty(mysqli_error($this->sqlConnection)))
			{
				$this->wallet = ($this->wallet - $this->cost);
				$q=("UPDATE UserAccounts SET user_wallet = '".$this->wallet."' WHERE user_id = ".$res['user_id']);
				$sqlResult = @mysqli_query($this->sqlConnection,$q);
				$num = @mysqli_num_rows($sqlResult);
	
				if(empty(mysqli_error($this->sqlConnection)))
				{
					$this->wallet = ($this->wallet - $this->cost);
					$q=("INSERT INTO UserPortfolios (user_id, symbol, stock_owned, stock_initial) 
					VALUES ('".$res['user_id']."','".$res['symbol']."','".$res['qty']."','".$res['price']."')");
					$sqlResult = @mysqli_query($this->sqlConnection,$q);
					$num = @mysqli_num_rows($sqlResult);

					if(empty(mysqli_error($this->sqlConnection)))
					{
						
						return true;
					}else{
						$logger->logError("Error Inserting to UserPortfolios","ERROR",mysqli_error($this->sqlConnection));
						
						echo ("Error inserting to UserPortfolios: " . mysqli_error($this->sqlConnection));
					}//ifelse insert UserPortfolios
				}else{
					$logger->logError("Error in Update","ERROR",mysqli_error($this->sqlConnection));
				echo ("Error in update: " . mysqli_error($this->sqlConnection));
				}//ifelse update UserAccounts

			
			}else{
				$logger->logError("Error Inserting new Transaction to table","ERROR", mysqli_error($this->sqlConnection));
			echo ("Error inserting new transaction to table" . mysqli_error($this->sqlConnection));
			}//ifelse insert Transactions
		}else{echo "You can't afford this stock, broke ass.";
		$logger->logDebug("Not enough Funds",'Not enough funds in the wallet to complete transaction');
		} //if wallet>=cost
		
	}else{
		$logger->logError("Error Getting User Account",'ERROR',mysqli_error($this->sqlConnection));
	echo ("Error getting user account" . mysqli_error($this->sqlConnection));
	}//ifelse get UserAccount

	}//function

	function getTransactionsByUser($id)
	{
		$this->user_id = $id;

		$q="SELECT * FROM Transactions WHERE (user_id='$id') ORDER BY timestamp DESC"; 

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);
		$out = array();
		if(empty(mysqli_error($this->sqlConnection)))
		{
			if($num >= 1) //should be one result or more
			{
				$out[0]['returnCode']=1;
				echo("Found " . $num. " entries".PHP_EOL);
				$count = 0;
				while ($row = mysqli_fetch_assoc($sqlResult)){
				$count=$count+=1;
					$out[$count]['trid']=$row['trans_id'];
					$out[$count]['symbol']=$row['symbol'];
					$out[$count]['price']=$row['stock_value'];
					$out[$count]['qty']=$row['trans_volume'];
					$out[$count]['time']=$row['timestamp'];
				}
				
				return $out;
				mysqli_close();
			}
			else
			{
				echo("No results in DB".PHP_EOL);
				$out[0]['returnCode']=2;
				return $out;
				mysqli_close();
			}
		}
		else
		{
		echo("SQL Error" . mysqli_error($this->sqlConnection));
		}
}


/*
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
	
*/


}

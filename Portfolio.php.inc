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

	function getPortfolio($id)
	{

		require_once('Logger.php.inc');
		$logger = new logger("Portfolio.php.inc->getPortfolio",'rabbit/rabbit.ini');

		$this->user_id = $id;

	$q="SELECT * FROM UserPortfolios WHERE (user_id='$id') ORDER BY symbol ASC"; 

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);
		$out = array();


		$stock = new stockDataLink($this->sqlConnection);
		


		if(empty(mysqli_error($this->sqlConnection)))
		{
			if($num >= 1) //should be one result or more
			{

				$out[0]['returnCode']=1;
				echo("Found " . $num. " entries".PHP_EOL);
				$count = 0;
				$readstocks=array();
				while ($row = mysqli_fetch_assoc($sqlResult))
				{
				$count=$count+=1;
				if(!in_array($row['symbol'],$readstocks)){
				$stockupd = $stock->getStock($row['symbol']);
				$readstocks[$count] = $row['symbol'];
				$q2=("UPDATE UserPortfolios SET stock_current = '".$stockupd['price']."' WHERE symbol = '".$row['symbol']."'");
				$sqlResult2 = @mysqli_query($this->sqlConnection,$q2);
				if(empty(mysqli_error($this->sqlConnection)))
				{
					//echo "no error";
				}else{echo ("error updating userportfolio" . mysqli_error($this->sqlConnection));}

				echo ("Stock".$row['symbol']."updated");
				}else{echo ("Stock".$row['symbol']."already updated, ignored");}

					$out[$count]['symbol']=$row['symbol'];
					$out[$count]['stock_owned']=$row['stock_owned'];
					$out[$count]['stock_initial']=$row['stock_initial'];
					$out[$count]['stock_current']=$stockupd['price']; //update is saved but not in active query
				}//while

				$q="SELECT * FROM UserAccounts WHERE (user_id='$id')"; 

				$sqlResult = @mysqli_query($this->sqlConnection,$q);
				$num = @mysqli_num_rows($sqlResult);
			
				if(empty(mysqli_error($this->sqlConnection)))
				{
				$row = mysqli_fetch_assoc($sqlResult);
				$out[0]['wallet']=$row['user_wallet'];
				
				return $out;
				mysqli_close();


				}else{ ///3rd if
				echo "error getting the wallet".mysqli_error($this->sqlConnection);
				$logger->logError("Error Updating User Wallet", "Error", mysqli_error($this-sqlConnection));
				

				}

			}else{
				$logger->logDebug("No results in DB". PHP_EOL);
				echo("No results in DB".PHP_EOL);
				$out[0]['returnCode']=2;
				return $out;
				mysqli_close();
			}

		}else{
			$logger->logError("SQL ERROR","ERROR",mysqli_error($this-sqlConnection));
			echo("SQL Error" . mysqli_error($this->sqlConnection));
		}
}

	



}

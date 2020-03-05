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
		$this->user_id = $id;

	$q="SELECT * FROM UserPortfolios WHERE (user_id='$id') ORDER BY symbol ASC"; 

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
				while ($row = mysqli_fetch_assoc($sqlResult))
				{
				$count=$count+=1;

					$out[$count]['symbol']=$row['symbol'];
					$out[$count]['stock_owned']=$row['stock_owned'];
					$out[$count]['stock_initial']=$row['stock_initial'];
				//$out[$count]['stock_current']=$row['stock_current'];
				}//while

				//want to get users wallet
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
				}
			}else{ //second if, num
				echo("No results in DB".PHP_EOL);
				$out[0]['returnCode']=2;
				return $out;
				mysqli_close();
			}
		
		}else{
		echo("SQL Error" . mysqli_error($this->sqlConnection));
		}
}

	



}

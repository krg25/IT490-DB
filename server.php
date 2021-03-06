#!/usr/bin/php
<?php
require_once('rabbit/path.inc');
require_once('rabbit/get_host_info.inc');
require_once('rabbit/rabbitMQLib.inc');
require_once('StockData.php.inc');
require_once('Transaction.php.inc');
require_once('Portfolio.php.inc');
ini_set('display_errors', 'On');

function doTransaction($res){
	require_once('Logger.php.inc');
	$log = new logger('server.php->doTransaction','rabbit/rabbit.ini');

        if (!isset($dbc)){
        require('mysqli_connect.php');
        }
	//$symbol = strtoupper($symbol);
	$transaction = new transactionLink($dbc);
	$out = array();
	$out = $transaction->storeTransaction($res);
	$log->logDebug("Success","Transaction Successful");
	return $out;
	

}

function getStockData($symbol){
	require_once('Logger.php.inc');
	$log = new logger('server.php->getStockData','rabbit/rabbit.ini');

        if (!isset($dbc)){
        require('mysqli_connect.php');
        }
	$symbol = strtoupper($symbol);
	$stock = new stockDataLink($dbc);
	$data = $stock->getStock($symbol);
	$out = array();
	if(isset($data['symbol'])){//$data['returnCode'] == 1){
	$out['returnCode'] = 1;
	$out['symbol']	 = $data['symbol'];
	$out['open']	 = $data['open'];
	$out['high']	 = $data['high'];
	$out['low']	 = $data['low'];
	$out['price']	 = $data['price'];
	$out['volume']	 = $data['volume'];
	$out['prvclose'] = $data['prvclose'];
	}else{
	$out['returnCode'] = 2;
	}
	
	//var_dump($out);
	return $out;
	
}
function getUserInfo($id){
	require_once('Logger.php.inc');
	$log = new logger('server.php->getUserInfo','rabbit/rabbit.ini');

        if (!isset($dbc)){
        require('mysqli_connect.php');
        }
        $q = "SELECT * FROM SiteUsers WHERE (user_id='$id')";
        $r = @mysqli_query($dbc, $q);
        $num = @mysqli_num_rows($r);
	$report = mysqli_fetch_assoc($r);


	if (empty(mysqli_error($dbc))){
           if ($num == 1){
		$out['0']=true;
		$out['id']=$report['user_id'];
		$out['user']=$report['username'];
		$out['fname']=$report['first_name'];
		$out['lname']=$report['last_name'];
		$out['date']=$report['date_joined'];
        	mysqli_close($dbc);
                return $out;
            }
            else
            {
                $out['0']=false;
                mysqli_close($dbc);
                return $out;
            }
        }

}
function getPortfolio($id){
        if (!isset($dbc)){
        require('mysqli_connect.php');
        }
	$port = new portfolioLink($dbc);
	$out = $port->getPortfolio($id);

	if (empty(mysqli_error($dbc))){
 
        	mysqli_close($dbc);
                return $out;
        }

}
function getTransactions($id){
        if (!isset($dbc)){
        require('mysqli_connect.php');
        }
	$translink = new transactionLink($dbc);
	$out = $translink->getTransactionsByUser($id);

	if (empty(mysqli_error($dbc))){
 
        	mysqli_close($dbc);
                return $out;
        }

}
function doLogin($username,$password)
{
	require_once('Logger.php.inc');
	$log = new logger('server.php->doLogin','rabbit/rabbit.ini');

	if (!isset($dbc)){
	require('mysqli_connect.php');
	}
	$q = "SELECT * FROM SiteUsers WHERE (username='$username' AND password='$password')";
	$r = @mysqli_query($dbc, $q);
	$num = @mysqli_num_rows($r);
	$report = mysqli_fetch_assoc($r);

	if (empty(mysqli_error($dbc))){
		if ($num == 1){
			$out['0'] = true;
			$out['id']=$report['user_id'];
		mysqli_close($dbc);
		$log->logDebug('Success','User logged in successfully');	
		echo "Valid login!\n";
		return  $out;
		}
		else
		{
		$log->logDebug('Failed','Incorrect login credentials');	
		echo "Error: Incorrect login\n";
		mysqli_close($dbc);
			$out['0'] = false;
		return  $out;
		}
	}
	else
	{
		$log->logError('SQL Error','ERROR',mysqli_error($dbc));
		echo "SQL Error: ".mysqli_error($dbc)."\n";
		mysqli_close($dbc);
			$out['0'] = false;
		return  $out;
	}

}
function doRegister($username,$password,$email,$fname,$lname)
{
	require_once('Logger.php.inc');
	$log = new logger('server.php->doRegister','rabbit/rabbit.ini');

	$out=array();
	if (!isset($dbc)){
	require('mysqli_connect.php');
	}
	$q = "INSERT INTO SiteUsers (username, password, email, first_name, last_name) VALUES ('$username', '$password', '$email', '$fname', '$lname')";
	$r = @mysqli_query($dbc, $q);
	$num = @mysqli_num_rows($r);
	$report = "";

	if (empty(mysqli_error($dbc))){
		$q = "SELECT * FROM SiteUsers WHERE (username='$username' AND password='$password')";
		$r = @mysqli_query($dbc, $q);
		$num = @mysqli_num_rows($r);
		if (empty(mysqli_error($dbc))){
			$report = mysqli_fetch_assoc($r);
			$out['0'] = true;
			$out['id']=$report['user_id'];

			$q = "INSERT INTO UserAccounts (user_id) VALUES (".$report['user_id'].")";
			$r = @mysqli_query($dbc, $q);
			$num = @mysqli_num_rows($r);
		$report = "";

			if (empty(mysqli_error($dbc))){
			mysqli_close($dbc);
			$log->logDebug('Success','New user Registered');
			echo "New Registration!\n";
			return  $out;
			}else{
			echo(mysqli_error($dbc));
			}			

		}else{
		$log->logError("Registration Error",'ERROR',mysqli_error($dbc));	
		echo(mysqli_error($dbc)); //who gives a flying fuck
		}

	}
	else
	{
		
	$log->logError("Registration Error",'ERROR',mysqli_error($dbc));	
	echo "A registration resulted in an error: ".mysqli_error($dbc).PHP_EOL;
	$out['0']=false;
	mysqli_close($dbc);
	return $out;
	}
	


}

function insertChat($username, $content){
require ('mysqli_connect.php');
$timestamp = date('Y-m-d G:i:s');
$username = mysqli_real_escape_string($dbc, $username);
$content = mysqli_real_escape_string($dbc, $content);
$out = array();
$q = "INSERT INTO chat_log (username, content, timestamp) VALUES ('$username', '$content', '$timestamp')";
$r = @mysqli_query($dbc, $q);
if (empty(mysqli_error($dbc))){
	echo "Post submitted".PHP_EOL;
	$out['0'] = 1;
	return $out;
}else{
	echo "Error: " . mysqli_error($dbc) . PHP_EOL;
	$out['0'] = 2;
	return $out;
}
mysqli_close($dbc);
}

function getChatLog(){
require ('mysqli_connect.php');
$q = "SELECT username, content, timestamp FROM chat_log ORDER BY timestamp ASC";
$r = @mysqli_query($dbc, $q);

if (empty(mysqli_error($dbc))){
	//success, let's not flood the cli because this happens often
	$out['0'] = 1;	
	$i = 1;
	
	foreach($r as $line){
	$out[$i]['username']=$line['username'];
	$out[$i]['content']=$line['content'];
	$out[$i]['timestamp']=$line['timestamp'];
	$i = $i + 1;
	}

	return $out;
}else{
	echo "Error: " . mysqli_error($dbc) . PHP_EOL;
	$out['0'] = 2;
	return $out;
}



mysqli_close($dbc);

}


function requestProcessor($request)
{
  require_once('Logger.php.inc');
  $log = new logger('server.php->requestProcessor','rabbit/rabbit.ini');

  $log->logDebug('Request Received',PHP_EOL);
  echo "received request".PHP_EOL;
  var_dump($request);


  if(!isset($request['type']))
  {
    $log->logError("Processor Error","ERROR","Unsupported Message Type");
    return "ERROR: unsupported message type";
  }
  switch ($request['type'])
  {
    case "Login":
    $login=doLogin($request['username'],$request['password']);
     if($login['0']){
	return array("returnCode" => '1', 'message'=>"Successful Login", 'ID' => $login['id']);	
	}
	else
	{
	  return array("returnCode" => '2', 'message'=>"Unsuccessful Login");
	}
    

	//Ken was here, change this to operate like login.
     case "Register":
     $register=doRegister($request['username'],$request['password'],$request['email'],$request['fname'],$request['lname']);
     if($register['0']){
	  return array("returnCode" => '1', 'message'=>"Successful Registration", 'ID' => $register['id']);
	}
	else
	{
	  return array("returnCode" => '2', 'message'=>"Unsuccessful Registration");
	}


     case "Profile":
	$userinfo=getUserInfo($request['ID']);
	if($userinfo['0']){
	

	return array(
	"returnCode" => '1',
	"id" => $userinfo['id'],
	"username" => $userinfo['user'],
	"fname" => $userinfo['fname'],
	"lname" => $userinfo['lname'],
	"date" => $userinfo['date']
	);

	}else{
	return array("returnCode" => '2');
	}
     
     case "API":
	$stockData=getStockData($request['symbol']);
	if($stockData['returnCode'] == 1){
	echo ("Successful data get".PHP_EOL);
	//var_dump($stockData);
	return $stockData;
	}else{echo ("Unsuccessful data get, stock may not exist".PHP_EOL);
	return array("returnCode" => '2');
	}

	case "Transaction": 
	if(isset($request['sub'])){
if($request['sub'] == 1){
	if(doTransaction($request)){
	return array("returnCode" => '1');
	}else{
	return array("returnCode" => '2');
	}
}elseif($request['sub'] == 2){
	$out = getTransactions($request['ID']);	
	return $out;
	}
	}
	

	case "Portfolio":
	$out=getPortfolio($request['ID']);
	return $out;

	case "InsertChat":
	$out=insertChat($request['username'],$request['content']);
	return $out;

	case "GetChatLog":
	$out=getChatLog();
	return $out;

	
  
}

   return array("returnCode" => '0', 'message'=>"Error, unsupported message type");
}

$server = new rabbitMQServer("rabbit/rabbit.ini","database");

echo "testRabbitMQServer BEGIN".PHP_EOL;
$server->process_requests('requestProcessor');
echo "testRabbitMQServer END".PHP_EOL;
exit();
?>


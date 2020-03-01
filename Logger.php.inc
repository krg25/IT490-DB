<?php

require_once('rabbitMQLib.inc');
require_once('get_host_info.inc');
require_once('path.inc');


class logger
{
	private $sending_server;
	private $log_type;
	private $error_level;
	private $log_message;
	private $log_timestamp;
		
	
	private $rabbit_server;
	private $rabbit_message;
	
	function __construct($log_sender, $rabbitServ)
	{
		$this->rabbit_server = new rabbitMQClient($rabbitServ,"ErrorLogging");
		$this->sending_server = $log_sender;
	}

	function logError($errType, $errLevel, $errMessage)
	{
		$this->log_timestamp = date("m/d/Y H:i:s");
		$this->log_type = $errType;
		$this->error_level = $errLevel;
		$this->log_message = $errMessage;

		$this->rabbit_message = $this->log_timestamp."\t".$this->sending_server."\t".$this->log_type."\t".$this->error_level."\t".$this->log_message;

		$response = $this->rabbit_server->publish($this->rabbit_message);
		echo "Logged Error\n";
		echo $this->rabbit_message;
	}
	
	function logDebug($logType, $debugMessage)
	{
		$this->log_timestamp = date("m/d/Y H:i:s");
		$this->log_type = $logType;
		$this->error_level = "DEBUG";
		$this->log_message = $debugMessage;

		$this->rabbit_message = $this->log_timestamp."\t".$this->sending_server."\t".$this->log_type."\t".$this->error_level."\t".$this->log_message;

		$response = $this->rabbit_server->publish($this->rabbit_message);
		echo "Logged Debug\n";
		echo $this->rabbit_message;
	}
}



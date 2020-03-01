#!/usr/bin/php
<?php

class stockDataLink
{
	private $sqlConnection;
	private $stock_symbol;
	private $stock_open;
	private $stock_high;
	private $stock_low;
	private $stock_price;
	private $stock_volume;
	private $stock_lastClose;
	private $stock_lastCloseDay;
	private $stock_array;

	function __construct($sqlConn)
	{
		$this->sqlConnection = $sqlConn;
	}

	function getStock($symb)
	{
		$this->stock_symbol = $symb;

		$q="SELECT * FROM StockData WHERE (stock_sym='$stock_symbol')";

		$sqlResult = @mysqli_query($this->sqlConnection,$q);
		$num = @mysqli_num_rows($sqlResult);

		if(empty(mysqli_error($this->sqlConnection)))
		{
			if(num == 1)
			{
				$this->stock_array = @mysqli_fetch_assoc($sqlResult);
				return $this->stock_array;
			}
			else
			{
				 apiLookup($this->stock_symbol);
				return $this->stock_array;
			}
		}

		else
		{

		}
	}


	
	function apiLookup($symb)
	{
		
		$rabbit = new rabbitMQClient("rabbit.ini, "   " ");
		$response = $rabbit->send_request($symb);

		storeStock($response);

	}

	function storeStock($rabbitResponse)
	{
		$
		





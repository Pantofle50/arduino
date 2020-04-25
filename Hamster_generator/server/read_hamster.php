<?php
// Include config files
require_once 'config_db.php';


// var_dump(file_get_contents("php://input"));
// echo "<br>";
// echo "<br>";
// print_r($_REQUEST);
// echo "<br>";
// echo "<br>";
	
//Check if all fields are present
$postDataPresent = isset($_POST['VOL']) && isset($_POST['CUR']) && isset($_POST['POW']);

if($postDataPresent != true)
{
	die("ERROR: Received data incomplete.");
}

//Get the data from the request
$vol_raw = htmlspecialchars($_POST['VOL']);
$cur_raw = htmlspecialchars($_POST['CUR']);
$pow_raw = htmlspecialchars($_POST['POW']);
  
//Check if the received data is valid
$datasetValid = is_numeric($vol_raw) && is_numeric($cur_raw) && is_numeric($pow_raw);
  				   		
if($datasetValid != true)
{
	die("ERROR: Received data invalid.");
}
  
//Get numeric values
$vol = floatval($vol_raw);
$cur = floatval($cur_raw);
$pow = floatval($pow_raw);

/* Prepare MqSQL stuff*/
// Attempt to connect to MySQL database
$dbLink = mysqli_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD, DB_NAME);

// Check connection
if($dbLink === false)
{
	die("ERROR: Could not connect to the database. " . mysqli_connect_error());
}

// Prepare an insert statement
$sql = "INSERT INTO wifiSolarPower (voltage, current, power) VALUES (?, ?, ?)";


$stmt = mysqli_prepare ($dbLink, $sql);
	
if($stmt == false)
{
	echo "WARNING: DB data prepare failed";
}

// Bind variables to the prepared statement as parameters
mysqli_stmt_bind_param($stmt, "ddd", $vol, $cur, $pow);

// Attempt to execute the prepared statement
if (mysqli_stmt_execute ($stmt)) 
{
	$user_added = true;	//TODO TODO TODO TODO TODO
	echo "DOST DOBRY";

} 
else 
{
	echo "WARNING: DB data insert failed.";
}


  // Close connection
  mysqli_close($dbLink);
?>
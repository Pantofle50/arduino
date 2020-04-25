
   <!DOCTYPE html>

    <html lang="en">

    <head>

        <meta charset="UTF-8">

        <title>Showing hamster data</title>

        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.css">

        <style type="text/css">

            body{ font: 14px sans-serif; text-align: center; }

        </style>

    </head>
    
    <?php 
	if ((isset($_GET['date_from'])) && (!empty($_GET['date_from'])))
	{
		$date_from = htmlspecialchars($_GET['date_from']);
	}
	else
	{
		$date_from = date("Y-m-d");
	}
	
	if ((isset($_GET['date_to'])) && (!empty($_GET['date_to'])))
	{
		$date_to = htmlspecialchars($_GET['date_to']);
	}
	else
	{
		$date_to = date("Y-m-d");
	}
	?>
	
    <body>
    	<div class="container-fluid">
    	<div class="row">
   		<form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="get">
		
		<div class="form-group">
			 	<label>Date from </label>
				<input type="date" name="date_from" value="<?php echo $date_from ?>">
			
		</div>
		<div class="form-group">
			 	<label>Date to </label>
				<input type="date" name="date_to" value="<?php echo $date_to ?>">
			
		</div>
		<div class="form-group">

        	<input type="submit" class="btn btn-primary" value="Fetch data">

        </div>
		</form> 
		</div>

<?php
	require_once 'config_db.php';
	
	// Initialize the session
	
	session_start();
	
	// If session variable is not set it will redirect to login page
	
	if(!isset($_SESSION['username']) || empty($_SESSION['username']))
	{
	  header("location: login.php?src=show_hamster");
	  exit;
	}
	
	if ((isset($_GET['date_from'])) && (!empty($_GET['date_from'])))
	{
		$date_from = htmlspecialchars($_GET['date_from']);
	}
	else
	{
		$date_to = date("Y-m-d");
	}
		
	if ((isset($_GET['date_to'])) && (!empty($_GET['date_to'])))
	{
		$date_to = htmlspecialchars($_GET['date_to']);
	}
	else
	{
		$date_to = date("Y-m-d");
	}
		
	/* Prepare MqSQL stuff*/
	// Attempt to connect to MySQL database
	$dbLink = mysqli_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD, DB_NAME);
		
	// Check connection
	if($dbLink === false)
	{
		die("ERROR: Could not connect to the database. " . mysqli_connect_error());
	}
		
	// Prepare a select statement
	$sql = "SELECT * FROM hamsterPower WHERE (cast(timestamp as date) BETWEEN '".$date_from."' AND '".$date_to."')";

	$stmt = mysqli_prepare ($dbLink, $sql);
			
	if($stmt == false)
	{
		echo "WARNING: DB data prepare failed";
	}

	//Execute
	if(mysqli_stmt_execute ($stmt) == false)
	{
		die("ERROR: SQL query failed. " . mysqli_error($dbLink));
	}
		
	//Get the result
	$result = mysqli_stmt_get_result($stmt);
		
	//Get the number of rows
	$num_of_rows = mysqli_num_rows($result);
		
	if($num_of_rows == 0)
	{
		echo "WARNING: Zero records found.";
	}
	
	echo "Total number of records: ".$num_of_rows."<br><br>";
		
	while ($row = mysqli_fetch_array($result)) {
		$entry_vol .= "[new Date(".strtotime($row{'timestamp'})."*1000)".",".$row{'voltage'}."],";
		$entry_cur .= "[new Date(".strtotime($row{'timestamp'})."*1000)".",".$row{'current'}."],";
		$entry_pow .= "[new Date(".strtotime($row{'timestamp'})."*1000)".",".$row{'power'}."],";
		$entry_rpm .= "[new Date(".strtotime($row{'timestamp'})."*1000)".",".$row{'rpm'}."],";
	}
		
	/* free results */
	mysqli_free_result($stmt);
	
	mysqli_close($dbLink);
	  	
?>
		<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
	    <script type="text/javascript">
	    	google.charts.load('current', {'packages':['corechart']});
	    	google.charts.setOnLoadCallback(drawChart);
	
	    	function drawChart() {
	        	var data = google.visualization.arrayToDataTable([
	            ['Date',	'Voltage'],
	            <?php echo $entry_vol ?>
	            ]);
	            
	        	var date_formatter = new google.visualization.DateFormat({ 
	        	    pattern: 'HH:mm dd/MM/yy'
	        	}); 
	        	date_formatter.format(data, 0);  //Format the date (0th column) to get the correct date format for tooltips
	        	
				var options = {
					title: 'Hamster voltage',
					explorer:{},
					pointsVisible: true,
					legend: {
						position: 'top',
						alignment: 'center',
					},
					hAxis: {
						format:'dd/MM/yy HH:mm',
						gridlines: {color: 'gray'},
						slantedText: true,
					},

					vAxes: {
						0: {
							viewWindowMode:'pretty',
							gridlines: {color: 'gray'},
							format: '# V',
						},
					},
					series: {
						0: {targetAxisIndex:0},
					},
					colors: ["red"],
				};
	
				var chart = new google.visualization.LineChart(document.getElementById('curve_chart_vol'));
	
	      		chart.draw(data, options);
	    	}
	    	
	    </script>
	    
	    <script type="text/javascript">
	    	google.charts.load('current', {'packages':['corechart']});
	    	google.charts.setOnLoadCallback(drawChart);
	
	    	function drawChart() {
	        	var data = google.visualization.arrayToDataTable([
	            ['Date',	'Current'],
	            <?php echo $entry_cur ?>
	            ]);
	            
	        	var date_formatter = new google.visualization.DateFormat({ 
	        	    pattern: 'HH:mm dd/MM/yy'
	        	}); 
	        	date_formatter.format(data, 0);  //Format the date (0th column) to get the correct date format for tooltips
	        	
				var options = {
					title: 'Hamster current',
					explorer:{},
					pointsVisible: true,
					legend: {
						position: 'top',
						alignment: 'center',
					},
					hAxis: {
						format:'dd/MM/yy HH:mm',
						gridlines: {color: 'gray'},
						slantedText: true,
					},

					vAxes: {
						0: {
							viewWindowMode:'pretty',
							gridlines: {color: 'gray'},
							format: '# mA',
						},
					},
					series: {
						0: {targetAxisIndex:0},
					},
					colors: ["green"],
				};
	
				var chart = new google.visualization.LineChart(document.getElementById('curve_chart_cur'));
	
	      		chart.draw(data, options);
	    	}
	    	
	    </script>
	    
	    <script type="text/javascript">
	    	google.charts.load('current', {'packages':['corechart']});
	    	google.charts.setOnLoadCallback(drawChart);
	
	    	function drawChart() {
	        	var data = google.visualization.arrayToDataTable([
	            ['Date',	'Power'],
	            <?php echo $entry_pow ?>
	            ]);
	            
	        	var date_formatter = new google.visualization.DateFormat({ 
	        	    pattern: 'HH:mm dd/MM/yy'
	        	}); 
	        	date_formatter.format(data, 0);  //Format the date (0th column) to get the correct date format for tooltips
	        	
				var options = {
					title: 'Hamster power',
					explorer:{},
					pointsVisible: true,
					legend: {
						position: 'top',
						alignment: 'center',
					},
					hAxis: {
						format:'dd/MM/yy HH:mm',
						gridlines: {color: 'gray'},
						slantedText: true,
					},

					vAxes: {
						0: {
							viewWindowMode:'pretty',
							gridlines: {color: 'gray'},
							format: '# mW',
						},
					},
					series: {
						0: {targetAxisIndex:0},
					},
					colors: ["blue"],
				};
	
				var chart = new google.visualization.LineChart(document.getElementById('curve_chart_pow'));
	
	      		chart.draw(data, options);
	    	}
	    	
	    </script>
		
		<script type="text/javascript">
	    	google.charts.load('current', {'packages':['corechart']});
	    	google.charts.setOnLoadCallback(drawChart);
	
	    	function drawChart() {
	        	var data = google.visualization.arrayToDataTable([
	            ['Date',	'rpm'],
	            <?php echo $entry_rpm ?>
	            ]);
	            
	        	var date_formatter = new google.visualization.DateFormat({ 
	        	    pattern: 'HH:mm dd/MM/yy'
	        	}); 
	        	date_formatter.format(data, 0);  //Format the date (0th column) to get the correct date format for tooltips
	        	
				var options = {
					title: 'Hamster rpm',
					explorer:{},
					pointsVisible: true,
					legend: {
						position: 'top',
						alignment: 'center',
					},
					hAxis: {
						format:'dd/MM/yy HH:mm',
						gridlines: {color: 'gray'},
						slantedText: true,
					},

					vAxes: {
						0: {
							viewWindowMode:'pretty',
							gridlines: {color: 'gray'},
							format: '# rpm',
						},
					},
					series: {
						0: {targetAxisIndex:0},
					},
					colors: ["brown"],
				};
	
				var chart = new google.visualization.LineChart(document.getElementById('curve_chart_pow'));
	
	      		chart.draw(data, options);
	    	}
	    	
	    </script>
	    
    	<div class="row">
    	
    	<div class="col-md-1"></div>
    	<div class="col-md-1"></div>
    	<div class="col-md-4">
    	<?php 
	    		echo "<div class=\"wrapper\" id=\"curve_chart_vol\" style=\"width: 1200px; height: 500px\"></div>";
    	?>
		</div>
		<div class="col-md-1"></div>
		
		</div>
		
		<div class="row">
    	
    	<div class="col-md-1"></div>
    	<div class="col-md-1"></div>
    	<div class="col-md-4">
    	<?php 
	    		echo "<div class=\"wrapper\" id=\"curve_chart_cur\" style=\"width: 1200px; height: 500px\"></div>";
    	?>
		</div>
		<div class="col-md-1"></div>
		
		</div>
		
		<div class="row">
    	
    	<div class="col-md-1"></div>
    	<div class="col-md-1"></div>
    	<div class="col-md-4">
    	<?php 
	    		echo "<div class=\"wrapper\" id=\"curve_chart_pow\" style=\"width: 1200px; height: 500px\"></div>";
    	?>
		</div>
		<div class="col-md-1"></div>
		
		</div>
		
		<div class="wrapper">
		
					<p></p>
					<p>
					<?php
					if (logged_in == true){
						echo "Zpet na ";
						echo '<a href="index.php">';
						echo "index";
						echo '</a>';
						echo "      ";
						echo "Prihlasen jako: ";
						echo '<strong>';
						echo htmlspecialchars($_SESSION['username']);
						echo '</strong>';
						echo "      Odhlasit se muzete";
						echo '<a href="logout.php" class="strong">';
						echo " zde";
						echo '</a>';
					}
					?>
					</p>
		</div>
		
	</div>	
    </body>

    </html>
  


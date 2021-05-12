<?php
parse_str($_SERVER['QUERY_STRING'], $output);
?>
<html>
<body>
<?php
echo "<h2>Firstname: " . $output['firstname'] . "</h2></br>";
echo "<h2>Lastname: " . $output['lastname'] . "</h2></br>";

echo "<p>--add phpinfo for debugging--\n</p>";
echo "<p>";
phpinfo(16);
echo "</p>";
echo "<p>";
phpinfo(32);
echo "</p>";
?>
</body>
</html>


<?php
parse_str($_SERVER['QUERY_STRING'], $output);
echo "HTTP/1.1 200 OK\r\n";
echo "Content-Length: 85\r\n";
echo "Content-Type: text/html; charset=UTF-8\r\n";
echo "\r\n";
?>
<html>
<body>
<?php
echo "<h2>Firstname: " . $output['firstname'] . "</h2></br>";
echo "<h2>Lastname: " . $output['lastname'] . "</h2></br>";
?>
</body>
</html>

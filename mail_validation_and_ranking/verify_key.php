<?php
print_r($_COOKIE); // Debugging - display session data

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $entered_key = htmlspecialchars($_POST['key']);
    echo $entered_key;

    if (isset($_COOKIE['key'])) {
        if ($_COOKIE['key'] == $entered_key) {
            $xml = file_get_contents("http://localhost:5000/add-key/".$_POST["key"]);

            header("Location: http://localhost:5000/game-creator?key=".$_POST['key']);
            exit();
        } else {
            echo "Invalid key.";
        }
    } else {
        echo "No key found in session.";
    }
}
?>



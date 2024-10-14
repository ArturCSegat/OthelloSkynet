<?php
session_start();
print_r($_SESSION); // Debugging - display session data

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $entered_key = htmlspecialchars($_POST['key']);

    if (isset($_SESSION['user']['key'])) {
        if ($_SESSION['user']['key'] == $entered_key) {
            header("Location: othello_game.php");
            exit();
        } else {
            echo "Invalid key.";
        }
    } else {
        echo "No key found in session.";
    }
}
?>



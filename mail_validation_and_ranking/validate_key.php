<?php

if (isset($_GET['key'])) {
    $entered_key = htmlspecialchars($_GET['key']);

    if ($_COOKIE['key'] == $entered_key) {
        // Redirect to the game page
        $xml = file_get_contents("http://localhost:5000/add-key/".$_GET["key"]);

        header("Location: http://localhost:5000/game-creator?key=".$_GET['key']);
        exit();
    } else {
        echo "Invalid key.";
    }
} else {
    echo "No key provided.";
}
?>

<?php
session_start();

if (isset($_GET['key'])) {
    $entered_key = htmlspecialchars($_GET['key']);

    if ($_SESSION['user']['key'] == $entered_key) {
        // Redirect to the game page
        $xml = file_get_contents("https://oca.ctism.ufsm.br/othello/add-key/".$_GET["key"]);

        header("Location: https://oca.ctism.ufsm.br/othello/game-creator?key=".$_GET['key']);
        exit();
    } else {
        echo "Invalid key.";
    }
} else {
    echo "No key provided.";
}
?>

<?php
session_start();
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $name = $_SESSION['name'];
    $score = htmlspecialchars($_POST['score']);

    // Load existing scores
    $scores = json_decode(file_get_contents('scores.json'), true);

    // Add the new score
    $scores[] = [
        'name' => $name,
        'score' => $score
    ];

    // Save back to file
    file_put_contents('scores.json', json_encode($scores, JSON_PRETTY_PRINT));

    echo "Score saved!";
}
?>

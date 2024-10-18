<?php
$jsonFile = file_get_contents('scores.json');
$scores = json_decode($jsonFile,true);

// Sort by score descending
usort($scores, function ($a, $b) {
    return $b['score'] - $a['score'];
});

echo "<h1>Ranking</h1><ol>";
foreach ($scores as $score) {
    echo "<li>{$score['name']} - {$score['score']}</li>";
}
echo "</ol>";
?>

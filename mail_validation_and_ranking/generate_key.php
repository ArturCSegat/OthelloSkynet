<?php
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

require 'vendor/autoload.php';

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $name = htmlspecialchars($_POST['name']);
    $email = htmlspecialchars($_POST['email']);

    // Generate random hexadecimal key
    $key = bin2hex(random_bytes(3));

    // Store user information and key in a session
    $_SESSION['user'] = [
        'name' => $name,
        'email' => $email,
        'key' => $key
    ];
    setcookie("name", $name, time()+60*120, "/", "");
    setcookie("email", $email, time()+60*120, "/", "");
    setcookie("key", $key, time()+60*120, "/", "");

    // Send email via Gmail SMTP using PHPMailer
    $mail = new PHPMailer(true);
    // Create the link with the key as a query parameter
    $validation_link = "http://localhost:3000/validate_key.php?key=$key";

    try {
        // SMTP configuration
        $mail->isSMTP();
        $mail->Host = 'smtp.gmail.com';
        $mail->SMTPAuth = true;
        $mail->Username = 'othelloctism@gmail.com'; // Your Gmail address
        $mail->Password = 'uzpgsnnvvvudpbuj'; // App Password or Gmail Password
        $mail->SMTPSecure = 'tls';
        $mail->Port = 587;

        // Recipients
        $mail->setFrom('your_email@gmail.com', 'Othello Game');
        $mail->addAddress($email);

        // Email content
        $mail->isHTML(true);
        $mail->Subject = 'Your Othello Game Key';
        //$mail->Body = "Hello $name,<br>Your login key is: <b>$key</b>";
        $mail->Body = "Hello $name,<br>Your login key is: <b>$key</b><br>Click <a href='$validation_link'>here</a> to validate your key and start the game.";


        $mail->send();
        header("location: /");
    } catch (Exception $e) {
        echo "Message could not be sent. Mailer Error: {$mail->ErrorInfo}";
    }
}
?>

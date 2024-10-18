<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Othello Login</title>
    <!-- Importando a fonte Montserrat do Google Fonts -->
    <link href="https://fonts.googleapis.com/css2?family=Montserrat:wght@400;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="estilo.css">
</head>
<body>
    <div class="header-container">
        <img src="https://static.thenounproject.com/png/4114135-200.png" alt="Othello Icon" class="icon">
        <h1>Othello Game</h1>
    </div>
    <div class="login-container">
        <!-- Título "Login" dentro da caixa de login -->
        <h2 class="login-title">Login</h2>
        <form action="generate_key.php" method="POST">
            <label for="name">Nome:</label>
            <input type="text" id="name" name="name" required><br><br>

            <label for="email">E-mail:</label>
            <input type="email" id="email" name="email" required><br><br>

            <input type="submit" value="Enviar Chave">
        </form>
        <center>
            <h3>Ou</h3>
        </center>
        <h2 class="login-title">Entre com uma chave</h2>
        <form action="verify_key.php" method="POST">
            <label for="key">Chave:</label>
            <input type="text" id="key" name="key" required><br><br>

            <input type="submit" value="avaliar Chave">
        </form>
    </div>

    <div class="rules-button-container">
        <button id="rules-button">Regras</button>
    </div>


    <!-- Modal para as regras -->
    <div id="rules-modal" class="modal">
        <div class="modal-content">
            <span class="close">&times;</span>
            <h2>Regras do Jogo</h2>
            <p><strong>Tabuleiro:</strong> O jogo é jogado em um tabuleiro 8x8.</p>
            <p><strong>Peças:</strong> Existem duas cores de peças: preto e branco. Cada jogador controla uma cor.</p>
            <p><strong>Setup Inicial:</strong> No início, quatro peças são colocadas no centro do tabuleiro: duas pretas formando um quadrado com duas brancas.</p>
            <p><strong>Objetivo:</strong> O objetivo é ter mais peças da sua cor no tabuleiro ao final da partida.</p>
            <p><strong>Jogadas:</strong> Os jogadores se revezam colocando uma peça de sua cor no tabuleiro.</p>
            <p>Uma jogada é válida se, ao colocar sua peça, ele "captura" uma ou mais peças do oponente entre sua nova peça e outra peça já sua, em linha reta (horizontal, vertical ou diagonal).</p>
            <p>As peças capturadas são viradas para a cor do jogador que jogou.</p>
            <p><strong>Turnos:</strong> Se um jogador não pode fazer uma jogada válida, ele deve passar a vez.</p>
            <p><strong>Fim do Jogo:</strong> O jogo termina quando o tabuleiro estiver cheio ou nenhum jogador puder fazer uma jogada. O jogador com mais peças de sua cor vence.</p>
        </div>
    </div>

    <script>
        // Abrir o modal quando o botão de regras for clicado
        var modal = document.getElementById("rules-modal");
        var btn = document.getElementById("rules-button");
        var span = document.getElementsByClassName("close")[0];
    
        btn.onclick = function() {
            modal.style.display = "block";
        }
    
        // Fechar o modal quando o "x" for clicado
        span.onclick = function() {
            modal.style.display = "none";
        }
    
        // Fechar o modal se o usuário clicar fora dele
        window.onclick = function(event) {
            if (event.target == modal) {
                modal.style.display = "none";
            }
        }
    </script>

</body>
</html>
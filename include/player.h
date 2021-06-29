const char playerIndexHTML[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Player</title>
    
</head>

<body>
    <h3>COde ingeben</h3>

    <input type="text" name="gamecode-input" id="gamecode-input">

    <button id="enter-game">Spiel beitreten</button>
)=====";

const char playerIndexJS[] = R"=====(
<script>
    document.getElementById('enter-game').addEventListener('click', function () {
        ajaxCall("enterGame");
    });

    function ajaxCall(link){
        console.log(link);
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", "/"+link, true);
        xmlhttp.send();
    }
</script>
)=====";

const char playerQuestion[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Frage</title>
</head>
<body>
    <h1>Frage beantworten</h1>
    <p id="question">Frage</p>
    <div id="answers">
        <span id="answerA"></span>
        <span id="answerC"></span>
        <span id="answerD"></span>
        <span id="answerE"></span>
    </div>
    <button id="select-answer">Antwort einloggen</button>

    <div id="correct-answer">
        <p>A: Baum</p>
        <p id="explenation"></p>
    </div>
</body>
</html>
)=====";

const char playerIndex[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Player</title>
    
</head>

<body>
    <h3>COde ingeben</h3>

    <input type="text" name="gamecode-input" id="gamecode-input">

    <button id="enter-game">Spiel beitreten</button>
</body>
<script defer>
    document.getElementById('enter-game').addEventListener('click', function () {
        ajaxCall("enterGame");
    });

    function ajaxCall(link){
        console.log(link);
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", "/"+link, true);
        xmlhttp.send();
    }
</script>

</html>


)=====";
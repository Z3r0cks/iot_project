const char masterIndex[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Master Frage</title>
    <script src="index.js" defer></script>
    <link rel="stylesheet" href="style.css">
</head>

<body>
    <h3>Code</h3>
    <h1 id="gamecode">----</h1>
</body>

</html>
)=====";

const char masterIndexJS[] = R"=====(
document.querySelector("#gamecode").innerHTML="$KEY";
const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest(); 
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if(this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "validate-key", true); 
    xhttp.send(); 
}, 1000);
)=====";

const char masterQuestions[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Master Frage</title>
    <script src="questions.js" defer></script>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div id="cat1">
        <div id="01_01" class="question">
            <div id="01_02" class="question"></div>
            <div id="01_03" class="question"></div>
            <div id="01_04" class="question"></div>
        </div>
        <div id="cat2">
            <div id="02_01" class="question"></div>
            <div id="02_02" class="question"></div>
            <div id="02_03" class="question"></div>
            <div id="02_04" class="question"></div>
        </div>

        <div id="cat3">
            <div id="03_01" class="question"></div>
            <div id="03_02" class="question"></div>
            <div id="03_03" class="question"></div>
            <div id="03_04" class="question"></div>
        </div>
        <div id="cat4">
            <div id="04_01" class="question"></div>
            <div id="04_02" class="question"></div>
            <div id="04_03" class="question"></div>
            <div id="04_04" class="question"></div>
        </div>
</body>
</html>
)=====";

const char masterQuestionsJS[] = R"=====(
document.querySelectorAll(".question").forEach(function(item) {
        function clickHandler() {
        const xhttp = new XMLHttpRequest(); 
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                if(this.responseText.trim() == "$VALIDATION") {
                    item.removeEventListener("click", clickHandler);
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "choose-question?question=" + item.id, true); 
        xhttp.send(); 
    }
    item.addEventListener("click", clickHandler);
});
)=====";

const char masterSolution[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Richtige Antwort</title>
    <script src="solution.js" defer></script>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <p id="solution"></p>
    <p id="wait-info"></p>
</body>
</html>
)=====";

const char masterSolutionJS[] = R"=====(
const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest(); 
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if(this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "validate-answer", true); 
    xhttp.send(); 
}, 1000);
)=====";

const char masterVictory[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sieg</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Sieg</h3>
</body>
</html>
)=====";

const char masterDefeat[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Niederlage</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Niederlage</h3>
</body>
</html>
)=====";

const char masterError[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Fehler</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Ups!</h3>
    <p>Etwas ist schief gelaufen.<p>
</body>
</html>
)=====";

const char masterStyle[] = R"=====(
    .question {
        width: 50vw;
        height: 9vh;
        margin: 0 0 1vh 0;
        background-color: green;
    }
)=====";
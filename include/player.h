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
        <span id="answerA">Test</span>
        <span id="answerB">TEst2</span>
        <span id="answerC">fdsfds</span>
        <span id="answerD">adsad</span>
    </div>
    <button id="login-answer">Antwort einloggen</button>

    <div id="correct-answer">
        <p>A: Baum</p>
        <p id="explenation"></p>
    </div>
</body>
<script>
    document.getElementById('answerA').addEventListener('mouseover', function () {
        selectAnswer(this)
    });
    document.getElementById('answerB').addEventListener('mouseover', function () {
        selectAnswer(this)
    });
    document.getElementById('answerC').addEventListener('mouseover', function () {
        selectAnswer(this)
    });
    document.getElementById('answerD').addEventListener('mouseover', function () {
        selectAnswer(this)
    });

    var selectedAnswerGlobal

    var questionID = "$questionID";

    const myArray = [
        "01_01", [
            "Wegen der Höhenlage", "Wegen dem vielen Schnee", "Es wächst viel Löwenzahn in der Gegend",
            "Wegen der guten Luft",
            "Löwenzahn besitzen nicht nur eine Blüte sondern viele. Bienen werden davon angelockt.", "C"
        ],
        "01_02", [
            "Weil Tannen besser Wärme speichern können",
            "Weil die Nadeln eng beieinander liegen und sich dadurch gegenseitig wärmen",
            "Durch eine Art Salz, was als Frostschutzmittel dient",
            "Durch eine Art Zucker, was als Frostschutzmittel dient",
            "Tannen speichen außerdem weniger Wasser und die Nadeln sind dicker. Bei Laubblätter haben kaum festes Gewebe und würden im Winter deshalb erfrieren.",
            "Erklärung"
        ],
        "01_03", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "01_04", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "02_01", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "02_02", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "02_03", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "02_04", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "03_01", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "03_02", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "03_03", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "03_04", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "04_01", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "04_02", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "04_03", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ],
        "04_04", [
            "Antwort1", "Antwort2", "Antwort3", "Antwort4", "Erklärung"
        ]
    ];


    for (let i = 0; i < myArray.length; i++) {
        if (myArray[i] == questionID) {
            document.getElementById('answerA').innerHTML = myArray[i + 1][0];
            document.getElementById('answerB').innerHTML = myArray[i + 1][1];
            document.getElementById('answerC').innerHTML = myArray[i + 1][2];
            document.getElementById('answerD').innerHTML = myArray[i + 1][3];
            document.getElementById('explenation').innerHTML = myArray[i + 1][4];
        }
    }


    function selectAnswer(selectedAnswer) {
        selectedAnswerGlobal = selectedAnswer;
        //style Selected Answer
        console.log(selectedAnswer)

        //style unselected answers
        var allAnswers = document.getElementById('answers').children;
        console.log(allAnswers);
    }



    document.getElementById('login-answer').addEventListener('click', function () {
        var id = selectedAnswerGlobal.id;
        console.log(id)
        ajaxCall("/?selectedAnswer=" + id.substr(6));
        window.location.reload();
    });


    function ajaxCall(link) {
        console.log(link);
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", link, true);
        xmlhttp.send();
    }
</script>

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
<script>
    document.getElementById('enter-game').addEventListener('click', function () {
        ajaxCall('/checkKey='+document.getElementById('gamecode-input').value);
        window.location.reload();
    });
    function ajaxCall(link) {
        console.log(link);
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", link, true);
        xmlhttp.send();
    }

</script>
<span onload="window.location.assign('')"></span>

</html>

)=====";

const char playerWait[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Player Wartem</title>
</head>

<body>
    <p>Game Measter wählt eine Frage aus</p>
</body>
<script>
    const interval = setInterval(function () {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "RELOAD") {
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "command", true);
        xhttp.send();
    }, 1000);
</script>

</html>
)=====";

const char solutionHTML[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Richtige Antwort</title>
</head>

<body>
    <h1>Auflösung</h1>
    <p id="feedback"></p>
    <p id="solution"></p>
    <p id="wait-info">Der Game MAster wäjlt eine Fage aus. Bitte Warten</p>
</body>

<script>
    // const correctAnswerGlobal = "$correctAnswer";
    // const selectedAnswerGlobal = "$selectedAnswer";

    var explenationSet;

    if (!explenationSet) {
        const interval = setInterval(function () {
            const xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    console.log(this.responseText);
                    if (this.responseText.trim() != "") {
                        if (this.responseText.indexOf("RELOAD") < 0) {
                            const json = JSON.parse(this.responseText.trim());

                            if (json.selectedAnswer == json.correctAnswer) {
                                document.getElementById('feedback').innerHTML = 'Deine Antwort ist richtig';
                                ajaxCall("/?result=right");
                                console.log('RICHTIG');
                            } else {
                                document.getElementById('feedback').innerHTML =
                                    'Deine Antwort ist falsch. Die Richtige Antwort ist: ';
                                ajaxCall("/?result=wrong");
                                console.log('FALSCH');
                            }
                            document.getElementById('solution').innerHTML = json.correctAnswer + ': ' +
                                explanations['$QUESTION'];

                            document.selectedAnswer('');
                        } else {
                            console.log("reload response");
                            ajaxCall("/newQuestion");
                            window.location.reload();
                        }
                    }
                }
            }

            xhttp.open("GET", "command", true);
            xhttp.send();
        }, 1000);

        function ajaxCall(link) {
            console.log(link);
            var xmlhttp = new XMLHttpRequest();
            xmlhttp.open("GET", link, true);
            xmlhttp.send();
        }
    }



    const explanations = {
        "01_01": "explanation",
        "01_02": "explanation2",
        "01_03": "explanation",
        "01_04": "explanation",
        "02_01": "explanation",
        "02_02": "explanation",
        "02_03": "explanation",
        "02_04": "explanation",
        "03_01": "explanation",
        "03_02": "explanation",
        "03_03": "explanation",
        "03_04": "explanation",
        "04_01": "explanation",
        "04_02": "explanation",
        "04_03": "explanation",
        "04_04": "explanation"
    }

    // document.getElementById('solution').innerHTML = explanations["01_02"];
</script>


</html>
)=====";

const char playerCheckKey[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<body>
    <p>Code wird überprüft</p>
</body>
<script>
    const interval = setInterval(function () {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "RELOAD") {
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "command", true);
        xhttp.send();
    }, 1000);
</script>

</html>
)=====";


const char playerWin[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gwonnen</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Gewommen</h3>
</body>
</html>
)=====";


const char playerLoose[] = R"=====(
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